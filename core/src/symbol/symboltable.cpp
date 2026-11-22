#include "jack/symbol/symboltable.h"

#include <utility>

std::string Symbol::segmentoVM() const
{
    switch (category) {
    case SymbolCategory::Static:
        return "static";
    case SymbolCategory::Field:
        return "this";
    case SymbolCategory::Argument:
        return "argument";
    case SymbolCategory::Local:
        return "local";
    }

    return {};
}

void SymbolTable::iniciarClasse()
{
    classScope.clear();
    subroutineScope.clear();

    nextStaticIndex = 0;
    nextFieldIndex = 0;
    nextArgumentIndex = 0;
    nextLocalIndex = 0;
}

void SymbolTable::iniciarSubrotina(SubroutineKind tipo)
{
    subroutineScope.clear();
    nextArgumentIndex = (tipo == SubroutineKind::Method) ? 1 : 0;
    nextLocalIndex = 0;
}

bool SymbolTable::definir(const std::string& nome,
                          const std::string& tipo,
                          SymbolCategory categoria)
{
    Scope& scope = pertenceAoEscopoDeClasse(categoria)
                       ? classScope
                       : subroutineScope;

    if (scope.find(nome) != scope.end()) {
        return false;
    }

    std::size_t* nextIndex = nullptr;
    switch (categoria) {
    case SymbolCategory::Static:
        nextIndex = &nextStaticIndex;
        break;
    case SymbolCategory::Field:
        nextIndex = &nextFieldIndex;
        break;
    case SymbolCategory::Argument:
        nextIndex = &nextArgumentIndex;
        break;
    case SymbolCategory::Local:
        nextIndex = &nextLocalIndex;
        break;
    }

    if (nextIndex == nullptr) {
        return false;
    }

    const Symbol symbol{nome, tipo, categoria, *nextIndex};
    scope.emplace(nome, symbol);
    ++(*nextIndex);
    return true;
}

std::optional<Symbol> SymbolTable::resolver(const std::string& nome) const
{
    const auto subroutineIt = subroutineScope.find(nome);
    if (subroutineIt != subroutineScope.end()) {
        return subroutineIt->second;
    }

    const auto classIt = classScope.find(nome);
    if (classIt != classScope.end()) {
        return classIt->second;
    }

    return std::nullopt;
}

std::optional<std::string> SymbolTable::tipoDe(const std::string& nome) const
{
    const auto symbol = resolver(nome);
    if (!symbol) {
        return std::nullopt;
    }
    return symbol->type;
}

std::optional<SymbolCategory> SymbolTable::categoriaDe(
    const std::string& nome) const
{
    const auto symbol = resolver(nome);
    if (!symbol) {
        return std::nullopt;
    }
    return symbol->category;
}

std::optional<std::size_t> SymbolTable::indiceDe(const std::string& nome) const
{
    const auto symbol = resolver(nome);
    if (!symbol) {
        return std::nullopt;
    }
    return symbol->index;
}

std::size_t SymbolTable::quantidadeDe(SymbolCategory categoria) const
{
    const Scope& scope = pertenceAoEscopoDeClasse(categoria)
                             ? classScope
                             : subroutineScope;

    std::size_t quantity = 0;
    for (const auto& entry : scope) {
        if (entry.second.category == categoria) {
            ++quantity;
        }
    }
    return quantity;
}

bool SymbolTable::pertenceAoEscopoDeClasse(SymbolCategory categoria)
{
    return categoria == SymbolCategory::Static
           || categoria == SymbolCategory::Field;
}
