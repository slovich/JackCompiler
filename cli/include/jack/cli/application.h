#ifndef JACK_CLI_APPLICATION_H
#define JACK_CLI_APPLICATION_H

#include "jack/cli/terminal.h"
#include "jack/cli/menu.h"

namespace jack::cli {

class Application
{
public:
    Application();

    void executar();

private:
    Terminal terminal;
    Menu menu;

    int exibirMenuPrincipal();
    void processarOpcao(int opcao);
};

} // namespace jack::cli

#endif
