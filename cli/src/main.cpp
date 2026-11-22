#include "jack/cli/application.h"
#include "jack/cli/compilador_vm_cli.h"

int main(int argc, char* argv[])
{
    if (argc > 1) {
        jack::cli::CompiladorVMCLI compiler;
        return compiler.executar(argc, argv);
    }

    jack::cli::Application application;
    application.executar();

    return 0;
}
