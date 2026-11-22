#include "jack/cli/compilador_vm_cli.h"

#include "jack/lexer/analisadorlexico.h"
#include "jack/parser/analisadorsintatico.h"
#include "jack/vm/vmcodegenerator.h"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>
#include <utility>
#include <variant>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace jack::cli {
namespace {

struct CommandLineOptions {
    std::filesystem::path input;
    std::filesystem::path outputDirectory;
    bool outputDirectorySpecified = false;
};

struct DiagnosticLocation {
    std::size_t line = 0;
    std::size_t column = 0;
};

void printUsage(std::ostream& output)
{
    output
        << "Uso:\n"
        << "  jack_cli --compile <arquivo.jack|diretorio> [--output <diretorio>]\n"
        << "  jack_cli compile <arquivo.jack|diretorio> [-o <diretorio>]\n"
        << "  jack_cli --help\n\n"
        << "Quando --output/-o não é informado, os arquivos .vm são gravados\n"
        << "no mesmo diretório do arquivo de entrada ou do diretório processado.\n"
        << "A busca em diretórios é não recursiva.\n";
}

bool parseArguments(int argc, char* argv[], CommandLineOptions& options,
                    bool& showHelp, std::string& error)
{
    showHelp = false;

    if (argc < 2) {
        error = "Modo não interativo requer --compile ou compile.";
        return false;
    }

    const std::string first = argv[1] ? argv[1] : "";
    if (first == "--help" || first == "-h") {
        showHelp = true;
        return true;
    }

    if (first != "--compile" && first != "compile") {
        error = "Comando desconhecido: " + first;
        return false;
    }

    if (argc < 3 || argv[2] == nullptr || std::string(argv[2]).empty()) {
        error = "Caminho de entrada não informado.";
        return false;
    }

    options.input = std::filesystem::path(argv[2]);

    int index = 3;
    while (index < argc) {
        const std::string argument = argv[index] ? argv[index] : "";
        if (argument == "--output" || argument == "-o") {
            if (index + 1 >= argc || argv[index + 1] == nullptr
                || std::string(argv[index + 1]).empty()) {
                error = "Diretório de saída não informado após " + argument + ".";
                return false;
            }
            if (options.outputDirectorySpecified) {
                error = "O diretório de saída foi especificado mais de uma vez.";
                return false;
            }
            options.outputDirectory = std::filesystem::path(argv[index + 1]);
            options.outputDirectorySpecified = true;
            index += 2;
        } else {
            error = "Argumento não reconhecido: " + argument;
            return false;
        }
    }

    return true;
}

DiagnosticLocation extractLexicalLocation(const std::string& message)
{
    DiagnosticLocation location;
    std::smatch match;

    static const std::regex lineColumn(
        R"(linha\s+([0-9]+)\s*,\s*coluna\s+([0-9]+))",
        std::regex::icase);
    static const std::regex lineOnly(
        R"(linha\s+([0-9]+))",
        std::regex::icase);

    if (std::regex_search(message, match, lineColumn)) {
        try {
            location.line = static_cast<std::size_t>(std::stoul(match[1].str()));
            location.column = static_cast<std::size_t>(std::stoul(match[2].str()));
        } catch (...) {
            return {};
        }
        return location;
    }

    if (std::regex_search(message, match, lineOnly)) {
        try {
            location.line = static_cast<std::size_t>(std::stoul(match[1].str()));
        } catch (...) {
            return {};
        }
    }
    return location;
}

void printDiagnostic(const char* category,
                     const std::filesystem::path& source,
                     const std::string& message,
                     std::size_t line = 0,
                     std::size_t column = 0)
{
    std::cerr << '[' << category << "] " << source.string();
    if (line != 0) {
        std::cerr << ':' << line;
        if (column != 0) {
            std::cerr << ':' << column;
        }
    }
    std::cerr << ": " << message << '\n';
}

bool readSource(const std::filesystem::path& path, std::string& source,
                std::string& error)
{
    std::ifstream input(path, std::ios::binary);
    if (!input.is_open()) {
        error = "Não foi possível abrir o arquivo para leitura.";
        return false;
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    if (input.bad()) {
        error = "Falha durante a leitura do arquivo.";
        return false;
    }

    source = buffer.str();
    return true;
}

bool replaceFileAtomically(const std::filesystem::path& temporary,
                           const std::filesystem::path& destination,
                           std::string& error)
{
#ifdef _WIN32
    if (MoveFileExW(temporary.c_str(), destination.c_str(),
                    MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        return true;
    }

    error = "MoveFileExW falhou (código " +
            std::to_string(static_cast<unsigned long>(GetLastError())) + ").";
    return false;
#else
    std::error_code filesystemError;
    std::filesystem::rename(temporary, destination, filesystemError);
    if (filesystemError) {
        error = filesystemError.message();
        return false;
    }
    return true;
#endif
}

bool publishOutput(const std::filesystem::path& destination,
                   const std::string& contents,
                   std::string& error)
{
    std::error_code filesystemError;
    const std::filesystem::path parent = destination.parent_path().empty()
                                             ? std::filesystem::current_path()
                                             : destination.parent_path();

    if (!std::filesystem::exists(parent, filesystemError) || filesystemError
        || !std::filesystem::is_directory(parent, filesystemError)
        || filesystemError) {
        error = "O diretório de destino não existe ou não é acessível: "
                + parent.string();
        return false;
    }

    std::filesystem::path temporary;
    bool foundTemporaryName = false;
    for (unsigned int attempt = 0; attempt < 1000; ++attempt) {
        temporary = parent / (destination.filename().string()
                              + ".tmp." + std::to_string(attempt));
        filesystemError.clear();
        const bool exists = std::filesystem::exists(temporary, filesystemError);
        if (filesystemError) {
            error = "Não foi possível verificar o arquivo temporário: "
                    + filesystemError.message();
            return false;
        }
        if (!exists) {
            foundTemporaryName = true;
            break;
        }
    }

    if (!foundTemporaryName) {
        error = "Não foi possível reservar um nome de arquivo temporário.";
        return false;
    }

    {
        std::ofstream output(temporary, std::ios::binary | std::ios::out);
        if (!output.is_open()) {
            error = "Não foi possível criar o arquivo temporário: "
                    + temporary.string();
            return false;
        }

        output.write(contents.data(),
                     static_cast<std::streamsize>(contents.size()));
        output.flush();

        if (!output) {
            error = "Falha durante a gravação do arquivo temporário.";
            output.close();
            std::filesystem::remove(temporary, filesystemError);
            return false;
        }

        output.close();
        if (output.fail()) {
            error = "Falha ao fechar o arquivo temporário.";
            std::filesystem::remove(temporary, filesystemError);
            return false;
        }
    }

    if (!replaceFileAtomically(temporary, destination, error)) {
        std::filesystem::remove(temporary, filesystemError);
        return false;
    }

    return true;
}

bool collectInputFiles(const std::filesystem::path& input,
                       std::vector<std::filesystem::path>& files,
                       std::string& error)
{
    std::error_code filesystemError;
    if (!std::filesystem::exists(input, filesystemError) || filesystemError) {
        error = "Caminho de entrada inexistente ou inacessível: " + input.string();
        return false;
    }

    if (std::filesystem::is_regular_file(input, filesystemError) && !filesystemError) {
        if (input.extension() != ".jack") {
            error = "O arquivo de entrada deve possuir extensão .jack.";
            return false;
        }
        files.push_back(input);
        return true;
    }

    filesystemError.clear();
    if (!std::filesystem::is_directory(input, filesystemError) || filesystemError) {
        error = "A entrada deve ser um arquivo .jack ou um diretório.";
        return false;
    }

    std::filesystem::directory_iterator iterator(input, filesystemError);
    const std::filesystem::directory_iterator end;
    if (filesystemError) {
        error = "Não foi possível listar o diretório: "
                + filesystemError.message();
        return false;
    }

    while (iterator != end) {
        std::error_code entryError;
        if (iterator->is_regular_file(entryError) && !entryError
            && iterator->path().extension() == ".jack") {
            files.push_back(iterator->path());
        }

        iterator.increment(filesystemError);
        if (filesystemError) {
            error = "Falha durante a enumeração do diretório: "
                    + filesystemError.message();
            return false;
        }
    }

    std::sort(files.begin(), files.end(),
              [](const std::filesystem::path& left,
                 const std::filesystem::path& right) {
                  return left.filename().string() < right.filename().string();
              });

    if (files.empty()) {
        error = "Nenhum arquivo .jack foi encontrado no diretório.";
        return false;
    }
    return true;
}

const char* generationCategoryName(VMGenerationErrorCategory category)
{
    switch (category) {
    case VMGenerationErrorCategory::InvalidProgram:
        return "SEMÂNTICO";
    case VMGenerationErrorCategory::UnsupportedNode:
        return "TRADUÇÃO";
    case VMGenerationErrorCategory::OutputFailure:
        return "GRAVAÇÃO";
    }
    return "TRADUÇÃO";
}

bool compileOne(const std::filesystem::path& sourcePath,
                const std::filesystem::path& outputDirectory,
                std::vector<std::filesystem::path>& publishedTargets)
{
    std::string source;
    std::string readError;
    if (!readSource(sourcePath, source, readError)) {
        printDiagnostic("LEITURA", sourcePath, readError);
        return false;
    }

    std::vector<Token> tokens;
    std::string lexicalError;
    AnalisadorLexico lexer(source);
    if (!lexer.analisar(tokens, lexicalError)) {
        const DiagnosticLocation location = extractLexicalLocation(lexicalError);
        printDiagnostic("LÉXICO", sourcePath, lexicalError,
                        location.line, location.column);
        return false;
    }

    AnalisadorSintatico parser(tokens, source);
    SyntaxResult syntaxResult = parser.analisar();
    if (std::holds_alternative<SyntaxFailure>(syntaxResult)) {
        const SyntaxFailure& failure = std::get<SyntaxFailure>(syntaxResult);
        printDiagnostic("SINTÁTICO", sourcePath, failure.message,
                        failure.sourceSpan.startLine,
                        failure.sourceSpan.startColumn);
        return false;
    }

    SyntaxSuccess& syntaxSuccess = std::get<SyntaxSuccess>(syntaxResult);
    if (!syntaxSuccess.program) {
        printDiagnostic("SEMÂNTICO", sourcePath,
                        "O parser retornou um programa nulo.");
        return false;
    }

    const auto& classes = syntaxSuccess.program->obterClasses();
    if (classes.empty()) {
        printDiagnostic("SEMÂNTICO", sourcePath,
                        "O programa não contém declaração de classe.");
        return false;
    }

    bool fileSucceeded = true;
    for (const auto& classNode : classes) {
        if (!classNode) {
            printDiagnostic("SEMÂNTICO", sourcePath,
                            "O programa contém uma declaração de classe nula.");
            fileSucceeded = false;
            continue;
        }

        // A saída acompanha o nome do arquivo-fonte, convenção usada pelo
        // Project 11. Isso também permite processar diretórios de testes que
        // contenham classes homônimas em arquivos-fonte distintos.
        const std::filesystem::path destination =
            outputDirectory / (sourcePath.stem().string() + ".vm");

        if (std::find(publishedTargets.begin(), publishedTargets.end(),
                      destination) != publishedTargets.end()) {
            printDiagnostic("GRAVAÇÃO", sourcePath,
                            "Mais de uma classe produz o mesmo destino: "
                            + destination.string());
            fileSucceeded = false;
            continue;
        }

        std::ostringstream vmBuffer;
        VMCodeGenerator generator;
        const VMGenerationResult generation =
            generator.generate(*classNode, vmBuffer);

        if (!generation.success) {
            for (const VMGenerationDiagnostic& diagnostic : generation.diagnostics) {
                printDiagnostic(generationCategoryName(diagnostic.category),
                                sourcePath, diagnostic.message,
                                diagnostic.line, diagnostic.column);
            }
            fileSucceeded = false;
            continue;
        }

        std::string writeError;
        if (!publishOutput(destination, vmBuffer.str(), writeError)) {
            printDiagnostic("GRAVAÇÃO", destination, writeError);
            fileSucceeded = false;
            continue;
        }

        publishedTargets.push_back(destination);
        std::cout << "[OK] " << sourcePath.string()
                  << " -> " << destination.string() << '\n';
    }

    return fileSucceeded;
}

} // namespace

int CompiladorVMCLI::executar(int argc, char* argv[]) const
{
    CommandLineOptions options;
    bool showHelp = false;
    std::string argumentError;

    if (!parseArguments(argc, argv, options, showHelp, argumentError)) {
        std::cerr << "[USO] " << argumentError << '\n';
        printUsage(std::cerr);
        return 2;
    }

    if (showHelp) {
        printUsage(std::cout);
        return 0;
    }

    std::vector<std::filesystem::path> inputFiles;
    std::string inputError;
    if (!collectInputFiles(options.input, inputFiles, inputError)) {
        printDiagnostic("ENTRADA", options.input, inputError);
        return 2;
    }

    std::filesystem::path outputDirectory;
    if (options.outputDirectorySpecified) {
        outputDirectory = options.outputDirectory;
    } else if (inputFiles.size() == 1
               && std::filesystem::is_regular_file(options.input)) {
        outputDirectory = options.input.parent_path();
        if (outputDirectory.empty()) {
            outputDirectory = std::filesystem::current_path();
        }
    } else {
        outputDirectory = options.input;
    }

    std::error_code filesystemError;
    std::filesystem::create_directories(outputDirectory, filesystemError);
    if (filesystemError
        || !std::filesystem::is_directory(outputDirectory, filesystemError)
        || filesystemError) {
        printDiagnostic("GRAVAÇÃO", outputDirectory,
                        "Não foi possível preparar o diretório de saída."
                        + (filesystemError ? " " + filesystemError.message() : ""));
        return 3;
    }

    std::size_t succeeded = 0;
    std::size_t failed = 0;
    std::vector<std::filesystem::path> publishedTargets;

    for (const auto& inputFile : inputFiles) {
        if (compileOne(inputFile, outputDirectory, publishedTargets)) {
            ++succeeded;
        } else {
            ++failed;
        }
    }

    std::cout << "Resumo: " << succeeded << " arquivo(s) compilado(s), "
              << failed << " com falha.\n";

    return failed == 0 ? 0 : 1;
}

} // namespace jack::cli
