#include "jack/vm/vmwriter.h"

#include <cctype>
#include <stdexcept>
#include <sstream>
#include <string>

VMWriter::VMWriter(std::ostream& outputStream)
    : output(outputStream)
{
}

void VMWriter::writePush(const std::string& segment, int index)
{
    validateIndex(segment, index, true);

    std::ostringstream instruction;
    instruction << "push " << segment << ' ' << index;
    writeInstruction(instruction.str());
}

void VMWriter::writePop(const std::string& segment, int index)
{
    validateIndex(segment, index, false);

    std::ostringstream instruction;
    instruction << "pop " << segment << ' ' << index;
    writeInstruction(instruction.str());
}

void VMWriter::writeArithmetic(const std::string& command)
{
    static const char* const validCommands[] = {
        "add", "sub", "neg", "eq", "gt",
        "lt", "and", "or", "not"
    };

    bool valid = false;
    for (const char* candidate : validCommands) {
        if (command == candidate) {
            valid = true;
            break;
        }
    }

    if (!valid) {
        throw std::invalid_argument("Comando aritmetico VM invalido: " + command);
    }

    writeInstruction(command);
}

void VMWriter::writeLabel(const std::string& name)
{
    validateName(name, "rotulo");
    writeInstruction("label " + name);
}

void VMWriter::writeGoto(const std::string& name)
{
    validateName(name, "rotulo");
    writeInstruction("goto " + name);
}

void VMWriter::writeIf(const std::string& name)
{
    validateName(name, "rotulo");
    writeInstruction("if-goto " + name);
}

void VMWriter::writeCall(const std::string& name, int argumentCount)
{
    validateName(name, "nome de sub-rotina");
    if (argumentCount < 0) {
        throw std::invalid_argument(
            "A quantidade de argumentos nao pode ser negativa.");
    }

    std::ostringstream instruction;
    instruction << "call " << name << ' ' << argumentCount;
    writeInstruction(instruction.str());
}

void VMWriter::writeFunction(const std::string& name, int localCount)
{
    validateName(name, "nome de funcao");
    if (localCount < 0) {
        throw std::invalid_argument(
            "A quantidade de variaveis locais nao pode ser negativa.");
    }

    std::ostringstream instruction;
    instruction << "function " << name << ' ' << localCount;
    writeInstruction(instruction.str());
}

void VMWriter::writeReturn()
{
    writeInstruction("return");
}

void VMWriter::finalizar()
{
    if (finalized) {
        return;
    }

    if (failed || !output) {
        failed = true;
        throw std::ios_base::failure(
            "Nao foi possivel finalizar o VMWriter: fluxo de saida em falha.");
    }

    try {
        output.flush();
    } catch (...) {
        failed = true;
        throw;
    }

    if (!output) {
        failed = true;
        throw std::ios_base::failure(
            "Falha ao descarregar o fluxo de saida do VMWriter.");
    }

    finalized = true;
}

bool VMWriter::finalizado() const noexcept
{
    return finalized;
}

void VMWriter::ensureWritable()
{
    if (finalized) {
        throw std::logic_error(
            "Nao e permitido emitir instrucoes apos finalizar o VMWriter.");
    }

    if (failed || !output) {
        failed = true;
        throw std::ios_base::failure(
            "O fluxo de saida do VMWriter esta em estado de falha.");
    }
}

void VMWriter::writeInstruction(const std::string& instruction)
{
    ensureWritable();

    const std::string line = instruction + '\n';

    try {
        output.write(line.data(),
                     static_cast<std::streamsize>(line.size()));
    } catch (...) {
        failed = true;
        throw;
    }

    if (!output) {
        failed = true;
        throw std::ios_base::failure(
            "Falha ao escrever instrucao VM no fluxo de saida.");
    }
}

void VMWriter::validateName(const std::string& name,
                            const char* description)
{
    if (name.empty()) {
        throw std::invalid_argument(
            std::string("O ") + description + " nao pode ser vazio.");
    }

    for (unsigned char character : name) {
        if (std::isspace(character) || character < 0x20 || character == 0x7f) {
            throw std::invalid_argument(
                std::string("O ") + description
                + " nao pode conter espacos ou caracteres de controle.");
        }
    }
}

void VMWriter::validateIndex(const std::string& segment,
                             int index,
                             bool isPush)
{
    if (index < 0) {
        throw std::invalid_argument("O indice do segmento VM nao pode ser negativo.");
    }

    if (segment == "constant") {
        if (!isPush) {
            throw std::invalid_argument("pop constant nao e uma instrucao VM valida.");
        }
        if (index > 32767) {
            throw std::invalid_argument(
                "O indice de constant deve estar entre 0 e 32767.");
        }
        return;
    }

    if (segment == "pointer") {
        if (index > 1) {
            throw std::invalid_argument(
                "O indice de pointer deve ser 0 ou 1.");
        }
        return;
    }

    if (segment == "temp") {
        if (index > 7) {
            throw std::invalid_argument(
                "O indice de temp deve estar entre 0 e 7.");
        }
        return;
    }

    if (segment == "static") {
        if (index > 239) {
            throw std::invalid_argument(
                "O indice de static deve estar entre 0 e 239.");
        }
        return;
    }

    if (segment == "local" || segment == "argument"
        || segment == "this" || segment == "that") {
        return;
    }

    throw std::invalid_argument("Segmento VM invalido: " + segment);
}
