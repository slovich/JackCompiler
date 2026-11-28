#include "jack/xml/comparadorxml.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using jack::xml::ComparadorXml;
using jack::xml::XMLComparisonStatus;
using jack::xml::XMLDocumentType;

void exigir(bool condicao, const std::string& mensagem)
{
    if (!condicao)
        throw std::runtime_error(mensagem);
}

void testarComentarioComSequenciaDoctype()
{
    const std::string referencia =
        "<tokens><!-- <!DOCTYPE fake> -->"
        "<keyword>class</keyword></tokens>";
    const std::string gerado =
        "<tokens><keyword>class</keyword></tokens>";

    const auto resultado = ComparadorXml{}.comparar(
        referencia, gerado, XMLDocumentType::Lexical);

    exigir(resultado.status == XMLComparisonStatus::Equivalent,
           "Sequência DOCTYPE em comentário deve ser aceita e ignorada.");
}

void testarCdataComSequenciaDoctype()
{
    const std::string referencia =
        "<tokens><stringConstant><![CDATA[<!DOCTYPE fake>]]>"
        "</stringConstant></tokens>";
    const std::string gerado =
        "<tokens><stringConstant>&lt;!DOCTYPE fake&gt;"
        "</stringConstant></tokens>";

    const auto resultado = ComparadorXml{}.comparar(
        referencia, gerado, XMLDocumentType::Lexical);

    exigir(resultado.status == XMLComparisonStatus::Equivalent,
           "Sequência DOCTYPE em CDATA deve ser tratada como conteúdo textual.");
}

void testarRejeicaoDoctypeNaReferencia()
{
    const std::string referencia =
        "<!DOCTYPE tokens><tokens><keyword>class</keyword></tokens>";
    const std::string gerado =
        "<tokens><keyword>class</keyword></tokens>";

    const auto resultado = ComparadorXml{}.comparar(
        referencia, gerado, XMLDocumentType::Lexical);

    exigir(resultado.status == XMLComparisonStatus::InvalidExpectedXML,
           "DOCTYPE efetivo no XML de referência deve ser rejeitado.");
}

void testarRejeicaoDoctypeNoGerado()
{
    const std::string referencia =
        "<tokens><keyword>class</keyword></tokens>";
    const std::string gerado =
        "<!DOCTYPE tokens SYSTEM \"file:///arquivo-que-nao-deve-ser-lido\">"
        "<tokens><keyword>class</keyword></tokens>";

    const auto resultado = ComparadorXml{}.comparar(
        referencia, gerado, XMLDocumentType::Lexical);

    exigir(resultado.status == XMLComparisonStatus::InvalidActualXML,
           "DOCTYPE efetivo no XML gerado deve ser rejeitado.");
}

} // namespace

int main()
{
    const std::vector<std::pair<std::string, void (*)()>> testes = {
        {"DOCTYPE em comentário", testarComentarioComSequenciaDoctype},
        {"DOCTYPE em CDATA", testarCdataComSequenciaDoctype},
        {"Rejeição de DTD na referência", testarRejeicaoDoctypeNaReferencia},
        {"Rejeição de DTD no XML gerado", testarRejeicaoDoctypeNoGerado}
    };

    std::size_t aprovados = 0;
    for (const auto& teste : testes) {
        try {
            teste.second();
            ++aprovados;
            std::cout << "[OK] " << teste.first << '\n';
        } catch (const std::exception& erro) {
            std::cerr << "[FALHA] " << teste.first << ": "
                      << erro.what() << '\n';
        }
    }

    std::cout << aprovados << "/" << testes.size()
              << " testes preparados executados com sucesso.\n";

    return aprovados == testes.size() ? 0 : 1;
}
