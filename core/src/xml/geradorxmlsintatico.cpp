#include "jack/xml/geradorxmlsintatico.h"

#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include "jack/ast/programnode.h"
#include "jack/ast/classnode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/variablekind.h"
#include "jack/ast/subroutinedeclarationnode.h"
#include "jack/ast/subroutinekind.h"
#include "jack/ast/parameternode.h"
#include "jack/ast/letstatementnode.h"
#include "jack/ast/ifstatementnode.h"
#include "jack/ast/whilestatementnode.h"
#include "jack/ast/dostatementnode.h"
#include "jack/ast/returnstatementnode.h"
#include "jack/ast/statementnode.h"
#include "jack/ast/variablereferencenode.h"
#include "jack/ast/arrayaccessnode.h"
#include "jack/ast/expressionnode.h"
#include "jack/ast/binaryexpressionnode.h"
#include "jack/ast/binaryoperator.h"
#include "jack/ast/unaryexpressionnode.h"
#include "jack/ast/unaryoperator.h"
#include "jack/ast/integerliteralnode.h"
#include "jack/ast/stringliteralnode.h"
#include "jack/ast/keywordconstantnode.h"
#include "jack/ast/keywordconstant.h"
#include "jack/ast/parenthesizedexpressionnode.h"
#include "jack/ast/subroutinecallnode.h"

namespace {

std::string escaparTextoXml(const std::string& texto)
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

class EscritorXml
{
public:
    void abrirElemento(const std::string& nome)
    {
        escreverIndentacao();
        fluxo << "<" << nome << ">\n";
        ++nivelIndentacao;
    }

    void fecharElemento(const std::string& nome)
    {
        --nivelIndentacao;
        escreverIndentacao();
        fluxo << "</" << nome << ">\n";
    }

    void escreverTerminal(
        const std::string& tipo,
        const std::string& valor)
    {
        escreverIndentacao();

        fluxo << "<" << tipo << "> "
              << escaparTextoXml(valor)
              << " </" << tipo << ">\n";
    }

    std::string obterResultado() const
    {
        return fluxo.str();
    }

private:
    std::ostringstream fluxo;
    int nivelIndentacao = 0;

    void escreverIndentacao()
    {
        for (int i = 0; i < nivelIndentacao; ++i) {
            fluxo << "  ";
        }
    }
};

void gerarTipo(
    EscritorXml& escritor,
    const std::string& tipo)
{
    if (tipo == "int"
        || tipo == "char"
        || tipo == "boolean"
        || tipo == "void") {
        escritor.escreverTerminal("keyword", tipo);
    } else {
        escritor.escreverTerminal("identifier", tipo);
    }
}

void gerarListaParametros(
    EscritorXml& escritor,
    const std::vector<std::unique_ptr<ParameterNode>>& parametros)
{
    escritor.abrirElemento("parameterList");

    for (std::size_t i = 0; i < parametros.size(); ++i) {
        const auto& parametro = parametros[i];

        gerarTipo(escritor, parametro->obterTipo());
        escritor.escreverTerminal(
            "identifier",
            parametro->obterNome());

        if (i + 1 < parametros.size()) {
            escritor.escreverTerminal("symbol", ",");
        }
    }

    escritor.fecharElemento("parameterList");
}

void gerarTipoSubrotina(
    EscritorXml& escritor,
    SubroutineKind kind)
{
    switch (kind) {
    case SubroutineKind::Constructor:
        escritor.escreverTerminal("keyword", "constructor");
        break;

    case SubroutineKind::Function:
        escritor.escreverTerminal("keyword", "function");
        break;

    case SubroutineKind::Method:
        escritor.escreverTerminal("keyword", "method");
        break;
    }
}

void gerarOperadorBinario(
    EscritorXml& escritor,
    BinaryOperator operador)
{
    switch (operador) {
    case BinaryOperator::Add:
        escritor.escreverTerminal("symbol", "+");
        break;

    case BinaryOperator::Subtract:
        escritor.escreverTerminal("symbol", "-");
        break;

    case BinaryOperator::Multiply:
        escritor.escreverTerminal("symbol", "*");
        break;

    case BinaryOperator::Divide:
        escritor.escreverTerminal("symbol", "/");
        break;

    case BinaryOperator::And:
        escritor.escreverTerminal("symbol", "&");
        break;

    case BinaryOperator::Or:
        escritor.escreverTerminal("symbol", "|");
        break;

    case BinaryOperator::Equal:
        escritor.escreverTerminal("symbol", "=");
        break;

    case BinaryOperator::LessThan:
        escritor.escreverTerminal("symbol", "<");
        break;

    case BinaryOperator::GreaterThan:
        escritor.escreverTerminal("symbol", ">");
        break;
    }
}

void gerarOperadorUnario(
    EscritorXml& escritor,
    UnaryOperator operador)
{
    switch (operador) {
    case UnaryOperator::Negate:
        escritor.escreverTerminal("symbol", "-");
        break;

    case UnaryOperator::Not:
        escritor.escreverTerminal("symbol", "~");
        break;
    }
}

void gerarExpression(
    EscritorXml& escritor,
    const ExpressionNode& expressao);

void gerarStatement(
    EscritorXml& escritor,
    const StatementNode& instrucao);

void gerarCorpoSubrotina(
    EscritorXml& escritor,
    const SubroutineDeclarationNode& subrotina);

void gerarListaExpressoes(
    EscritorXml& escritor,
    const std::vector<std::unique_ptr<ExpressionNode>>& argumentos)
{
    escritor.abrirElemento("expressionList");

    for (std::size_t i = 0; i < argumentos.size(); ++i) {
        gerarExpression(escritor, *argumentos[i]);

        if (i + 1 < argumentos.size()) {
            escritor.escreverTerminal("symbol", ",");
        }
    }

    escritor.fecharElemento("expressionList");
}

void gerarSubroutineCall(
    EscritorXml& escritor,
    const SubroutineCallNode& chamada)
{
    if (chamada.obterQualificador().has_value()) {
        escritor.escreverTerminal(
            "identifier",
            chamada.obterQualificador().value());

        escritor.escreverTerminal("symbol", ".");
    }

    escritor.escreverTerminal(
        "identifier",
        chamada.obterNomeSubrotina());

    escritor.escreverTerminal("symbol", "(");

    gerarListaExpressoes(escritor, chamada.obterArgumentos());

    escritor.escreverTerminal("symbol", ")");
}

void gerarTerm(
    EscritorXml& escritor,
    const ExpressionNode& expressao)
{
    escritor.abrirElemento("term");

    if (const auto* inteiro =
        dynamic_cast<const IntegerLiteralNode*>(&expressao)) {

        escritor.escreverTerminal(
            "integerConstant",
            inteiro->obterLexema());

    } else if (const auto* texto =
               dynamic_cast<const StringLiteralNode*>(&expressao)) {

        escritor.escreverTerminal(
            "stringConstant",
            texto->obterValor());

    } else if (const auto* constante =
               dynamic_cast<const KeywordConstantNode*>(&expressao)) {

        std::string valor;

        switch (constante->obterValor()) {
        case KeywordConstant::True:
            valor = "true";
            break;

        case KeywordConstant::False:
            valor = "false";
            break;

        case KeywordConstant::Null:
            valor = "null";
            break;

        case KeywordConstant::This:
            valor = "this";
            break;
        }

        escritor.escreverTerminal("keyword", valor);

    } else if (const auto* variavel =
               dynamic_cast<const VariableReferenceNode*>(&expressao)) {

        escritor.escreverTerminal(
            "identifier",
            variavel->obterNome());

    } else if (const auto* acesso =
               dynamic_cast<const ArrayAccessNode*>(&expressao)) {

        escritor.escreverTerminal(
            "identifier",
            acesso->obterNome());

        escritor.escreverTerminal("symbol", "[");

        gerarExpression(escritor, acesso->obterIndice());

        escritor.escreverTerminal("symbol", "]");

    } else if (const auto* chamada =
               dynamic_cast<const SubroutineCallNode*>(&expressao)) {

        gerarSubroutineCall(escritor, *chamada);

    } else if (const auto* parenteses =
               dynamic_cast<const ParenthesizedExpressionNode*>(&expressao)) {

        escritor.escreverTerminal("symbol", "(");

        gerarExpression(escritor, parenteses->obterExpressao());

        escritor.escreverTerminal("symbol", ")");

    } else if (const auto* unaria =
               dynamic_cast<const UnaryExpressionNode*>(&expressao)) {

        gerarOperadorUnario(escritor, unaria->obterOperador());

        gerarTerm(escritor, unaria->obterOperando());
    }

    escritor.fecharElemento("term");
}

void gerarExpression(
    EscritorXml& escritor,
    const ExpressionNode& expressao)
{
    escritor.abrirElemento("expression");

    std::vector<const BinaryExpressionNode*> operacoes;

    const ExpressionNode* atual = &expressao;

    // Percorre a árvore da direita para a esquerda,
    // armazenando os nós de operação encontrados.
    while (const auto* binaria =
           dynamic_cast<const BinaryExpressionNode*>(atual)) {

        operacoes.push_back(binaria);

        atual = &binaria->obterOperandoEsquerdo();
    }

    // Em uma árvore associativa à esquerda, a ordem
    // original dos elementos é recuperada invertendo
    // a sequência das operações armazenadas.
    gerarTerm(escritor, *atual);

    for (auto it = operacoes.rbegin();
         it != operacoes.rend();
         ++it) {

        gerarOperadorBinario(escritor, (*it)->obterOperador());

        gerarTerm(
            escritor,
            (*it)->obterOperandoDireito());
    }

    escritor.fecharElemento("expression");
}

void gerarCabecalhoSubrotina(
    EscritorXml& escritor,
    const SubroutineDeclarationNode& subrotina)
{
    escritor.abrirElemento("subroutineDec");

    gerarTipoSubrotina(escritor, subrotina.obterKind());

    gerarTipo(escritor, subrotina.obterTipoRetorno());

    escritor.escreverTerminal(
        "identifier",
        subrotina.obterNome());

    escritor.escreverTerminal("symbol", "(");

    gerarListaParametros(escritor, subrotina.obterParametros());

    escritor.escreverTerminal("symbol", ")");

    gerarCorpoSubrotina(escritor, subrotina);

    escritor.fecharElemento("subroutineDec");
}

void gerarDeclaracaoVariavelClasse(
    EscritorXml& escritor,
    const VariableDeclarationNode& declaracao)
{
    escritor.abrirElemento("classVarDec");

    if (declaracao.obterKind() == VariableKind::Static) {
        escritor.escreverTerminal("keyword", "static");
    } else {
        escritor.escreverTerminal("keyword", "field");
    }

    gerarTipo(escritor, declaracao.obterTipo());

    const auto& nomes = declaracao.obterNomes();

    for (std::size_t i = 0; i < nomes.size(); ++i) {

        escritor.escreverTerminal("identifier", nomes[i]);

        if (i + 1 < nomes.size()) {
            escritor.escreverTerminal("symbol", ",");
        }
    }

    escritor.escreverTerminal("symbol", ";");

    escritor.fecharElemento("classVarDec");
}

void gerarDeclaracaoVariavelLocal(
    EscritorXml& escritor,
    const VariableDeclarationNode& declaracao)
{
    escritor.abrirElemento("varDec");

    escritor.escreverTerminal("keyword", "var");

    gerarTipo(escritor, declaracao.obterTipo());

    const auto& nomes = declaracao.obterNomes();

    for (std::size_t i = 0; i < nomes.size(); ++i) {
        escritor.escreverTerminal("identifier", nomes[i]);

        if (i + 1 < nomes.size()) {
            escritor.escreverTerminal("symbol", ",");
        }
    }

    escritor.escreverTerminal("symbol", ";");

    escritor.fecharElemento("varDec");
}

void gerarLetStatement(
    EscritorXml& escritor,
    const LetStatementNode& instrucao)
{
    escritor.abrirElemento("letStatement");

    escritor.escreverTerminal("keyword", "let");

    const ExpressionNode& alvo = instrucao.obterAlvo();

    if (const auto* acesso =
        dynamic_cast<const ArrayAccessNode*>(&alvo)) {

        escritor.escreverTerminal(
            "identifier",
            acesso->obterNome());

        escritor.escreverTerminal("symbol", "[");

        gerarExpression(escritor, acesso->obterIndice());

        escritor.escreverTerminal("symbol", "]");

    } else if (const auto* variavel =
               dynamic_cast<const VariableReferenceNode*>(&alvo)) {

        escritor.escreverTerminal(
            "identifier",
            variavel->obterNome());
    }

    escritor.escreverTerminal("symbol", "=");

    gerarExpression(escritor, instrucao.obterValor());

    escritor.escreverTerminal("symbol", ";");

    escritor.fecharElemento("letStatement");
}

void gerarDoStatement(
    EscritorXml& escritor,
    const DoStatementNode& instrucao)
{
    escritor.abrirElemento("doStatement");

    escritor.escreverTerminal("keyword", "do");

    gerarSubroutineCall(escritor, instrucao.obterChamada());

    escritor.escreverTerminal("symbol", ";");

    escritor.fecharElemento("doStatement");
}

void gerarReturnStatement(
    EscritorXml& escritor,
    const ReturnStatementNode& instrucao)
{
    escritor.abrirElemento("returnStatement");

    escritor.escreverTerminal("keyword", "return");

    if (instrucao.possuiExpressao()) {
        gerarExpression(escritor, *instrucao.obterExpressao());
    }

    escritor.escreverTerminal("symbol", ";");

    escritor.fecharElemento("returnStatement");
}

void gerarWhileStatement(
    EscritorXml& escritor,
    const WhileStatementNode& instrucao)
{
    escritor.abrirElemento("whileStatement");

    escritor.escreverTerminal("keyword", "while");
    escritor.escreverTerminal("symbol", "(");

    gerarExpression(escritor, instrucao.obterCondicao());

    escritor.escreverTerminal("symbol", ")");
    escritor.escreverTerminal("symbol", "{");

    escritor.abrirElemento("statements");

    for (const auto& instrucaoInterna : instrucao.obterCorpo()) {
        gerarStatement(escritor, *instrucaoInterna);
    }

    escritor.fecharElemento("statements");

    escritor.escreverTerminal("symbol", "}");

    escritor.fecharElemento("whileStatement");
}

void gerarIfStatement(
    EscritorXml& escritor,
    const IfStatementNode& instrucao)
{
    escritor.abrirElemento("ifStatement");

    escritor.escreverTerminal("keyword", "if");
    escritor.escreverTerminal("symbol", "(");

    gerarExpression(escritor, instrucao.obterCondicao());

    escritor.escreverTerminal("symbol", ")");
    escritor.escreverTerminal("symbol", "{");

    escritor.abrirElemento("statements");

    for (const auto& instrucaoInterna : instrucao.obterCorpoThen()) {
        gerarStatement(escritor, *instrucaoInterna);
    }

    escritor.fecharElemento("statements");

    escritor.escreverTerminal("symbol", "}");

    if (instrucao.possuiElse()) {
        escritor.escreverTerminal("keyword", "else");
        escritor.escreverTerminal("symbol", "{");

        escritor.abrirElemento("statements");

        for (const auto& instrucaoInterna : instrucao.obterCorpoElse()) {
            gerarStatement(escritor, *instrucaoInterna);
        }

        escritor.fecharElemento("statements");

        escritor.escreverTerminal("symbol", "}");
    }

    escritor.fecharElemento("ifStatement");
}

void gerarStatement(
    EscritorXml& escritor,
    const StatementNode& instrucao)
{
    if (const auto* let =
        dynamic_cast<const LetStatementNode*>(&instrucao)) {

        gerarLetStatement(escritor, *let);

    } else if (const auto* ifStatement =
               dynamic_cast<const IfStatementNode*>(&instrucao)) {

        gerarIfStatement(escritor, *ifStatement);

    } else if (const auto* whileStatement =
               dynamic_cast<const WhileStatementNode*>(&instrucao)) {

        gerarWhileStatement(escritor, *whileStatement);

    } else if (const auto* doStatement =
               dynamic_cast<const DoStatementNode*>(&instrucao)) {

        gerarDoStatement(escritor, *doStatement);

    } else if (const auto* returnStatement =
               dynamic_cast<const ReturnStatementNode*>(&instrucao)) {

        gerarReturnStatement(escritor, *returnStatement);
    }
}

void gerarCorpoSubrotina(
    EscritorXml& escritor,
    const SubroutineDeclarationNode& subrotina)
{
    escritor.abrirElemento("subroutineBody");

    escritor.escreverTerminal("symbol", "{");

    for (const auto& variavel : subrotina.obterVariaveisLocais()) {
        gerarDeclaracaoVariavelLocal(escritor, *variavel);
    }

    escritor.abrirElemento("statements");

    for (const auto& instrucao : subrotina.obterCorpo()) {
        gerarStatement(escritor, *instrucao);
    }

    escritor.fecharElemento("statements");

    escritor.escreverTerminal("symbol", "}");

    escritor.fecharElemento("subroutineBody");
}


void gerarClasse(
    EscritorXml& escritor,
    const ClassNode& classe)
{
    escritor.abrirElemento("class");

    escritor.escreverTerminal("keyword", "class");
    escritor.escreverTerminal("identifier", classe.obterNome());
    escritor.escreverTerminal("symbol", "{");

    // Declarações de variáveis da classe
    for (const auto& variavel : classe.obterVariaveisClasse()) {
        gerarDeclaracaoVariavelClasse(escritor, *variavel);
    }

    // Declarações de subrotinas
    for (const auto& subrotina : classe.obterSubrotinas()) {
        gerarCabecalhoSubrotina(escritor, *subrotina);
    }

    escritor.escreverTerminal("symbol", "}");

    escritor.fecharElemento("class");
}


}

std::string GeradorXmlSintatico::gerar(
    const ProgramNode& programa) const
{
    EscritorXml escritor;

    for (const auto& classe : programa.obterClasses()) {
        gerarClasse(escritor, *classe);
    }

    return escritor.obterResultado();
}