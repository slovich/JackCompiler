# Estado atual do JackCompiler

> Fotografia documental baseada na auditoria inicial realizada em 20/11/2026. Commit de referência: `db2c2726526160a158a4a2579f9ebf74df7edefa`. Confirmar novamente o estado do repositório antes de qualquer intervenção posterior.

## Identificação

- Projeto: JackCompiler.
- Repositório: `https://github.com/slovich/JackCompiler`.
- Branch examinada: `main`.
- Branch remota: `origin/main`.
- Commit: `db2c2726526160a158a4a2579f9ebf74df7edefa`.
- Mensagem: `docs: restore README and refine build documentation`.
- Estado reportado pelo Git: `main...origin/main`, sem alterações locais.
- Contexto acadêmico: disciplina de Compiladores, Engenharia da Computação, UFMA.
- Linguagem de implementação: C++17.
- Interface gráfica: Qt Widgets.
- Sistema de build: CMake.

## Estrutura de alvos

O `CMakeLists.txt` declara:

- `jack_core`: biblioteca estática com lexer, parser, AST e geradores XML.
- `jack_cli`: aplicação de terminal vinculada a `jack_core`, sem dependência Qt.
- `compilador`: aplicação GUI criada quando `BUILD_GUI` está habilitado; depende de Qt Widgets e Qt Xml.
- `BUILD_GUI` tem valor padrão `ON`.
- O CMake procura Qt 6 ou Qt 5, componentes Widgets e Xml, quando a GUI está habilitada.
- `JACK_PROJECT_ROOT` é definido como macro de compilação para `jack_cli`, com valor igual a `CMAKE_CURRENT_SOURCE_DIR`.

Não há, no CMake examinado, `enable_testing()`, `include(CTest)`, `add_test()` ou `BUILD_TESTING`. Portanto, não há integração CTest configurada nesse arquivo.

## Funcionalidades confirmadas

### Lexer
A interface pública verificada de `AnalisadorLexico` é:

`bool analisar(std::vector<Token>& tokens, std::string& erro) const;`

### Parser e AST
A interface pública verificada do parser é:

`SyntaxResult analisar();`

O resultado diferencia sucesso e falha (`SyntaxSuccess` / `SyntaxFailure`). Em caso de sucesso, o parser constrói um `ProgramNode` e transfere para ele a classe analisada. O método também rejeita tokens remanescentes após o fim da classe.

A inspeção de `compileExpression()` confirmou construção binária acumulando o lado esquerdo, com associatividade à esquerda e sem níveis separados de precedência para os operadores binários.

`registrarErro()` preserva a primeira falha sintática registrada. Não foi identificada recuperação sintática nas evidências examinadas.

### XML
Os geradores do core retornam `std::string`:
- `GeradorXmlLexico::gerar(const std::vector<Token>&)`
- `GeradorXmlSintatico::gerar(const ProgramNode&)`

As interfaces examinadas não dependem de tipos Qt.

### GUI
No método `on_pushButton_selecionar_clicked()`:
1. limpa estado e resultados sintáticos anteriores;
2. verifica se há arquivo selecionado;
3. abre e lê o arquivo;
4. executa análise léxica;
5. em caso de falha léxica, informa o erro e encerra o fluxo;
6. em caso de sucesso, atualiza tokens, resumo, código e habilita exportação lexical;
7. executa análise sintática;
8. no sucesso, guarda a AST, gera XML sintático, atualiza a visualização XML e constrói a árvore;
9. habilita a aba sintática e a exportação sintática.

A árvore sintática da GUI é construída lendo o XML sintático com `QXmlStreamReader` e preenchendo um `QStandardItemModel`. Portanto, a visualização hierárquica é derivada do XML, não construída diretamente percorrendo a AST.

### CLI — opção 8: testes funcionais classificados
`Application::executarTestes()`:
- usa `JACK_PROJECT_ROOT/tests/inputs` como diretório padrão;
- aceita outro diretório; caminhos relativos são resolvidos em relação à raiz do projeto;
- exige os subdiretórios `validos` e `invalidos`;
- enumera arquivos regulares `.jack` diretamente em cada pasta;
- ordena os caminhos;
- executa análise léxica e, se esta passar, análise sintática;
- considera aprovado um arquivo de `validos` quando ambas as análises passam;
- considera aprovado um arquivo de `invalidos` quando a análise léxica ou a sintática falha;
- contabiliza aprovados, reprovados e erros de execução.

Inventário observado em `tests/inputs`:
- `validos`: 8 arquivos `.jack`.
- `invalidos`: 9 arquivos `.jack`.

### CLI — opção 9: processamento em lote
`Application::testarArquivosEmLote()` solicita caminho absoluto e chama `TestadorLote::executar()`.

O `TestadorLote`:
- exige caminho absoluto;
- valida existência e diretório;
- considera arquivos regulares com extensão exata `.jack` diretamente no diretório, sem recursão;
- ordena os arquivos;
- distingue arquivos encontrados de arquivos efetivamente processados;
- trata erros de leitura, léxicos e sintáticos separadamente;
- só executa análise sintática quando a análise léxica passa;
- apresenta o primeiro erro identificado em cada etapa conforme o comportamento implementado.

A opção 9 não usa a classificação prévia `validos`/`invalidos`.

## Inventário documental observado

Na coleta de 20/11/2026 foram listados sete arquivos sob `docs/`:

- `docs/desenvolvimento/uso-de-inteligencia-artificial.md`
- `docs/instalacao/compilacao.md`
- `docs/tecnico/arquitetura.md`
- `docs/tecnico/linguagem-jack.md`
- `docs/tecnico/testes.md`
- `docs/usuario/manual-cli.md`
- `docs/usuario/manual-gui.md`

O README principal está na raiz. `docs/contexto/` não apareceu na listagem dessa coleta; este pacote propõe criá-la sem alterar ou mover a documentação existente.

## Atualização após a etapa de geração de código VM

**Data de referência:** 21/11/2026.

Esta seção atualiza a fotografia documental inicial de 20/11/2026 com os resultados apresentados nos relatórios de validação da SymbolTable, do VMWriter e do VMCodeGenerator.

### Componentes implementados

| Componente | Estado registrado |
|---|---|
| Lexer | Implementado |
| Parser | Implementado |
| AST | Implementada |
| SymbolTable | Implementada; validação comportamental aprovada em 8/8 cenários |
| VMWriter | Implementado; validação comportamental aprovada em 14/14 cenários |
| VMCodeGenerator | Implementado e validado no escopo definido |
| Geradores XML | Implementados |
| Integração CLI/VM | Validada nos cenários apresentados |

### Validação funcional do Project 11

O encerramento da etapa de geração de código VM registrou os seguintes resultados:

- Seis programas oficiais compilados com sucesso: Seven, ConvertToBin, Square, Average, Pong e ComplexArrays.
- Seis programas com funcionalidades principais validadas no escopo dos testes.
- Seis diretórios processados.
- Onze arquivos VM gerados.
- 2.396 linhas de código VM submetidas à inspeção textual.

Os resultados foram obtidos em testes envolvendo o VM Emulator desktop e o emulador online oficial do Nand2Tetris.

### Limitações e ressalvas conhecidas

**Square — redimensionamento:** o redimensionamento pelas teclas X e Z não funcionou. O mesmo comportamento foi reproduzido utilizando o compilador oficial no mesmo emulador. Não há evidência suficiente para atribuir essa ocorrência ao JackCompiler.

**VM Emulator desktop:** foram observadas inconsistências na interação por teclado. A causa não foi determinada. As características do ambiente utilizado permanecem registradas no relatório de validação, sem atribuição causal à JVM, ao Java ou ao sistema operacional.

**`pointer 0`:** a hipótese de perda de contexto em chamadas encadeadas de métodos permanece como registro investigativo histórico, sem defeito confirmado.

### Alcance da aprovação

A etapa de geração de código VM foi encerrada com aprovação nos cenários apresentados. Não foram identificadas correções obrigatórias com base nas evidências disponíveis.

A aprovação não representa prova exaustiva de correção para todas as construções da linguagem Jack. Permanecem fora do alcance da validação a cobertura integral da linguagem, a ausência de regressões em programas não testados e a exaustividade dos cenários de chamadas, objetos, arrays e strings.

Os resultados do emulador online foram relatados pelo desenvolvedor e não reproduzidos pessoalmente pelo Testador nesta conversa. Essa origem deve ser preservada ao citar tais evidências.

### Referências de validação

Os resultados acima estão fundamentados nos seguintes registros:

- Relatório de validação da SymbolTable — 20/11/2026.
- Relatório de validação técnica do VMWriter — 20/11/2026.
- Parecer final de validação do VMCodeGenerator e integração CLI/VM — etapa encerrada.

Esta atualização substitui a classificação anterior de geração VM como funcionalidade não implementada. A fotografia inicial de 20/11/2026 permanece preservada como registro histórico, mas não representa o estado posterior à conclusão da etapa de geração VM.

## Formalização da frente UX Designer

**Data de referência:** 22/11/2026.

Foi formalizada uma frente especializada de UX Designer, responsável pelo estudo e pela especificação da experiência de uso da interface gráfica do JackCompiler.

Seu escopo compreende:

- organização visual da interface;
- navegação e hierarquia das informações;
- fluxos de interação;
- estados visuais e feedback ao usuário;
- responsividade;
- integração visual de funcionalidades disponibilizadas pelo `jack_core`, quando sua disponibilidade estiver confirmada.

A atuação do UX Designer é de estudo e especificação. Não inclui implementação de código de produção, alteração da arquitetura do núcleo ou da CLI, nem aprovação autônoma de decisões visuais.

As propostas produzidas por essa frente não devem ser interpretadas como funcionalidades implementadas ou decisões aprovadas. Após aprovação do responsável pelo projeto, as especificações devem ser encaminhadas ao Analista, que ficará responsável por formalizar requisitos e critérios de aceitação para as etapas posteriores.

Esta formalização registra uma organização de trabalho do projeto. **Não documenta alteração implementada na GUI**, nem confirma que as oportunidades de melhoria identificadas pelo UX Designer já tenham sido incorporadas ao software.
