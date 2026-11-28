# Handoff — Analista

## Papel

Atuar na análise e especificação do JackCompiler: requisitos, regras da linguagem, comportamento esperado, critérios de aceitação e delimitação de escopo.

## Primeiro passo

Ler os documentos de contexto e confirmar o estado atual do repositório. Usar `docs/tecnico/linguagem-jack.md`, `docs/tecnico/arquitetura.md` e `docs/tecnico/testes.md` como documentação oficial inicial, confrontando afirmações relevantes com código e testes quando necessário.

Antes de iniciar uma atividade que dependa do histórico ou de decisões anteriores do projeto, consultar também as conversas e documentos de contexto disponíveis. Na ausência de informações suficientes, consultar o Arquiteto de Documentação ou solicitar o documento de estado atualizado. Não presumir decisões, funcionalidades ou implementações que não estejam documentadas ou verificadas.

## Limites

- Não presumir que exista uma etapa completa de análise semântica. A geração de código VM está implementada no escopo documentado por meio do fluxo não interativo da CLI; antes de afirmar que ela permanece presente no checkout atual, confirmar a implementação e sua integração nas fontes.
- Não confundir a existência de `SymbolTable`, `VMWriter` ou `VMCodeGenerator` com a implementação de uma etapa completa de análise semântica.
- Separar regras da linguagem Jack de limitações específicas da implementação atual.
- Distinguir comportamento observado, requisito aprovado e proposta futura.
- Não alterar código.
- Não alterar documentação oficial sem autorização.
- Não transformar inferência em decisão.
- Distinguir informações comunicadas pelo Desenvolvedor, resultados reportados pelo Testador, testes executados pelo responsável pelo projeto e verificações feitas diretamente pelo Analista.
- Não considerar o projeto publicado apenas porque uma implementação foi concluída ou validada: verificar separadamente commit, tag, release e push.

## Estado consolidado conhecido — 28/11/2026

> Esta seção registra o estado comunicado nos relatórios e atualizações disponíveis. Não substitui a conferência do checkout atual, dos testes ou da documentação oficial do repositório.

### Geração de código VM

- A geração VM foi reportada como implementada e integrada ao fluxo não interativo da CLI.
- `jack_core` é o núcleo C++17 compartilhado, independente de Qt; a CLI também não depende de Qt.
- A existência e a validação isolada de `SymbolTable` e `VMWriter` foram reportadas anteriormente. Isso não equivale, por si só, a uma etapa completa de análise semântica.
- O relatório de validação do Comparador XML, de 28/11/2026, registra uma regressão da geração VM com seis projetos oficiais do Nand2Tetris Project 11: 11 arquivos Jack processados e 11 arquivos VM gerados, executados no VM Emulator.
- Os resultados de `Seven`, `ConvertToBin`, `Average`, `ComplexArrays` e `Pong` foram aprovados. `Square` apresentou movimentação, mas não redimensionamento; o mesmo comportamento foi reproduzido com o compilador oficial, portanto não foi classificado como defeito confirmado do JackCompiler.
- Antes de afirmar que esse estado permanece no repositório atual, conferir fontes, CMake e testes.

### Runtime estático da CLI Windows

- Foi reportada a integração permanente, no `CMakeLists.txt` raiz, das opções `-static-libgcc` e `-static-libstdc++` somente para o alvo `jack_cli`, em Windows com compilador GNU.
- A integração não aplica opções globais a `jack_core` nem usa `-static` genérico.
- `libwinpthread-1.dll` permanece como dependência externa a distribuir junto ao executável Windows.
- O Desenvolvedor reportou testes funcionais, comparação byte a byte dos arquivos VM entre builds original e experimental e validação em outro ambiente Windows sem MinGW instalado.
- Essas informações derivam do relatório do Desenvolvedor; verificar o CMake e a distribuição atual antes de orientar uma nova compilação ou release.

### Comparador XML — implementação concluída segundo os relatórios

- Em 28/11/2026, o Desenvolvedor reportou a implementação concluída, integrada ao `jack_core` e submetida à avaliação independente do Testador.
- Arquivos principais reportados:
  - `core/include/jack/xml/comparadorxml.h`
  - `core/src/xml/comparadorxml.cpp`
- A dependência reportada é `pugixml 1.16`, em `third_party/pugixml/`, integrada por meio do alvo estático `jack_pugixml` e ligada privadamente ao `jack_core`. Seus tipos não são expostos na interface pública do comparador.
- A implementação reportada compara XML léxico (raiz esperada `<tokens>`) e sintático (raiz esperada `<class>`), considerando estrutura, ordem dos elementos, atributos e conteúdo textual.
- A política aprovada de whitespace foi preservada: ignorar somente whitespace estrutural entre elementos e preservar integralmente o texto dos nós terminais, sem `trim()` ou compressão.
- Comentários, declaração XML e instruções de processamento são desconsiderados. DTD efetivo é rejeitado e não há resolução de recursos externos.
- Os estados de resultado reportados são `Equivalent`, `Different`, `InvalidExpectedXML`, `InvalidActualXML`, `IncompatibleDocuments` e `InternalError`. O limite padrão reportado é de 20 divergências detalhadas, com contabilização do total.
- A nomenclatura dos XML exportados foi corrigida para `Nome_lexico.xml` e `Nome_sintatico.xml`. As referências oficiais podem manter a convenção Nand2Tetris: `NomeT.xml` para léxico e `Nome.xml` para sintático.
- A interface final, por decisão posterior do responsável pelo projeto, utiliza duas opções independentes no menu interativo:
  - **10 — Comparador Léxico**
  - **11 — Comparador Sintático**
- Cada operação solicita o caminho do XML de referência e o caminho do XML gerado. Os arquivos podem estar em diretórios diferentes e não precisam ter a mesma nomenclatura.
- Não fazem parte do fluxo final: comando externo `compare-xml`, comparação por diretórios, busca automática, listas CSV/TXT ou comparação em lote.
- A CLI realiza interação, leitura e validação dos caminhos; o `jack_core` executa a comparação estrutural.
- O relatório registra aprovação do Testador para comparações léxica e sintática, caminhos com espaços, arquivos inexistentes, diretórios no lugar de arquivos, XML malformado, diagnósticos de linha/coluna, raízes incompatíveis, retorno ao menu, repetição das exportações e falha real de permissão de leitura.
- O responsável pelo projeto reportou comparações reais de XML léxico e sintático do projeto `ArrayTest`, ambas equivalentes às respectivas referências.
- O relatório também registra compilação Windows com `BUILD_GUI=OFF` e `BUILD_XML_COMPARATOR_REGRESSION_TESTS=ON`, com `jack_pugixml`, `jack_core`, `jack_cli` e `comparadorxml_regression_tests` compilados; CTest: 1/1 aprovado.
- O Testador reportou avaliação em outro computador Windows 11 sem Qt, CMake ou MinGW, com o executável e a DLL fornecida.
- O parecer do Desenvolvedor foi favorável ao encerramento da implementação. Não foram realizados commit, tag, release ou push nesta etapa.

### Decisões de arquitetura e interface relacionadas ao comparador

Decisões aprovadas pelo responsável e preservadas na implementação reportada:

- Comparador integrado ao `jack_core`, mantendo independência de Qt.
- Uso de `pugixml`, encapsulada e sem tipos da biblioteca na interface pública.
- Exportação dos XMLs do compilador com sufixos `_lexico` e `_sintatico`.
- Preservação exata do texto terminal; somente whitespace estrutural entre elementos pode ser ignorado.
- Interface final com as opções 10 e 11 do menu interativo, em substituição ao comando não interativo `compare-xml` proposto originalmente.

As funcionalidades de comparação em lote e por diretórios foram retiradas do escopo final por decisão posterior; não devem ser tratadas como pendências ou defeitos.

### Situação de publicação

- O relatório de 28/11/2026 declara expressamente que não houve commit, tag, release ou push da etapa do Comparador XML.
- A integração do runtime estático também havia sido reportada sem commit/tag/release/push na ocasião de seu relatório.
- Portanto, não afirmar que as alterações estão publicadas no repositório remoto. A situação atual deve ser conferida com o responsável e no Git antes de qualquer procedimento de entrega.

## Entregas

- especificação ou análise com escopo e premissas;
- regras e requisitos identificados;
- critérios de aceitação verificáveis;
- casos-limite e ambiguidades;
- evidências e referências aos arquivos;
- decisões que dependem do responsável;
- handoff de retorno ao Arquiteto de Documentação.
