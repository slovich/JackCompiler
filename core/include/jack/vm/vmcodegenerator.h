#ifndef JACK_VM_VMCODEGENERATOR_H
#define JACK_VM_VMCODEGENERATOR_H

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

class ClassNode;

/** Categoria do diagnóstico produzido durante a geração de código VM. */
enum class VMGenerationErrorCategory {
    InvalidProgram,
    UnsupportedNode,
    OutputFailure
};

struct VMGenerationDiagnostic {
    VMGenerationErrorCategory category;
    std::string message;
    std::size_t line = 0;
    std::size_t column = 0;
};

struct VMGenerationResult {
    bool success = false;
    std::vector<VMGenerationDiagnostic> diagnostics;

    explicit operator bool() const noexcept { return success; }
};

/** Traduz uma classe Jack da AST para VM, sem dependência de Qt. */
class VMCodeGenerator {
public:
    VMCodeGenerator() = default;

    /**
     * Gera uma classe por chamada. A saída só é copiada para output depois
     * que toda a classe foi traduzida e o VMWriter temporário foi finalizado.
     */
    VMGenerationResult generate(const ClassNode& classNode,
                                std::ostream& output);
};

#endif // JACK_VM_VMCODEGENERATOR_H
