# JackCompiler

Compilador para a linguagem **Jack**, desenvolvido em C++ com
arquitetura modular e interfaces gráfica e de linha de comando.

O projeto implementa análise léxica e sintática, construção de AST,
geração de representações XML, comparação estrutural de documentos XML e
geração de código para a máquina virtual Jack (VM). Sua arquitetura separa o núcleo de processamento das
interfaces de execução e compartilha os componentes do núcleo utilizados
por cada fluxo, mantendo as capacidades específicas de cada interface.

## Sobre o projeto

O JackCompiler surgiu no contexto de um projeto acadêmico da disciplina
de Compiladores, do curso de Engenharia da Computação da Universidade
Federal do Maranhão (UFMA).

Ao longo do desenvolvimento, o projeto evoluiu para uma aplicação
estruturada em módulos independentes, com foco na organização do código,
reutilização de componentes e possibilidade de execução em diferentes
ambientes.

Atualmente, o projeto disponibiliza duas interfaces:

-   **GUI:** interface gráfica desenvolvida com Qt Widgets.
-   **CLI:** interface de linha de comando independente do Qt.

As duas interfaces utilizam o núcleo `jack_core` para as análises léxica
e sintática, a construção da AST e a geração das representações XML. O
núcleo também contém os componentes de geração de código VM, mas essa
funcionalidade é acionada pelo fluxo de compilação não interativo da CLI.
A GUI não oferece geração VM no estado documentado. A existência de
`SymbolTable` e dos componentes VM não comprova uma etapa de análise
semântica completa.

## Funcionalidades

### Análise léxica

-   Identificação e classificação dos tokens da linguagem Jack.
-   Reconhecimento de palavras-chave, símbolos, identificadores e
    constantes.
-   Detecção e apresentação de erros léxicos.
-   Geração e exportação da representação XML dos tokens.

### Análise sintática

-   Análise sintática por descida recursiva.
-   Construção de uma árvore sintática abstrata (AST).
-   Identificação de erros sintáticos com indicação de localização no
    código-fonte.
-   Geração e exportação da representação XML sintática.

### Geração de código VM

-   Geração de código para a máquina virtual Jack a partir dos programas
    aceitos pelo fluxo de compilação não interativo da CLI.
-   Utilização dos componentes `SymbolTable`, `VMCodeGenerator` e
    `VMWriter` no contexto da geração.
-   Tradução dos operadores Jack `*` e `/` em chamadas a `Math.multiply`
    e `Math.divide`, respectivamente.

A validação da geração VM foi realizada com os programas do Project 11.
Os resultados, os ambientes utilizados, a origem das evidências e as
ressalvas funcionais estão resumidos na seção [Escopo atual e trabalhos
futuros](#escopo-atual-e-trabalhos-futuros) e detalhados em
[Testes e validação](docs/tecnico/testes.md).

### Interface gráfica

-   Carregamento e visualização de arquivos Jack.
-   Realce léxico no editor de código.
-   Execução das análises léxica e sintática.
-   Visualização dos resultados e da estrutura hierárquica da árvore
    sintática.
-   Exportação dos documentos XML.

### Comparação de XML

-   Comparação estrutural entre XML léxico e XML sintático gerados pelo
    JackCompiler e seus documentos de referência.
-   Verificação da estrutura, ordem dos elementos, atributos e conteúdo
    textual, sem depender de igualdade textual ou de formatação idêntica.
-   Identificação de divergências e apresentação de seus caminhos
    estruturais. DTD efetivo é rejeitado.

A comparação está disponível no menu interativo da CLI, nas opções 10
(XML léxico) e 11 (XML sintático). Cada operação solicita os caminhos do
arquivo de referência e do arquivo gerado.

### Interface de linha de comando

-   Execução das análises léxica, sintática e completa.
-   Exportação dos XML léxico e sintático, com nomes sugeridos distintos
    (`_lexico.xml` e `_sintatico.xml`).
-   Comparação dos XML léxico e sintático com documentos de referência.
-   Execução automatizada de testes com arquivos válidos e inválidos.
-   Processamento em lote de arquivos Jack, com apresentação
    individualizada dos resultados e consolidação das estatísticas.
-   Compilação não interativa de um arquivo `.jack` ou de todos os
    arquivos `.jack` diretamente contidos em um diretório.
-   Geração de um arquivo `.vm` por classe compilada, com opção de
    especificar o diretório de destino.
-   Diagnósticos associados às etapas de processamento e código de saída
    diferente de zero em caso de falha.

Exemplos de uso:

``` text
jack_cli --compile caminho/Programa.jack
jack_cli --compile caminho/dos/fontes --output caminho/da/saida
jack_cli compile caminho/Programa.jack -o caminho/da/saida
jack_cli --help
```

A busca em diretórios é não recursiva. Sem `--output`/`-o`, os arquivos
VM são gravados no diretório do arquivo de entrada ou no próprio
diretório de entrada.

## Arquitetura

O projeto é organizado em três módulos principais:

| Módulo | Responsabilidade |
|---|---|
| `core` | Núcleo do compilador. Contém os analisadores léxico e sintático, a AST, os tokens, os geradores XML, o Comparador XML e os componentes de tabela de símbolos e geração VM. |
| `cli` | Interface de linha de comando. Utiliza o núcleo do compilador sem depender do Qt e disponibiliza o fluxo não interativo de geração VM. |
| `src` | Aplicação gráfica desenvolvida com Qt Widgets. Utiliza o núcleo compartilhado para análise léxica, análise sintática e visualização/exportação XML. |

Essa organização permite manter as regras de processamento da linguagem
independentes dos mecanismos de interação com o usuário.

## Tecnologias utilizadas

-   **C++17:** linguagem de implementação.
-   **CMake:** configuração e gerenciamento do processo de compilação.
-   **Qt Widgets:** desenvolvimento da interface gráfica.
-   **MinGW-w64:** toolchain utilizada no ambiente de desenvolvimento
    Windows.

## Estrutura do repositório

``` text
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

A documentação do projeto está organizada em documentos específicos,
permitindo consultar instruções de utilização, informações técnicas e
procedimentos de desenvolvimento sem sobrecarregar este arquivo.

### Desenvolvimento

-   [Uso de Inteligência
    Artificial](docs/desenvolvimento/uso-de-inteligencia-artificial.md)

### Instalação e compilação

-   [Instruções de compilação](docs/instalacao/compilacao.md)

### Manuais de utilização

-   [Manual da interface gráfica (GUI)](docs/usuario/manual-gui.md)
-   [Manual da interface de linha de comando
    (CLI)](docs/usuario/manual-cli.md)

### Documentação técnica

-   [Arquitetura do projeto](docs/tecnico/arquitetura.md)
-   [Linguagem Jack e escopo da
    implementação](docs/tecnico/linguagem-jack.md)
-   [Testes e validação](docs/tecnico/testes.md)

## Escopo atual e trabalhos futuros

O escopo atualmente implementado contempla as análises léxica e
sintática da linguagem Jack, a construção da árvore sintática abstrata,
a geração das representações XML e a geração de código para a máquina
virtual Jack por meio do fluxo não interativo da CLI.

Na regressão Windows W-12, os seis projetos oficiais do Project 11
foram compilados com sucesso: 11 arquivos-fonte Jack foram processados e
11 arquivos VM foram gerados. Os arquivos foram executados no VM Emulator
desktop. Nos cenários funcionais registrados, Seven, ConvertToBin,
Average e ComplexArrays foram aprovados. Pong também foi aprovado,
conforme relato e evidências encaminhadas pelo responsável. Em Square, a
movimentação foi aprovada, mas o redimensionamento não funcionou. Essa
limitação também foi reproduzida com o compilador oficial do Nand2Tetris
no emulador online, conforme comparação relatada anteriormente pelo
Desenvolvedor; portanto, não há evidência para classificá-la como defeito
confirmado do JackCompiler. Esses resultados se limitam aos cenários
avaliados e não representam aprovação integral de todas as
funcionalidades dos seis programas. Os resultados, ressalvas e origens
das evidências estão detalhados em [Testes e validação](docs/tecnico/testes.md).

Os resultados desta etapa de comparação XML e regressão VM ainda não
estão associados a um hash de commit específico; a implementação e a
avaliação foram concluídas, mas a publicação no GitHub depende do processo
de versionamento. A existência de `SymbolTable` e dos componentes de
geração VM não comprova, por si só, uma etapa de análise semântica
completa. O projeto mantém uma estrutura modular que permite a ampliação
gradual de suas funcionalidades e a incorporação de novas etapas do
processo de compilação.

## Contexto acadêmico

Projeto originado na disciplina de Compiladores do curso de Engenharia
da Computação da Universidade Federal do Maranhão (UFMA).

O desenvolvimento contou com o apoio de ferramentas de Inteligência
Artificial, cujo processo de utilização está documentado em [Uso de
Inteligência
Artificial](docs/desenvolvimento/uso-de-inteligencia-artificial.md).
