#ifndef JACK_CLI_MENU_H
#define JACK_CLI_MENU_H

#include "jack/cli/terminal.h"

#include <string>
#include <vector>

namespace jack::cli {

class Menu
{
public:
    struct Opcao
    {
        int codigo;
        std::string descricao;
    };

    explicit Menu(Terminal& terminal);

    int exibirMenu(
        const std::string& titulo,
        const std::vector<Opcao>& opcoes
        ) const;

private:
    Terminal& terminal;

    bool validarOpcao(
        const std::string& entrada,
        const std::vector<Opcao>& opcoes,
        int& codigo
        ) const;
};

} // namespace jack::cli

#endif
