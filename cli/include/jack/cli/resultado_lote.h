#ifndef JACK_CLI_RESULTADO_LOTE_H
#define JACK_CLI_RESULTADO_LOTE_H

#include <cstddef>
#include <string>
#include <vector>

namespace jack::cli {

enum class CategoriaResultado {
    Sucesso,
    ErroLexico,
    ErroSintatico,
    ErroLeitura
};

struct ResultadoArquivo {
    std::string nomeArquivo;
    std::string caminhoArquivo;

    CategoriaResultado categoria;

    bool leituraConcluida = false;
    bool analiseLexicaExecutada = false;
    bool analiseLexicaValida = false;
    bool analiseSintaticaExecutada = false;
    bool analiseSintaticaValida = false;

    std::size_t quantidadeTokens = 0;
    bool quantidadeTokensDisponivel = false;

    std::size_t quantidadeErrosLexicos = 0;
    std::size_t quantidadeErrosSintaticos = 0;

    std::string mensagemErroLeitura;
    std::string mensagemErroLexico;
    std::string mensagemErroSintatico;

    std::size_t linhaErroLexico = 0;
    std::size_t colunaErroLexico = 0;

    std::size_t linhaErroSintatico = 0;
    std::size_t colunaErroSintatico = 0;

};

struct ResultadoLote {
    std::string diretorio;
    std::vector<ResultadoArquivo> arquivos;

    std::size_t totalArquivosEncontrados = 0;
    std::size_t totalArquivosProcessados = 0;

    std::size_t totalSucesso = 0;
    std::size_t totalErroLexico = 0;
    std::size_t totalErroSintatico = 0;
    std::size_t totalErroLeitura = 0;

    std::size_t totalTokens = 0;
    std::size_t totalErrosLexicos = 0;
    std::size_t totalErrosSintaticos = 0;

    std::string mensagemErro;
};

} // namespace jack::cli

#endif // JACK_CLI_RESULTADO_LOTE_H