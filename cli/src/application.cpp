#include "jack/cli/application.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <variant>
#include <utility>
#include <filesystem>
#include <fstream>

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

    const std::string nomeSugerido =
        caminhoOriginal.parent_path().string() +
        "/" +
        caminhoOriginal.stem().string() +
        ".xml";

    terminal.exibirInformacao(
        "Destino sugerido: " + nomeSugerido
        );

    std::string caminhoDestino = terminal.lerTexto(
        "Informe o caminho de destino (ENTER para utilizar o sugerido): "
        );

    if (caminhoDestino.empty())
        caminhoDestino = nomeSugerido;

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
        (caminhoOriginal.stem().string() + ".xml");

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

void Application::analisarCompleto()
{
    terminal.exibirTitulo("Análise Completa");

    if (!analisarLexico())
        return;

    analisarSintatico();
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
        terminal.exibirInformacao(
            "Execução de testes ainda não implementada."
            );
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
