#ifndef JACK_SYMBOL_SYMBOLTABLE_H
#define JACK_SYMBOL_SYMBOLTABLE_H

#include "jack/ast/subroutinekind.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>

/** Categoria de armazenamento de um identificador Jack. */
enum class SymbolCategory {
    Static,
    Field,
    Argument,
    Local
};

/** Registro completo de um símbolo declarado. */
struct Symbol {
    std::string name;
    std::string type;
    SymbolCategory category;
    std::size_t index;

    /** Retorna o segmento correspondente no modelo de memória VM. */
    std::string segmentoVM() const;
};

/**
 * Mantém os símbolos de classe e de uma única sub-rotina Jack.
 *
 * A tabela não realiza análise ou compatibilidade de tipos. Uma declaração
 * duplicada é recusada por definir(), enquanto consultas não resolvidas
 * retornam std::nullopt.
 */
class SymbolTable {
public:
    SymbolTable() = default;

    /** Limpa os dois escopos e reinicia todos os índices. */
    void iniciarClasse();

    /**
     * Inicia um novo escopo de sub-rotina, preservando o escopo de classe.
     * Em métodos, reserva o índice argument 0 para o objeto implícito.
     */
    void iniciarSubrotina(SubroutineKind tipo);

    /**
     * Registra um símbolo. Retorna false se o nome já existir no mesmo
     * escopo; nesse caso, nenhum registro ou contador é alterado.
     */
    bool definir(const std::string& nome,
                 const std::string& tipo,
                 SymbolCategory categoria);

    /** Resolve primeiro no escopo de sub-rotina e depois no de classe. */
    std::optional<Symbol> resolver(const std::string& nome) const;

    std::optional<std::string> tipoDe(const std::string& nome) const;
    std::optional<SymbolCategory> categoriaDe(const std::string& nome) const;
    std::optional<std::size_t> indiceDe(const std::string& nome) const;

    /** Quantidade de símbolos explicitamente registrados na categoria. */
    std::size_t quantidadeDe(SymbolCategory categoria) const;

private:
    using Scope = std::map<std::string, Symbol>;

    static bool pertenceAoEscopoDeClasse(SymbolCategory categoria);

    Scope classScope;
    Scope subroutineScope;
    std::size_t nextStaticIndex = 0;
    std::size_t nextFieldIndex = 0;
    std::size_t nextArgumentIndex = 0;
    std::size_t nextLocalIndex = 0;
};

#endif // JACK_SYMBOL_SYMBOLTABLE_H
