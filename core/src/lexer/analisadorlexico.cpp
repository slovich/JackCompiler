#include "jack/lexer/analisadorlexico.h"

#include <unordered_set>
#include <stdexcept>

AnalisadorLexico::AnalisadorLexico(const std::string& codigoFonte)
    : codigoFonte(codigoFonte)
{
}

bool AnalisadorLexico::analisar(
    std::vector<Token>& tokens,
    std::string& erro) const
{
    tokens.clear();
    erro.clear();

    const std::unordered_set<std::string> palavrasChave = {
        "class", "constructor", "function", "method",
        "field", "static", "var", "int", "char",
        "boolean", "void", "true", "false", "null",
        "this", "let", "do", "if", "else", "while",
        "return"
    };

    const std::string simbolos = "{}()[].,;+-*/&|<>=~";

    const std::size_t tamanho = codigoFonte.size();

    std::size_t posicao = 0;
    int linha = 1;
    std::size_t inicioLinha = 0;

    auto ehLetra = [](char caractere) {
        return (caractere >= 'a' && caractere <= 'z')
        || (caractere >= 'A' && caractere <= 'Z');
    };

    auto ehDigito = [](char caractere) {
        return caractere >= '0' && caractere <= '9';
    };

    auto ehEspaco = [](char caractere) {
        return caractere == ' '
               || caractere == '\t'
               || caractere == '\r'
               || caractere == '\n';
    };

    auto adicionarToken = [&](TipoToken tipo,
                              const std::string& lexema,
                              int linhaToken,
                              int colunaToken) {
        tokens.push_back(Token{
            tipo,
            lexema,
            linhaToken,
            colunaToken
        });
    };

    while (posicao < tamanho) {
        const char atual = codigoFonte.at(posicao);

        // Ignora espaços em branco e atualiza a linha.
        if (ehEspaco(atual)) {
            if (atual == '\n') {
                ++linha;
            }

            ++posicao;

            if (atual == '\n') {
                inicioLinha = posicao;
            }

            continue;
        }

        // Comentários de linha: // ...
        if (atual == '/'
            && posicao + 1 < tamanho
            && codigoFonte.at(posicao + 1) == '/') {

            posicao += 2;

            while (posicao < tamanho
                   && codigoFonte.at(posicao)
                          != '\n') {
                ++posicao;
            }

            continue;
        }

        // Comentários de bloco: /* ... */
        if (atual == '/'
            && posicao + 1 < tamanho
            && codigoFonte.at(posicao + 1) == '*') {

            const int linhaInicio = linha;
            posicao += 2;

            bool fechado = false;

            while (posicao < tamanho) {
                if (codigoFonte.at(posicao) == '\n') {
                    ++linha;
                    ++posicao;
                    inicioLinha = posicao;
                    continue;
                }

                if (codigoFonte.at(posicao) == '*'
                    && posicao + 1 < tamanho
                    && codigoFonte.at(posicao + 1)
                           == '/') {
                    posicao += 2;
                    fechado = true;
                    break;
                }

                ++posicao;
            }

            if (!fechado) {
                erro = "Comentário de bloco não terminado, iniciado na linha "
                       + std::to_string(linhaInicio) + ".";

                tokens.clear();
                return false;
            }

            continue;
        }

        const int linhaToken = linha;
        const int colunaToken = posicao - inicioLinha;

        // Constantes de texto: "..."
        if (atual == '"') {
            ++posicao;
            const std::size_t inicio = posicao;

            while (posicao < tamanho
                   && codigoFonte.at(posicao)
                          != '"'
                   && codigoFonte.at(posicao)
                          != '\n'
                   && codigoFonte.at(posicao)
                          != '\r') {
                ++posicao;
            }

            if (posicao >= tamanho
                || codigoFonte.at(posicao) != '"') {
                erro = "Constante de texto não terminada na linha "
                       + std::to_string(linhaToken) + ".";
                tokens.clear();
                return false;
            }

            const std::string lexema =
                codigoFonte.substr(inicio, posicao - inicio);

            adicionarToken(
                TipoToken::StringConstant,
                lexema,
                linhaToken,
                colunaToken
                );

            ++posicao; // Aspas finais.
            continue;
        }

        // Identificadores e palavras-chave.
        if (ehLetra(atual)) {
            const std::size_t inicio = posicao;

            while (posicao < tamanho) {
                const char caractere = codigoFonte.at(posicao);

                if (!ehLetra(caractere)
                    && !ehDigito(caractere)
                    && caractere != '_') {
                    break;
                }

                ++posicao;
            }

            const std::string lexema =
                codigoFonte.substr(inicio, posicao - inicio);

            const TipoToken tipo =
                palavrasChave.find(lexema) != palavrasChave.end()
                    ? TipoToken::Keyword
                    : TipoToken::Identifier;

            adicionarToken(
                tipo,
                lexema,
                linhaToken,
                colunaToken
                );

            continue;
        }

        // Constantes inteiras.
        if (ehDigito(atual)) {
            const std::size_t inicio = posicao;

            while (posicao < tamanho
                   && ehDigito(codigoFonte.at(posicao))) {
                ++posicao;
            }

            const std::string lexema =
                codigoFonte.substr(inicio, posicao - inicio);

            bool conversaoValida = true;
            int valor = 0;

            try {
                valor = std::stoi(lexema);
            } catch (const std::invalid_argument&) {
                conversaoValida = false;
            } catch (const std::out_of_range&) {
                conversaoValida = false;
            }

            if (!conversaoValida || valor > 32767) {
                erro = "Constante inteira fora do intervalo permitido "
                       "(0 a 32767), na linha "
                       + std::to_string(linhaToken) + ": " + lexema + ".";

                tokens.clear();
                return false;
            }

            adicionarToken(
                TipoToken::IntegerConstant,
                lexema,
                linhaToken,
                colunaToken
                );

            continue;
        }

        // Símbolos da linguagem Jack.
        if (simbolos.find(atual) != std::string::npos) {
            adicionarToken(
                TipoToken::Symbol,
                std::string(1, atual),
                linhaToken,
                colunaToken
                );

            ++posicao;
            continue;
        }

        // Caractere não reconhecido.
        erro = "Caractere inválido na linha "
               + std::to_string(linhaToken)
               + ", coluna " + std::to_string(colunaToken)
               + ": '" + std::string(1, atual) + "'.";

        tokens.clear();
        return false;
    }

    return true;
}
