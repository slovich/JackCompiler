#ifndef JACK_VM_VMWRITER_H
#define JACK_VM_VMWRITER_H

#include <ios>
#include <ostream>
#include <string>

/**
 * Emite instrucoes da linguagem VM do Nand2Tetris em um fluxo existente.
 *
 * O VMWriter nao assume propriedade do fluxo e nao conhece AST, SymbolTable
 * ou caminhos do sistema de arquivos. Cada instrucao e validada e montada
 * integralmente antes de ser enviada ao fluxo.
 */
class VMWriter {
public:
    explicit VMWriter(std::ostream& output);

    VMWriter(const VMWriter&) = delete;
    VMWriter& operator=(const VMWriter&) = delete;
    VMWriter(VMWriter&&) = delete;
    VMWriter& operator=(VMWriter&&) = delete;

    void writePush(const std::string& segment, int index);
    void writePop(const std::string& segment, int index);

    void writeArithmetic(const std::string& command);

    void writeLabel(const std::string& name);
    void writeGoto(const std::string& name);
    void writeIf(const std::string& name);

    void writeCall(const std::string& name, int argumentCount);
    void writeFunction(const std::string& name, int localCount);
    void writeReturn();

    /**
     * Descarrega o fluxo e encerra a emissao.
     *
     * Pode ser chamada mais de uma vez; chamadas posteriores nao alteram
     * o fluxo. Falhas de escrita sao propagadas como std::ios_base::failure.
     */
    void finalizar();

    bool finalizado() const noexcept;

private:
    void ensureWritable();
    void writeInstruction(const std::string& instruction);
    static void validateName(const std::string& name, const char* description);
    static void validateIndex(const std::string& segment, int index,
                              bool isPush);

    std::ostream& output;
    bool finalized = false;
    bool failed = false;
};

#endif // JACK_VM_VMWRITER_H
