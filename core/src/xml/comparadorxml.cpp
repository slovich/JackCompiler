#include "jack/xml/comparadorxml.h"

#include "pugixml.hpp"

#include <algorithm>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

namespace jack::xml {
namespace {

struct ComparableChild {
    enum class Kind { Element, Text };
    Kind kind;
    pugi::xml_node node;
    std::string text;
    std::string path;
};

std::string expectedRoot(XMLDocumentType type)
{
    return type == XMLDocumentType::Lexical ? "tokens" : "class";
}

std::string describeNode(const ComparableChild& child)
{
    if (child.kind == ComparableChild::Kind::Element)
        return "<" + std::string(child.node.name()) + ">";
    return child.text;
}

void addDifference(XMLComparisonResult& result,
                   std::size_t maxDifferences,
                   const std::string& path,
                   const std::string& description,
                   const std::string& expected,
                   const std::string& actual)
{
    ++result.totalDifferences;
    if (result.differences.size() < maxDifferences) {
        result.differences.push_back({path, description, expected, actual});
    }
}

std::string elementPath(const pugi::xml_node& node,
                        const std::string& parentPath)
{
    const std::string name = node.name();
    std::size_t index = 0;
    for (pugi::xml_node sibling = node.parent().first_child();
         sibling;
         sibling = sibling.next_sibling()) {
        if (sibling.type() == pugi::node_element && name == sibling.name()) {
            ++index;
            if (sibling == node)
                break;
        }
    }
    return parentPath + "/" + name + "[" + std::to_string(index) + "]";
}

std::vector<ComparableChild> comparableChildren(
    const pugi::xml_node& parent,
    const std::string& parentPath)
{
    bool hasElementChildren = false;
    bool hasNonWhitespaceText = false;
    for (pugi::xml_node child = parent.first_child(); child;
         child = child.next_sibling()) {
        if (child.type() == pugi::node_element) {
            hasElementChildren = true;
        } else if (child.type() == pugi::node_pcdata
                   || child.type() == pugi::node_cdata) {
            const std::string value = child.value();
            const bool whitespaceOnly = std::all_of(
                value.begin(), value.end(),
                [](unsigned char c) {
                    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
                });
            if (!whitespaceOnly)
                hasNonWhitespaceText = true;
        }
    }

    // In mixed content, whitespace may be significant text. Only discard
    // whitespace-only nodes when they are formatting around child elements.
    const bool ignoreStructuralWhitespace =
        hasElementChildren && !hasNonWhitespaceText;

    std::vector<ComparableChild> result;
    for (pugi::xml_node child = parent.first_child(); child;
         child = child.next_sibling()) {
        if (child.type() == pugi::node_element) {
            result.push_back({
                ComparableChild::Kind::Element,
                child,
                {},
                elementPath(child, parentPath)
            });
        } else if (child.type() == pugi::node_pcdata
                   || child.type() == pugi::node_cdata) {
            const std::string value = child.value();

            // Whitespace-only text is formatting only when the parent
            // contains element children. In leaf elements it is terminal data.
            const bool whitespaceOnly = std::all_of(
                value.begin(), value.end(),
                [](unsigned char c) {
                    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
                });

            if (ignoreStructuralWhitespace && whitespaceOnly)
                continue;

            result.push_back({
                ComparableChild::Kind::Text,
                {},
                value,
                parentPath + "/text()"
            });
        }
        // Comments, declarations and processing instructions are ignored.
    }
    return result;
}

void compareElements(const pugi::xml_node& expected,
                     const pugi::xml_node& actual,
                     const std::string& path,
                     XMLComparisonResult& result,
                     std::size_t maxDifferences)
{
    if (std::string(expected.name()) != actual.name()) {
        addDifference(result, maxDifferences, path,
                      "Nome do elemento divergente",
                      expected.name(), actual.name());
        return;
    }

    for (pugi::xml_attribute attribute : expected.attributes()) {
        const pugi::xml_attribute actualAttribute =
            actual.attribute(attribute.name());
        if (!actualAttribute) {
            addDifference(result, maxDifferences,
                          path + "/@" + attribute.name(),
                          "Atributo ausente no XML gerado",
                          attribute.value(), "<ausente>");
        } else if (std::string(attribute.value()) != actualAttribute.value()) {
            addDifference(result, maxDifferences,
                          path + "/@" + attribute.name(),
                          "Valor de atributo divergente",
                          attribute.value(), actualAttribute.value());
        }
    }

    for (pugi::xml_attribute attribute : actual.attributes()) {
        if (!expected.attribute(attribute.name())) {
            addDifference(result, maxDifferences,
                          path + "/@" + attribute.name(),
                          "Atributo adicional no XML gerado",
                          "<ausente>", attribute.value());
        }
    }

    const std::vector<ComparableChild> expectedChildren =
        comparableChildren(expected, path);
    const std::vector<ComparableChild> actualChildren =
        comparableChildren(actual, path);

    const std::size_t common = std::min(expectedChildren.size(),
                                        actualChildren.size());
    for (std::size_t i = 0; i < common; ++i) {
        const ComparableChild& left = expectedChildren[i];
        const ComparableChild& right = actualChildren[i];

        if (left.kind != right.kind) {
            addDifference(result, maxDifferences,
                          left.kind == ComparableChild::Kind::Element
                              ? left.path : right.path,
                          "Tipo ou ordem de nó divergente",
                          describeNode(left), describeNode(right));
            continue;
        }

        if (left.kind == ComparableChild::Kind::Element) {
            if (std::string(left.node.name()) != right.node.name()) {
                addDifference(result, maxDifferences, left.path,
                              "Elemento divergente na posição correspondente",
                              left.node.name(), right.node.name());
            } else {
                compareElements(left.node, right.node, left.path,
                                result, maxDifferences);
            }
        } else if (left.text != right.text) {
            addDifference(result, maxDifferences, left.path,
                          "Conteúdo textual divergente",
                          left.text, right.text);
        }
    }

    for (std::size_t i = common; i < expectedChildren.size(); ++i) {
        const auto& child = expectedChildren[i];
        addDifference(result, maxDifferences, child.path,
                      "Nó ausente no XML gerado",
                      describeNode(child), "<ausente>");
    }
    for (std::size_t i = common; i < actualChildren.size(); ++i) {
        const auto& child = actualChildren[i];
        addDifference(result, maxDifferences, child.path,
                      "Nó adicional no XML gerado",
                      "<ausente>", describeNode(child));
    }
}


void setErrorLocation(const std::string& xml, std::ptrdiff_t offset,
                      std::size_t& line, std::size_t& column)
{
    if (offset < 0)
        return;
    line = 1;
    column = 1;
    const std::size_t limit = std::min(
        static_cast<std::size_t>(offset), xml.size());
    for (std::size_t i = 0; i < limit; ++i) {
        if (xml[i] == '\r') {
            ++line;
            column = 1;
            if (i + 1 < limit && xml[i + 1] == '\n')
                ++i;
        } else if (xml[i] == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }
    }
}

bool parseDocument(const std::string& xml, pugi::xml_document& document,
                   pugi::xml_parse_result& parseResult,
                   bool& doctypeFound)
{
    doctypeFound = false;

    if (xml.empty())
        return false;

    // Solicita ao pugixml que represente uma declaração DOCTYPE como nó
    // estrutural. Assim, sequências iguais dentro de comentários ou CDATA
    // não são confundidas com uma declaração efetiva.
    //
    // O pugixml não recebe nenhum resolver externo; recursos externos não
    // são carregados. Se houver um DOCTYPE real, o documento será rejeitado
    // logo após o parsing, antes da comparação.
    const unsigned int flags =
        pugi::parse_default | pugi::parse_ws_pcdata | pugi::parse_doctype;

    parseResult = document.load_buffer(
        xml.data(), xml.size(), flags, pugi::encoding_auto);

    if (!parseResult)
        return false;

    for (pugi::xml_node child = document.first_child();
         child;
         child = child.next_sibling()) {
        if (child.type() == pugi::node_doctype) {
            doctypeFound = true;
            return false;
        }
    }

    return true;
}

} // namespace

XMLComparisonResult ComparadorXml::comparar(
    const std::string& xmlReferencia,
    const std::string& xmlGerado,
    XMLDocumentType tipo,
    std::size_t maxDifferences) const
{
    XMLComparisonResult result;
    try {
        pugi::xml_document expectedDocument;
        pugi::xml_document actualDocument;
        pugi::xml_parse_result expectedParse;
        pugi::xml_parse_result actualParse;
        bool expectedHasDoctype = false;
        bool actualHasDoctype = false;

        if (!parseDocument(xmlReferencia, expectedDocument, expectedParse,
                           expectedHasDoctype)) {
            result.status = XMLComparisonStatus::InvalidExpectedXML;
            result.message = xmlReferencia.empty()
                ? "O XML de referência está vazio."
                : (expectedHasDoctype
                    ? "DTD e declarações de entidades não são suportados."
                    : "XML de referência malformado: "
                        + std::string(expectedParse.description()));
            setErrorLocation(xmlReferencia, expectedParse.offset,
                             result.expectedErrorLine,
                             result.expectedErrorColumn);
            return result;
        }

        if (!parseDocument(xmlGerado, actualDocument, actualParse,
                           actualHasDoctype)) {
            result.status = XMLComparisonStatus::InvalidActualXML;
            result.message = xmlGerado.empty()
                ? "O XML gerado está vazio."
                : (actualHasDoctype
                    ? "DTD e declarações de entidades não são suportados."
                    : "XML gerado malformado: "
                        + std::string(actualParse.description()));
            setErrorLocation(xmlGerado, actualParse.offset,
                             result.actualErrorLine,
                             result.actualErrorColumn);
            return result;
        }

        const pugi::xml_node expectedRootNode = expectedDocument.document_element();
        const pugi::xml_node actualRootNode = actualDocument.document_element();
        const std::string requiredRoot = expectedRoot(tipo);

        if (!expectedRootNode || !actualRootNode
            || requiredRoot != expectedRootNode.name()
            || requiredRoot != actualRootNode.name()) {
            result.status = XMLComparisonStatus::IncompatibleDocuments;
            result.message = "Raiz XML incompatível. Esperada: <"
                + requiredRoot + ">; referência: <"
                + (expectedRootNode ? expectedRootNode.name() : "ausente")
                + ">; gerado: <"
                + (actualRootNode ? actualRootNode.name() : "ausente") + ">.";
            return result;
        }

        compareElements(expectedRootNode, actualRootNode,
                        "/" + requiredRoot + "[1]", result, maxDifferences);

        result.status = result.totalDifferences == 0
            ? XMLComparisonStatus::Equivalent
            : XMLComparisonStatus::Different;
        result.message = result.status == XMLComparisonStatus::Equivalent
            ? "Os documentos XML são estruturalmente equivalentes."
            : "Foram identificadas divergências estruturais.";
        return result;
    } catch (const std::exception& exception) {
        result.status = XMLComparisonStatus::InternalError;
        result.message = std::string("Erro interno durante a comparação: ")
                         + exception.what();
        return result;
    } catch (...) {
        result.status = XMLComparisonStatus::InternalError;
        result.message = "Erro interno não identificado durante a comparação.";
        return result;
    }
}

} // namespace jack::xml
