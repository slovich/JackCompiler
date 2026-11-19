#ifndef JACK_XML_GERADORXMLLEXICO_H
#define JACK_XML_GERADORXMLLEXICO_H

#include <string>
#include <vector>

#include "jack/lexer/token.h"

class GeradorXmlLexico
{
public:
    std::string gerar(const std::vector<Token>& tokens) const;
};

#endif // JACK_XML_GERADORXMLLEXICO_H