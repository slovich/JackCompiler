#include "jack/cli/menu.h"

#include <stdexcept>

namespace jack::cli {

Menu::Menu(Terminal& terminal)
    : terminal(terminal)
{
}

int Menu::exibirMenu(
    const std::string& titulo,
    const std::vector<Opcao>& opcoes
    ) const
{
    int codigo;

    while (true)
    {
        terminal.limparTela();

        terminal.exibirTitulo(titulo);

        for (const Opcao& opcao : opcoes)
        {
            terminal.exibirTexto(
                std::to_string(opcao.codigo) + " - " + opcao.descricao + "\n"
                );
        }

        terminal.exibirSeparador();

        std::string entrada = terminal.lerTexto("Escolha uma opção: ");

        if (validarOpcao(entrada, opcoes, codigo))
            return codigo;

        terminal.exibirErro("Opção inválida!");
        terminal.lerTexto("Pressione ENTER para tentar novamente...");
    }
}

bool Menu::validarOpcao(
    const std::string& entrada,
    const std::vector<Opcao>& opcoes,
    int& codigo
    ) const
{
    try
    {
        std::size_t posicao;
        int valor = std::stoi(entrada, &posicao);

        // Verifica se toda a entrada foi convertida.
        if (posicao != entrada.size())
            return false;

        // Verifica se o código corresponde a uma opção disponível.
        for (const Opcao& opcao : opcoes)
        {
            if (opcao.codigo == valor)
            {
                codigo = valor;
                return true;
            }
        }
    }
    catch (const std::invalid_argument&)
    {
        return false;
    }
    catch (const std::out_of_range&)
    {
        return false;
    }

    return false;
}

} // namespace jack::cli