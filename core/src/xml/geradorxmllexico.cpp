#include "jack/xml/geradorxmllexico.h"

#include <sstream>

namespace
{

std::string obterNomeElemento(TipoToken tipo)
{
    switch (tipo) {
    case TipoToken::Keyword:
        return "keyword";

    case TipoToken::Symbol:
        return "symbol";

    case TipoToken::IntegerConstant:
        return "integerConstant";

    case TipoToken::StringConstant:
        return "stringConstant";

    case TipoToken::Identifier:
        return "identifier";
    }

    return {};
}

std::string escaparXml(const std::string& texto)
{
    std::string resultado;

    for (char caractere : texto) {
        switch (caractere) {
        case '&':
            resultado += "&amp;";
            break;

        case '<':
            resultado += "&lt;";
            break;

        case '>':
            resultado += "&gt;";
            break;

        default:
            resultado += caractere;
            break;
        }
    }

    return resultado;
}

}

std::string GeradorXmlLexico::gerar(
    const std::vector<Token>& tokens
    ) const
{
    std::ostringstream xml;

    xml << "<tokens>\n";

    for (const Token& token : tokens) {
        const std::string categoria =
            obterNomeElemento(token.tipo);

        xml << "  <" << categoria << "> "
            << escaparXml(token.lexema)
            << " </" << categoria << ">\n";
    }

    xml << "</tokens>\n";

    return xml.str();
}
