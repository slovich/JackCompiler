#include "analisadorsintatico.h"

AnalisadorSintatico::AnalisadorSintatico(const std::vector<Token>& tokens)
    : tokens(tokens),
    indiceAtual(0)
{
}

const Token* AnalisadorSintatico::tokenAtual() const
{
    if (indiceAtual < 0
        || indiceAtual >= static_cast<int>(tokens.size())) {
        return nullptr;
    }

    return &tokens[indiceAtual];
}

const Token* AnalisadorSintatico::peek() const
{
    const int proximoIndice = indiceAtual + 1;

    if (proximoIndice < 0
        || proximoIndice >= static_cast<int>(tokens.size())) {
        return nullptr;
    }

    return &tokens[proximoIndice];
}

bool AnalisadorSintatico::match(TipoToken tipo) const
{
    const Token* token = tokenAtual();
    return token != nullptr && token->tipo == tipo;
}

bool AnalisadorSintatico::match(
    TipoToken tipo,
    const std::string& lexema) const
{
    const Token* token = tokenAtual();

    return token != nullptr
           && token->tipo == tipo
           && token->lexema == lexema;
}

void AnalisadorSintatico::advance()
{
    if (indiceAtual < static_cast<int>(tokens.size())) {
        ++indiceAtual;
    }
}

bool AnalisadorSintatico::consume(TipoToken tipo)
{
    if (match(tipo)) {
        const Token* token = tokenAtual();

        if (token != nullptr) {
            adicionarTokenXML(*token);
        }

        advance();
        return true;
    }

    const Token* token = tokenAtual();

    if (token != nullptr) {
        erro = "Erro sintático na linha "
               + std::to_string(token->linha)
               + ", coluna "
               + std::to_string(token->coluna)
               + ": token inesperado '"
               + token->lexema
               + "'.";
    } else {
        erro = "Erro sintático: fim inesperado dos tokens.";
    }

    return false;
}

bool AnalisadorSintatico::consume(
    TipoToken tipo,
    const std::string& lexema)
{
    if (match(tipo, lexema)) {
        const Token* token = tokenAtual();

        if (token != nullptr) {
            adicionarTokenXML(*token);
        }

        advance();
        return true;
    }

    const Token* token = tokenAtual();

    if (token != nullptr) {
        erro = "Erro sintático na linha "
               + std::to_string(token->linha)
               + ", coluna "
               + std::to_string(token->coluna)
               + ": esperado '"
               + lexema
               + "', encontrado '"
               + token->lexema
               + "'.";
    } else {
        erro = "Erro sintático: esperado '"
               + lexema
               + "', mas o arquivo terminou.";
    }

    return false;
}

void AnalisadorSintatico::abrirTag(const std::string& tag)
{
    xml += "<" + tag + ">\n";
}

void AnalisadorSintatico::fecharTag(const std::string& tag)
{
    xml += "</" + tag + ">\n";
}

std::string AnalisadorSintatico::escaparXML(const std::string& texto) const
{
    std::string resultado;
    resultado.reserve(texto.size());

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
        case '"':
            resultado += "&quot;";
            break;
        default:
            resultado += caractere;
            break;
        }
    }

    return resultado;
}

void AnalisadorSintatico::adicionarTokenXML(const Token& token)
{
    std::string nomeTag;

    switch (token.tipo) {
    case TipoToken::Keyword:
        nomeTag = "keyword";
        break;
    case TipoToken::Symbol:
        nomeTag = "symbol";
        break;
    case TipoToken::IntegerConstant:
        nomeTag = "integerConstant";
        break;
    case TipoToken::StringConstant:
        nomeTag = "stringConstant";
        break;
    case TipoToken::Identifier:
        nomeTag = "identifier";
        break;
    }

    xml += "<" + nomeTag + "> "
           + escaparXML(token.lexema)
           + " </" + nomeTag + ">\n";
}

bool AnalisadorSintatico::compileClassVarDec()
{
    abrirTag("classVarDec");

    // ('static' | 'field')
    if (!match(TipoToken::Keyword, "static")
        && !match(TipoToken::Keyword, "field")) {
        erro = (
            "Erro sintático: esperada a palavra-chave 'static' ou 'field'.");
        return false;
    }

    const Token *token = tokenAtual();
    adicionarTokenXML(*token);
    advance();

    // type: 'int' | 'char' | 'boolean' | className
    if (match(TipoToken::Keyword, "int")
        || match(TipoToken::Keyword, "char")
        || match(TipoToken::Keyword, "boolean")) {

        token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else if (match(TipoToken::Identifier)) {

        token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else {
        erro = (
            "Erro sintático: esperado um tipo válido na declaração de variável de classe.");
        return false;
    }

    // varName
    if (!consume(TipoToken::Identifier)) {
        return false;
    }

    // (',' varName)*
    while (match(TipoToken::Symbol, ",")) {
        if (!consume(TipoToken::Symbol, ",")) {
            return false;
        }

        if (!consume(TipoToken::Identifier)) {
            return false;
        }
    }

    // ';'
    if (!consume(TipoToken::Symbol, ";")) {
        return false;
    }

    fecharTag("classVarDec");

    return true;
}

bool AnalisadorSintatico::compileExpression()
{
    abrirTag("expression");

    // O primeiro termo é obrigatório.
    if (!compileTerm()) {
        return false;
    }

    // Operadores binários da gramática Jack:
    // + - * / & | < > =
    while (match(TipoToken::Symbol, "+")
           || match(TipoToken::Symbol, "-")
           || match(TipoToken::Symbol, "*")
           || match(TipoToken::Symbol, "/")
           || match(TipoToken::Symbol, "&")
           || match(TipoToken::Symbol, "|")
           || match(TipoToken::Symbol, "<")
           || match(TipoToken::Symbol, ">")
           || match(TipoToken::Symbol, "=")) {

        // Consome e registra o operador.
        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

        // Cada operador binário deve ser seguido por outro termo.
        if (!compileTerm()) {
            return false;
        }
    }

    fecharTag("expression");

    return true;
}

bool AnalisadorSintatico::compileExpressionList()
{
    abrirTag("expressionList");

    // A lista pode estar vazia.
    if (!match(TipoToken::Symbol, ")")) {

        // Analisa a primeira expressão.
        if (!compileExpression()) {
            return false;
        }

        // Analisa as expressões seguintes, separadas por vírgulas.
        while (match(TipoToken::Symbol, ",")) {

            if (!consume(TipoToken::Symbol, ",")) {
                return false;
            }

            if (!compileExpression()) {
                return false;
            }
        }
    }

    fecharTag("expressionList");

    return true;
}

bool AnalisadorSintatico::compileTerm()
{
    abrirTag("term");

    // integerConstant
    if (match(TipoToken::IntegerConstant)) {
        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

        // stringConstant
    } else if (match(TipoToken::StringConstant)) {
        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

        // keywordConstant: true, false, null, this
    } else if (match(TipoToken::Keyword, "true")
               || match(TipoToken::Keyword, "false")
               || match(TipoToken::Keyword, "null")
               || match(TipoToken::Keyword, "this")) {

        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

        // unaryOp term: '-' term ou '~' term
    } else if (match(TipoToken::Symbol, "-")
               || match(TipoToken::Symbol, "~")) {

        // Registra e consome o operador unário.
        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

        // O operador deve ser seguido por outro termo.
        if (!compileTerm()) {
            return false;
        }

        // '(' expression ')'
    } else if (match(TipoToken::Symbol, "(")) {

        if (!consume(TipoToken::Symbol, "(")) {
            return false;
        }

        if (!compileExpression()) {
            return false;
        }

        if (!consume(TipoToken::Symbol, ")")) {
            return false;
        }

        // identifier: varName, varName '[' expression ']',
        // subroutineName '(' expressionList ')' ou
        // (className | varName) '.' subroutineName '(' expressionList ')'
    } else if (match(TipoToken::Identifier)) {

        // Registra e consome o primeiro identificador.
        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

        // Caso 1: acesso a elemento de array.
        if (match(TipoToken::Symbol, "[")) {

            if (!consume(TipoToken::Symbol, "[")) {
                return false;
            }

            if (!compileExpression()) {
                return false;
            }

            if (!consume(TipoToken::Symbol, "]")) {
                return false;
            }

            // Caso 2: chamada qualificada, como Math.multiply(...).
        } else if (match(TipoToken::Symbol, ".")) {

            if (!consume(TipoToken::Symbol, ".")) {
                return false;
            }

            if (!consume(TipoToken::Identifier)) {
                return false;
            }

            if (!consume(TipoToken::Symbol, "(")) {
                return false;
            }

            if (!compileExpressionList()) {
                return false;
            }

            if (!consume(TipoToken::Symbol, ")")) {
                return false;
            }

            // Caso 3: chamada direta, como obterValor(...).
        } else if (match(TipoToken::Symbol, "(")) {

            if (!consume(TipoToken::Symbol, "(")) {
                return false;
            }

            if (!compileExpressionList()) {
                return false;
            }

            if (!consume(TipoToken::Symbol, ")")) {
                return false;
            }

        }

    } else {
        // Se nenhuma alternativa da regra term corresponder ao token atual,
        // a expressão contém um termo inválido.
        const Token *token = tokenAtual();

        if (token != nullptr) {
            erro = "Erro sintático na linha "
                   + std::to_string(token->linha)
                   + ", coluna "
                   + std::to_string(token->coluna)
                   + ": termo inválido, encontrado '"
                   + token->lexema
                   + "'.";
        } else {
            erro = (
                "Erro sintático: esperado um termo, "
                "mas o arquivo terminou.");
        }

        return false;
    }

    fecharTag("term");

    return true;
}

bool AnalisadorSintatico::compileLet()
{
    abrirTag("letStatement");

    // 'let'
    if (!consume(TipoToken::Keyword, "let")) {
        return false;
    }

    // varName
    if (!consume(TipoToken::Identifier)) {
        return false;
    }

    // Acesso opcional a elemento de array: varName[expression]
    if (match(TipoToken::Symbol, "[")) {

        // '['
        if (!consume(TipoToken::Symbol, "[")) {
            return false;
        }

        // expression: índice do array
        if (!compileExpression()) {
            return false;
        }

        // ']'
        if (!consume(TipoToken::Symbol, "]")) {
            return false;
        }
    }

    // '='
    if (!consume(TipoToken::Symbol, "=")) {
        return false;
    }

    // expression: valor atribuído
    if (!compileExpression()) {
        return false;
    }

    // ';'
    if (!consume(TipoToken::Symbol, ";")) {
        return false;
    }

    fecharTag("letStatement");

    return true;
}

bool AnalisadorSintatico::compileIf()
{
    abrirTag("ifStatement");

    // Consome a palavra-chave "if".
    if (!consume(TipoToken::Keyword, "if")) {
        return false;
    }

    // Consome "(".
    if (!consume(TipoToken::Symbol, "(")) {
        return false;
    }

    // Analisa a condição.
    if (!compileExpression()) {
        return false;
    }

    // Consome ")".
    if (!consume(TipoToken::Symbol, ")")) {
        return false;
    }

    // Consome "{".
    if (!consume(TipoToken::Symbol, "{")) {
        return false;
    }

    // Analisa os comandos dentro do bloco.
    if (!compileStatements()) {
        return false;
    }

    // Consome "}".
    if (!consume(TipoToken::Symbol, "}")) {
        return false;
    }

    // Verifica a presença opcional de "else".
    if (match(TipoToken::Keyword, "else")) {

        if (!consume(TipoToken::Keyword, "else")) {
            return false;
        }

        if (!consume(TipoToken::Symbol, "{")) {
            return false;
        }

        if (!compileStatements()) {
            return false;
        }

        if (!consume(TipoToken::Symbol, "}")) {
            return false;
        }
    }

    fecharTag("ifStatement");

    return true;
}

bool AnalisadorSintatico::compileDo()
{
    abrirTag("doStatement");

    // Consome a palavra-chave "do".
    if (!consume(TipoToken::Keyword, "do")) {
        return false;
    }

    // Consome o primeiro identificador:
    // nome da sub-rotina, classe ou variável.
    if (!consume(TipoToken::Identifier)) {
        return false;
    }

    // Verifica se a chamada possui um qualificador:
    // ClassName.subroutineName ou varName.methodName.
    if (match(TipoToken::Symbol, ".")) {
        if (!consume(TipoToken::Symbol, ".")) {
            return false;
        }

        if (!consume(TipoToken::Identifier)) {
            return false;
        }
    }

    // Abre a lista de expressões.
    if (!consume(TipoToken::Symbol, "(")) {
        return false;
    }

    // Analisa a lista de argumentos da chamada.
    if (!compileExpressionList()) {
        return false;
    }

    // Fecha a chamada.
    if (!consume(TipoToken::Symbol, ")")) {
        return false;
    }

    // Todo comando do termina com ponto e vírgula.
    if (!consume(TipoToken::Symbol, ";")) {
        return false;
    }

    fecharTag("doStatement");

    return true;
}

bool AnalisadorSintatico::compileWhile()
{
    abrirTag("whileStatement");

    // Consome a palavra-chave "while".
    if (!consume(TipoToken::Keyword, "while")) {
        return false;
    }

    // Consome "(".
    if (!consume(TipoToken::Symbol, "(")) {
        return false;
    }

    // Analisa a condição do laço.
    if (!compileExpression()) {
        return false;
    }

    // Consome ")".
    if (!consume(TipoToken::Symbol, ")")) {
        return false;
    }

    // Consome "{".
    if (!consume(TipoToken::Symbol, "{")) {
        return false;
    }

    // Analisa os comandos dentro do laço.
    if (!compileStatements()) {
        return false;
    }

    // Consome "}".
    if (!consume(TipoToken::Symbol, "}")) {
        return false;
    }

    fecharTag("whileStatement");

    return true;
}

bool AnalisadorSintatico::compileReturn()
{
    abrirTag("returnStatement");

    // Consome a palavra-chave "return".
    if (!consume(TipoToken::Keyword, "return")) {
        return false;
    }

    // A expressão de retorno é opcional.
    if (!match(TipoToken::Symbol, ";")) {
        if (!compileExpression()) {
            return false;
        }
    }

    // Todo comando return termina com ponto e vírgula.
    if (!consume(TipoToken::Symbol, ";")) {
        return false;
    }

    fecharTag("returnStatement");

    return true;
}

bool AnalisadorSintatico::compileStatements()
{
    abrirTag("statements");

    while (match(TipoToken::Keyword, "let")
           || match(TipoToken::Keyword, "if")
           || match(TipoToken::Keyword, "while")
           || match(TipoToken::Keyword, "do")
           || match(TipoToken::Keyword, "return")) {

        if (match(TipoToken::Keyword, "let")) {
            if (!compileLet()) {
                return false;
            }
        } else if (match(TipoToken::Keyword, "if")) {
            if (!compileIf()) {
                return false;
            }
        } else if (match(TipoToken::Keyword, "while")) {
            if (!compileWhile()) {
                return false;
            }
        } else if (match(TipoToken::Keyword, "do")) {
            if (!compileDo()) {
                return false;
            }
        } else if (match(TipoToken::Keyword, "return")) {
            if (!compileReturn()) {
                return false;
            }
        }
    }

    fecharTag("statements");

    return true;
}

bool AnalisadorSintatico::compileSubroutineBody()
{
    abrirTag("subroutineBody");

    // '{'
    if (!consume(TipoToken::Symbol, "{")) {
        return false;
    }

    // varDec*
    while (match(TipoToken::Keyword, "var")) {
        if (!compileVarDec()) {
            return false;
        }
    }

    if (!compileStatements()) {
        return false;
    }

    // '}'
    if (!consume(TipoToken::Symbol, "}")) {
        return false;
    }

    fecharTag("subroutineBody");

    return true;
}

bool AnalisadorSintatico::compileVarDec()
{
    abrirTag("varDec");

    // 'var'
    if (!consume(TipoToken::Keyword, "var")) {
        return false;
    }

    // type: 'int' | 'char' | 'boolean' | className
    if (match(TipoToken::Keyword, "int")
        || match(TipoToken::Keyword, "char")
        || match(TipoToken::Keyword, "boolean")) {

        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else if (match(TipoToken::Identifier)) {

        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else {
        erro = (
            "Erro sintático: esperado um tipo válido na declaração de variável local.");
        return false;
    }

    // varName
    if (!consume(TipoToken::Identifier)) {
        return false;
    }

    // (',' varName)*
    while (match(TipoToken::Symbol, ",")) {

        if (!consume(TipoToken::Symbol, ",")) {
            return false;
        }

        if (!consume(TipoToken::Identifier)) {
            return false;
        }
    }

    // ';'
    if (!consume(TipoToken::Symbol, ";")) {
        return false;
    }

    fecharTag("varDec");

    return true;
}

bool AnalisadorSintatico::compileSubroutine()
{
    abrirTag("subroutineDec");

    // ('constructor' | 'function' | 'method')
    if (match(TipoToken::Keyword, "constructor")
        || match(TipoToken::Keyword, "function")
        || match(TipoToken::Keyword, "method")) {

        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else {
        erro = (
            "Erro sintático: esperado 'constructor', 'function' ou 'method'.");
        return false;
    }

    // ('void' | type)
    if (match(TipoToken::Keyword, "void")
        || match(TipoToken::Keyword, "int")
        || match(TipoToken::Keyword, "char")
        || match(TipoToken::Keyword, "boolean")) {

        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else if (match(TipoToken::Identifier)) {

        const Token *token = tokenAtual();
        adicionarTokenXML(*token);
        advance();

    } else {
        erro = (
            "Erro sintático: esperado um tipo de retorno válido.");
        return false;
    }

    // subroutineName
    if (!consume(TipoToken::Identifier)) {
        return false;
    }

    // '('
    if (!consume(TipoToken::Symbol, "(")) {
        return false;
    }

    // parameterList
    if (!compileParameterList()) {
        return false;
    }

    // ')'
    if (!consume(TipoToken::Symbol, ")")) {
        return false;
    }

    // subroutineBody
    if (!compileSubroutineBody()) {
        return false;
    }

    fecharTag("subroutineDec");

    return true;
}

bool AnalisadorSintatico::compileParameterList()
{
    abrirTag("parameterList");

    // Uma lista de parâmetros pode estar vazia.
    if (!match(TipoToken::Symbol, ")")) {

        // type: 'int' | 'char' | 'boolean' | className
        if (match(TipoToken::Keyword, "int")
            || match(TipoToken::Keyword, "char")
            || match(TipoToken::Keyword, "boolean")) {

            const Token *token = tokenAtual();
            adicionarTokenXML(*token);
            advance();

        } else if (match(TipoToken::Identifier)) {

            const Token *token = tokenAtual();
            adicionarTokenXML(*token);
            advance();

        } else {
            erro = (
                "Erro sintático: esperado um tipo válido na lista de parâmetros.");
            return false;
        }

        // varName
        if (!consume(TipoToken::Identifier)) {
            return false;
        }

        // (',' type varName)*
        while (match(TipoToken::Symbol, ",")) {

            if (!consume(TipoToken::Symbol, ",")) {
                return false;
            }

            if (match(TipoToken::Keyword, "int")
                || match(TipoToken::Keyword, "char")
                || match(TipoToken::Keyword, "boolean")) {

                const Token *token = tokenAtual();
                adicionarTokenXML(*token);
                advance();

            } else if (match(TipoToken::Identifier)) {

                const Token *token = tokenAtual();
                adicionarTokenXML(*token);
                advance();

            } else {
                erro = (
                    "Erro sintático: esperado um tipo válido após a vírgula.");
                return false;
            }

            if (!consume(TipoToken::Identifier)) {
                return false;
            }
        }
    }

    fecharTag("parameterList");

    return true;
}

bool AnalisadorSintatico::compileClass()
{
    abrirTag("class");

    // 'class'
    if (!consume(TipoToken::Keyword, "class")) {
        return false;
    }

    // className
    if (!consume(TipoToken::Identifier)) {
        return false;
    }

    // '{'
    if (!consume(TipoToken::Symbol, "{")) {
        return false;
    }

    // classVarDec*
    while (match(TipoToken::Keyword, "static")
           || match(TipoToken::Keyword, "field")) {
        if (!compileClassVarDec()) {
            return false;
        }
    }

    // subroutineDec*
    while (match(TipoToken::Keyword, "constructor")
           || match(TipoToken::Keyword, "function")
           || match(TipoToken::Keyword, "method")) {
        if (!compileSubroutine()) {
            return false;
        }
    }

    // '}'
    if (!consume(TipoToken::Symbol, "}")) {
        return false;
    }

    fecharTag("class");

    return true;
}

bool AnalisadorSintatico::analisar()
{
    indiceAtual = 0;
    xml.clear();
    erro.clear();

    if (tokens.empty()) {
        erro = (
            "Erro sintático: não há tokens para analisar.");

        return false;
    }

    if (!compileClass()) {
        xml.clear();
        return false;
    }

    // Não permitir tokens extras após a classe.
    if (tokenAtual() != nullptr) {
        const Token *token = tokenAtual();

        erro = "Erro sintático na linha "
               + std::to_string(token->linha)
               + ", coluna "
               + std::to_string(token->coluna)
               + ": token inesperado '"
               + token->lexema
               + "' após o fim da classe.";

        xml.clear();
        return false;
    }

    return true;
}

std::string AnalisadorSintatico::obterXML() const
{
    return xml;
}

std::string AnalisadorSintatico::obterErro() const
{
    return erro;
}