
#ifndef JACK_CLI_TERMINAL_H
#define JACK_CLI_TERMINAL_H

#include <string>

namespace jack::cli {

class Terminal
{
public:
    enum class Cor
    {
        Padrao,
        Vermelho,
        Verde,
        Amarelo,
        Azul,
        Ciano,
        Magenta,
        Branco
    };

    Terminal();

    void limparTela() const;

    void exibirTitulo(const std::string& titulo) const;
    void exibirSeparador() const;

    void exibirTexto(
        const std::string& texto,
        Cor cor = Cor::Padrao
        ) const;

    void exibirInformacao(const std::string& mensagem) const;
    void exibirSucesso(const std::string& mensagem) const;
    void exibirAviso(const std::string& mensagem) const;
    void exibirErro(const std::string& mensagem) const;

    std::string lerTexto(const std::string& mensagem) const;

private:
    bool coresHabilitadas;

    void inicializarSuporteCores();
    std::string obterCodigoCor(Cor cor) const;
};

} // namespace jack::cli

#endif // JACK_CLI_TERMINAL_H
