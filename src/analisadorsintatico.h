#ifndef ANALISADORSINTATICO_H
#define ANALISADORSINTATICO_H

#include <string>
#include <vector>
#include <optional>
#include <memory>

#include "token.h"
#include "jack/parser/syntaxresult.h"
#include "jack/ast/classnode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/subroutinedeclarationnode.h"
#include "jack/ast/parameternode.h"
#include "jack/ast/statementnode.h"
#include "jack/ast/expressionnode.h"
#include "jack/ast/integerliteralnode.h"
#include "jack/ast/stringliteralnode.h"
#include "jack/ast/keywordconstantnode.h"
#include "jack/ast/variablereferencenode.h"
#include "jack/ast/arrayaccessnode.h"
#include "jack/ast/binaryexpressionnode.h"
#include "jack/ast/unaryexpressionnode.h"
#include "jack/ast/subroutinecallnode.h"
#include "jack/ast/letstatementnode.h"
#include "jack/ast/dostatementnode.h"
#include "jack/ast/returnstatementnode.h"
#include "jack/ast/whilestatementnode.h"
#include "jack/ast/ifstatementnode.h"
#include "jack/ast/programnode.h"
#include "jack/ast/parenthesizedexpressionnode.h"

class AnalisadorSintatico
{
public:
    explicit AnalisadorSintatico(
        const std::vector<Token>& tokens,
        const std::string& codigoFonte
        );

    SyntaxResult analisar();

private:
    std::vector<Token> tokens;

    std::string codigoFonte;

    int indiceAtual;

    std::optional<SyntaxFailure> erroSintatico;

    std::unique_ptr<ClassNode> classeAtual;

    std::vector<std::unique_ptr<VariableDeclarationNode>>
        variaveisClasseAtuais;

    std::vector<std::unique_ptr<SubroutineDeclarationNode>>
        subrotinasAtuais;

    std::vector<std::unique_ptr<ParameterNode>>
        parametrosAtuais;

    std::vector<std::unique_ptr<VariableDeclarationNode>>
        variaveisLocaisAtuais;

    std::vector<std::unique_ptr<StatementNode>>
        instrucoesAtuais;

    // Acesso aos tokens
    const Token* tokenAtual() const;
    const Token* peek() const;

    // Reconhecimento de tokens
    bool match(TipoToken tipo) const;
    bool match(TipoToken tipo, const std::string& lexema) const;

    bool consume(TipoToken tipo);
    bool consume(TipoToken tipo, const std::string& lexema);

    void advance();

    // Localização e tratamento de erros
    SourceSpan obterSourceSpanToken(const Token& token) const;
    SourceSpan obterPosicaoAtual() const;
    void registrarErro(const std::string& mensagem);

    // Regras da gramática
    bool compileClass();
    bool compileClassVarDec();
    bool compileSubroutine();
    bool compileParameterList();
    bool compileSubroutineBody();
    bool compileVarDec();
    std::unique_ptr<LetStatementNode> compileLet();
    std::unique_ptr<IfStatementNode> compileIf();
    std::unique_ptr<DoStatementNode> compileDo();
    std::unique_ptr<WhileStatementNode> compileWhile();
    std::unique_ptr<ReturnStatementNode> compileReturn();
    std::unique_ptr<ExpressionNode> compileExpression();
    std::vector<std::unique_ptr<ExpressionNode>> compileExpressionList();
    std::unique_ptr<ExpressionNode> compileTerm();
    bool compileStatements(
        std::vector<std::unique_ptr<StatementNode>>& instrucoes
        );
};

#endif // ANALISADORSINTATICO_H
