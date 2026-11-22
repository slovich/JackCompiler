#include "jack/vm/vmcodegenerator.h"

#include "jack/ast/arrayaccessnode.h"
#include "jack/ast/binaryexpressionnode.h"
#include "jack/ast/classnode.h"
#include "jack/ast/dostatementnode.h"
#include "jack/ast/expressionnode.h"
#include "jack/ast/ifstatementnode.h"
#include "jack/ast/integerliteralnode.h"
#include "jack/ast/keywordconstantnode.h"
#include "jack/ast/letstatementnode.h"
#include "jack/ast/parameternode.h"
#include "jack/ast/parenthesizedexpressionnode.h"
#include "jack/ast/returnstatementnode.h"
#include "jack/ast/stringliteralnode.h"
#include "jack/ast/statementnode.h"
#include "jack/ast/subroutinecallnode.h"
#include "jack/ast/subroutinedeclarationnode.h"
#include "jack/ast/unaryexpressionnode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/variablereferencenode.h"
#include "jack/ast/whilestatementnode.h"
#include "jack/symbol/symboltable.h"
#include "jack/vm/vmwriter.h"

#include <cstdint>
#include <limits>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

struct GenerationFailure {
    VMGenerationErrorCategory category;
    std::string message;
    SourceSpan span{};
};

class ClassGenerator {
public:
    ClassGenerator(const ClassNode& node, VMWriter& vmWriter)
        : classNode(node), writer(vmWriter)
    {
    }

    void run()
    {
        symbols.iniciarClasse();
        className = classNode.obterNome();
        if (className.empty()) {
            fail(classNode, VMGenerationErrorCategory::InvalidProgram,
                 "Nome de classe vazio.");
        }

        registerClassVariables();
        for (const auto& subroutine : classNode.obterSubrotinas()) {
            if (!subroutine) {
                fail(classNode, VMGenerationErrorCategory::InvalidProgram,
                     "Declaração de sub-rotina nula na AST.");
            }
            generateSubroutine(*subroutine);
        }
    }

private:
    const ClassNode& classNode;
    VMWriter& writer;
    SymbolTable symbols;
    std::string className;
    SubroutineKind currentKind = SubroutineKind::Function;
    std::size_t labelCounter = 0;

    [[noreturn]] static void fail(const ASTNode& node,
                                  VMGenerationErrorCategory category,
                                  const std::string& message)
    {
        throw GenerationFailure{category, message, node.obterSourceSpan()};
    }

    static int checkedInt(std::size_t value, const ASTNode& node,
                          const char* description)
    {
        if (value > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
            fail(node, VMGenerationErrorCategory::InvalidProgram,
                 std::string(description) + " excede o limite suportado.");
        }
        return static_cast<int>(value);
    }

    void registerClassVariables()
    {
        for (const auto& declaration : classNode.obterVariaveisClasse()) {
            if (!declaration) {
                fail(classNode, VMGenerationErrorCategory::InvalidProgram,
                     "Declaração de variável de classe nula na AST.");
            }
            SymbolCategory category;
            switch (declaration->obterKind()) {
            case VariableKind::Static: category = SymbolCategory::Static; break;
            case VariableKind::Field: category = SymbolCategory::Field; break;
            case VariableKind::Var:
                fail(*declaration, VMGenerationErrorCategory::InvalidProgram,
                     "Declaração local encontrada no escopo da classe.");
            }
            for (const std::string& name : declaration->obterNomes()) {
                if (!symbols.definir(name, declaration->obterTipo(), category)) {
                    fail(*declaration, VMGenerationErrorCategory::InvalidProgram,
                         "Declaração duplicada no escopo de classe: " + name);
                }
            }
        }
    }

    void generateSubroutine(const SubroutineDeclarationNode& subroutine)
    {
        currentKind = subroutine.obterKind();
        symbols.iniciarSubrotina(currentKind);

        for (const auto& parameter : subroutine.obterParametros()) {
            if (!parameter) {
                fail(subroutine, VMGenerationErrorCategory::InvalidProgram,
                     "Parâmetro nulo na AST.");
            }
            if (!symbols.definir(parameter->obterNome(), parameter->obterTipo(),
                                SymbolCategory::Argument)) {
                fail(*parameter, VMGenerationErrorCategory::InvalidProgram,
                     "Parâmetro duplicado: " + parameter->obterNome());
            }
        }

        for (const auto& declaration : subroutine.obterVariaveisLocais()) {
            if (!declaration) {
                fail(subroutine, VMGenerationErrorCategory::InvalidProgram,
                     "Declaração de variável local nula na AST.");
            }
            if (declaration->obterKind() != VariableKind::Var) {
                fail(*declaration, VMGenerationErrorCategory::InvalidProgram,
                     "Declaração não local encontrada no corpo da sub-rotina.");
            }
            for (const std::string& name : declaration->obterNomes()) {
                if (!symbols.definir(name, declaration->obterTipo(),
                                     SymbolCategory::Local)) {
                    fail(*declaration, VMGenerationErrorCategory::InvalidProgram,
                         "Identificador local duplicado: " + name);
                }
            }
        }

        const int localCount = checkedInt(
            symbols.quantidadeDe(SymbolCategory::Local), subroutine,
            "Quantidade de variáveis locais");
        writer.writeFunction(className + "." + subroutine.obterNome(), localCount);

        if (currentKind == SubroutineKind::Method) {
            writer.writePush("argument", 0);
            writer.writePop("pointer", 0);
        } else if (currentKind == SubroutineKind::Constructor) {
            const int fieldCount = checkedInt(
                symbols.quantidadeDe(SymbolCategory::Field), subroutine,
                "Quantidade de campos");
            writer.writePush("constant", fieldCount);
            writer.writeCall("Memory.alloc", 1);
            writer.writePop("pointer", 0);
        }

        for (const auto& statement : subroutine.obterCorpo()) {
            if (!statement) {
                fail(subroutine, VMGenerationErrorCategory::InvalidProgram,
                     "Instrução nula no corpo da sub-rotina.");
            }
            generateStatement(*statement);
        }
    }

    std::string nextLabel(const std::string& prefix)
    {
        return prefix + std::to_string(labelCounter++);
    }

    Symbol resolveSymbol(const std::string& name, const ASTNode& node)
    {
        const auto symbol = symbols.resolver(name);
        if (!symbol) {
            fail(node, VMGenerationErrorCategory::InvalidProgram,
                 "Identificador não declarado: " + name);
        }
        if (currentKind == SubroutineKind::Function
            && symbol->category == SymbolCategory::Field) {
            fail(node, VMGenerationErrorCategory::InvalidProgram,
                 "Acesso a campo de instância em function estática: " + name);
        }
        return *symbol;
    }

    void pushSymbol(const Symbol& symbol)
    {
        writer.writePush(symbol.segmentoVM(), checkedInt(
            symbol.index, classNode, "Índice de símbolo"));
    }

    void popSymbol(const Symbol& symbol)
    {
        writer.writePop(symbol.segmentoVM(), checkedInt(
            symbol.index, classNode, "Índice de símbolo"));
    }

    void generateStatement(const StatementNode& statement)
    {
        if (const auto* let = dynamic_cast<const LetStatementNode*>(&statement)) {
            generateLet(*let);
            return;
        }
        if (const auto* conditional = dynamic_cast<const IfStatementNode*>(&statement)) {
            generateIf(*conditional);
            return;
        }
        if (const auto* loop = dynamic_cast<const WhileStatementNode*>(&statement)) {
            generateWhile(*loop);
            return;
        }
        if (const auto* doStatement = dynamic_cast<const DoStatementNode*>(&statement)) {
            generateCall(doStatement->obterChamada());
            writer.writePop("temp", 0);
            return;
        }
        if (const auto* returnStatement =
                dynamic_cast<const ReturnStatementNode*>(&statement)) {
            if (returnStatement->possuiExpressao()) {
                generateExpression(*returnStatement->obterExpressao());
            } else {
                writer.writePush("constant", 0);
            }
            writer.writeReturn();
            return;
        }
        fail(statement, VMGenerationErrorCategory::UnsupportedNode,
             "Tipo de instrução não suportado pelo VMCodeGenerator.");
    }

    void generateLet(const LetStatementNode& statement)
    {
        if (const auto* variable =
                dynamic_cast<const VariableReferenceNode*>(&statement.obterAlvo())) {
            generateExpression(statement.obterValor());
            popSymbol(resolveSymbol(variable->obterNome(), *variable));
            return;
        }
        if (const auto* array =
                dynamic_cast<const ArrayAccessNode*>(&statement.obterAlvo())) {
            const Symbol base = resolveSymbol(array->obterNome(), *array);
            pushSymbol(base);
            generateExpression(array->obterIndice());
            writer.writeArithmetic("add");
            // O endereço permanece na pilha enquanto o RHS é avaliado.
            generateExpression(statement.obterValor());
            writer.writePop("temp", 0);
            writer.writePop("pointer", 1);
            writer.writePush("temp", 0);
            writer.writePop("that", 0);
            return;
        }
        fail(statement.obterAlvo(), VMGenerationErrorCategory::UnsupportedNode,
             "Alvo de let não é uma variável nem um acesso a array.");
    }

    void generateIf(const IfStatementNode& statement)
    {
        const std::string falseLabel = nextLabel("IF_FALSE");
        const std::string endLabel = statement.possuiElse()
                                         ? nextLabel("IF_END") : falseLabel;
        generateExpression(statement.obterCondicao());
        writer.writeArithmetic("not");
        writer.writeIf(falseLabel);
        for (const auto& child : statement.obterCorpoThen()) {
            if (!child) fail(statement, VMGenerationErrorCategory::InvalidProgram,
                             "Instrução nula no bloco then.");
            generateStatement(*child);
        }
        if (statement.possuiElse()) {
            writer.writeGoto(endLabel);
            writer.writeLabel(falseLabel);
            for (const auto& child : statement.obterCorpoElse()) {
                if (!child) fail(statement, VMGenerationErrorCategory::InvalidProgram,
                                 "Instrução nula no bloco else.");
                generateStatement(*child);
            }
            writer.writeLabel(endLabel);
        } else {
            writer.writeLabel(falseLabel);
        }
    }

    void generateWhile(const WhileStatementNode& statement)
    {
        const std::string expressionLabel = nextLabel("WHILE_EXP");
        const std::string endLabel = nextLabel("WHILE_END");
        writer.writeLabel(expressionLabel);
        generateExpression(statement.obterCondicao());
        writer.writeArithmetic("not");
        writer.writeIf(endLabel);
        for (const auto& child : statement.obterCorpo()) {
            if (!child) fail(statement, VMGenerationErrorCategory::InvalidProgram,
                             "Instrução nula no corpo do while.");
            generateStatement(*child);
        }
        writer.writeGoto(expressionLabel);
        writer.writeLabel(endLabel);
    }

    void generateExpression(const ExpressionNode& expression)
    {
        if (const auto* integer = dynamic_cast<const IntegerLiteralNode*>(&expression)) {
            const int value = integer->obterValor();
            if (value < 0 || value > 32767) {
                fail(*integer, VMGenerationErrorCategory::InvalidProgram,
                     "Constante inteira fora do intervalo VM: " + integer->obterLexema());
            }
            writer.writePush("constant", value);
            return;
        }
        if (const auto* keyword = dynamic_cast<const KeywordConstantNode*>(&expression)) {
            switch (keyword->obterValor()) {
            case KeywordConstant::False:
            case KeywordConstant::Null:
                writer.writePush("constant", 0);
                return;
            case KeywordConstant::True:
                writer.writePush("constant", 1);
                writer.writeArithmetic("neg");
                return;
            case KeywordConstant::This:
                if (currentKind == SubroutineKind::Function) {
                    fail(*keyword, VMGenerationErrorCategory::InvalidProgram,
                         "Uso de this em uma function estática.");
                }
                writer.writePush("pointer", 0);
                return;
            }
        }
        if (const auto* variable =
                dynamic_cast<const VariableReferenceNode*>(&expression)) {
            pushSymbol(resolveSymbol(variable->obterNome(), *variable));
            return;
        }
        if (const auto* array = dynamic_cast<const ArrayAccessNode*>(&expression)) {
            const Symbol base = resolveSymbol(array->obterNome(), *array);
            pushSymbol(base);
            generateExpression(array->obterIndice());
            writer.writeArithmetic("add");
            writer.writePop("pointer", 1);
            writer.writePush("that", 0);
            return;
        }
        if (const auto* binary =
                dynamic_cast<const BinaryExpressionNode*>(&expression)) {
            generateExpression(binary->obterOperandoEsquerdo());
            generateExpression(binary->obterOperandoDireito());
            switch (binary->obterOperador()) {
            case BinaryOperator::Add: writer.writeArithmetic("add"); break;
            case BinaryOperator::Subtract: writer.writeArithmetic("sub"); break;
            case BinaryOperator::Multiply: writer.writeCall("Math.multiply", 2); break;
            case BinaryOperator::Divide: writer.writeCall("Math.divide", 2); break;
            case BinaryOperator::And: writer.writeArithmetic("and"); break;
            case BinaryOperator::Or: writer.writeArithmetic("or"); break;
            case BinaryOperator::Equal: writer.writeArithmetic("eq"); break;
            case BinaryOperator::LessThan: writer.writeArithmetic("lt"); break;
            case BinaryOperator::GreaterThan: writer.writeArithmetic("gt"); break;
            }
            return;
        }
        if (const auto* unary =
                dynamic_cast<const UnaryExpressionNode*>(&expression)) {
            generateExpression(unary->obterOperando());
            switch (unary->obterOperador()) {
            case UnaryOperator::Negate: writer.writeArithmetic("neg"); break;
            case UnaryOperator::Not: writer.writeArithmetic("not"); break;
            }
            return;
        }
        if (const auto* parenthesized =
                dynamic_cast<const ParenthesizedExpressionNode*>(&expression)) {
            generateExpression(parenthesized->obterExpressao());
            return;
        }
        if (const auto* string = dynamic_cast<const StringLiteralNode*>(&expression)) {
            generateString(*string);
            return;
        }
        if (const auto* call = dynamic_cast<const SubroutineCallNode*>(&expression)) {
            generateCall(*call);
            return;
        }
        fail(expression, VMGenerationErrorCategory::UnsupportedNode,
             "Tipo de expressão não suportado pelo VMCodeGenerator.");
    }

    static std::vector<std::uint32_t> decodeUtf8(const std::string& value,
                                                  const ASTNode& node)
    {
        std::vector<std::uint32_t> result;
        for (std::size_t i = 0; i < value.size();) {
            const unsigned char first = static_cast<unsigned char>(value[i]);
            std::uint32_t codepoint = 0;
            std::size_t length = 0;
            if (first <= 0x7f) { codepoint = first; length = 1; }
            else if ((first & 0xe0) == 0xc0) { codepoint = first & 0x1f; length = 2; }
            else if ((first & 0xf0) == 0xe0) { codepoint = first & 0x0f; length = 3; }
            else if ((first & 0xf8) == 0xf0) { codepoint = first & 0x07; length = 4; }
            else fail(node, VMGenerationErrorCategory::InvalidProgram,
                      "String contém sequência UTF-8 inválida.");
            if (i + length > value.size()) {
                fail(node, VMGenerationErrorCategory::InvalidProgram,
                     "String contém sequência UTF-8 incompleta.");
            }
            for (std::size_t j = 1; j < length; ++j) {
                const unsigned char continuation = static_cast<unsigned char>(value[i + j]);
                if ((continuation & 0xc0) != 0x80) {
                    fail(node, VMGenerationErrorCategory::InvalidProgram,
                         "String contém sequência UTF-8 inválida.");
                }
                codepoint = (codepoint << 6) | (continuation & 0x3f);
            }
            const bool overlong = (length == 2 && codepoint < 0x80)
                                  || (length == 3 && codepoint < 0x800)
                                  || (length == 4 && codepoint < 0x10000);
            if (overlong || codepoint > 32767
                || (codepoint >= 0xd800 && codepoint <= 0xdfff)) {
                fail(node, VMGenerationErrorCategory::InvalidProgram,
                     "Caractere de string fora do intervalo suportado pela VM Jack.");
            }
            result.push_back(codepoint);
            i += length;
        }
        return result;
    }

    void generateString(const StringLiteralNode& string)
    {
        const auto characters = decodeUtf8(string.obterValor(), string);
        writer.writePush("constant", checkedInt(characters.size(), string,
                                                  "Comprimento da string"));
        writer.writeCall("String.new", 1);
        for (std::uint32_t character : characters) {
            writer.writePush("constant", static_cast<int>(character));
            writer.writeCall("String.appendChar", 2);
        }
    }

    void generateCall(const SubroutineCallNode& call)
    {
        const auto& qualifier = call.obterQualificador();
        std::string targetClass;
        int implicitArguments = 0;

        if (qualifier) {
            const auto symbol = symbols.resolver(*qualifier);
            if (symbol) {
                if (currentKind == SubroutineKind::Function
                    && symbol->category == SymbolCategory::Field) {
                    fail(call, VMGenerationErrorCategory::InvalidProgram,
                         "Chamada por campo de instância em function estática: "
                         + *qualifier);
                }
                pushSymbol(*symbol);
                targetClass = symbol->type;
                implicitArguments = 1;
            } else {
                targetClass = *qualifier;
            }
        } else {
            if (currentKind == SubroutineKind::Function) {
                fail(call, VMGenerationErrorCategory::InvalidProgram,
                     "Chamada sem qualificador em function estática: "
                     + call.obterNomeSubrotina());
            }
            writer.writePush("pointer", 0);
            targetClass = className;
            implicitArguments = 1;
        }

        for (const auto& argument : call.obterArgumentos()) {
            if (!argument) {
                fail(call, VMGenerationErrorCategory::InvalidProgram,
                     "Argumento nulo em chamada de sub-rotina.");
            }
            generateExpression(*argument);
        }
        const std::size_t total = call.obterArgumentos().size()
                                  + static_cast<std::size_t>(implicitArguments);
        writer.writeCall(targetClass + "." + call.obterNomeSubrotina(),
                         checkedInt(total, call, "Quantidade de argumentos"));
    }
};

} // namespace

VMGenerationResult VMCodeGenerator::generate(const ClassNode& classNode,
                                              std::ostream& output)
{
    VMGenerationResult result;
    std::ostringstream buffer;
    try {
        VMWriter writer(buffer);
        ClassGenerator generator(classNode, writer);
        generator.run();
        writer.finalizar();

        const std::string generated = buffer.str();
        output.write(generated.data(), static_cast<std::streamsize>(generated.size()));
        if (!output) {
            result.diagnostics.push_back({
                VMGenerationErrorCategory::OutputFailure,
                "Falha ao publicar o código VM gerado no fluxo de saída.",
                classNode.obterSourceSpan().startLine,
                classNode.obterSourceSpan().startColumn});
            return result;
        }
        result.success = true;
    } catch (const GenerationFailure& failure) {
        result.diagnostics.push_back({failure.category, failure.message,
                                      failure.span.startLine,
                                      failure.span.startColumn});
    } catch (const std::ios_base::failure& failure) {
        result.diagnostics.push_back({
            VMGenerationErrorCategory::OutputFailure,
            failure.what(), classNode.obterSourceSpan().startLine,
            classNode.obterSourceSpan().startColumn});
    } catch (const std::exception& failure) {
        result.diagnostics.push_back({
            VMGenerationErrorCategory::InvalidProgram,
            failure.what(), classNode.obterSourceSpan().startLine,
            classNode.obterSourceSpan().startColumn});
    }
    return result;
}
