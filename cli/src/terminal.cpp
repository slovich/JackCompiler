#include "jack/cli/terminal.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace jack::cli {

Terminal::Terminal()
    : coresHabilitadas(false)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    inicializarSuporteCores();
}

void Terminal::inicializarSuporteCores()
{
#ifdef _WIN32
    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);

    if (saida == INVALID_HANDLE_VALUE || saida == nullptr)
        return;

    DWORD modoAtual = 0;

    if (!GetConsoleMode(saida, &modoAtual))
        return;

    if (SetConsoleMode(
            saida,
            modoAtual | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        coresHabilitadas = true;
    }
#else
    coresHabilitadas = isatty(STDOUT_FILENO);
#endif
}

std::string Terminal::obterCodigoCor(Cor cor) const
{
    switch (cor)
    {
    case Cor::Vermelho: return "\033[31m";
    case Cor::Verde:    return "\033[32m";
    case Cor::Amarelo:  return "\033[33m";
    case Cor::Azul:     return "\033[34m";
    case Cor::Magenta:  return "\033[35m";
    case Cor::Ciano:    return "\033[36m";
    case Cor::Branco:   return "\033[37m";
    case Cor::Padrao:   return "\033[0m";
    }

    return "\033[0m";
}

void Terminal::limparTela() const
{
    if (coresHabilitadas)
        std::cout << "\033[2J\033[H";
    else
        std::cout << "\n";

    std::cout.flush();
}

void Terminal::exibirTitulo(const std::string& titulo) const
{
    exibirTexto("\n========================================\n", Cor::Ciano);
    exibirTexto("  " + titulo + "\n", Cor::Ciano);
    exibirTexto("========================================\n", Cor::Ciano);
}

void Terminal::exibirSeparador() const
{
    exibirTexto("----------------------------------------\n", Cor::Branco);
}

void Terminal::exibirTexto(const std::string& texto, Cor cor) const
{
    if (coresHabilitadas)
        std::cout << obterCodigoCor(cor) << texto << obterCodigoCor(Cor::Padrao);
    else
        std::cout << texto;

    std::cout.flush();
}

void Terminal::exibirInformacao(const std::string& mensagem) const
{
    exibirTexto("[INFO] ", Cor::Ciano);
    exibirTexto(mensagem + "\n");
}

void Terminal::exibirSucesso(const std::string& mensagem) const
{
    exibirTexto("[OK] ", Cor::Verde);
    exibirTexto(mensagem + "\n");
}

void Terminal::exibirAviso(const std::string& mensagem) const
{
    exibirTexto("[AVISO] ", Cor::Amarelo);
    exibirTexto(mensagem + "\n");
}

void Terminal::exibirErro(const std::string& mensagem) const
{
    exibirTexto("[ERRO] ", Cor::Vermelho);
    exibirTexto(mensagem + "\n");
}

std::string Terminal::lerTexto(const std::string& mensagem) const
{
    exibirTexto(mensagem, Cor::Ciano);

    std::string entrada;
    std::getline(std::cin, entrada);

    return entrada;
}

} // namespace jack::cli
