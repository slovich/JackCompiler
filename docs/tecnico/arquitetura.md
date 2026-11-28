# Arquitetura do JackCompiler

## 1. Objetivo e escopo

Este documento descreve a organização interna do **JackCompiler**, suas principais responsabilidades, os componentes envolvidos no processamento de programas Jack e as relações entre eles.

O projeto implementa análise léxica, análise sintática, construção de uma árvore sintática abstrata (AST), manutenção de uma tabela de símbolos, geração de código para a máquina virtual Jack (VM), geração de representações XML e comparação estrutural de XML léxico e sintático. A aplicação dispõe de uma interface gráfica, desenvolvida com Qt, e de uma interface de linha de comando.

A implementação da SymbolTable e do VMWriter foi submetida a testes comportamentais independentes. O VMCodeGenerator e sua integração com o fluxo não interativo da CLI foram submetidos à validação funcional utilizando os seis programas oficiais do Project 11 do Nand2Tetris.

A existência desses componentes e os resultados de validação não representam, por si só, prova de cobertura exaustiva de todas as construções da linguagem Jack. A implementação de geração VM está disponível no fluxo não interativo da CLI, mas a análise semântica completa não está comprovada pelas evidências documentais consideradas nesta revisão.

Assim, é necessário distinguir a geração de código VM já implementada da análise semântica completa: a existência da SymbolTable não comprova, isoladamente, que todas as verificações semânticas da linguagem estejam implementadas e integradas.

## 2. Visão geral da arquitetura

A arquitetura separa o processamento da linguagem das interfaces utilizadas para acioná-lo:

- **`core`**: contém a implementação compartilhada dos analisadores léxico e sintático, da AST, da tabela de símbolos, dos componentes de geração de código VM, dos geradores XML e do Comparador XML.
- **`cli`**: implementa a interface de linha de comando, incluindo o menu interativo e o fluxo não interativo de geração de código VM.
- **`src`**: implementa a interface gráfica Qt e utiliza o núcleo compartilhado para análise léxica, análise sintática e operações relacionadas à representação XML.
- **`tests`**: contém arquivos Jack para testes funcionais e arquivos de testes independentes dos componentes do núcleo.

A biblioteca `jack_core` concentra a lógica de processamento. Tanto `jack_cli` quanto `compilador` dependem dela, evitando manter implementações distintas dos analisadores para cada interface.

```mermaid
flowchart TD
    A["Arquivo-fonte Jack"] --> B["Analisador léxico"]
    B -->|Sucesso| C["Sequência de tokens"]
    B -->|Falha| E["Erro léxico"]
    C --> D["Analisador sintático"]
    C --> F["Gerador XML léxico"]
    D -->|Sucesso| G["Árvore sintática abstrata (AST)"]
    D -->|Falha| H["Erro sintático"]
    G --> I["Gerador XML sintático"]
    I --> J["Documento XML sintático"]
    F --> K["Documento XML léxico"]
```

As interfaces CLI e GUI são responsáveis por apresentar os resultados, receber entradas do usuário e solicitar operações ao núcleo. A geração dos tokens, a análise sintática, a construção da AST e a geração dos documentos XML pertencem ao `core`.

## 3. Organização do repositório

A estrutura relevante do projeto é organizada da seguinte maneira:

```text
JackCompiler/
├── assets/
├── cli/
│   ├── include/jack/cli/
│   └── src/
├── core/
│   ├── include/jack/
│   │   ├── ast/
│   │   ├── lexer/
│   │   ├── parser/
│   │   ├── symbol/
│   │   ├── vm/
│   │   └── xml/
│   └── src/
│       ├── ast/
│       ├── lexer/
│       ├── parser/
│       ├── symbol/
│       ├── vm/
│       └── xml/
├── docs/
├── src/
├── tests/
│   ├── expected/
│   ├── inputs/
│   │   ├── invalidos/
│   │   └── validos/
│   ├── comparadorxml_regression_tests.cpp
│   ├── symboltable_test.cpp
│   ├── vmwriter_tests.cpp
│   ├── vmcodegenerator_tests.cpp
│   └── vmcodegenerator_extended_tests.cpp
├── third_party/
│   └── pugixml/
└── CMakeLists.txt
```

### 3.1 Núcleo (`core`)

O diretório `core` é dividido por responsabilidade:

| Diretório | Responsabilidade |
|---|---|
| `ast` | Declaração e implementação dos nós da árvore sintática abstrata |
| `lexer` | Representação dos tokens e análise léxica |
| `parser` | Resultado da análise sintática e reconhecimento da gramática |
| `symbol` | Implementação da tabela de símbolos |
| `vm` | Implementação do emissor de instruções VM e do gerador de código VM |
| `xml` | Geração das representações XML léxica e sintática e comparação estrutural de documentos XML |

Os diretórios `include` e `src` separam as interfaces públicas das implementações. Os cabeçalhos públicos ficam em `core/include/jack`, enquanto as implementações correspondentes ficam em `core/src`.

### 3.2 Interfaces

O diretório `cli` contém a aplicação de terminal, incluindo menu, controle do fluxo de operações, apresentação de mensagens e execução de testes em lote.

O diretório `src` contém a aplicação gráfica Qt, a janela principal, os realçadores de sintaxe e a janela de apresentação das informações do projeto.

### 3.3 Testes

Os arquivos de entrada utilizados pelo mecanismo de testes ficam em `tests/inputs`. A organização atual separa os exemplos em `validos` e `invalidos`. Essa estrutura é utilizada pela opção de execução de testes da interface CLI.

## 4. Construção e dependências

O projeto utiliza CMake para configurar a compilação e exige suporte a **C++17**. O `CMakeLists.txt` define a biblioteca estática `jack_core` e os executáveis das interfaces.

### 4.1 Alvos de compilação

| Alvo CMake | Tipo | Responsabilidade |
|---|---|---|
| `jack_pugixml` | Biblioteca estática | Disponibilizar pugixml 1.16 como dependência privada do núcleo. |
| `jack_core` | Biblioteca estática | Reunir os componentes compartilhados do núcleo: lexer, parser, AST, SymbolTable, VMWriter, VMCodeGenerator, geradores XML e Comparador XML. |
| `jack_cli` | Executável | Disponibilizar a interface de linha de comando, incluindo o fluxo não interativo de geração de código VM. |
| `compilador` | Executável | Disponibilizar a interface gráfica Qt. O alvo é criado somente quando `BUILD_GUI` está habilitado. |
| `comparadorxml_regression_tests` | Executável de teste | Executar os quatro casos automatizados de regressão do Comparador XML; criado somente quando `BUILD_XML_COMPARATOR_REGRESSION_TESTS=ON`. |

A opção CMake `BUILD_GUI` controla a inclusão da interface gráfica e tem valor padrão `ON`.

A opção `BUILD_XML_COMPARATOR_REGRESSION_TESTS` controla a criação do
alvo de regressão do Comparador XML e tem valor padrão `OFF`. Quando
habilitada, registra o teste no CTest.

Quando `BUILD_GUI=OFF`, o CMake não procura os componentes Qt e não cria o alvo `compilador`. O núcleo e a interface CLI continuam disponíveis. Essa separação permite compilar e utilizar a CLI sem instalar Qt.

O Comparador XML utiliza **pugixml 1.16**, mantida no diretório
`third_party/pugixml/`. O CMake disponibiliza a dependência pelo alvo
estático `jack_pugixml`, vinculado privadamente ao `jack_core`. Os tipos
da pugixml não fazem parte da interface pública do Comparador XML; assim,
a dependência permanece encapsulada no núcleo.

Quando a interface gráfica está habilitada, o projeto procura Qt 5 ou Qt 6 com os componentes `Widgets` e `Xml`. O executável gráfico é vinculado a `jack_core` e aos módulos Qt correspondentes.

A interface gráfica utiliza recursos do Qt para interpretar o XML sintático produzido pelo núcleo. Em particular, `MainWindow::gerarArvoreSintatica()` utiliza `QXmlStreamReader`, disponibilizado pelo Qt Core, para percorrer o documento XML e construir o modelo hierárquico apresentado na interface. Os geradores do `core`, por sua vez, produzem o conteúdo XML como `std::string` e não dependem das classes Qt. Embora o CMake também declare a dependência de `Qt Xml`, o uso de `QXmlStreamReader` não justifica essa dependência, pois a classe pertence ao Qt Core; a associação de `Qt Xml` a uma funcionalidade específica depende da identificação de classes desse módulo efetivamente utilizadas pela GUI.

### 4.2 Dependências entre os componentes

O núcleo não depende de Qt. Sua implementação utiliza C++17, estruturas da biblioteca padrão e a dependência externa encapsulada pugixml 1.16, utilizada pelo Comparador XML.

As duas interfaces dependem do núcleo. A interface gráfica também utiliza Qt Widgets para a apresentação visual e recursos do Qt Core para percorrer o documento XML sintático:

```mermaid
flowchart TD
    A["jack_cli"] --> C["jack_core"]
    B["compilador (GUI)"] --> C
    C --> F["jack_pugixml / pugixml 1.16"]
    B --> D["Qt Widgets"]
    B --> E["Qt Core"]
```

Essa organização mantém a lógica de análise e geração XML independente da tecnologia de interface. A GUI utiliza recursos do Qt Core para percorrer o documento XML, enquanto os geradores do núcleo produzem o conteúdo XML como `std::string`, sem dependência das classes Qt.

O Comparador XML também pertence ao `jack_core` e recebe os conteúdos
dos dois documentos em memória. Ele não acessa arquivos, diretórios,
terminal ou Qt. A CLI é responsável por solicitar os caminhos, abrir e
ler os arquivos e apresentar o resultado da comparação.

## 5. Fluxo de processamento

O processamento de um arquivo Jack ocorre em etapas encadeadas:

1. A interface obtém o conteúdo do arquivo.
2. O analisador léxico percorre o código-fonte e produz uma sequência de tokens.
3. Se a análise léxica falhar, o processamento sintático não pode prosseguir com uma sequência válida de tokens.
4. Se a análise léxica for bem-sucedida, o analisador sintático recebe os tokens e o código-fonte.
5. O analisador sintático verifica a organização dos elementos segundo a gramática implementada e constrói a AST.
6. A AST pode ser percorrida pelo gerador XML sintático.
7. A sequência de tokens pode ser utilizada pelo gerador XML léxico.

A geração do XML léxico depende dos tokens e não exige a construção da AST. Já a geração do XML sintático depende de uma AST válida.

Além desse fluxo de análise e geração XML, o núcleo contém os componentes `SymbolTable`, `VMWriter` e `VMCodeGenerator`. A geração de arquivos VM está integrada ao fluxo não interativo da CLI. Essa funcionalidade não deve ser atribuída à GUI: não há evidência de que a interface gráfica ofereça geração VM.

## 6. Análise léxica

### 6.1 Responsabilidade e interface

O analisador léxico está implementado em `core/src/lexer/analisadorlexico.cpp`, com sua interface declarada em `core/include/jack/lexer/analisadorlexico.h`.

A classe `AnalisadorLexico` recebe o código-fonte em seu construtor. O método `analisar` executa a varredura e recebe, por referência, os parâmetros de saída para a sequência de tokens e a mensagem de erro:

```cpp
explicit AnalisadorLexico(const std::string& codigoFonte);

bool analisar(
    std::vector<Token>& tokens,
    std::string& erro
) const;
```

O método retorna `true` quando a análise é concluída com sucesso e `false` quando encontra um erro léxico.

### 6.2 Representação dos tokens

Cada token é representado pela estrutura `Token`, definida em `token.h`:

```cpp
enum class TipoToken {
    Keyword,
    Symbol,
    IntegerConstant,
    StringConstant,
    Identifier
};

struct Token {
    TipoToken tipo;
    std::string lexema;
    int linha;
    int coluna;
};
```

Assim, cada ocorrência reconhecida mantém sua categoria, seu lexema e sua posição no código-fonte. A posição é utilizada para apresentar informações de localização durante o diagnóstico de erros e a exibição dos resultados.

### 6.3 Estratégia de varredura

A implementação percorre o código sequencialmente, mantendo a posição atual e informações de linha. Não utiliza uma etapa de geração de tokens baseada em expressões regulares: o reconhecimento é feito por inspeção dos caracteres e aplicação de condições específicas.

Durante a varredura, o analisador:

- ignora espaços em branco, tabulações e quebras de linha;
- ignora comentários de linha iniciados por `//`;
- ignora comentários de bloco delimitados por `/*` e `*/`;
- reconhece palavras-chave, identificadores, símbolos e constantes;
- registra a posição dos tokens reconhecidos.

As palavras-chave reconhecidas são:

`class`, `constructor`, `function`, `method`, `field`, `static`, `var`, `int`, `char`, `boolean`, `void`, `true`, `false`, `null`, `this`, `let`, `do`, `if`, `else`, `while` e `return`.

Os símbolos reconhecidos são:

```text
{}()[].,;+-*/&|<>=~
```

Identificadores começam com uma letra ASCII e podem continuar com letras ASCII, dígitos ou sublinhado (`_`). Constantes inteiras são reconhecidas como sequências de dígitos e aceitas no intervalo de 0 a 32767. Constantes de texto são delimitadas por aspas duplas; o lexema armazenado não inclui as aspas.

### 6.4 Tratamento de erros

O analisador identifica, entre outras situações:

- comentário de bloco sem delimitador de fechamento;
- constante de texto sem aspas de fechamento;
- quebra de linha dentro de uma constante de texto;
- constante inteira fora do intervalo aceito;
- caractere que não pertence aos símbolos ou padrões reconhecidos.

Quando ocorre um erro léxico, o método retorna `false`, informa a mensagem correspondente e limpa a sequência de tokens. Dessa forma, uma análise parcialmente concluída não é apresentada como se fosse uma sequência válida.

## 7. Análise sintática

### 7.1 Responsabilidade e estratégia

O analisador sintático está implementado em `core/src/parser/analisadorsintatico.cpp`, com interface em `core/include/jack/parser/analisadorsintatico.h`.

A classe `AnalisadorSintatico` recebe a sequência de tokens e o código-fonte original. A implementação utiliza **análise descendente recursiva**: cada método responsável por uma construção da linguagem reconhece os elementos correspondentes e, quando aplicável, constrói os nós da AST.

O método público `analisar()` retorna um `SyntaxResult`, que representa sucesso ou falha.

### 7.2 Resultado da análise

O resultado é definido em `syntaxresult.h`:

```cpp
struct SyntaxSuccess {
    std::unique_ptr<ProgramNode> program;
};

struct SyntaxFailure {
    std::string message;
    SourceSpan sourceSpan;
};

using SyntaxResult = std::variant<SyntaxSuccess, SyntaxFailure>;
```

Em caso de sucesso, o resultado contém a raiz da AST. Em caso de falha, contém uma mensagem e um intervalo de origem (`SourceSpan`) associado ao erro.

O analisador registra o primeiro erro sintático encontrado. A implementação não realiza recuperação para continuar analisando o arquivo depois desse erro.

### 7.3 Construções reconhecidas

Os métodos privados do analisador correspondem às principais construções da gramática Jack implementada. Entre eles estão:

| Método | Construção reconhecida |
|---|---|
| `compileClass` | Declaração de classe |
| `compileClassVarDec` | Declaração de variável de classe |
| `compileSubroutine` | Declaração de sub-rotina |
| `compileParameterList` | Lista de parâmetros |
| `compileSubroutineBody` | Corpo de sub-rotina |
| `compileVarDec` | Declaração de variável local |
| `compileStatements` | Sequência de instruções |
| `compileLet` | Instrução de atribuição |
| `compileIf` | Instrução condicional |
| `compileWhile` | Laço de repetição |
| `compileDo` | Chamada de sub-rotina |
| `compileReturn` | Instrução de retorno |
| `compileExpression` | Expressão |
| `compileTerm` | Termo |
| `compileExpressionList` | Lista de expressões |

O analisador também trata chamadas de sub-rotina qualificadas e não qualificadas, acesso a elementos de arranjo, expressões entre parênteses e operações unárias.

### 7.4 Unidade de compilação

A implementação atual considera **uma classe por arquivo `.jack`**. O método `analisar()` reconhece uma declaração de classe e verifica se não existem tokens adicionais depois dela. Essa é uma decisão do projeto para a unidade de análise atual.

Embora `ProgramNode` armazene uma coleção de classes, o analisador sintático atualmente constrói um programa com uma única classe por arquivo.

### 7.5 Expressões e operadores

O método `compileExpression` reconhece um termo seguido por zero ou mais pares formados por operador binário e outro termo. Os operadores tratados nessa etapa são:

`+`, `-`, `*`, `/`, `&`, `|`, `<`, `>` e `=`.

A construção da AST encadeia as operações da esquerda para a direita. A implementação não cria níveis separados de precedência entre esses operadores; o agrupamento segue a estrutura da regra de expressão utilizada pelo analisador.

## 8. Árvore sintática abstrata (AST)

### 8.1 Finalidade e estrutura

A AST representa a estrutura sintática reconhecida pelo parser sem depender da interface gráfica ou da representação XML. Ela é formada por nós especializados, organizados por relações de herança e composição.

A classe abstrata `ASTNode` é a base dos nós e mantém um `SourceSpan`, que representa a localização da construção no código-fonte. As classes `StatementNode` e `ExpressionNode` funcionam como bases para categorias específicas de instruções e expressões.

Os principais grupos de nós são:

| Grupo | Exemplos |
|---|---|
| Estrutura do programa | `ProgramNode`, `ClassNode` |
| Declarações | `VariableDeclarationNode`, `SubroutineDeclarationNode`, `ParameterNode` |
| Instruções | `LetStatementNode`, `IfStatementNode`, `WhileStatementNode`, `DoStatementNode`, `ReturnStatementNode` |
| Expressões | `BinaryExpressionNode`, `UnaryExpressionNode`, `ParenthesizedExpressionNode`, `ArrayAccessNode`, `SubroutineCallNode` |
| Valores e referências | `IntegerLiteralNode`, `StringLiteralNode`, `KeywordConstantNode`, `VariableReferenceNode` |

Enums complementares representam operadores e categorias da linguagem, como `BinaryOperator`, `UnaryOperator`, `KeywordConstant`, `SubroutineKind` e `VariableKind`.

### 8.2 Composição e gerenciamento de memória

Os nós que compõem a árvore são mantidos por meio de `std::unique_ptr`. Esse modelo estabelece propriedade exclusiva: o objeto que contém o ponteiro é responsável pela vida útil do nó apontado.

Por exemplo, `ProgramNode` mantém as classes que compõem o programa, e `ClassNode` mantém suas declarações de variáveis e sub-rotinas. Nós de instruções e expressões, por sua vez, mantêm os elementos subordinados necessários para representar suas construções.

O uso de ponteiros inteligentes evita a necessidade de liberar manualmente cada nó e torna explícita a transferência de propriedade durante a construção da árvore.

### 8.3 Informações de origem

Cada `ASTNode` possui um `SourceSpan`, com posições inicial e final de linha e coluna. Essas informações permitem associar elementos da árvore a trechos do código-fonte e dão suporte à localização de erros sintáticos.

## 9. Geração de XML

A geração XML está separada em dois componentes: um para os tokens e outro para a AST.

### 9.1 XML léxico

`GeradorXmlLexico`, declarado em `core/include/jack/xml/geradorxmllexico.h`, recebe a sequência de tokens e retorna uma string contendo o documento XML.

O documento utiliza o elemento raiz `<tokens>`. Cada token é representado por um elemento correspondente à sua categoria:

| Categoria interna | Elemento XML |
|---|---|
| `Keyword` | `keyword` |
| `Symbol` | `symbol` |
| `IntegerConstant` | `integerConstant` |
| `StringConstant` | `stringConstant` |
| `Identifier` | `identifier` |

O gerador escapa os caracteres `&`, `<` e `>` nos textos dos elementos, evitando que esses caracteres sejam interpretados como parte da marcação XML.

### 9.2 XML sintático

`GeradorXmlSintatico`, declarado em `core/include/jack/xml/geradorxmlsintatico.h`, recebe um `ProgramNode` e retorna uma string XML que representa a estrutura sintática da AST.

O gerador percorre os nós e emite elementos que correspondem às construções da gramática, como `class`, `classVarDec`, `subroutineDec`, `parameterList`, `subroutineBody`, `varDec`, `statements`, `letStatement`, `ifStatement`, `whileStatement`, `doStatement`, `returnStatement`, `expression`, `term` e `expressionList`.

A implementação utiliza um escritor interno para controlar abertura e fechamento de elementos, indentação e emissão de terminais. Também identifica os tipos concretos de nós de expressão e instrução para produzir a representação correspondente.

Assim como no gerador léxico, os caracteres `&`, `<` e `>` são escapados quando aparecem nos textos dos terminais.

### 9.3 Comparador XML

O Comparador XML é declarado em
`core/include/jack/xml/comparadorxml.h` e implementado em
`core/src/xml/comparadorxml.cpp`. Ele compara estruturalmente documentos
XML léxicos e sintáticos, recebidos como strings em memória.

A modalidade léxica espera a raiz `<tokens>`; a modalidade sintática
espera a raiz `<class>`. A comparação considera nomes e hierarquia dos
elementos, quantidade e ordem dos nós, atributos e conteúdo textual. A
ordem textual dos atributos não é significativa. Whitespace estrutural
entre elementos pode ser ignorado, mas o conteúdo textual dos nós
terminais é preservado integralmente, inclusive espaços iniciais/finais
e conteúdo formado somente por whitespace.

Comentários, declaração XML e instruções de processamento são
desconsiderados. DTD efetivo é rejeitado e não há resolução de entidades
ou recursos externos. A sequência `<!DOCTYPE` presente dentro de
comentário ou CDATA não é classificada como DTD.

A interface do núcleo retorna um `XMLComparisonResult`, com os estados
`Equivalent`, `Different`, `InvalidExpectedXML`, `InvalidActualXML`,
`IncompatibleDocuments` e `InternalError`. O limite padrão é de 20
divergências detalhadas exibidas, mantendo a contabilização total.

A dependência pugixml 1.16 é encapsulada na implementação. Seus tipos
não são expostos na interface pública do comparador.

## 10. Interfaces de utilização

### 10.1 Interface de linha de comando

A aplicação CLI é iniciada por `main()`, que seleciona o fluxo de execução conforme os argumentos recebidos. Quando há argumentos, instancia `CompiladorVMCLI` e encaminha a execução para o processamento não interativo. Quando não há argumentos, inicia o fluxo interativo, coordenado pela classe `Application`, responsável pelo menu, pela seleção e leitura de arquivos, pela execução das análises e pela apresentação dos resultados.

A CLI oferece operações interativas para executar as análises léxica e sintática, exportar XML, executar os testes organizados no projeto, analisar arquivos em lote e comparar XML. As opções 10 e 11 realizam, respectivamente, a comparação léxica e sintática. Cada operação solicita o caminho do XML de referência e o caminho do XML gerado; os arquivos podem estar em diretórios distintos.

A CLI realiza a interação, validação dos caminhos e leitura dos arquivos. Em seguida, encaminha os conteúdos em memória ao `jack::xml::ComparadorXml`, pertencente ao `jack_core`, e apresenta equivalência, divergências ou erros.

Separadamente, a CLI disponibiliza um fluxo não interativo acionado por argumentos, utilizado para gerar arquivos de código VM a partir de programas Jack. A geração VM é, portanto, uma funcionalidade da CLI não interativa, apoiada pelos componentes do `jack_core`. Ela não faz parte do menu interativo nem da interface gráfica, conforme o escopo validado nesta etapa.

A CLI utiliza `jack_core`; não contém uma segunda implementação dos analisadores.

### 10.2 Interface gráfica

A aplicação gráfica utiliza Qt Widgets. A janela principal organiza a apresentação em áreas de análise léxica e sintática, com visualização do código-fonte, tokens, XML e árvore hierárquica.

A seleção de um arquivo na interface gráfica inicia o processamento: primeiro é executada a análise léxica e, se ela for bem-sucedida, a análise sintática. A interface apresenta os resultados e habilita a visualização sintática quando há uma AST válida.

Os realçadores de sintaxe e os componentes visuais pertencem à camada gráfica. Eles não substituem os analisadores do núcleo.

## 11. Testes funcionais por arquivos

O projeto inclui arquivos de entrada em:

- `tests/inputs/validos`: exemplos que devem ser aceitos pelas análises léxica e sintática;
- `tests/inputs/invalidos`: exemplos destinados a provocar falhas léxicas ou sintáticas.

A organização atual contém oito arquivos na categoria `validos` e nove na categoria `invalidos`.

A interface CLI oferece uma opção que executa os testes dessas duas categorias e compara o resultado obtido com o resultado esperado para cada grupo. Também oferece uma operação para analisar em lote os arquivos `.jack` encontrados diretamente em um diretório informado.

O componente `TestadorLote` realiza a leitura dos arquivos, executa a análise léxica e, quando ela é bem-sucedida, executa a análise sintática. Os resultados são contabilizados por arquivo e consolidados em um resumo.

Esse mecanismo é um executor funcional baseado em arquivos de entrada para as análises léxica e sintática. Existem também testes independentes para SymbolTable, VMWriter e VMCodeGenerator, executados separadamente. Para o Comparador XML, foi acrescentado um alvo opcional de regressão integrado ao CTest, controlado pela opção `BUILD_XML_COMPARATOR_REGRESSION_TESTS`, desativada por padrão. Quando habilitada, essa configuração cria o alvo `comparadorxml_regression_tests` e registra um teste no CTest. O teste automatizado contém quatro casos para distinguir ocorrências de `<!DOCTYPE` em comentário/CDATA de declarações DTD efetivas.

Na validação registrada em 28/11/2026, a configuração com `BUILD_XML_COMPARATOR_REGRESSION_TESTS=ON` compilou `jack_pugixml`, `jack_core`, `jack_cli` e `comparadorxml_regression_tests`; o CTest apresentou 1/1 teste aprovado. Esse resultado se refere ao conjunto de quatro casos de regressão do comparador e não deve ser interpretado como execução de toda a suíte do projeto.

## 12. Decisões arquiteturais e limites atuais

As decisões e características a seguir ajudam a compreender o comportamento atual do projeto:

- **Núcleo independente das interfaces:** os analisadores, a AST e os geradores XML ficam em `core`, compartilhados pela CLI e pela GUI.
- **Separação entre léxico e sintaxe:** o parser recebe tokens produzidos pelo lexer e também recebe o código-fonte original para apoiar a localização de construções e erros.
- **Construção explícita da AST:** a análise sintática não se limita a validar a sequência de tokens; ela constrói uma representação estruturada do programa.
- **Uma classe por arquivo:** a unidade de análise atual reconhece uma declaração de classe por arquivo `.jack` e rejeita tokens excedentes após essa declaração.
- **Falha sintática sem recuperação:** o parser registra o primeiro erro e encerra a análise, sem tentar sincronizar e continuar a partir de uma construção posterior.
- **Análise semântica não comprovada como etapa completa:** a existência da SymbolTable não demonstra, por si só, que verificações como resolução completa de identificadores, compatibilidade de tipos, argumentos e parâmetros ou consistência de retornos estejam implementadas e integradas.
- **Geração VM disponível pela CLI não interativa:** o `VMCodeGenerator` e o `VMWriter` estão implementados no núcleo e são utilizados pelo fluxo não interativo da CLI. Não há evidência de geração VM pela GUI.
- **Saídas distintas:** os geradores XML produzem representações léxica e sintática; o Comparador XML compara os documentos correspondentes; o fluxo VM da CLI produz arquivos `.vm`.
- **Comparação XML no núcleo:** a comparação é estrutural e independente de Qt e de acesso a arquivos; a CLI executa a interação e a leitura dos documentos.
- **Dependência XML encapsulada:** pugixml 1.16 é vinculada privadamente ao `jack_core` por meio do alvo `jack_pugixml`.

Esses limites delimitam o que se pode concluir a partir de uma execução bem-sucedida: o arquivo foi aceito pelas regras léxicas e sintáticas implementadas, mas isso não significa que todas as propriedades semânticas do programa tenham sido verificadas.

## 13. Possíveis pontos de evolução

A separação entre núcleo e interfaces permite evoluções sem exigir que toda a aplicação seja reestruturada. Entre os caminhos tecnicamente compatíveis com a arquitetura atual estão:

- aprimorar as mensagens e a localização de erros;
- avaliar a inclusão de recuperação sintática, caso seja desejável reportar mais de um erro por execução;
- completar ou ampliar a análise semântica, caso isso venha a ser definido como requisito;
- ampliar a cobertura de testes automatizados para além dos quatro casos de regressão XML e dos cenários funcionais já documentados;
- adicionar outras interfaces que reutilizem `jack_core`, caso necessário.

Esses itens são possibilidades de evolução, não funcionalidades já implementadas.

---

*Documento técnico referente à arquitetura atualmente implementada no JackCompiler.*
