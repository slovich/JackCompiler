#include "analisadorsintatico.h"

AnalisadorSintatico::AnalisadorSintatico(
    const std::vector<Token>& tokens,
    const std::string& codigoFonte
    )
    : tokens(tokens),
    codigoFonte(codigoFonte),
    indiceAtual(0),
    erroSintatico(std::nullopt)
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

SourceSpan AnalisadorSintatico::obterSourceSpanToken(
    const Token& token) const
{
    const std::size_t colunaInicial =
        static_cast<std::size_t>(token.coluna) + 1;

    std::size_t tamanhoToken = token.lexema.size();

    // O analisador léxico remove as aspas do lexema.
    // Elas precisam ser consideradas na localização original.
    if (token.tipo == TipoToken::StringConstant) {
        tamanhoToken += 2;
    }

    return SourceSpan{
        static_cast<std::size_t>(token.linha),
        colunaInicial,
        static_cast<std::size_t>(token.linha),
        colunaInicial + tamanhoToken - 1
    };
}

SourceSpan AnalisadorSintatico::obterPosicaoAtual() const
{
    const Token* token = tokenAtual();

    if (token != nullptr) {
        return obterSourceSpanToken(*token);
    }

    std::size_t linha = 1;
    std::size_t coluna = 1;

    for (char caractere : codigoFonte) {
        if (caractere == '\n') {
            ++linha;
            coluna = 1;
        } else {
            ++coluna;
        }
    }

    return SourceSpan{
        linha,
        coluna,
        linha,
        coluna
    };
}

void AnalisadorSintatico::registrarErro(
    const std::string& mensagem)
{
    if (!erroSintatico.has_value()) {
        erroSintatico = SyntaxFailure{
            mensagem,
            obterPosicaoAtual()
        };
    }
}

bool AnalisadorSintatico::consume(TipoToken tipo)
{
    if (match(tipo)) {
        advance();
        return true;
    }

    const Token* token = tokenAtual();

    if (token != nullptr) {
        registrarErro(
            "Token inesperado '" + token->lexema + "'."
            );
    } else {
        registrarErro(
            "Fim inesperado do arquivo."
            );
    }

    return false;
}

bool AnalisadorSintatico::consume(
    TipoToken tipo,
    const std::string& lexema)
{
    if (match(tipo, lexema)) {
        advance();
        return true;
    }

    const Token* token = tokenAtual();

    if (token != nullptr) {
        registrarErro(
            "Esperado '" + lexema
            + "', encontrado '" + token->lexema + "'."
            );
    } else {
        registrarErro(
            "Esperado '" + lexema
            + "', mas o arquivo terminou."
            );
    }

    return false;
}

bool AnalisadorSintatico::compileClassVarDec()
{
    // Registra o início da declaração.
    const Token* tokenInicial = tokenAtual();

    if (tokenInicial == nullptr) {
        registrarErro("Declaração de variável de classe inesperada.");
        return false;
    }

    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenInicial);

    // Identifica o tipo de variável: static ou field.
    VariableKind kind;

    if (match(TipoToken::Keyword, "static")) {
        kind = VariableKind::Static;
    } else if (match(TipoToken::Keyword, "field")) {
        kind = VariableKind::Field;
    } else {
        registrarErro(
            "Esperada a palavra-chave 'static' ou 'field'."
            );
        return false;
    }

    advance();

    // Captura o tipo: int, char, boolean ou nome de classe.
    std::string tipo;

    if (match(TipoToken::Keyword, "int")
        || match(TipoToken::Keyword, "char")
        || match(TipoToken::Keyword, "boolean")
        || match(TipoToken::Identifier)) {

        tipo = tokenAtual()->lexema;
        advance();

    } else {
        registrarErro(
            "Esperado um tipo válido na declaração de variável de classe."
            );
        return false;
    }

    // Captura os nomes das variáveis.
    std::vector<std::string> nomes;

    while (true) {
        if (!match(TipoToken::Identifier)) {
            registrarErro(
                "Esperado um identificador na declaração de variável de classe."
                );
            return false;
        }

        nomes.push_back(tokenAtual()->lexema);
        advance();

        if (!match(TipoToken::Symbol, ",")) {
            break;
        }

        advance();
    }

    // Registra a posição final da declaração.
    if (!match(TipoToken::Symbol, ";")) {
        registrarErro("Esperado ';' ao final da declaração de variável de classe.");
        return false;
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenAtual());

    advance();

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    // Constrói o nó da declaração.
    auto declaracao = std::make_unique<VariableDeclarationNode>(
        sourceSpan,
        kind,
        tipo,
        nomes
        );

    variaveisClasseAtuais.push_back(std::move(declaracao));

    return true;
}

std::unique_ptr<ExpressionNode> AnalisadorSintatico::compileExpression()
{
    // Analisa o primeiro termo da expressão.
    std::unique_ptr<ExpressionNode> expressaoEsquerda = compileTerm();

    if (!expressaoEsquerda) {
        return nullptr;
    }

    // Operadores binários da gramática Jack.
    while (match(TipoToken::Symbol, "+")
           || match(TipoToken::Symbol, "-")
           || match(TipoToken::Symbol, "*")
           || match(TipoToken::Symbol, "/")
           || match(TipoToken::Symbol, "&")
           || match(TipoToken::Symbol, "|")
           || match(TipoToken::Symbol, "<")
           || match(TipoToken::Symbol, ">")
           || match(TipoToken::Symbol, "=")) {

        const Token* tokenOperador = tokenAtual();

        BinaryOperator operador;

        if (tokenOperador->lexema == "+") {
            operador = BinaryOperator::Add;
        } else if (tokenOperador->lexema == "-") {
            operador = BinaryOperator::Subtract;
        } else if (tokenOperador->lexema == "*") {
            operador = BinaryOperator::Multiply;
        } else if (tokenOperador->lexema == "/") {
            operador = BinaryOperator::Divide;
        } else if (tokenOperador->lexema == "&") {
            operador = BinaryOperator::And;
        } else if (tokenOperador->lexema == "|") {
            operador = BinaryOperator::Or;
        } else if (tokenOperador->lexema == "<") {
            operador = BinaryOperator::LessThan;
        } else if (tokenOperador->lexema == ">") {
            operador = BinaryOperator::GreaterThan;
        } else {
            operador = BinaryOperator::Equal;
        }

        advance();

        // Analisa o termo à direita do operador.
        std::unique_ptr<ExpressionNode> expressaoDireita = compileTerm();

        if (!expressaoDireita) {
            return nullptr;
        }

        // O novo nó abrange os dois operandos.
        const SourceSpan spanEsquerda =
            expressaoEsquerda->obterSourceSpan();

        const SourceSpan spanDireita =
            expressaoDireita->obterSourceSpan();

        const SourceSpan sourceSpan{
            spanEsquerda.startLine,
            spanEsquerda.startColumn,
            spanDireita.endLine,
            spanDireita.endColumn
        };

        expressaoEsquerda = std::make_unique<BinaryExpressionNode>(
            sourceSpan,
            std::move(expressaoEsquerda),
            operador,
            std::move(expressaoDireita)
            );
    }

    return expressaoEsquerda;
}


std::vector<std::unique_ptr<ExpressionNode>>
AnalisadorSintatico::compileExpressionList()
{
    std::vector<std::unique_ptr<ExpressionNode>> argumentos;

    // A lista pode estar vazia.
    if (match(TipoToken::Symbol, ")")) {
        return argumentos;
    }

    // Analisa a primeira expressão.
    std::unique_ptr<ExpressionNode> expressao = compileExpression();

    if (!expressao) {
        return {};
    }

    argumentos.push_back(std::move(expressao));

    // Analisa as expressões seguintes, separadas por vírgulas.
    while (match(TipoToken::Symbol, ",")) {

        advance();

        expressao = compileExpression();

        if (!expressao) {
            return {};
        }

        argumentos.push_back(std::move(expressao));
    }

    return argumentos;
}


std::unique_ptr<ExpressionNode> AnalisadorSintatico::compileTerm()
{
    // integerConstant
    if (match(TipoToken::IntegerConstant)) {
        const Token* token = tokenAtual();
        const SourceSpan sourceSpan = obterSourceSpanToken(*token);

        const std::string lexema = token->lexema;
        const int valor = std::stoi(lexema);

        advance();

        return std::make_unique<IntegerLiteralNode>(
            sourceSpan,
            valor,
            lexema
            );
    }

    // stringConstant
    if (match(TipoToken::StringConstant)) {
        const Token* token = tokenAtual();
        const SourceSpan sourceSpan = obterSourceSpanToken(*token);

        const std::string valor = token->lexema;

        advance();

        return std::make_unique<StringLiteralNode>(
            sourceSpan,
            valor
            );
    }

    // keywordConstant: true, false, null, this
    if (match(TipoToken::Keyword, "true")
        || match(TipoToken::Keyword, "false")
        || match(TipoToken::Keyword, "null")
        || match(TipoToken::Keyword, "this")) {

        const Token* token = tokenAtual();
        const SourceSpan sourceSpan = obterSourceSpanToken(*token);

        KeywordConstant valor;

        if (token->lexema == "true") {
            valor = KeywordConstant::True;
        } else if (token->lexema == "false") {
            valor = KeywordConstant::False;
        } else if (token->lexema == "null") {
            valor = KeywordConstant::Null;
        } else {
            valor = KeywordConstant::This;
        }

        advance();

        return std::make_unique<KeywordConstantNode>(
            sourceSpan,
            valor
            );
    }

    // unaryOp term: '-' term ou '~' term
    if (match(TipoToken::Symbol, "-")
        || match(TipoToken::Symbol, "~")) {

        const Token* tokenOperador = tokenAtual();
        const SourceSpan spanOperador =
            obterSourceSpanToken(*tokenOperador);

        const UnaryOperator operador =
            tokenOperador->lexema == "-"
                ? UnaryOperator::Negate
                : UnaryOperator::Not;

        advance();

        std::unique_ptr<ExpressionNode> operando = compileTerm();

        if (!operando) {
            return nullptr;
        }

        const SourceSpan spanOperando =
            operando->obterSourceSpan();

        const SourceSpan sourceSpan{
            spanOperador.startLine,
            spanOperador.startColumn,
            spanOperando.endLine,
            spanOperando.endColumn
        };

        return std::make_unique<UnaryExpressionNode>(
            sourceSpan,
            operador,
            std::move(operando)
            );
    }

    // '(' expression ')'
    if (match(TipoToken::Symbol, "(")) {

        const Token* tokenAbertura = tokenAtual();
        const SourceSpan spanAbertura =
            obterSourceSpanToken(*tokenAbertura);

        advance();

        std::unique_ptr<ExpressionNode> expressao =
            compileExpression();

        if (!expressao) {
            return nullptr;
        }

        const Token* tokenFechamento = tokenAtual();

        if (!consume(TipoToken::Symbol, ")")) {
            return nullptr;
        }

        const SourceSpan spanFechamento =
            obterSourceSpanToken(*tokenFechamento);

        const SourceSpan sourceSpan{
            spanAbertura.startLine,
            spanAbertura.startColumn,
            spanFechamento.endLine,
            spanFechamento.endColumn
        };

        return std::make_unique<ParenthesizedExpressionNode>(
            sourceSpan,
            std::move(expressao)
            );
    }

    // identifier
    if (match(TipoToken::Identifier)) {

        const Token* tokenIdentificador = tokenAtual();

        const std::string nome = tokenIdentificador->lexema;
        const SourceSpan spanIdentificador =
            obterSourceSpanToken(*tokenIdentificador);

        advance();

        // Caso 1: acesso a elemento de array.
        if (match(TipoToken::Symbol, "[")) {

            advance();

            std::unique_ptr<ExpressionNode> indice =
                compileExpression();

            if (!indice) {
                return nullptr;
            }

            const Token* tokenFechamento = tokenAtual();

            if (!consume(TipoToken::Symbol, "]")) {
                return nullptr;
            }

            const SourceSpan spanFechamento =
                obterSourceSpanToken(*tokenFechamento);

            const SourceSpan sourceSpan{
                spanIdentificador.startLine,
                spanIdentificador.startColumn,
                spanFechamento.endLine,
                spanFechamento.endColumn
            };

            return std::make_unique<ArrayAccessNode>(
                sourceSpan,
                nome,
                std::move(indice)
                );
        }

        // Caso 2: chamada qualificada, como Math.multiply(...).
        if (match(TipoToken::Symbol, ".")) {

            advance();

            if (!match(TipoToken::Identifier)) {
                registrarErro("Nome da subrotina esperado.");
                return nullptr;
            }

            const Token* tokenSubrotina = tokenAtual();

            const std::string nomeSubrotina =
                tokenSubrotina->lexema;

            advance();

            if (!consume(TipoToken::Symbol, "(")) {
                return nullptr;
            }

            std::vector<std::unique_ptr<ExpressionNode>> argumentos =
                compileExpressionList();

            if (erroSintatico) {
                return nullptr;
            }

            const Token* tokenFechamento = tokenAtual();

            if (!consume(TipoToken::Symbol, ")")) {
                return nullptr;
            }

            const SourceSpan spanFechamento =
                obterSourceSpanToken(*tokenFechamento);

            const SourceSpan sourceSpan{
                spanIdentificador.startLine,
                spanIdentificador.startColumn,
                spanFechamento.endLine,
                spanFechamento.endColumn
            };

            auto chamada = std::make_unique<SubroutineCallNode>(
                sourceSpan,
                std::optional<std::string>{nome},
                nomeSubrotina
                );

            for (auto& argumento : argumentos) {
                chamada->adicionarArgumento(std::move(argumento));
            }

            return chamada;
        }

        // Caso 3: chamada direta, como obterValor(...).
        if (match(TipoToken::Symbol, "(")) {

            advance();

            std::vector<std::unique_ptr<ExpressionNode>> argumentos =
                compileExpressionList();

            if (erroSintatico) {
                return nullptr;
            }

            const Token* tokenFechamento = tokenAtual();

            if (!consume(TipoToken::Symbol, ")")) {
                return nullptr;
            }

            const SourceSpan spanFechamento =
                obterSourceSpanToken(*tokenFechamento);

            const SourceSpan sourceSpan{
                spanIdentificador.startLine,
                spanIdentificador.startColumn,
                spanFechamento.endLine,
                spanFechamento.endColumn
            };

            auto chamada = std::make_unique<SubroutineCallNode>(
                sourceSpan,
                std::nullopt,
                nome
                );

            for (auto& argumento : argumentos) {
                chamada->adicionarArgumento(std::move(argumento));
            }

            return chamada;
        }

        // Caso 4: referência simples a variável.
        return std::make_unique<VariableReferenceNode>(
            spanIdentificador,
            nome
            );
    }

    registrarErro("Termo inválido.");
    return nullptr;
}

std::unique_ptr<LetStatementNode> AnalisadorSintatico::compileLet()
{
    const Token* tokenLet = tokenAtual();
    const SourceSpan spanInicial = obterSourceSpanToken(*tokenLet);

    // 'let'
    if (!consume(TipoToken::Keyword, "let")) {
        return nullptr;
    }

    // varName
    if (!match(TipoToken::Identifier)) {
        registrarErro("Nome de variável esperado.");
        return nullptr;
    }

    const Token* tokenVariavel = tokenAtual();

    const std::string nomeVariavel = tokenVariavel->lexema;
    const SourceSpan spanVariavel =
        obterSourceSpanToken(*tokenVariavel);

    advance();

    std::unique_ptr<ExpressionNode> alvo;

    // Acesso opcional a elemento de array: varName[expression]
    if (match(TipoToken::Symbol, "[")) {

        advance();

        std::unique_ptr<ExpressionNode> indice =
            compileExpression();

        if (!indice) {
            return nullptr;
        }

        const Token* tokenFechamento = tokenAtual();

        if (!consume(TipoToken::Symbol, "]")) {
            return nullptr;
        }

        const SourceSpan spanFechamento =
            obterSourceSpanToken(*tokenFechamento);

        const SourceSpan spanAlvo{
            spanVariavel.startLine,
            spanVariavel.startColumn,
            spanFechamento.endLine,
            spanFechamento.endColumn
        };

        alvo = std::make_unique<ArrayAccessNode>(
            spanAlvo,
            nomeVariavel,
            std::move(indice)
            );

    } else {

        alvo = std::make_unique<VariableReferenceNode>(
            spanVariavel,
            nomeVariavel
            );
    }

    // '='
    if (!consume(TipoToken::Symbol, "=")) {
        return nullptr;
    }

    // expression: valor atribuído
    std::unique_ptr<ExpressionNode> valor =
        compileExpression();

    if (!valor) {
        return nullptr;
    }

    // ';'
    const Token* tokenPontoVirgula = tokenAtual();

    if (!consume(TipoToken::Symbol, ";")) {
        return nullptr;
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenPontoVirgula);

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    return std::make_unique<LetStatementNode>(
        sourceSpan,
        std::move(alvo),
        std::move(valor)
        );
}


std::unique_ptr<IfStatementNode> AnalisadorSintatico::compileIf()
{
    const Token* tokenIf = tokenAtual();
    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenIf);

    // Consome a palavra-chave "if".
    if (!consume(TipoToken::Keyword, "if")) {
        return nullptr;
    }

    // Consome "(".
    if (!consume(TipoToken::Symbol, "(")) {
        return nullptr;
    }

    // Analisa a condição.
    auto condicao = compileExpression();

    if (!condicao) {
        return nullptr;
    }

    // Consome ")".
    if (!consume(TipoToken::Symbol, ")")) {
        return nullptr;
    }

    // Consome "{" do bloco then.
    if (!consume(TipoToken::Symbol, "{")) {
        return nullptr;
    }

    // Analisa as instruções do bloco then.
    std::vector<std::unique_ptr<StatementNode>> instrucoesThen;

    if (!compileStatements(instrucoesThen)) {
        return nullptr;
    }

    // Consome "}" do bloco then.
    const Token* tokenFechamentoThen = tokenAtual();

    if (!consume(TipoToken::Symbol, "}")) {
        return nullptr;
    }

    SourceSpan spanFinal =
        obterSourceSpanToken(*tokenFechamentoThen);

    // Analisa o bloco else, caso exista.
    std::vector<std::unique_ptr<StatementNode>> instrucoesElse;
    bool possuiElse = false;

    if (match(TipoToken::Keyword, "else")) {
        if (!consume(TipoToken::Keyword, "else")) {
            return nullptr;
        }

        if (!consume(TipoToken::Symbol, "{")) {
            return nullptr;
        }

        possuiElse = true;

        if (!compileStatements(instrucoesElse)) {
            return nullptr;
        }

        const Token* tokenFechamentoElse = tokenAtual();

        if (!consume(TipoToken::Symbol, "}")) {
            return nullptr;
        }

        spanFinal = obterSourceSpanToken(*tokenFechamentoElse);
    }

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    auto ifStatement = std::make_unique<IfStatementNode>(
        sourceSpan,
        std::move(condicao)
        );

    if (possuiElse) {
        ifStatement->definirPossuiElse();
    }

    for (auto& instrucao : instrucoesThen) {
        ifStatement->adicionarInstrucaoThen(std::move(instrucao));
    }

    for (auto& instrucao : instrucoesElse) {
        ifStatement->adicionarInstrucaoElse(std::move(instrucao));
    }

    return ifStatement;
}


std::unique_ptr<DoStatementNode> AnalisadorSintatico::compileDo()
{
    const Token* tokenDo = tokenAtual();
    const SourceSpan spanInicial = obterSourceSpanToken(*tokenDo);

    // Consome a palavra-chave "do".
    if (!consume(TipoToken::Keyword, "do")) {
        return nullptr;
    }

    // Primeiro identificador: nome da subrotina ou qualificador.
    if (!match(TipoToken::Identifier)) {
        registrarErro("Nome da subrotina esperado.");
        return nullptr;
    }

    const Token* tokenIdentificador = tokenAtual();

    const std::string primeiroNome = tokenIdentificador->lexema;
    const SourceSpan spanInicioChamada =
        obterSourceSpanToken(*tokenIdentificador);

    advance();

    std::optional<std::string> qualificador;
    std::string nomeSubrotina;

    // Chamada qualificada: ClassName.subroutineName.
    if (match(TipoToken::Symbol, ".")) {

        advance();

        if (!match(TipoToken::Identifier)) {
            registrarErro("Nome da subrotina esperado após o qualificador.");
            return nullptr;
        }

        qualificador = primeiroNome;
        nomeSubrotina = tokenAtual()->lexema;

        advance();

    } else {

        // Chamada direta: subroutineName.
        nomeSubrotina = primeiroNome;
    }

    // Abre a lista de argumentos.
    if (!consume(TipoToken::Symbol, "(")) {
        return nullptr;
    }

    std::vector<std::unique_ptr<ExpressionNode>> argumentos =
        compileExpressionList();

    if (erroSintatico) {
        return nullptr;
    }

    // Fecha a chamada.
    const Token* tokenFechamentoChamada = tokenAtual();

    if (!consume(TipoToken::Symbol, ")")) {
        return nullptr;
    }

    const SourceSpan spanFechamentoChamada =
        obterSourceSpanToken(*tokenFechamentoChamada);

    // Todo comando do termina com ponto e vírgula.
    const Token* tokenPontoVirgula = tokenAtual();

    if (!consume(TipoToken::Symbol, ";")) {
        return nullptr;
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenPontoVirgula);

    // Intervalo da chamada de subrotina.
    const SourceSpan spanSubrotina{
        spanInicioChamada.startLine,
        spanInicioChamada.startColumn,
        spanFechamentoChamada.endLine,
        spanFechamentoChamada.endColumn
    };

    // Intervalo completo da instrução do.
    const SourceSpan spanChamada{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    // Constrói a chamada de subrotina.
    auto chamada = std::make_unique<SubroutineCallNode>(
        spanSubrotina,
        qualificador,
        nomeSubrotina
        );

    for (auto& argumento : argumentos) {
        chamada->adicionarArgumento(std::move(argumento));
    }

    // Constrói a instrução do.
    return std::make_unique<DoStatementNode>(
        spanChamada,
        std::move(chamada)
        );
}


std::unique_ptr<WhileStatementNode> AnalisadorSintatico::compileWhile()
{
    const Token* tokenWhile = tokenAtual();
    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenWhile);

    // Consome a palavra-chave "while".
    if (!consume(TipoToken::Keyword, "while")) {
        return nullptr;
    }

    // Consome "(".
    if (!consume(TipoToken::Symbol, "(")) {
        return nullptr;
    }

    // Analisa a condição do laço.
    auto condicao = compileExpression();

    if (!condicao) {
        return nullptr;
    }

    // Consome ")".
    if (!consume(TipoToken::Symbol, ")")) {
        return nullptr;
    }

    // Consome "{".
    if (!consume(TipoToken::Symbol, "{")) {
        return nullptr;
    }

    // Analisa os comandos dentro do laço.
    std::vector<std::unique_ptr<StatementNode>> instrucoes;

    if (!compileStatements(instrucoes)) {
        return nullptr;
    }

    // Consome "}".
    const Token* tokenFechamento = tokenAtual();

    if (!consume(TipoToken::Symbol, "}")) {
        return nullptr;
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenFechamento);

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    auto whileStatement = std::make_unique<WhileStatementNode>(
        sourceSpan,
        std::move(condicao)
        );

    for (auto& instrucao : instrucoes) {
        whileStatement->adicionarInstrucao(std::move(instrucao));
    }

    return whileStatement;
}


std::unique_ptr<ReturnStatementNode> AnalisadorSintatico::compileReturn()
{
    const Token* tokenReturn = tokenAtual();
    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenReturn);

    // Consome a palavra-chave "return".
    if (!consume(TipoToken::Keyword, "return")) {
        return nullptr;
    }

    std::unique_ptr<ExpressionNode> expressao;

    // A expressão de retorno é opcional.
    if (!match(TipoToken::Symbol, ";")) {

        expressao = compileExpression();

        if (!expressao) {
            return nullptr;
        }
    }

    // Todo comando return termina com ponto e vírgula.
    const Token* tokenPontoVirgula = tokenAtual();

    if (!consume(TipoToken::Symbol, ";")) {
        return nullptr;
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenPontoVirgula);

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    return std::make_unique<ReturnStatementNode>(
        sourceSpan,
        std::move(expressao)
        );
}

bool AnalisadorSintatico::compileStatements(
    std::vector<std::unique_ptr<StatementNode>>& instrucoes
    )
{
    while (match(TipoToken::Keyword, "let")
           || match(TipoToken::Keyword, "if")
           || match(TipoToken::Keyword, "while")
           || match(TipoToken::Keyword, "do")
           || match(TipoToken::Keyword, "return")) {

        if (match(TipoToken::Keyword, "let")) {
            auto instrucao = compileLet();

            if (!instrucao) {
                return false;
            }

            instrucoes.push_back(std::move(instrucao));

        } else if (match(TipoToken::Keyword, "if")) {
            auto instrucao = compileIf();

            if (!instrucao) {
                return false;
            }

            instrucoes.push_back(std::move(instrucao));

        } else if (match(TipoToken::Keyword, "while")) {
            auto instrucao = compileWhile();

            if (!instrucao) {
                return false;
            }

            instrucoes.push_back(std::move(instrucao));

        } else if (match(TipoToken::Keyword, "do")) {
            auto instrucao = compileDo();

            if (!instrucao) {
                return false;
            }

            instrucoes.push_back(std::move(instrucao));

        } else if (match(TipoToken::Keyword, "return")) {
            auto instrucao = compileReturn();

            if (!instrucao) {
                return false;
            }

            instrucoes.push_back(std::move(instrucao));
        }
    }

    return true;
}

bool AnalisadorSintatico::compileSubroutineBody()
{
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

    // statements
    if (!compileStatements(instrucoesAtuais)) {
        return false;
    }

    // '}'
    if (!consume(TipoToken::Symbol, "}")) {
        return false;
    }

    return true;
}

bool AnalisadorSintatico::compileVarDec()
{
    // Registra o início da declaração.
    const Token* tokenInicial = tokenAtual();

    if (tokenInicial == nullptr) {
        registrarErro("Declaração de variável local inesperada.");
        return false;
    }

    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenInicial);

    // 'var'
    if (!consume(TipoToken::Keyword, "var")) {
        return false;
    }

    // type: 'int' | 'char' | 'boolean' | className
    std::string tipo;

    if (match(TipoToken::Keyword, "int")
        || match(TipoToken::Keyword, "char")
        || match(TipoToken::Keyword, "boolean")
        || match(TipoToken::Identifier)) {

        tipo = tokenAtual()->lexema;
        advance();

    } else {
        registrarErro(
            "Esperado um tipo válido na declaração de variável local."
            );
        return false;
    }

    // Captura os nomes das variáveis.
    std::vector<std::string> nomes;

    while (true) {
        if (!match(TipoToken::Identifier)) {
            registrarErro(
                "Esperado um identificador na declaração de variável local."
                );
            return false;
        }

        nomes.push_back(tokenAtual()->lexema);
        advance();

        if (!match(TipoToken::Symbol, ",")) {
            break;
        }

        advance();
    }

    // ';'
    if (!match(TipoToken::Symbol, ";")) {
        registrarErro("Esperado ';' ao final da declaração de variável local.");
        return false;
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenAtual());

    advance();

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    // Constrói o nó da declaração.
    auto declaracao = std::make_unique<VariableDeclarationNode>(
        sourceSpan,
        VariableKind::Var,
        tipo,
        nomes
        );

    variaveisLocaisAtuais.push_back(std::move(declaracao));

    return true;
}

bool AnalisadorSintatico::compileSubroutine()
{
    // Registra o início da declaração.
    const Token* tokenInicial = tokenAtual();

    if (tokenInicial == nullptr) {
        registrarErro("Declaração de subrotina inesperada.");
        return false;
    }

    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenInicial);

    // Identifica o tipo da subrotina.
    SubroutineKind kind;

    if (match(TipoToken::Keyword, "constructor")) {
        kind = SubroutineKind::Constructor;
    } else if (match(TipoToken::Keyword, "function")) {
        kind = SubroutineKind::Function;
    } else if (match(TipoToken::Keyword, "method")) {
        kind = SubroutineKind::Method;
    } else {
        registrarErro(
            "Esperado 'constructor', 'function' ou 'method'."
            );
        return false;
    }

    advance();

    // Captura o tipo de retorno.
    std::string tipoRetorno;

    if (match(TipoToken::Keyword, "void")
        || match(TipoToken::Keyword, "int")
        || match(TipoToken::Keyword, "char")
        || match(TipoToken::Keyword, "boolean")
        || match(TipoToken::Identifier)) {

        tipoRetorno = tokenAtual()->lexema;
        advance();

    } else {
        registrarErro("Esperado um tipo de retorno válido.");
        return false;
    }

    // Captura o nome da subrotina.
    if (!match(TipoToken::Identifier)) {
        registrarErro("Esperado o nome da subrotina.");
        return false;
    }

    const std::string nome = tokenAtual()->lexema;
    advance();

    // '('
    if (!consume(TipoToken::Symbol, "(")) {
        return false;
    }

    // Prepara o armazenamento temporário da subrotina.
    parametrosAtuais.clear();
    variaveisLocaisAtuais.clear();
    instrucoesAtuais.clear();

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

    // Registra a posição final da subrotina.
    const Token* tokenFinal = &tokens[indiceAtual - 1];

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenFinal);

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    // Constrói o nó da subrotina.
    auto subrotina = std::make_unique<SubroutineDeclarationNode>(
        sourceSpan,
        kind,
        tipoRetorno,
        nome
        );

    // Transfere os parâmetros para a subrotina.
    for (auto& parametro : parametrosAtuais) {
        subrotina->adicionarParametro(std::move(parametro));
    }

    // Transfere as variáveis locais para a subrotina.
    for (auto& variavel : variaveisLocaisAtuais) {
        subrotina->adicionarVariavelLocal(std::move(variavel));
    }

    // Transfere as instruções para a subrotina.
    for (auto& instrucao : instrucoesAtuais) {
        subrotina->adicionarInstrucao(std::move(instrucao));
    }

    // Armazena a subrotina na classe em construção.
    subrotinasAtuais.push_back(std::move(subrotina));

    return true;
}

bool AnalisadorSintatico::compileParameterList()
{
    // Uma lista de parâmetros pode estar vazia.
    if (match(TipoToken::Symbol, ")")) {
        return true;
    }

    while (true) {

        // Registra o início do parâmetro.
        const Token* tokenInicial = tokenAtual();

        if (tokenInicial == nullptr) {
            registrarErro(
                "Esperado um parâmetro na lista de parâmetros."
                );
            return false;
        }

        const SourceSpan spanInicial =
            obterSourceSpanToken(*tokenInicial);

        // type: 'int' | 'char' | 'boolean' | className
        std::string tipo;

        if (match(TipoToken::Keyword, "int")
            || match(TipoToken::Keyword, "char")
            || match(TipoToken::Keyword, "boolean")
            || match(TipoToken::Identifier)) {

            tipo = tokenAtual()->lexema;
            advance();

        } else {
            registrarErro(
                "Esperado um tipo válido na lista de parâmetros."
                );
            return false;
        }

        // varName
        if (!match(TipoToken::Identifier)) {
            registrarErro(
                "Esperado o nome do parâmetro na lista de parâmetros."
                );
            return false;
        }

        const Token* tokenNome = tokenAtual();
        const std::string nome = tokenNome->lexema;

        const SourceSpan spanFinal =
            obterSourceSpanToken(*tokenNome);

        advance();

        // Constrói o nó do parâmetro.
        const SourceSpan sourceSpan{
            spanInicial.startLine,
            spanInicial.startColumn,
            spanFinal.endLine,
            spanFinal.endColumn
        };

        auto parametro = std::make_unique<ParameterNode>(
            sourceSpan,
            tipo,
            nome
            );

        parametrosAtuais.push_back(std::move(parametro));

        // Verifica se há outro parâmetro.
        if (!match(TipoToken::Symbol, ",")) {
            break;
        }

        if (!consume(TipoToken::Symbol, ",")) {
            return false;
        }
    }

    return true;
}

bool AnalisadorSintatico::compileClass()
{
    // Prepara o armazenamento temporário da classe.
    classeAtual.reset();
    variaveisClasseAtuais.clear();
    subrotinasAtuais.clear();

    // Registra o início da declaração da classe.
    const Token* tokenInicial = tokenAtual();

    if (tokenInicial == nullptr) {
        registrarErro("Declaração de classe inesperada.");
        return false;
    }

    const SourceSpan spanInicial =
        obterSourceSpanToken(*tokenInicial);

    // 'class'
    if (!consume(TipoToken::Keyword, "class")) {
        return false;
    }

    // className
    if (!match(TipoToken::Identifier)) {
        return consume(TipoToken::Identifier);
    }

    const std::string nomeClasse = tokenAtual()->lexema;
    advance();

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

    // Registra a posição final da classe.
    if (!match(TipoToken::Symbol, "}")) {
        return consume(TipoToken::Symbol, "}");
    }

    const SourceSpan spanFinal =
        obterSourceSpanToken(*tokenAtual());

    advance();

    const SourceSpan sourceSpan{
        spanInicial.startLine,
        spanInicial.startColumn,
        spanFinal.endLine,
        spanFinal.endColumn
    };

    // Constrói o nó da classe.
    classeAtual = std::make_unique<ClassNode>(
        sourceSpan,
        nomeClasse
        );

    // Transfere as declarações de variáveis para a classe.
    for (auto& variavel : variaveisClasseAtuais) {
        classeAtual->adicionarVariavelClasse(
            std::move(variavel)
            );
    }

    variaveisClasseAtuais.clear();

    // Transfere as declarações de subrotinas para a classe.
    for (auto& subrotina : subrotinasAtuais) {
        classeAtual->adicionarSubrotina(
            std::move(subrotina)
            );
    }

    subrotinasAtuais.clear();

    return true;
}

SyntaxResult AnalisadorSintatico::analisar()
{
    indiceAtual = 0;
    erroSintatico.reset();

    classeAtual.reset();
    variaveisClasseAtuais.clear();
    subrotinasAtuais.clear();
    parametrosAtuais.clear();
    variaveisLocaisAtuais.clear();
    instrucoesAtuais.clear();

    if (tokens.empty()) {
        registrarErro(
            "Não há tokens para analisar."
            );

        return *erroSintatico;
    }

    if (!compileClass()) {
        if (erroSintatico.has_value()) {
            return *erroSintatico;
        }

        return SyntaxFailure{
            "Falha durante a análise sintática.",
            obterPosicaoAtual()
        };
    }

    // Não permitir tokens extras após a classe.
    if (tokenAtual() != nullptr) {
        registrarErro(
            "Token inesperado '" + tokenAtual()->lexema
            + "' após o fim da classe."
            );

        return *erroSintatico;
    }

    // Constrói o nó raiz do programa.
    auto programa = std::make_unique<ProgramNode>(
        classeAtual->obterSourceSpan()
        );

    // Transfere a classe construída para o programa.
    programa->adicionarClasse(std::move(classeAtual));

    // Retorna o resultado da análise sintática.
    return SyntaxSuccess{
        std::move(programa)
    };
}