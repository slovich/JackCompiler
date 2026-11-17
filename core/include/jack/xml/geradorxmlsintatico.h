#ifndef JACK_XML_GERADORXMLSINTATICO_H
#define JACK_XML_GERADORXMLSINTATICO_H

#include <string>

class ProgramNode;

class GeradorXmlSintatico
{
public:
    std::string gerar(const ProgramNode& programa) const;
};

#endif // JACK_XML_GERADORXMLSINTATICO_H