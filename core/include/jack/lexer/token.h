#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TipoToken
{
    Keyword,
    Symbol,
    IntegerConstant,
    StringConstant,
    Identifier
};

struct Token
{
    TipoToken tipo;
    std::string lexema;
    int linha;
    int coluna;
};

#endif // TOKEN_H