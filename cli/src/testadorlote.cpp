#include "jack/cli/testadorlote.h"
#include "jack/lexer/analisadorlexico.h"
#include "jack/parser/analisadorsintatico.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>
#include <utility>
#include <variant>
#include <vector>

namespace jack::cli {

ResultadoLote TestadorLote::executar(const std::string& diretorio) const
{
    ResultadoLote resultado;
    resultado.diretorio = diretorio;

    const std::filesystem::path caminho(diretorio);

    // Validação do caminho absoluto.
    if (!caminho.is_absolute()) {
        resultado.mensagemErro = "O caminho informado deve ser absoluto.";
        return resultado;
    }

    std::error_code erro;

    // Verifica se o caminho existe.
    if (!std::filesystem::exists(caminho, erro) || erro) {
        resultado.mensagemErro = "O diretório informado não existe ou não pode ser acessado.";
        return resultado;
    }

    // Verifica se o caminho corresponde a um diretório.
    if (!std::filesystem::is_directory(caminho, erro) || erro) {
        resultado.mensagemErro = "O caminho informado não corresponde a um diretório.";
        return resultado;
    }

    // Localiza arquivos .jack diretamente no diretório.
    std::vector<std::filesystem::path> arquivosJack;

    std::filesystem::directory_iterator iterador(caminho, erro);
    const std::filesystem::directory_iterator fim;

    if (erro) {
        resultado.mensagemErro = "Não foi possível acessar o conteúdo do diretório.";
        return resultado;
    }

    while (iterador != fim) {
        std::error_code erroEntrada;

        if (iterador->is_regular_file(erroEntrada) && !erroEntrada &&
            iterador->path().extension() == ".jack") {
            arquivosJack.push_back(iterador->path());
        }

        iterador.increment(erro);

        if (erro) {
            resultado.mensagemErro = "Ocorreu um erro durante a leitura do diretório.";
            return resultado;
        }
    }

    // Ordena os arquivos pelo nome.
    std::sort(
        arquivosJack.begin(),
        arquivosJack.end(),
        [](const std::filesystem::path& a,
           const std::filesystem::path& b) {
            return a.filename().string() < b.filename().string();
        }
        );

    resultado.totalArquivosEncontrados = arquivosJack.size();

    if (arquivosJack.empty()) {
        resultado.mensagemErro = "Nenhum arquivo com extensão .jack foi encontrado no diretório.";
        return resultado;
    }

    // Processa a leitura individual dos arquivos.
    for (const auto& caminhoArquivo : arquivosJack) {
        ResultadoArquivo arquivo;

        arquivo.nomeArquivo = caminhoArquivo.filename().string();
        arquivo.caminhoArquivo = caminhoArquivo.string();

        std::ifstream entrada(caminhoArquivo, std::ios::binary);

        if (!entrada.is_open()) {
            arquivo.categoria = CategoriaResultado::ErroLeitura;
            arquivo.mensagemErroLeitura = "Não foi possível abrir o arquivo.";

            resultado.totalErroLeitura++;
            resultado.arquivos.push_back(std::move(arquivo));
            continue;
        }

        std::ostringstream conteudo;
        conteudo << entrada.rdbuf();

        if (entrada.bad()) {
            arquivo.categoria = CategoriaResultado::ErroLeitura;
            arquivo.mensagemErroLeitura = "Ocorreu um erro durante a leitura do arquivo.";

            resultado.totalErroLeitura++;
            resultado.arquivos.push_back(std::move(arquivo));
            continue;
        }

        arquivo.leituraConcluida = true;
        resultado.totalArquivosProcessados++;

        const std::string codigoFonte = conteudo.str();

        // Análise léxica.
        std::vector<Token> tokens;
        std::string erroLexico;

        AnalisadorLexico analisadorLexico(codigoFonte);

        arquivo.analiseLexicaExecutada = true;

        if (!analisadorLexico.analisar(tokens, erroLexico)) {
            arquivo.categoria = CategoriaResultado::ErroLexico;
            arquivo.analiseLexicaValida = false;

            arquivo.quantidadeErrosLexicos = 1;
            arquivo.mensagemErroLexico = erroLexico;

            resultado.totalErroLexico++;
            resultado.totalErrosLexicos++;

            resultado.arquivos.push_back(std::move(arquivo));
            continue;
        }

        arquivo.analiseLexicaValida = true;
        arquivo.quantidadeTokens = tokens.size();
        arquivo.quantidadeTokensDisponivel = true;

        resultado.totalTokens += tokens.size();

        // Análise sintática.
        AnalisadorSintatico analisadorSintatico(tokens, codigoFonte);

        arquivo.analiseSintaticaExecutada = true;

        SyntaxResult resultadoSintatico = analisadorSintatico.analisar();

        if (std::holds_alternative<SyntaxFailure>(resultadoSintatico)) {
            const auto& falha = std::get<SyntaxFailure>(resultadoSintatico);

            arquivo.categoria = CategoriaResultado::ErroSintatico;
            arquivo.analiseSintaticaValida = false;

            arquivo.quantidadeErrosSintaticos = 1;
            arquivo.mensagemErroSintatico = falha.message;

            arquivo.linhaErroSintatico = falha.sourceSpan.startLine;
            arquivo.colunaErroSintatico = falha.sourceSpan.startColumn;

            resultado.totalErroSintatico++;
            resultado.totalErrosSintaticos++;
        } else {
            arquivo.categoria = CategoriaResultado::Sucesso;
            arquivo.analiseSintaticaValida = true;

            resultado.totalSucesso++;
        }

        resultado.arquivos.push_back(std::move(arquivo));
    }

    return resultado;
}

} // namespace jack::cli