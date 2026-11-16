#include "jack/cli/terminal.h"

#include <iostream>
#include <string>

int main()
{
    jack::cli::Terminal terminal;

    terminal.limparTela();
    terminal.exibirTitulo("Jack Compiler - CLI");

    terminal.exibirInformacao("Teste de mensagem informativa.");
    terminal.exibirSucesso("Teste de mensagem de sucesso.");
    terminal.exibirAviso("Teste de mensagem de aviso.");
    terminal.exibirErro("Teste de mensagem de erro.");

    terminal.exibirSeparador();

    std::string nome = terminal.lerTexto("Digite seu nome: ");

    terminal.exibirTexto(
        "Olá, " + nome + "!\n",
        jack::cli::Terminal::Cor::Verde
        );

    return 0;
}