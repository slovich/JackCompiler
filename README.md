# JackCompiler

Compilador para a linguagem **Jack**, desenvolvido em C++ com arquitetura modular e interfaces gráfica e de linha de comando.

O projeto implementa as etapas de análise léxica e sintática da linguagem, produzindo representações XML dos resultados. Sua arquitetura separa o núcleo de processamento das interfaces de execução, permitindo utilizar os mesmos componentes independentemente da interface escolhida.

## Sobre o projeto

O JackCompiler surgiu no contexto de um projeto acadêmico da disciplina de Compiladores, do curso de Engenharia da Computação da Universidade Federal do Maranhão (UFMA).

Ao longo do desenvolvimento, o projeto evoluiu para uma aplicação estruturada em módulos independentes, com foco na organização do código, reutilização de componentes e possibilidade de execução em diferentes ambientes.

Atualmente, o projeto disponibiliza duas interfaces:

- **GUI:** interface gráfica desenvolvida com Qt Widgets.
- **CLI:** interface de linha de comando independente do Qt.

Ambas utilizam o mesmo núcleo de processamento, responsável pelas análises léxica e sintática e pela geração das representações XML.

## Funcionalidades

### Análise léxica

- Identificação e classificação dos tokens da linguagem Jack.
- Reconhecimento de palavras-chave, símbolos, identificadores e constantes.
- Detecção e apresentação de erros léxicos.
- Geração e exportação da representação XML dos tokens.

### Análise sintática

- Análise sintática por descida recursiva.
- Construção de uma árvore sintática abstrata (AST).
- Identificação de erros sintáticos com indicação de localização no código-fonte.
- Geração e exportação da representação XML sintática.

### Interface gráfica

- Carregamento e visualização de arquivos Jack.
- Realce léxico no editor de código.
- Execução das análises léxica e sintática.
- Visualização dos resultados e da estrutura hierárquica da árvore sintática.
- Exportação dos documentos XML.

### Interface de linha de comando

- Execução das análises léxica, sintática e completa.
- Exportação dos XML léxico e sintático.
- Execução automatizada de testes com arquivos válidos e inválidos.
- Processamento em lote de arquivos Jack, com apresentação individualizada dos resultados e consolidação das estatísticas.

## Arquitetura

O projeto é organizado em três módulos principais:

| Módulo | Responsabilidade |
|---|---|
| `core` | Núcleo do compilador. Contém os analisadores léxico e sintático, a estrutura da AST, os tokens e os geradores XML. |
| `cli` | Interface de linha de comando. Utiliza o núcleo do compilador sem depender do Qt. |
| `src` | Aplicação gráfica desenvolvida com Qt Widgets. Utiliza o mesmo núcleo de processamento da CLI. |

Essa organização permite manter as regras de processamento da linguagem independentes dos mecanismos de interação com o usuário.

## Tecnologias utilizadas

- **C++17:** linguagem de implementação.
- **CMake:** configuração e gerenciamento do processo de compilação.
- **Qt Widgets:** desenvolvimento da interface gráfica.
- **MinGW-w64:** toolchain utilizada no ambiente de desenvolvimento Windows.

## Estrutura do repositório

```text
JackCompiler/
├── assets/       # Recursos utilizados pela aplicação
├── cli/          # Interface de linha de comando
├── core/         # Núcleo independente do compilador
├── docs/         # Documentação do projeto
├── src/          # Interface gráfica Qt
├── tests/        # Casos de teste
├── assets.qrc    # Registro de recursos da aplicação Qt
├── CMakeLists.txt
└── README.md
```

## Documentação

A documentação do projeto está organizada em documentos específicos, permitindo consultar instruções de utilização, informações técnicas e procedimentos de desenvolvimento sem sobrecarregar este arquivo.

### Desenvolvimento

- [Uso de Inteligência Artificial](docs/desenvolvimento/uso-de-inteligencia-artificial.md)

### Instalação e compilação

- [Instruções de compilação](docs/instalacao/compilacao.md)

### Manuais de utilização

- [Manual da interface gráfica (GUI)](docs/usuario/manual-gui.md)
- [Manual da interface de linha de comando (CLI)](docs/usuario/manual-cli.md)

### Documentação técnica

- [Arquitetura do projeto](docs/tecnico/arquitetura.md)
- [Linguagem Jack e escopo da implementação](docs/tecnico/linguagem-jack.md)
- [Testes e validação](docs/tecnico/testes.md)

## Escopo atual e trabalhos futuros

O escopo atualmente implementado contempla as análises léxica e sintática da linguagem Jack, a construção da árvore sintática abstrata e a geração das representações XML correspondentes.

A análise semântica e a geração de código para a máquina virtual Jack não fazem parte das funcionalidades concluídas nesta versão.

O projeto mantém uma estrutura modular que permite a ampliação gradual de suas funcionalidades e a incorporação de novas etapas do processo de compilação.

## Contexto acadêmico

Projeto originado na disciplina de Compiladores do curso de Engenharia da Computação da Universidade Federal do Maranhão (UFMA).

O desenvolvimento contou com o apoio de ferramentas de Inteligência Artificial, cujo processo de utilização está documentado em [Uso de Inteligência Artificial](docs/desenvolvimento/uso-de-inteligencia-artificial.md).
