#ifndef JACK_CLI_TESTADOR_LOTE_H
#define JACK_CLI_TESTADOR_LOTE_H

#include "jack/cli/resultado_lote.h"

#include <string>

namespace jack::cli {

class TestadorLote {
public:
    ResultadoLote executar(const std::string& diretorio) const;
};

} // namespace jack::cli

#endif // JACK_CLI_TESTADOR_LOTE_H