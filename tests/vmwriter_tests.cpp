#include "jack/vm/vmwriter.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

namespace {

struct TestResult {
    std::string name;
    bool passed;
    std::string detail;
};

class FailingStreamBuf : public std::streambuf {
public:
    explicit FailingStreamBuf(bool failWrites = true, bool failFlush = false)
        : failWrites_(failWrites), failFlush_(failFlush) {}

protected:
    std::streamsize xsputn(const char*, std::streamsize count) override {
        if (failWrites_) {
            return count > 0 ? count - 1 : 0;
        }
        return count;
    }

    int_type overflow(int_type character = traits_type::eof()) override {
        if (failWrites_) {
            return traits_type::eof();
        }
        return traits_type::not_eof(character);
    }

    int sync() override {
        return failFlush_ ? -1 : 0;
    }

private:
    bool failWrites_;
    bool failFlush_;
};

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template <typename Exception, typename Callable>
void requireThrows(Callable&& callable, const std::string& message) {
    try {
        callable();
    } catch (const Exception&) {
        return;
    } catch (const std::exception& error) {
        throw std::runtime_error(message + " (excecao diferente: " + error.what() + ")");
    }
    throw std::runtime_error(message + " (nenhuma excecao foi lancada)");
}

void runTest(std::vector<TestResult>& results,
             const std::string& name,
             const std::function<void()>& test) {
    try {
        test();
        results.push_back({name, true, {}});
    } catch (const std::exception& error) {
        results.push_back({name, false, error.what()});
    } catch (...) {
        results.push_back({name, false, "excecao desconhecida"});
    }
}

} // namespace

int main() {
    std::vector<TestResult> results;

    runTest(results, "01 - Push/pop validos e formato exato", [] {
        std::ostringstream output;
        VMWriter writer(output);

        writer.writePush("constant", 32767);
        writer.writePush("local", 0);
        writer.writePush("argument", 2);
        writer.writePush("this", 3);
        writer.writePush("that", 4);
        writer.writePush("pointer", 1);
        writer.writePush("temp", 7);
        writer.writePush("static", 239);
        writer.writePop("local", 0);
        writer.writePop("argument", 2);
        writer.writePop("this", 3);
        writer.writePop("that", 4);
        writer.writePop("pointer", 0);
        writer.writePop("temp", 7);
        writer.writePop("static", 239);

        const std::string expected =
            "push constant 32767\n"
            "push local 0\n"
            "push argument 2\n"
            "push this 3\n"
            "push that 4\n"
            "push pointer 1\n"
            "push temp 7\n"
            "push static 239\n"
            "pop local 0\n"
            "pop argument 2\n"
            "pop this 3\n"
            "pop that 4\n"
            "pop pointer 0\n"
            "pop temp 7\n"
            "pop static 239\n";
        require(output.str() == expected, "saida push/pop diverge do texto esperado");
    });

    runTest(results, "02 - Nove comandos aritmeticos/logicos", [] {
        std::ostringstream output;
        VMWriter writer(output);

        const std::vector<std::string> commands = {
            "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"
        };
        for (const auto& command : commands) {
            writer.writeArithmetic(command);
        }

        const std::string expected =
            "add\nsub\nneg\neq\ngt\nlt\nand\nor\nnot\n";
        require(output.str() == expected, "comandos aritmeticos/logicos divergentes");
    });

    runTest(results, "03 - Rejeicao de comando aritmetico desconhecido", [] {
        std::ostringstream output;
        VMWriter writer(output);
        requireThrows<std::invalid_argument>(
            [&] { writer.writeArithmetic("multiply"); },
            "comando aritmetico invalido deveria ser rejeitado");
        require(output.str().empty(), "instrucao invalida alterou o fluxo");
    });

    runTest(results, "04 - label/goto/if-goto", [] {
        std::ostringstream output;
        VMWriter writer(output);
        writer.writeLabel("LOOP_START");
        writer.writeGoto("LOOP_END");
        writer.writeIf("IF_TRUE");
        require(output.str() ==
                    "label LOOP_START\ngoto LOOP_END\nif-goto IF_TRUE\n",
                "saida de label/goto/if-goto divergente");
    });

    runTest(results, "05 - call/function/return", [] {
        std::ostringstream output;
        VMWriter writer(output);
        writer.writeCall("Main.main", 2);
        writer.writeFunction("Main.main", 3);
        writer.writeReturn();
        require(output.str() ==
                    "call Main.main 2\nfunction Main.main 3\nreturn\n",
                "saida de call/function/return divergente");
    });

    runTest(results, "06 - Rejeicao de pop constant", [] {
        std::ostringstream output;
        VMWriter writer(output);
        requireThrows<std::invalid_argument>(
            [&] { writer.writePop("constant", 0); },
            "pop constant deveria ser rejeitado");
        require(output.str().empty(), "pop constant invalido alterou o fluxo");
    });

    runTest(results, "07 - Limites de pointer/temp/static/constant", [] {
        std::ostringstream output;
        VMWriter writer(output);

        writer.writePush("pointer", 0);
        writer.writePush("pointer", 1);
        writer.writePush("temp", 0);
        writer.writePush("temp", 7);
        writer.writePush("static", 0);
        writer.writePush("static", 239);
        writer.writePush("constant", 0);
        writer.writePush("constant", 32767);

        requireThrows<std::invalid_argument>(
            [&] { writer.writePush("pointer", 2); }, "pointer 2 deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writePush("temp", 8); }, "temp 8 deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writePush("static", 240); }, "static 240 deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writePush("constant", 32768); },
            "constant 32768 deveria ser rejeitado");

        require(output.str() ==
                    "push pointer 0\npush pointer 1\n"
                    "push temp 0\npush temp 7\n"
                    "push static 0\npush static 239\n"
                    "push constant 0\npush constant 32767\n",
                "limites validos nao produziram a saida esperada");
    });

    runTest(results, "08 - Rejeicao de indice negativo", [] {
        std::ostringstream output;
        VMWriter writer(output);
        requireThrows<std::invalid_argument>(
            [&] { writer.writePush("local", -1); },
            "indice negativo em push deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writePop("temp", -1); },
            "indice negativo em pop deveria ser rejeitado");
        require(output.str().empty(), "indice negativo alterou o fluxo");
    });

    runTest(results, "09 - Rejeicao de nomes vazios ou com whitespace/controle", [] {
        std::ostringstream output;
        VMWriter writer(output);

        requireThrows<std::invalid_argument>(
            [&] { writer.writeLabel(""); }, "rotulo vazio deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writeGoto("ROTULO COM ESPACO"); },
            "rotulo com espaco deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writeIf("ROTULO\nQUEBRADO"); },
            "rotulo com quebra de linha deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writeCall("", 0); }, "nome de sub-rotina vazio deveria ser rejeitado");
        requireThrows<std::invalid_argument>(
            [&] { writer.writeFunction("Funcao\tInvalida", 0); },
            "nome de funcao com tab deveria ser rejeitado");
        require(output.str().empty(), "nome invalido alterou o fluxo");
    });

    runTest(results, "10 - Quantidades negativas em call/function", [] {
        std::ostringstream output;
        VMWriter writer(output);
        requireThrows<std::invalid_argument>(
            [&] { writer.writeCall("Main.f", -1); },
            "quantidade negativa de argumentos deveria ser rejeitada");
        requireThrows<std::invalid_argument>(
            [&] { writer.writeFunction("Main.f", -1); },
            "quantidade negativa de locais deveria ser rejeitada");
        require(output.str().empty(), "quantidade negativa alterou o fluxo");
    });

    runTest(results, "11 - Finalizacao e idempotencia", [] {
        std::ostringstream output;
        VMWriter writer(output);
        writer.writePush("constant", 7);
        writer.finalizar();
        require(writer.finalizado(), "finalizado() deveria retornar true");
        const std::string beforeSecondFinalize = output.str();
        writer.finalizar();
        require(output.str() == beforeSecondFinalize,
                "segunda finalizacao alterou o fluxo");
    });

    runTest(results, "12 - Rejeicao de escrita apos finalizacao", [] {
        std::ostringstream output;
        VMWriter writer(output);
        writer.finalizar();
        requireThrows<std::logic_error>(
            [&] { writer.writeReturn(); },
            "escrita apos finalizar deveria lancar logic_error");
        require(output.str().empty(), "escrita apos finalizar alterou o fluxo");
    });

    runTest(results, "13 - Deteccao de falha durante escrita", [] {
        FailingStreamBuf buffer(true, false);
        std::ostream output(&buffer);
        VMWriter writer(output);

        requireThrows<std::ios_base::failure>(
            [&] { writer.writeReturn(); },
            "falha de escrita deveria ser propagada como ios_base::failure");
        requireThrows<std::ios_base::failure>(
            [&] { writer.writeReturn(); },
            "writer deveria continuar marcando o fluxo como falho");
    });

    runTest(results, "14 - Deteccao de falha durante flush/finalizacao", [] {
        FailingStreamBuf buffer(false, true);
        std::ostream output(&buffer);
        VMWriter writer(output);

        requireThrows<std::ios_base::failure>(
            [&] { writer.finalizar(); },
            "falha de flush deveria ser propagada como ios_base::failure");
        require(!writer.finalizado(),
                "writer nao deveria marcar finalizado apos falha de flush");
        requireThrows<std::ios_base::failure>(
            [&] { writer.writeReturn(); },
            "writer deveria bloquear escrita apos falha de flush");
    });

    int passed = 0;
    int failed = 0;

    for (const auto& result : results) {
        if (result.passed) {
            ++passed;
            std::cout << "[PASS] " << result.name << '\n';
        } else {
            ++failed;
            std::cout << "[FAIL] " << result.name
                      << " -- " << result.detail << '\n';
        }
    }

    std::cout << "Total:      " << results.size() << '\n'
              << "Aprovados:  " << passed << '\n'
              << "Reprovados: " << failed << '\n'
              << "RESULTADO: "
              << (failed == 0 ? "TODOS OS TESTES PASSARAM."
                              : "HA TESTES REPROVADOS.")
              << '\n';

    return failed == 0 ? 0 : 1;
}
