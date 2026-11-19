#ifndef JACK_CLI_APPLICATION_H
#define JACK_CLI_APPLICATION_H

#include "jack/cli/terminal.h"
#include "jack/cli/menu.h"
#include "jack/lexer/analisadorlexico.h"
#include "jack/parser/analisadorsintatico.h"
#include "jack/xml/geradorxmllexico.h"
#include "jack/lexer/token.h"

#include <vector>
#include <string>
#include <memory>

namespace jack::cli {

class Application
{
public:
    Application();

    void executar();

private:
    Terminal terminal;
    Menu menu;

    std::string caminhoArquivo;
    std::string codigoFonte;
    std::vector<Token> tokens;
    std::unique_ptr<ProgramNode> programa;
    bool arquivoCarregado;
    bool analiseLexicaValida;
    bool analiseSintaticaValida;

    int exibirMenuPrincipal();
    void processarOpcao(int opcao);

    void selecionarArquivo();
    bool analisarLexico();
    bool analisarSintatico();
    void analisarCompleto();
    void exportarXmlLexico();
    void exibirArquivoSelecionado() const;
};

} // namespace jack::cli

#endif
