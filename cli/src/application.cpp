#include "jack/cli/application.h"

#include <vector>

namespace jack::cli {

Application::Application()
    : terminal(),
      menu(terminal)
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


void Application::processarOpcao(int opcao)
{
    switch (opcao)
    {
    case 1:
        terminal.exibirInformacao(
            "Funcionalidade de seleção de arquivo ainda não implementada."
            );
        break;

    case 2:
        terminal.exibirInformacao(
            "Funcionalidade de exibição de arquivo ainda não implementada."
            );
        break;

    case 3:
        terminal.exibirInformacao(
            "Análise léxica ainda não implementada."
            );
        break;

    case 4:
        terminal.exibirInformacao(
            "Análise sintática ainda não implementada."
            );
        break;

    case 5:
        terminal.exibirInformacao(
            "Análise completa ainda não implementada."
            );
        break;

    case 6:
        terminal.exibirInformacao(
            "Exportação do XML léxico ainda não implementada."
            );
        break;

    case 7:
        terminal.exibirInformacao(
            "Exportação do XML sintático ainda não implementada."
            );
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
