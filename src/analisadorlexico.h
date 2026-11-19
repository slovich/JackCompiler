#ifndef ANALISADORLEXICO_H
#define ANALISADORLEXICO_H

#include <string>
#include <vector>

#include "jack/lexer/token.h"

class AnalisadorLexico
{
public:
    explicit AnalisadorLexico(const std::string& codigoFonte);

    bool analisar(std::vector<Token>& tokens, std::string& erro) const;

private:
    std::string codigoFonte;
};

#endif // ANALISADORLEXICO_H