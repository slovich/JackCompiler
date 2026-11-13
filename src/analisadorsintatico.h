#ifndef ANALISADORSINTATICO_H
#define ANALISADORSINTATICO_H

#include <string>
#include <vector>

#include "token.h"

class AnalisadorSintatico
{
public:
    explicit AnalisadorSintatico(const std::vector<Token>& tokens);

    bool analisar();

    std::string obterXML() const;
    std::string obterErro() const;

private:
    std::vector<Token> tokens;
    int indiceAtual;

    std::string xml;
    std::string erro;

    // Acesso aos tokens
    const Token* tokenAtual() const;
    const Token* peek() const;

    // Reconhecimento de tokens
    bool match(TipoToken tipo) const;
    bool match(TipoToken tipo, const std::string& lexema) const;

    bool consume(TipoToken tipo);
    bool consume(TipoToken tipo, const std::string& lexema);

    void advance();

    // Construção do XML
    void abrirTag(const std::string& tag);
    void fecharTag(const std::string& tag);
    void adicionarTokenXML(const Token& token);
    std::string escaparXML(const std::string& texto) const;

    // Regras da gramática
    bool compileClass();
    bool compileClassVarDec();
    bool compileSubroutine();
    bool compileParameterList();
    bool compileSubroutineBody();
    bool compileVarDec();
    bool compileLet();
    bool compileIf();
    bool compileDo();
    bool compileWhile();
    bool compileReturn();
    bool compileExpression();
    bool compileExpressionList();
    bool compileTerm();
    bool compileStatements();
};

#endif // ANALISADORSINTATICO_H