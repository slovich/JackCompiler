#include "jack/cli/application.h"
#include "jack/xml/comparadorxml.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <variant>
#include <utility>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace jack::cli {

Application::Application()
    : terminal(),
      menu(terminal),
      arquivoCarregado(false),
      analiseLexicaValida(false),
      analiseSintaticaValida(false)
{
}

int Application::exibirMenuPrincipal()
{
    const std::vector<Menu::Opcao> opcoes = {
        {1, "Selecionar arquivo Jack"},
        {2, "Exibir arquivo selecionado"},
        {3, "Análise léxica"},
        {4, "Análise sintática"},
        {5, "Análise completa"},
        {6, "Exportar XML léxico"},
        {7, "Exportar XML sintático"},
        {8, "Executar testes"},
        {9, "Testar arquivos Jack em lote"},
        {10, "Comparar XML léxico"},
        {11, "Comparar XML sintático"},
        {0, "Sair"}
    };

    return menu.exibirMenu("Jack Compiler - Menu Principal", opcoes);
}

void Application::selecionarArquivo()
{
    terminal.exibirTitulo("Selecionar arquivo Jack");

    const std::string caminho = terminal.lerTexto(
        "Informe o caminho do arquivo: "
        );

    if (caminho.empty())
    {
        terminal.exibirAviso("Nenhum caminho foi informado.");
        return;
    }

    std::ifstream arquivo(caminho);

    if (!arquivo.is_open())
    {
        terminal.exibirErro("Não foi possível abrir o arquivo informado.");
        return;
    }

    std::stringstream buffer;
    buffer << arquivo.rdbuf();

    if (arquivo.bad())
    {
        terminal.exibirErro("Ocorreu um erro durante a leitura do arquivo.");
        return;
    }

    caminhoArquivo = caminho;
    codigoFonte = buffer.str();
    arquivoCarregado = true;

    terminal.exibirSucesso("Arquivo carregado com sucesso.");
    terminal.exibirInformacao("Arquivo: " + caminhoArquivo);
}

void Application::exibirArquivoSelecionado() const
{
    terminal.exibirTitulo("Arquivo selecionado");

    if (!arquivoCarregado)
    {
        terminal.exibirAviso("Nenhum arquivo foi selecionado.");
        return;
    }

    terminal.exibirInformacao("Arquivo: " + caminhoArquivo);
    terminal.exibirSeparador();

    terminal.exibirTexto(codigoFonte);

    terminal.exibirSeparador();
}

bool Application::analisarLexico()
{
    terminal.exibirTitulo("Análise Léxica");

    if (!arquivoCarregado)
    {
        terminal.exibirAviso("Nenhum arquivo foi selecionado.");
        return false;
    }

    std::vector<Token> novosTokens;
    std::string erro;

    AnalisadorLexico analisador(codigoFonte);

    if (!analisador.analisar(novosTokens, erro))
    {
        terminal.exibirErro("Falha na análise léxica.");
        terminal.exibirInformacao(erro);
        return false;
    }

    tokens = std::move(novosTokens);
    analiseLexicaValida = true;

    terminal.exibirSucesso("Análise léxica concluída com sucesso.");

    terminal.exibirInformacao(
        "Quantidade de tokens: " + std::to_string(tokens.size())
        );

    terminal.exibirSeparador();

    for (const Token& token : tokens)
    {
        terminal.exibirTexto(
            "Linha: " + std::to_string(token.linha) +
            " | Coluna: " + std::to_string(token.coluna) +
            " | Lexema: " + token.lexema + "\n"
            );
    }

    terminal.exibirSeparador();
    return true;
}

void Application::exportarXmlLexico()
{
    terminal.exibirTitulo("Exportar XML Léxico");

    if (!analiseLexicaValida)
    {
        terminal.exibirAviso(
            "Não existe uma análise léxica válida para exportação."
            );
        return;
    }

    const std::filesystem::path caminhoOriginal(caminhoArquivo);

    const std::filesystem::path caminhoSugerido =
        caminhoOriginal.parent_path() /
        (caminhoOriginal.stem().string() + "_lexico.xml");

    terminal.exibirInformacao(
        "Destino sugerido: " + caminhoSugerido.string()
        );

    std::string caminhoDestino = terminal.lerTexto(
        "Informe o caminho de destino (ENTER para utilizar o sugerido): "
        );

    if (caminhoDestino.empty())
        caminhoDestino = caminhoSugerido.string();

    std::filesystem::path caminhoXml(caminhoDestino);

    if (caminhoXml.extension() != ".xml")
        caminhoXml += ".xml";

    GeradorXmlLexico gerador;
    const std::string xml = gerador.gerar(tokens);

    std::ofstream arquivoXml(
        caminhoXml,
        std::ios::out | std::ios::binary
        );

    if (!arquivoXml.is_open())
    {
        terminal.exibirErro(
            "Não foi possível criar o arquivo XML: " +
            caminhoXml.string()
            );
        return;
    }

    arquivoXml << xml;
    arquivoXml.close();

    if (arquivoXml.fail())
    {
        terminal.exibirErro(
            "Ocorreu um erro durante a gravação do XML."
            );
        return;
    }

    terminal.exibirSucesso("XML léxico exportado com sucesso.");

    terminal.exibirInformacao(
        "Arquivo gerado: " + caminhoXml.string()
        );
}

bool Application::analisarSintatico()
{
    terminal.exibirTitulo("Análise Sintática");

    if (!arquivoCarregado)
    {
        terminal.exibirAviso("Nenhum arquivo foi selecionado.");
        return false;
    }

    if (!analiseLexicaValida)
    {
        terminal.exibirAviso(
            "Execute primeiro uma análise léxica bem-sucedida."
            );
        return false;
    }

    AnalisadorSintatico analisador(tokens, codigoFonte);

    SyntaxResult resultado = analisador.analisar();

    if (std::holds_alternative<SyntaxFailure>(resultado))
    {
        const SyntaxFailure& falha =
            std::get<SyntaxFailure>(resultado);

        programa.reset();
        analiseSintaticaValida = false;

        terminal.exibirErro("Falha na análise sintática.");
        terminal.exibirInformacao(falha.message);

        terminal.exibirInformacao(
            "Linha: " + std::to_string(falha.sourceSpan.startLine) +
            " | Coluna: " + std::to_string(falha.sourceSpan.startColumn)
            );

        return false;
    }

    SyntaxSuccess& sucesso = std::get<SyntaxSuccess>(resultado);

    programa = std::move(sucesso.program);
    analiseSintaticaValida = true;

    terminal.exibirSucesso(
        "Análise sintática concluída com sucesso."
        );

    terminal.exibirInformacao(
        "A estrutura do programa foi reconhecida corretamente."
        );
    return true;
}

void Application::exportarXmlSintatico()
{
    terminal.exibirTitulo("Exportar XML Sintático");

    if (!analiseSintaticaValida || !programa)
    {
        terminal.exibirAviso(
            "Não existe uma análise sintática válida para exportação."
            );
        return;
    }

    const std::filesystem::path caminhoOriginal(caminhoArquivo);

    const std::filesystem::path caminhoSugerido =
        caminhoOriginal.parent_path() /
        (caminhoOriginal.stem().string() + "_sintatico.xml");

    terminal.exibirInformacao(
        "Destino sugerido: " + caminhoSugerido.string()
        );

    std::string caminhoDestino = terminal.lerTexto(
        "Informe o caminho de destino (ENTER para utilizar o sugerido): "
        );

    if (caminhoDestino.empty())
        caminhoDestino = caminhoSugerido.string();

    std::filesystem::path caminhoXml(caminhoDestino);

    if (caminhoXml.extension() != ".xml")
        caminhoXml += ".xml";

    GeradorXmlSintatico gerador;
    const std::string xml = gerador.gerar(*programa);

    std::ofstream arquivoXml(
        caminhoXml,
        std::ios::out | std::ios::binary
        );

    if (!arquivoXml.is_open())
    {
        terminal.exibirErro(
            "Não foi possível criar o arquivo XML: " +
            caminhoXml.string()
            );
        return;
    }

    arquivoXml << xml;
    arquivoXml.close();

    if (arquivoXml.fail())
    {
        terminal.exibirErro(
            "Ocorreu um erro durante a gravação do XML."
            );
        return;
    }

    terminal.exibirSucesso("XML sintático exportado com sucesso.");

    terminal.exibirInformacao(
        "Arquivo gerado: " + caminhoXml.string()
        );
}

void Application::executarTestes()
{
    terminal.exibirTitulo("Execução de Testes");

    const std::filesystem::path raizProjeto(JACK_PROJECT_ROOT);

    const std::filesystem::path diretorioPadrao =
        raizProjeto / "tests" / "inputs";

    terminal.exibirInformacao(
        "Diretório padrão: " + diretorioPadrao.string()
        );

    std::string caminhoInformado = terminal.lerTexto(
        "Informe outro diretório ou pressione ENTER para utilizar o padrão: "
        );

    std::filesystem::path diretorioTestes;

    if (caminhoInformado.empty())
    {
        diretorioTestes = diretorioPadrao;
    }
    else
    {
        diretorioTestes = caminhoInformado;

        if (diretorioTestes.is_relative())
            diretorioTestes = raizProjeto / diretorioTestes;
    }

    if (!std::filesystem::exists(diretorioTestes))
    {
        terminal.exibirErro(
            "O diretório informado não existe: " +
            diretorioTestes.string()
            );
        return;
    }

    if (!std::filesystem::is_directory(diretorioTestes))
    {
        terminal.exibirErro(
            "O caminho informado não corresponde a um diretório."
            );
        return;
    }

    const std::filesystem::path diretorioValidos =
        diretorioTestes / "validos";

    const std::filesystem::path diretorioInvalidos =
        diretorioTestes / "invalidos";

    if (!std::filesystem::exists(diretorioValidos) ||
        !std::filesystem::is_directory(diretorioValidos))
    {
        terminal.exibirErro(
            "O subdiretório 'validos' não foi encontrado."
            );
        return;
    }

    if (!std::filesystem::exists(diretorioInvalidos) ||
        !std::filesystem::is_directory(diretorioInvalidos))
    {
        terminal.exibirErro(
            "O subdiretório 'invalidos' não foi encontrado."
            );
        return;
    }

    std::vector<std::filesystem::path> arquivosValidos;
    std::vector<std::filesystem::path> arquivosInvalidos;

    for (const auto& entrada :
         std::filesystem::directory_iterator(diretorioValidos))
    {
        if (entrada.is_regular_file() &&
            entrada.path().extension() == ".jack")
        {
            arquivosValidos.push_back(entrada.path());
        }
    }

    for (const auto& entrada :
         std::filesystem::directory_iterator(diretorioInvalidos))
    {
        if (entrada.is_regular_file() &&
            entrada.path().extension() == ".jack")
        {
            arquivosInvalidos.push_back(entrada.path());
        }
    }

    if (arquivosValidos.empty())
    {
        terminal.exibirErro(
            "O diretório 'validos' não contém arquivos Jack."
            );
        return;
    }

    if (arquivosInvalidos.empty())
    {
        terminal.exibirErro(
            "O diretório 'invalidos' não contém arquivos Jack."
            );
        return;
    }

    std::sort(arquivosValidos.begin(), arquivosValidos.end());
    std::sort(arquivosInvalidos.begin(), arquivosInvalidos.end());

    terminal.exibirSucesso("Diretório de testes validado.");

    terminal.exibirInformacao(
        "Diretório: " + diretorioTestes.string()
        );

    terminal.exibirInformacao(
        "Testes válidos encontrados: " +
        std::to_string(arquivosValidos.size())
        );

    terminal.exibirInformacao(
        "Testes inválidos encontrados: " +
        std::to_string(arquivosInvalidos.size())
        );

    int testesAprovados = 0;
    int testesReprovados = 0;
    int errosExecucao = 0;

    auto executarTeste = [&](const std::filesystem::path& caminho,
                             bool esperadoValido)
    {
        terminal.exibirSeparador();
        terminal.exibirInformacao(caminho.filename().string());

        std::ifstream arquivo(caminho);

        if (!arquivo.is_open())
        {
            terminal.exibirErro("Não foi possível abrir o arquivo.");
            ++errosExecucao;
            return;
        }

        std::stringstream buffer;
        buffer << arquivo.rdbuf();

        if (arquivo.bad())
        {
            terminal.exibirErro("Erro durante a leitura do arquivo.");
            ++errosExecucao;
            return;
        }

        const std::string codigoFonte = buffer.str();

        AnalisadorLexico analisadorLexico(codigoFonte);

        std::vector<Token> tokens;
        std::string erroLexico;

        const bool lexicoValido =
            analisadorLexico.analisar(tokens, erroLexico);

        bool sintaticoValido = false;
        std::string erroSintatico;
        SourceSpan localErroSintatico{};

        if (lexicoValido)
        {
            AnalisadorSintatico analisadorSintatico(tokens, codigoFonte);

            SyntaxResult resultado = analisadorSintatico.analisar();

            if (std::holds_alternative<SyntaxSuccess>(resultado))
            {
                sintaticoValido = true;
            }
            else
            {
                const SyntaxFailure& falha =
                    std::get<SyntaxFailure>(resultado);

                erroSintatico = falha.message;
                localErroSintatico = falha.sourceSpan;
            }
        }

        const bool resultadoEsperado =
            esperadoValido
                ? lexicoValido && sintaticoValido
                : !lexicoValido || (lexicoValido && !sintaticoValido);

        if (resultadoEsperado)
        {
            terminal.exibirSucesso("APROVADO");
            ++testesAprovados;
        }
        else
        {
            terminal.exibirErro("REPROVADO");
            ++testesReprovados;
        }

        if (!lexicoValido)
        {
            terminal.exibirInformacao("Resultado: falha léxica.");
            terminal.exibirInformacao(erroLexico);
        }
        else if (!sintaticoValido)
        {
            terminal.exibirInformacao("Resultado: falha sintática.");
            terminal.exibirInformacao(erroSintatico);

            terminal.exibirInformacao(
                "Linha: " +
                std::to_string(localErroSintatico.startLine) +
                " | Coluna: " +
                std::to_string(localErroSintatico.startColumn)
                );
        }
        else
        {
            terminal.exibirInformacao(
                "Análises léxica e sintática concluídas com sucesso."
                );
        }
    };

    terminal.exibirTitulo("Testes de Arquivos Válidos");

    for (const auto& caminho : arquivosValidos)
        executarTeste(caminho, true);

    terminal.exibirTitulo("Testes de Arquivos Inválidos");

    for (const auto& caminho : arquivosInvalidos)
        executarTeste(caminho, false);

    terminal.exibirSeparador();
    terminal.exibirTitulo("Resumo da Execução");

    terminal.exibirInformacao(
        "Total de testes: " +
        std::to_string(arquivosValidos.size() + arquivosInvalidos.size())
        );

    terminal.exibirSucesso(
        "Testes aprovados: " + std::to_string(testesAprovados)
        );

    terminal.exibirErro(
        "Testes reprovados: " + std::to_string(testesReprovados)
        );

    terminal.exibirAviso(
        "Erros de execução: " + std::to_string(errosExecucao)
        );
}


void Application::testarArquivosEmLote()
{
    terminal.exibirTitulo("Teste de Arquivos Jack em Lote");

    const std::string caminho = terminal.lerTexto(
        "Informe o caminho absoluto do diretório: "
        );

    if (caminho.empty())
    {
        terminal.exibirAviso("Nenhum caminho foi informado.");
        return;
    }

    TestadorLote testador;
    ResultadoLote resultado = testador.executar(caminho);

    if (!resultado.mensagemErro.empty())
    {
        terminal.exibirErro(resultado.mensagemErro);
        return;
    }

    terminal.exibirSucesso("Processamento em lote concluído.");

    terminal.exibirInformacao(
        "Diretório: " + resultado.diretorio
        );

    // Resultados individuais.
    terminal.exibirTitulo("Resultados Individuais");

    for (const ResultadoArquivo& arquivo : resultado.arquivos)
    {
        terminal.exibirSeparador();

        terminal.exibirInformacao(
            "Arquivo: " + arquivo.nomeArquivo
            );

        if (arquivo.categoria == CategoriaResultado::ErroLeitura)
        {
            terminal.exibirErro("Falha na leitura do arquivo.");
            terminal.exibirInformacao(arquivo.mensagemErroLeitura);
            continue;
        }

        terminal.exibirSucesso("Leitura concluída.");

        if (arquivo.categoria == CategoriaResultado::ErroLexico)
        {
            terminal.exibirErro("Análise léxica reprovada.");
            terminal.exibirInformacao(arquivo.mensagemErroLexico);
            terminal.exibirAviso("Análise sintática não executada.");
            terminal.exibirAviso(
                "O lexer interrompeu no primeiro erro léxico identificado; "
                "outros possíveis erros não foram verificados."
                );

            continue;
        }

        terminal.exibirSucesso("Análise léxica aprovada.");

        terminal.exibirInformacao(
            "Quantidade de tokens: " +
            std::to_string(arquivo.quantidadeTokens)
            );

        if (arquivo.categoria == CategoriaResultado::ErroSintatico)
        {
            terminal.exibirErro("Análise sintática reprovada.");

            terminal.exibirInformacao(
                arquivo.mensagemErroSintatico
                );

            terminal.exibirInformacao(
                "Linha: " +
                std::to_string(arquivo.linhaErroSintatico) +
                " | Coluna: " +
                std::to_string(arquivo.colunaErroSintatico)
                );

            terminal.exibirAviso(
                "Somente o primeiro erro sintático identificado é apresentado."
                );

            continue;
        }

        terminal.exibirSucesso("Análise sintática aprovada.");
    }

    // Resumo geral.
    terminal.exibirSeparador();
    terminal.exibirTitulo("Resumo do Processamento em Lote");

    terminal.exibirInformacao(
        "Arquivos encontrados: " +
        std::to_string(resultado.totalArquivosEncontrados)
        );

    terminal.exibirInformacao(
        "Arquivos processados: " +
        std::to_string(resultado.totalArquivosProcessados)
        );

    terminal.exibirSucesso(
        "Sucessos: " + std::to_string(resultado.totalSucesso)
        );

    terminal.exibirErro(
        "Erros léxicos: " + std::to_string(resultado.totalErroLexico)
        );

    terminal.exibirErro(
        "Erros sintáticos: " + std::to_string(resultado.totalErroSintatico)
        );

    terminal.exibirErro(
        "Erros de leitura: " + std::to_string(resultado.totalErroLeitura)
        );

    terminal.exibirInformacao(
        "Total de tokens: " + std::to_string(resultado.totalTokens)
        );

    terminal.exibirInformacao(
        "Ocorrências de erros léxicos: " +
        std::to_string(resultado.totalErrosLexicos)
        );

    terminal.exibirInformacao(
        "Ocorrências de erros sintáticos: " +
        std::to_string(resultado.totalErrosSintaticos)
        );
}


void Application::analisarCompleto()
{
    terminal.exibirTitulo("Análise Completa");

    if (!analisarLexico())
        return;

    analisarSintatico();
}


void Application::compararXml(bool lexico)
{
    using jack::xml::ComparadorXml;
    using jack::xml::XMLComparisonStatus;
    using jack::xml::XMLDocumentType;

    const XMLDocumentType tipo = lexico
        ? XMLDocumentType::Lexical
        : XMLDocumentType::Syntax;
    const std::string modalidade = lexico ? "léxico" : "sintático";

    terminal.exibirTitulo("Comparação de XML " + modalidade);

    const std::string caminhoReferenciaInformado = terminal.lerTexto(
        "Informe o caminho do XML de referência: "
        );
    if (caminhoReferenciaInformado.empty()) {
        terminal.exibirAviso("Caminho do XML de referência não informado.");
        return;
    }

    const std::string caminhoGeradoInformado = terminal.lerTexto(
        "Informe o caminho do XML gerado: "
        );
    if (caminhoGeradoInformado.empty()) {
        terminal.exibirAviso("Caminho do XML gerado não informado.");
        return;
    }

    const std::filesystem::path caminhoReferencia(
        caminhoReferenciaInformado);
    const std::filesystem::path caminhoGerado(caminhoGeradoInformado);

    auto lerArquivoXml = [this](const std::filesystem::path& caminho,
                                const std::string& descricao,
                                std::string& conteudo) {
        std::error_code erroFilesystem;

        const bool existe = std::filesystem::exists(caminho, erroFilesystem);
        if (erroFilesystem) {
            terminal.exibirErro(
                "Não foi possível verificar o arquivo " + descricao
                + ": " + erroFilesystem.message());
            return false;
        }
        if (!existe) {
            terminal.exibirErro(
                "Arquivo " + descricao + " não encontrado: "
                + caminho.string());
            return false;
        }

        const bool arquivoRegular =
            std::filesystem::is_regular_file(caminho, erroFilesystem);
        if (erroFilesystem) {
            terminal.exibirErro(
                "Falha ao verificar o arquivo " + descricao + ": "
                + erroFilesystem.message());
            return false;
        }
        if (!arquivoRegular) {
            terminal.exibirErro(
                "O caminho informado para " + descricao
                + " não corresponde a um arquivo regular: "
                + caminho.string());
            return false;
        }

        std::ifstream arquivo(caminho, std::ios::in | std::ios::binary);
        if (!arquivo.is_open()) {
            terminal.exibirErro(
                "Não foi possível abrir o arquivo " + descricao + ": "
                + caminho.string()
                + ". Verifique o caminho e as permissões de leitura.");
            return false;
        }

        std::ostringstream buffer;
        buffer << arquivo.rdbuf();

        if (arquivo.bad()) {
            terminal.exibirErro(
                "Ocorreu uma falha durante a leitura do arquivo "
                + descricao + ": " + caminho.string());
            return false;
        }

        conteudo = buffer.str();
        return true;
    };

    std::string xmlReferencia;
    std::string xmlGerado;

    if (!lerArquivoXml(caminhoReferencia, "de referência", xmlReferencia))
        return;

    if (!lerArquivoXml(caminhoGerado, "gerado", xmlGerado))
        return;

    terminal.exibirInformacao("Iniciando comparação...");
    terminal.exibirInformacao(
        "Referência: " + caminhoReferencia.string());
    terminal.exibirInformacao(
        "Gerado: " + caminhoGerado.string());

    const ComparadorXml comparador;
    const auto resultado = comparador.comparar(
        xmlReferencia,
        xmlGerado,
        tipo,
        ComparadorXml::MaxDifferencesDefault
        );

    terminal.exibirSeparador();

    if (resultado.status == XMLComparisonStatus::Equivalent) {
        terminal.exibirSucesso("Os documentos XML são equivalentes.");
        return;
    }

    if (resultado.status == XMLComparisonStatus::Different) {
        terminal.exibirErro("Os documentos XML apresentam divergências.");
        terminal.exibirInformacao(
            "Quantidade total de divergências: "
            + std::to_string(resultado.totalDifferences));

        for (const auto& diferenca : resultado.differences) {
            terminal.exibirInformacao(
                diferenca.path + " — " + diferenca.description);
            terminal.exibirInformacao(
                "  Referência: " + diferenca.expected);
            terminal.exibirInformacao(
                "  Gerado: " + diferenca.actual);
        }

        if (resultado.totalDifferences > resultado.differences.size()) {
            terminal.exibirAviso(
                "Exibição limitada a "
                + std::to_string(resultado.differences.size())
                + " divergências; o total foi contabilizado.");
        }
        return;
    }

    terminal.exibirErro(resultado.message);

    if (resultado.expectedErrorLine != 0) {
        terminal.exibirInformacao(
            "Erro no XML de referência — linha "
            + std::to_string(resultado.expectedErrorLine)
            + ", coluna "
            + std::to_string(resultado.expectedErrorColumn));
    }

    if (resultado.actualErrorLine != 0) {
        terminal.exibirInformacao(
            "Erro no XML gerado — linha "
            + std::to_string(resultado.actualErrorLine)
            + ", coluna "
            + std::to_string(resultado.actualErrorColumn));
    }
}

void Application::processarOpcao(int opcao)
{
    switch (opcao)
    {
    case 1:
        selecionarArquivo();
        break;

    case 2:
        exibirArquivoSelecionado();
        break;

    case 3:
        analisarLexico();
        break;

    case 4:
        analisarSintatico();
        break;

    case 5:
        analisarCompleto();
        break;

    case 6:
        exportarXmlLexico();
        break;

    case 7:
        exportarXmlSintatico();
        break;

    case 8:
        executarTestes();
        break;

    case 9:
        testarArquivosEmLote();
        break;

    case 10:
        compararXml(true);
        break;

    case 11:
        compararXml(false);
        break;

    }

    terminal.lerTexto("Pressione ENTER para continuar...");
}


void Application::executar()
{
    int opcao;

    do
    {
        opcao = exibirMenuPrincipal();

        if (opcao != 0)
            processarOpcao(opcao);

    } while (opcao != 0);

    terminal.limparTela();
    terminal.exibirSucesso("Jack Compiler encerrado.");
}

} // namespace jack::cli
