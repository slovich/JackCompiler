#ifndef JACK_CLI_COMPILADOR_VM_CLI_H
#define JACK_CLI_COMPILADOR_VM_CLI_H

namespace jack::cli {

/**
 * Interface não interativa para compilar arquivos ou diretórios Jack em VM.
 *
 * Uso:
 *   jack_cli --compile <arquivo.jack|diretorio> [--output <diretorio>]
 *   jack_cli compile <arquivo.jack|diretorio> [-o <diretorio>]
 */
class CompiladorVMCLI {
public:
    int executar(int argc, char* argv[]) const;
};

} // namespace jack::cli

#endif // JACK_CLI_COMPILADOR_VM_CLI_H
