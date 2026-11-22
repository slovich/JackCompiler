
#include "jack/symbol/symboltable.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int aprovados = 0;
int reprovados = 0;

void verificar(bool condicao, const std::string& mensagem)
{
    if (!condicao) {
        throw std::runtime_error(mensagem);
    }
}

void teste01_SeparacaoDeEscopos()
{
    SymbolTable tabela;
    tabela.iniciarClasse();

    verificar(tabela.definir("contador", "int", SymbolCategory::Static),
              "Falha ao declarar static.");

    verificar(tabela.definir("idade", "int", SymbolCategory::Field),
              "Falha ao declarar field.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.resolver("contador").has_value(),
              "Simbolo de classe static nao foi preservado.");

    verificar(tabela.resolver("idade").has_value(),
              "Simbolo de classe field nao foi preservado.");

    verificar(!tabela.resolver("inexistente").has_value(),
              "Tabela deveria estar sem esse identificador.");
}

void teste02_IndexacaoIndependente()
{
    SymbolTable tabela;
    tabela.iniciarClasse();
    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("s1", "int", SymbolCategory::Static),
              "Falha ao definir static s1.");
    verificar(tabela.definir("s2", "int", SymbolCategory::Static),
              "Falha ao definir static s2.");

    verificar(tabela.definir("f1", "int", SymbolCategory::Field),
              "Falha ao definir field f1.");
    verificar(tabela.definir("f2", "int", SymbolCategory::Field),
              "Falha ao definir field f2.");

    verificar(tabela.definir("a1", "int", SymbolCategory::Argument),
              "Falha ao definir argument a1.");
    verificar(tabela.definir("a2", "int", SymbolCategory::Argument),
              "Falha ao definir argument a2.");

    verificar(tabela.definir("l1", "int", SymbolCategory::Local),
              "Falha ao definir local l1.");
    verificar(tabela.definir("l2", "int", SymbolCategory::Local),
              "Falha ao definir local l2.");

    verificar(tabela.indiceDe("s1") == 0 &&
              tabela.indiceDe("s2") == 1,
              "Indices static incorretos.");

    verificar(tabela.indiceDe("f1") == 0 &&
              tabela.indiceDe("f2") == 1,
              "Indices field incorretos.");

    verificar(tabela.indiceDe("a1") == 0 &&
              tabela.indiceDe("a2") == 1,
              "Indices argument incorretos.");

    verificar(tabela.indiceDe("l1") == 0 &&
              tabela.indiceDe("l2") == 1,
              "Indices local incorretos.");

    verificar(tabela.quantidadeDe(SymbolCategory::Static) == 2,
              "Quantidade static incorreta.");
    verificar(tabela.quantidadeDe(SymbolCategory::Field) == 2,
              "Quantidade field incorreta.");
    verificar(tabela.quantidadeDe(SymbolCategory::Argument) == 2,
              "Quantidade argument incorreta.");
    verificar(tabela.quantidadeDe(SymbolCategory::Local) == 2,
              "Quantidade local incorreta.");
}

void teste03_ReservaArgumentZeroEmMetodo()
{
    SymbolTable tabela;
    tabela.iniciarClasse();

    tabela.iniciarSubrotina(SubroutineKind::Method);

    verificar(tabela.definir("thisArg", "int", SymbolCategory::Argument),
              "Falha ao definir primeiro argumento do metodo.");

    verificar(tabela.definir("segundoArg", "boolean",
                             SymbolCategory::Argument),
              "Falha ao definir segundo argumento do metodo.");

    verificar(tabela.indiceDe("thisArg") == 1,
              "Primeiro argumento explicito deveria ter indice 1.");

    verificar(tabela.indiceDe("segundoArg") == 2,
              "Segundo argumento explicito deveria ter indice 2.");

    verificar(tabela.quantidadeDe(SymbolCategory::Argument) == 2,
              "Quantidade de argumentos explicitos incorreta.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("parametro", "int",
                             SymbolCategory::Argument),
              "Falha ao definir argumento de funcao.");

    verificar(tabela.indiceDe("parametro") == 0,
              "Argumento de funcao deveria iniciar em zero.");
}

void teste04_ReinicializacaoDeSubrotina()
{
    SymbolTable tabela;
    tabela.iniciarClasse();

    verificar(tabela.definir("atributo", "int", SymbolCategory::Field),
              "Falha ao definir atributo da classe.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("parametro1", "int",
                             SymbolCategory::Argument),
              "Falha ao definir parametro.");

    verificar(tabela.definir("variavel1", "int", SymbolCategory::Local),
              "Falha ao definir variavel local.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(!tabela.resolver("parametro1").has_value(),
              "Parametro anterior nao foi removido.");

    verificar(!tabela.resolver("variavel1").has_value(),
              "Variavel local anterior nao foi removida.");

    verificar(tabela.resolver("atributo").has_value(),
              "Simbolo de classe foi removido indevidamente.");

    verificar(tabela.definir("parametro2", "int",
                             SymbolCategory::Argument),
              "Falha ao definir novo parametro.");

    verificar(tabela.definir("variavel2", "int", SymbolCategory::Local),
              "Falha ao definir nova variavel local.");

    verificar(tabela.indiceDe("parametro2") == 0,
              "Indice de argumento nao foi reiniciado.");

    verificar(tabela.indiceDe("variavel2") == 0,
              "Indice de local nao foi reiniciado.");

    verificar(tabela.quantidadeDe(SymbolCategory::Argument) == 1 &&
              tabela.quantidadeDe(SymbolCategory::Local) == 1,
              "Contagem da nova subrotina incorreta.");
}

void teste05_ReinicializacaoDeClasse()
{
    SymbolTable tabela;
    tabela.iniciarClasse();
    tabela.iniciarSubrotina(SubroutineKind::Method);

    verificar(tabela.definir("estatico", "int", SymbolCategory::Static),
              "Falha ao definir static.");
    verificar(tabela.definir("campo", "int", SymbolCategory::Field),
              "Falha ao definir field.");
    verificar(tabela.definir("argumento", "int",
                             SymbolCategory::Argument),
              "Falha ao definir argument.");
    verificar(tabela.definir("local", "int", SymbolCategory::Local),
              "Falha ao definir local.");

    tabela.iniciarClasse();

    verificar(!tabela.resolver("estatico").has_value(),
              "Static anterior permaneceu apos reiniciar classe.");
    verificar(!tabela.resolver("campo").has_value(),
              "Field anterior permaneceu apos reiniciar classe.");
    verificar(!tabela.resolver("argumento").has_value(),
              "Argument anterior permaneceu apos reiniciar classe.");
    verificar(!tabela.resolver("local").has_value(),
              "Local anterior permaneceu apos reiniciar classe.");

    verificar(tabela.quantidadeDe(SymbolCategory::Static) == 0,
              "Contagem static nao foi zerada.");
    verificar(tabela.quantidadeDe(SymbolCategory::Field) == 0,
              "Contagem field nao foi zerada.");
    verificar(tabela.quantidadeDe(SymbolCategory::Argument) == 0,
              "Contagem argument nao foi zerada.");
    verificar(tabela.quantidadeDe(SymbolCategory::Local) == 0,
              "Contagem local nao foi zerada.");

    verificar(tabela.definir("novoStatic", "int", SymbolCategory::Static),
              "Falha ao definir novo static.");
    verificar(tabela.indiceDe("novoStatic") == 0,
              "Indice static nao foi reiniciado.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("novoArgumento", "int",
                             SymbolCategory::Argument),
              "Falha ao definir novo argument.");
    verificar(tabela.indiceDe("novoArgumento") == 0,
              "Indice argument nao foi reiniciado.");
}

void teste06_PrecedenciaDeEscopos()
{
    SymbolTable tabela;
    tabela.iniciarClasse();

    verificar(tabela.definir("valor", "int", SymbolCategory::Static),
              "Falha ao definir simbolo de classe.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("valor", "boolean", SymbolCategory::Local),
              "Falha ao redefinir nome em escopo de subrotina.");

    auto simbolo = tabela.resolver("valor");

    verificar(simbolo.has_value(),
              "Simbolo nao foi resolvido.");

    verificar(simbolo->type == "boolean",
              "Escopo de subrotina nao teve precedencia.");

    verificar(simbolo->category == SymbolCategory::Local,
              "Categoria resolvida incorretamente.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    simbolo = tabela.resolver("valor");

    verificar(simbolo.has_value() && simbolo->type == "int",
              "Simbolo de classe nao voltou a ser visivel.");
}

void teste07_Duplicidades()
{
    SymbolTable tabela;
    tabela.iniciarClasse();

    verificar(tabela.definir("duplicado", "int",
                             SymbolCategory::Static),
              "Primeira declaracao deveria ser aceita.");

    verificar(!tabela.definir("duplicado", "boolean",
                              SymbolCategory::Field),
              "Duplicidade no escopo de classe deveria ser recusada.");

    verificar(tabela.quantidadeDe(SymbolCategory::Static) == 1,
              "Contador static alterado indevidamente.");

    verificar(tabela.quantidadeDe(SymbolCategory::Field) == 0,
              "Contador field alterado indevidamente.");

    verificar(tabela.definir("outro", "int", SymbolCategory::Static),
              "Nova declaracao deveria ser aceita.");

    verificar(tabela.indiceDe("outro") == 1,
              "Duplicidade alterou o indice static.");

    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("variavel", "int", SymbolCategory::Local),
              "Primeira variavel local deveria ser aceita.");

    verificar(!tabela.definir("variavel", "boolean",
                              SymbolCategory::Argument),
              "Duplicidade no escopo de subrotina deveria ser recusada.");

    verificar(tabela.quantidadeDe(SymbolCategory::Local) == 1,
              "Contador local alterado indevidamente.");

    verificar(tabela.quantidadeDe(SymbolCategory::Argument) == 0,
              "Contador argument alterado indevidamente.");

    verificar(tabela.definir("outroArgumento", "int",
                             SymbolCategory::Argument),
              "Novo argumento deveria ser aceito.");

    verificar(tabela.indiceDe("outroArgumento") == 0,
              "Duplicidade alterou o indice argument.");
}

void teste08_ConsultasEIdentificadoresInexistentes()
{
    SymbolTable tabela;
    tabela.iniciarClasse();
    tabela.iniciarSubrotina(SubroutineKind::Function);

    verificar(tabela.definir("quantidade", "int",
                             SymbolCategory::Local),
              "Falha ao definir simbolo para consulta.");

    verificar(tabela.resolver("quantidade").has_value(),
              "resolver() nao encontrou identificador existente.");

    verificar(tabela.tipoDe("quantidade") ==
              std::optional<std::string>("int"),
              "tipoDe() retornou valor incorreto.");

    verificar(tabela.categoriaDe("quantidade") ==
              std::optional<SymbolCategory>(SymbolCategory::Local),
              "categoriaDe() retornou valor incorreto.");

    verificar(tabela.indiceDe("quantidade") ==
              std::optional<std::size_t>(0),
              "indiceDe() retornou valor incorreto.");

    verificar(!tabela.resolver("fantasma").has_value(),
              "resolver() deveria retornar nullopt.");

    verificar(!tabela.tipoDe("fantasma").has_value(),
              "tipoDe() deveria retornar nullopt.");

    verificar(!tabela.categoriaDe("fantasma").has_value(),
              "categoriaDe() deveria retornar nullopt.");

    verificar(!tabela.indiceDe("fantasma").has_value(),
              "indiceDe() deveria retornar nullopt.");
}

using Teste = void (*)();

struct CasoTeste {
    const char* nome;
    Teste executar;
};

} // namespace

int main()
{
    const std::vector<CasoTeste> testes = {
        {"01 - Separacao de escopos", teste01_SeparacaoDeEscopos},
        {"02 - Indexacao independente", teste02_IndexacaoIndependente},
        {"03 - Reserva de argument 0 em metodo",
         teste03_ReservaArgumentZeroEmMetodo},
        {"04 - Reinicializacao de subrotina",
         teste04_ReinicializacaoDeSubrotina},
        {"05 - Reinicializacao de classe",
         teste05_ReinicializacaoDeClasse},
        {"06 - Precedencia entre escopos",
         teste06_PrecedenciaDeEscopos},
        {"07 - Duplicidades", teste07_Duplicidades},
        {"08 - Consultas e identificadores inexistentes",
         teste08_ConsultasEIdentificadoresInexistentes}
    };

    std::cout << "\n============================================\n";
    std::cout << " TESTES UNITARIOS - SYMBOL TABLE\n";
    std::cout << "============================================\n";

    for (const auto& teste : testes) {
        try {
            teste.executar();
            ++aprovados;
            std::cout << "[PASS] " << teste.nome << '\n';
        }
        catch (const std::exception& erro) {
            ++reprovados;
            std::cout << "[FAIL] " << teste.nome
                      << " | " << erro.what() << '\n';
        }
        catch (...) {
            ++reprovados;
            std::cout << "[FAIL] " << teste.nome
                      << " | Erro desconhecido.\n";
        }
    }

    std::cout << "\n============================================\n";
    std::cout << " RESULTADO FINAL\n";
    std::cout << "============================================\n";
    std::cout << "Total:     " << testes.size() << '\n';
    std::cout << "Aprovados: " << aprovados << '\n';
    std::cout << "Reprovados:" << reprovados << '\n';

    if (reprovados == 0) {
        std::cout << "\nRESULTADO: TODOS OS TESTES PASSARAM.\n";
        return 0;
    }

    std::cout << "\nRESULTADO: EXISTEM FALHAS.\n";
    return 1;
}
