#ifndef JACK_XML_COMPARADORXML_H
#define JACK_XML_COMPARADORXML_H

#include <cstddef>
#include <string>
#include <vector>

namespace jack::xml {

enum class XMLDocumentType {
    Lexical,
    Syntax
};

enum class XMLComparisonStatus {
    Equivalent,
    Different,
    InvalidExpectedXML,
    InvalidActualXML,
    IncompatibleDocuments,
    InternalError
};

struct XMLDifference {
    std::string path;
    std::string description;
    std::string expected;
    std::string actual;
};

struct XMLComparisonResult {
    XMLComparisonStatus status = XMLComparisonStatus::InternalError;
    std::vector<XMLDifference> differences;
    std::size_t totalDifferences = 0;
    std::string message;
    std::size_t expectedErrorLine = 0;
    std::size_t expectedErrorColumn = 0;
    std::size_t actualErrorLine = 0;
    std::size_t actualErrorColumn = 0;
};

class ComparadorXml {
public:
    static constexpr std::size_t MaxDifferencesDefault = 20;

    XMLComparisonResult comparar(
        const std::string& xmlReferencia,
        const std::string& xmlGerado,
        XMLDocumentType tipo,
        std::size_t maxDifferences = MaxDifferencesDefault
    ) const;
};

} // namespace jack::xml

#endif // JACK_XML_COMPARADORXML_H
