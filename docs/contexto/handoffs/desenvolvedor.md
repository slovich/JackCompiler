# Handoff — Desenvolvedor

**Projeto:** JackCompiler  
**Repositório informado:** `slovich/JackCompiler`  
**Atualização:** 28/11/2026
**Escopo:** desenvolvimento dos componentes do JackCompiler, incluindo o `VMCodeGenerator`, o Comparador XML, a investigação das ocorrências em Square e Pong e a portabilidade da CLI Windows.

## Papel

Implementar mudanças de código do JackCompiler somente quando houver escopo e critérios suficientemente definidos e aprovados.

A cada conclusão de fase, atualizar este documento com fatos verificáveis, decisões, arquivos afetados, comandos e resultados. Distinguir implementação, integração e validação. Não registrar funcionalidades apenas planejadas como implementadas.

## Primeiro passo

Antes de iniciar uma atividade dependente do histórico ou das decisões anteriores:

1. Ler os documentos de contexto do projeto e este handoff.
2. Confirmar branch, commit e estado da working tree no checkout do usuário.
3. Examinar a arquitetura, os arquivos e as interfaces reais antes de propor ou editar implementação.
4. Se o checkout ou os documentos necessários não estiverem disponíveis, solicitar o material atualizado. Não inferir que um pacote antigo representa o estado atual.

## Limites

- Não presumir que uma funcionalidade futura já foi aprovada.
- Não ampliar escopo por conveniência.
- Preservar a separação entre `jack_core`, `jack_cli` e GUI Qt.
- Não introduzir dependência Qt no core ou na CLI sem decisão arquitetural explícita.
- Não alterar documentos oficiais como substituto de corrigir uma divergência técnica.
- Não fazer commit, tag, release ou push sem autorização explícita.
- Não declarar build, teste ou execução que não tenha sido efetivamente realizada.
- Não tratar especificação como evidência de implementação.
- Não introduzir correção especulativa sem identificar a causa e estabelecer um teste de regressão.

## Entregas

- resumo da implementação;
- arquivos criados/alterados/removidos;
- decisões técnicas e alternativas consideradas;
- comandos de build e validação executados;
- resultados e limitações;
- riscos e pendências;
- handoff de retorno ao Arquiteto de Documentação.

---

# Estado técnico registrado

## 1. Componentes implementados antes do VMCodeGenerator

### SymbolTable

Estado informado e validado pelo responsável em etapa anterior:

- Arquivos:
  - `core/include/jack/symbol/symboltable.h`
  - `core/src/symbol/symboltable.cpp`
- `CMakeLists.txt` atualizado para integrar o componente.
- Escopos de classe e subrotina, índices por categoria, argumento implícito 0 em métodos, precedência de resolução subrotina sobre classe e recusa de duplicidades sem consumir índice.
- Validação anteriormente reportada: build de `jack_core` e `jack_cli` no Windows com CMake/MinGW; 8/8 testes aprovados, conforme registro de transferência anterior.

### VMWriter

Estado informado e validado pelo responsável em etapa anterior:

- Arquivos:
  - `core/include/jack/vm/vmwriter.h`
  - `core/src/vm/vmwriter.cpp`
- `CMakeLists.txt` atualizado para integrar o componente.
- Emissão de comandos VM, validação de segmentos/índices/nomes, `finalizar()` com flush e bloqueio de novas emissões após finalização.
- Validação anteriormente reportada: build de `jack_core` e `jack_cli` no Windows com CMake/MinGW; 14/14 testes aprovados, conforme registro de transferência anterior.

Os resultados acima são históricos e devem ser novamente executados no checkout vigente caso uma próxima atividade dependa deles.

## 2. VMCodeGenerator

### Objetivo e escopo

Implementar a tradução de uma classe Jack representada pela AST para código VM, em C++17, sem dependência de Qt, utilizando `SymbolTable` e `VMWriter` existentes.

A implementação registrada contempla tradução de declarações e sub-rotinas, comandos, expressões, chamadas, objetos, arrays e strings, dentro do escopo da especificação fornecida para a atividade. A existência de uma construção no código não substitui testes de cobertura ou validação funcional.

### Arquivos identificados no pacote de checkout examinado

- `core/src/vm/vmcodegenerator.cpp`
- `core/include/jack/vm/vmcodegenerator.h`
- `CMakeLists.txt` (integração do componente)
- testes:
  - `tests/vmcodegenerator_tests.cpp`
  - `tests/vmcodegenerator_extended_tests.cpp`

O pacote também continha os componentes `SymbolTable`, `VMWriter`, AST e CLI. Esta lista descreve o pacote examinado nesta fase; não afirma que o working tree atual do usuário esteja idêntico, pois branch, commit e `git status` do checkout local não foram confirmados nesta conversa.

### Decisões técnicas registradas

- O gerador recebe uma `ClassNode` e publica o resultado em um fluxo de saída.
- A geração é preparada em um `ostringstream` temporário e publicada somente após a tradução completa e a finalização do `VMWriter`, evitando publicar VM parcial em caso de erro de geração.
- Para métodos, o prólogo gerado associa `argument 0` a `pointer 0`.
- Para construtores, aloca a quantidade de campos via `Memory.alloc` e associa o resultado a `pointer 0`.
- Chamadas qualificadas por variável usam o objeto como argumento implícito; chamadas qualificadas por nome de classe não adicionam receiver; chamadas sem qualificador usam o objeto corrente como receiver.
- Não foi introduzido salvamento/restauração manual de `pointer 0` ao redor de cada chamada. A hipótese foi considerada, mas não demonstrada; o protocolo VM `call`/`return` deve preservar/restaurar os segmentos do chamador.
- Acesso a arrays utiliza `pointer 1`/`that`, portanto não constitui, por si só, evidência de alteração de `THIS`.
- Não modificar `generateCall()` ou adicionar tratamento especial de `THIS` sem evidência executável e teste de regressão.

### Integração

A CLI foi integrada ao VMCodeGenerator no pacote examinado. O responsável informou que a CLI compilou os seis programas oficiais do Project 11 e produziu 11 arquivos VM. Isso demonstra integração no fluxo de compilação CLI para os exemplos utilizados, mas não comprova integração equivalente na GUI Qt. Não afirmar que a GUI oferece geração VM sem confirmação específica.

## 3. Build e testes do checkout empacotado

Nesta conversa, os pacotes fornecidos pelo responsável foram examinados:

- `JackCompiler-CLI-VM-Integration.zip` (pacote anterior já disponível no contexto da investigação);
- `nand2tetris_projects.zip`;
- `vm-output.zip`.

O checkout extraído do pacote `JackCompiler-CLI-VM-Integration.zip` foi compilado no ambiente de análise com GCC 14.2.0 e CMake, desativando apenas a interface Qt. Os alvos `jack_core` e `jack_cli` foram compilados com sucesso.

**Limite de evidência:** este build foi executado no checkout empacotado, não no ambiente Windows do usuário. Não equivale à confirmação do estado atual da branch/working tree local.

Os testes unitários existentes no pacote incluem testes específicos de `VMCodeGenerator`, incluindo geração de expressões, construtores, prólogo de método, arrays, strings, chamadas qualificadas, chamadas de função de classe, self-call, `if` e `while`. Esses testes foram identificados por inspeção; não registrar aqui uma contagem ou aprovação de toda a suíte, pois tal resultado não foi estabelecido nesta etapa.

## 4. Validação dos seis programas do Project 11

O usuário informou que compilou os seis programas oficiais por meio da CLI integrada ao VMCodeGenerator, gerando 11 arquivos VM. Os comandos de compilação retornaram código 0.

Programas: Seven, Average, ComplexArrays, ConvertToBin, Square e Pong.

### Comparação dos arquivos gerados

Os arquivos de `vm-output.zip` foram comparados com arquivos regenerados a partir dos fontes Jack oficiais de `nand2tetris_projects.zip`. Os 11 arquivos VM regenerados foram idênticos, byte a byte, aos arquivos entregues em `vm-output.zip`.

Isso comprova reprodutibilidade da geração no checkout empacotado examinado, não correção semântica de todos os programas.

### Resultados relatados inicialmente no VM Emulator desktop 2.5

O usuário executou os arquivos individualmente no VM Emulator 2.5 com Jack OS interno e relatou:

- Seven: aprovado;
- Average: aprovado;
- ComplexArrays: aprovado;
- ConvertToBin: aprovado;
- Square: falha funcional (teclas reconhecidas pela interface, mas sem movimento/redimensionamento esperado);
- Pong: falha funcional (bola com comportamento de aparecer/desaparecer e controles sem resposta).

Posteriormente, o usuário compilou os fontes Square e Pong com o compilador oficial do Nand2Tetris e executou os resultados no mesmo emulador desktop. Relatou comportamento equivalente ao dos arquivos do projeto. Também compilou e executou o `KeyboardTest` oficial: o emulador mostrava a tecla pressionada, mas a aplicação só registrava a entrada após manter a tecla pressionada por cerca de 10 segundos.

Esses resultados enfraqueceram a hipótese de defeito específico do VMCodeGenerator e motivaram teste em outro ambiente.

### Resultados relatados no VM Emulator online oficial

O usuário executou os programas no emulador online oficial do Nand2Tetris e informou:

- Pong: funcionou perfeitamente;
- Square: movimentou-se nas quatro direções;
- Square: comandos de aumentar e diminuir tamanho não funcionaram.

O usuário repetiu a compilação oficial de Square e obteve o mesmo resultado: movimentação nas quatro direções, mas sem redimensionamento.

### Conclusão técnica desta fase

- O comportamento de Pong no emulador online é evidência funcional positiva para o arquivo gerado pelo projeto nesse ambiente.
- A movimentação de Square nas quatro direções também foi observada como funcional.
- A ausência de redimensionamento de Square foi reproduzida com o arquivo gerado pelo compilador oficial, no mesmo emulador online. Portanto, não há evidência para atribuir essa limitação ao JackCompiler.
- O comportamento anômalo do VM Emulator desktop foi observado também com programas compilados oficialmente e com o `KeyboardTest`. A causa não foi identificada.
- A versão Java informada pelo usuário é `1.8.0_513`, Java SE Runtime Environment, HotSpot Client VM. Não foi demonstrado que o Java seja a causa dos problemas do emulador desktop.
- Não se deve afirmar que o emulador desktop é incompatível com Java 8 ou que a versão Java causou a falha.
- Não foi executado aqui o emulador online nem o VM Emulator desktop; esses resultados são relatos do usuário.

**Decisão:** encerrar a etapa de implementação do VMCodeGenerator sem alteração corretiva especulativa baseada nos comportamentos descritos. O encerramento não significa garantia de ausência de defeitos; significa que os testes realizados não forneceram evidência suficiente para justificar uma correção nesta etapa.

## 5. Limitações, riscos e pendências

1. Antes de qualquer nova alteração, confirmar no checkout local vigente a branch, commit, working tree e conteúdo real dos arquivos. O estado local não deve ser inferido a partir de snapshots ZIP anteriores.
2. Não há validação funcional da geração VM pela GUI Qt registrada neste handoff.
3. A causa do comportamento anômalo observado no VM Emulator desktop permanece desconhecida. Os resultados variaram entre o emulador desktop e o emulador online, inclusive com programas compilados pelo compilador oficial.
4. O redimensionamento de Square não apresentou comportamento funcional no ambiente avaliado, mas a limitação foi reproduzida com o compilador oficial. Não deve ser classificada como defeito confirmado do JackCompiler.
5. Não há evidência suficiente para afirmar cobertura integral de todas as construções Jack ou ausência absoluta de defeitos no VMCodeGenerator.
6. A compatibilidade com outras versões do emulador, do Java ou do sistema operacional não foi investigada sistematicamente.
7. A avaliação de distribuição Windows do Comparador XML foi realizada em um ambiente independente específico, sem MinGW, Qt ou CMake instalados. O resultado não garante compatibilidade com todas as configurações possíveis do Windows.
8. Não foi registrado hash do executável obtido no computador do Testador para comparação criptográfica com o binário encaminhado. A correspondência da versão avaliada foi estabelecida pelo fluxo de entrega e pelo relato do Testador, não por igualdade de hashes coletados nos dois ambientes.
9. A regressão VM W-12 cobre os seis projetos oficiais do Project 11 e 11 arquivos Jack/VM, mas não equivale a uma prova de correção de todas as construções da linguagem.
10. Não foram realizados commit, tag, release ou push para consolidar esta fase.

## 6. Próximas orientações de desenvolvimento

- Considerar a implementação do VMCodeGenerator concluída para o escopo desta fase, conforme decisão do responsável.
- Não reabrir a investigação de `THIS` nem alterar a estratégia de chamadas apenas com base na suspeita inicial: os testes comparativos posteriores apontaram para comportamento equivalente do compilador oficial nos casos analisados.
- Qualquer nova correção deve começar com reprodução mínima, identificação da primeira divergência entre semântica Jack e VM gerada e teste de regressão.
- Preservar a separação arquitetural: `jack_core` contém a lógica de compilação independente de Qt; `jack_cli` consome o core sem Qt; GUI permanece na camada Qt. A integração VM com GUI não foi confirmada.
- Não realizar commit, tag, release ou push sem autorização explícita.
- Ao concluir uma próxima fase, atualizar este handoff e entregar o documento junto ao relatório.

---


## 7. Vinculação estática dos runtimes GCC no `jack_cli` — integração concluída em 24/11/2026

### Objetivo

Permitir que o executável Windows da CLI seja executado em máquinas que não tenham MinGW instalado, eliminando as dependências externas `libstdc++-6.dll` e `libgcc_s_seh-1.dll`, sem alterar a arquitetura do projeto ou introduzir vinculação estática global.

### Alteração permanente no CMake

Foi integrada ao `CMakeLists.txt` raiz, imediatamente após a vinculação de `jack_cli` com `jack_core`, a seguinte condição:

```cmake
# Vincula estaticamente os runtimes GCC somente no executável CLI
# em builds Windows com compilador GNU (MinGW).
# A dependência libwinpthread-1.dll permanece externa e deve ser
# considerada no empacotamento da distribuição Windows.
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(jack_cli PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()
```

**Escopo e decisões preservadas:**

- As opções são aplicadas somente ao alvo `jack_cli`, por meio de `target_link_options(... PRIVATE ...)`.
- A condição limita a alteração a builds Windows com compilador GNU/MinGW.
- Não foi usado `-static` genérico.
- `jack_core`, GUI Qt, opções globais de compilação, toolchain e padrão C++17 não receberam alterações por essa decisão.
- `libwinpthread-1.dll` permanece uma dependência externa intencional e deve acompanhar o executável na distribuição Windows.
- Não foi criada regra automática de empacotamento; a inclusão da DLL é uma etapa explícita do procedimento de release.
- O arquivo auxiliar experimental de vinculação total (`-static`) não faz parte da solução e não deve ser utilizado.

### Ambiente e build de validação registrados

Ambiente Windows informado e utilizado pelo responsável:

- Toolchain MinGW Qt GCC 13.1.0: `C:\Qt\Tools\mingw1310_64\bin`.
- CMake: 3.30.5, em `C:\Qt\Tools\CMake_64\bin`.
- Gerador: `MinGW Makefiles`.
- `BUILD_GUI=OFF` para gerar a CLI sem depender da GUI Qt.
- Diretório de build independente: `build-static-runtime-integrated`.
- Executável produzido: `build-static-runtime-integrated\jack_cli.exe`.

Comandos de configuração e compilação utilizados:

```powershell
cmake -S . -B build-static-runtime-integrated `
    -G "MinGW Makefiles" `
    -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe" `
    -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" `
    -DBUILD_GUI=OFF

cmake --build .\build-static-runtime-integrated --target jack_cli --parallel
```

Resultado observado: configuração concluída e alvo `jack_cli` compilado com sucesso. O log mostrou a compilação de `jack_core`, seguida da compilação e linkedição de `jack_cli`.

### Evidências da vinculação

O arquivo `build-static-runtime-integrated\CMakeFiles\jack_cli.dir\link.txt` foi inspecionado. A linha de link efetiva continha:

```text
g++.exe -static-libgcc -static-libstdc++ ...
```

Dados do executável integrado registrado nesta validação:

- Tamanho: 5.721.199 bytes.
- SHA-256: `EE2E2BC5F8B0AA31576C1CA45EDB128D73DB86987F888126AD7874B698D98228`.

A tabela de importação foi examinada com:

```powershell
& "C:\Qt\Tools\mingw1310_64\bin\objdump.exe" -p `
    .\build-static-runtime-integrated\jack_cli.exe |
    Select-String "DLL Name"
```

Dependências observadas:

```text
KERNEL32.dll
msvcrt.dll
libwinpthread-1.dll
```

Portanto, `libstdc++-6.dll` e `libgcc_s_seh-1.dll` não aparecem como dependências do executável integrado. A DLL `libwinpthread-1.dll` foi localizada em `C:\Qt\Tools\mingw1310_64\bin\libwinpthread-1.dll`, com 53.248 bytes.

### Testes funcionais comparativos executados

O executável integrado foi comparado com o executável original preservado em `build-cli\jack_cli.exe`. As saídas de compilação foram direcionadas a diretórios separados para evitar sobrescrita.

1. **Ajuda (`--help`):** executou com sucesso no binário integrado. A apresentação de caracteres acentuados no terminal permaneceu corrompida da mesma forma observada no build original; não foi identificada regressão atribuível à vinculação.
2. **Compilação individual:** `tests\inputs\validos\teste_chamadas_output.jack` foi compilado pelos dois executáveis. Ambos retornaram código `0`; os arquivos `.vm` tinham 703 bytes e `fc.exe /b` confirmou que eram idênticos. SHA-256 da saída: `67C1025C7673D43835B0F0CCD932F17B42124EBEC7A3D58E04EF2B6E1DF2F679`.
3. **Compilação em lote:** `nand2tetris\projects\10\ExpressionLessSquare` foi compilado pelos dois executáveis. Ambos compilaram três arquivos, sem falhas, com código `0`. `Main.vm`, `Square.vm` e `SquareGame.vm` foram comparados individualmente com `fc.exe /b`; nenhum apresentou diferença.
4. **Entrada sintaticamente inválida:** `tests\inputs\invalidos\teste_classe_incompleta.jack` produziu o mesmo diagnóstico nos dois executáveis (linha 1, coluna 7, token `{` inesperado), resumo de zero compilados e uma falha, código de saída `1`. Nenhum dos diretórios de saída recebeu arquivo `.vm`.
5. **Destino de saída inválido:** foi usado um arquivo comum chamado `destino-bloqueado` como destino, impedindo a criação/preparação do diretório. Os dois executáveis produziram o mesmo diagnóstico `Not a directory` e código de saída `3`.

Os testes demonstram equivalência nos cenários exercitados; não constituem prova matemática de ausência de regressões em todos os programas e ambientes.

### Teste em Windows sem MinGW

O responsável informou que preparou e executou o pacote em um ambiente Windows sem MinGW, contendo o executável `jack_cli.exe` e `libwinpthread-1.dll`, sem incluir `libstdc++-6.dll` ou `libgcc_s_seh-1.dll`. O usuário confirmou: **“funcionou sem qualquer erros.”**

Esse resultado é um relato de execução do responsável. Não foram registrados nesta conversa o modelo/versão exata do Windows, a configuração detalhada do ambiente, nem logs completos dessa execução externa. Ainda assim, a confirmação direta atende ao objetivo prático do teste independente informado pelo usuário.

### Procedimento obrigatório para repetir a cada release Windows da CLI

A vinculação estática dos runtimes é uma configuração de build, não uma propriedade que deve ser presumida para qualquer executável. A cada release, gerar novamente a CLI usando o `CMakeLists.txt` com a condição acima e repetir as verificações abaixo:

1. Configurar um diretório de build limpo ou independente com Windows + MinGW/GNU e `BUILD_GUI=OFF`, indicando explicitamente o `g++.exe` e o `mingw32-make.exe` da mesma instalação MinGW.
2. Compilar o alvo `jack_cli` com `cmake --build <diretorio-build> --target jack_cli --parallel`.
3. Inspecionar `CMakeFiles\jack_cli.dir\link.txt` e confirmar que a linha de link contém `-static-libgcc` e `-static-libstdc++`.
4. Inspecionar a tabela de importação do novo executável com `objdump -p` e confirmar que `libstdc++-6.dll` e `libgcc_s_seh-1.dll` não aparecem. Registrar as demais dependências.
5. Incluir `libwinpthread-1.dll` correspondente à mesma instalação/toolchain MinGW usada na compilação. Não incluir `libstdc++-6.dll` nem `libgcc_s_seh-1.dll` se a inspeção confirmar sua ausência.
6. Testar `jack_cli.exe --help` e uma compilação real.
7. Comparar, quando aplicável, saídas `.vm` do release com as do build de referência, incluindo pelo menos um caso individual e um lote representativo.
8. Testar o pacote em ambiente Windows independente, sem MinGW instalado ou disponível no `PATH`, contendo somente o executável, `libwinpthread-1.dll` e os arquivos de entrada necessários ao teste.
9. Registrar versão do toolchain, comando de build, hash SHA-256 do executável, dependências PE, arquivos incluídos no pacote e resultado do teste independente.

Exemplo de inspeção de dependências:

```powershell
& "<caminho-do-MinGW>\objdump.exe" -p `
    .\<diretorio-build>\jack_cli.exe |
    Select-String "DLL Name"
```

**Critério de distribuição:** não considerar a CLI Windows validada para release apenas porque compilou. Exigir a confirmação das flags de link, a inspeção das DLLs importadas e o teste do pacote em Windows sem MinGW. A dependência `libwinpthread-1.dll` deve permanecer explícita no pacote.

### Estado da fase

A integração permanente das opções de vinculação estática no CMake e a validação funcional descrita acima foram concluídas. O teste em Windows sem MinGW também foi confirmado pelo responsável. A alteração está tecnicamente validada para os cenários executados e pode ser considerada concluída no escopo desta fase.

Não foi realizado commit, tag, release ou push nesta atividade.

---

## 8. Comparador XML — primeira avaliação e correções solicitadas em 25/11/2026

### Objetivo e decisões

O Comparador XML compara estruturalmente XML léxico e sintático do JackCompiler com referências Nand2Tetris. A implementação reside no `jack_core`, utiliza pugixml 1.16 e não expõe tipos pugixml na interface pública.

Decisões aprovadas e preservadas:

- Arquivos exportados: `Nome_lexico.xml` e `Nome_sintatico.xml`.
- Referências: `NomeT.xml` para léxico e `Nome.xml` para sintático.
- Raízes esperadas: `<tokens>` para léxico e `<class>` para sintático.
- Comparação estrutural de nomes, hierarquia, quantidade e ordem dos elementos, atributos e conteúdo textual.
- Ordem textual de atributos não é significativa.
- Whitespace estrutural entre elementos pode ser ignorado; conteúdo terminal deve ser preservado integralmente, inclusive espaços iniciais/finais, múltiplos espaços e conteúdo formado somente por whitespace.
- Comentários, declaração XML e instruções de processamento são ignorados.
- DTD efetivo é rejeitado; não se deve resolver entidades ou recursos externos.
- Limite padrão de 20 divergências exibidas, com contagem total preservada.
- Acesso exclusivamente pelo menu interativo da CLI: opção 10 para XML léxico e opção 11 para XML sintático. Não introduzir o comando não interativo `compare-xml`.

### Arquitetura e arquivos

- `core/include/jack/xml/comparadorxml.h` — API pública.
- `core/src/xml/comparadorxml.cpp` — parsing e comparação com pugixml encapsulada.
- `cli/include/jack/cli/application.h` — declaração da integração.
- `cli/src/application.cpp` — opções 10/11, seleção de arquivos, processamento e apresentação.
- `CMakeLists.txt` — dependência pugixml e integração dos componentes.
- `third_party/pugixml/` — distribuição pugixml 1.16, com fontes, cabeçalhos, licença e README.

Na implementação inicial, a CLI comparava por diretórios e nomes-base, com busca não recursiva. A lista podia ser informada por um nome, nomes separados por vírgula ou arquivo `.txt`; linhas vazias e comentários `#` eram ignorados e duplicatas eram ignoradas com aviso. Esse fluxo foi substituído pela readequação registrada na seção 9. Correspondências históricas:
- léxico: referência `<Nome>T.xml`, gerado `<Nome>_lexico.xml`;
- sintático: referência `<Nome>.xml`, gerado `<Nome>_sintatico.xml`.

O core recebe strings XML em memória e retorna `XMLComparisonResult`, com estados `Equivalent`, `Different`, `InvalidExpectedXML`, `InvalidActualXML`, `IncompatibleDocuments` e `InternalError`. Não acessa arquivos, diretórios, terminal ou Qt.

### Primeira avaliação independente

O Testador reportou resultados positivos em TST-01 a TST-07, TST-11 a TST-15 e TST-16, com ressalvas documentadas. TST-10 foi adaptado: três classes reais disponíveis foram comparadas por tipo, 3/3 equivalentes em léxico e 3/3 em sintático. TST-17 foi considerado não aplicável por causa da decisão posterior de usar menu interativo. TST-08 não foi executado, TST-09 ficou bloqueado e TST-18 foi parcial.

A primeira avaliação identificou dois defeitos que impedem a aprovação:

**DEF-XML-01 — colisão nos nomes sugeridos pelos exportadores.** Ambos sugeriam `Nome.xml`, permitindo que a exportação sintática sobrescrevesse a léxica.

**DEF-XML-02 — detecção textual indevida de DTD.** A implementação procurava a sequência literal `<!DOCTYPE` no texto XML e rejeitava documentos válidos quando a sequência aparecia em comentário ou CDATA.

### Correções implementadas após a primeira avaliação

**DEF-XML-01:** em `cli/src/application.cpp`, os destinos sugeridos foram separados:
- exportação léxica: `<nome-base>_lexico.xml`;
- exportação sintática: `<nome-base>_sintatico.xml`.

Os geradores continuam responsáveis por produzir o conteúdo XML; a nomenclatura dos arquivos é definida pelo fluxo de exportação da CLI.

**DEF-XML-02:** em `core/src/xml/comparadorxml.cpp`, foi removida a busca textual por `<!DOCTYPE`. O parsing passa a solicitar `pugi::parse_doctype` e inspeciona os nós de primeiro nível em busca de `pugi::node_doctype`. Assim, a sequência dentro de comentário ou CDATA não é classificada como DTD. Uma declaração DOCTYPE efetiva continua sendo rejeitada após o parsing. Não é configurado resolver externo; a implementação não deve carregar recursos externos.

Foi preparado o arquivo `tests/comparadorxml_regression_tests.cpp` com quatro casos:
1. sequência `<!DOCTYPE` em comentário — deve ser aceita;
2. sequência `<!DOCTYPE` em CDATA — deve ser aceita como conteúdo;
3. DOCTYPE efetivo no XML de referência — deve ser rejeitado;
4. DOCTYPE efetivo no XML gerado, incluindo identificador externo — deve ser rejeitado.

Foi acrescentada ao `CMakeLists.txt` a opção `BUILD_XML_COMPARATOR_REGRESSION_TESTS`, desativada por padrão. Quando habilitada, cria o alvo `comparadorxml_regression_tests` e registra-o no CTest. Os quatro casos foram preparados pelo Desenvolvedor e posteriormente compilados e executados pelo responsável no checkout Windows, conforme os resultados registrados abaixo.

### Estado de integração e validação

Após a aplicação do pacote de correções no checkout Windows do responsável, foram executados os seguintes comandos:

```powershell
cmake -S . -B build-cli -DBUILD_GUI=OFF -DBUILD_XML_COMPARATOR_REGRESSION_TESTS=ON
cmake --build build-cli --target jack_cli comparadorxml_regression_tests
ctest --test-dir build-cli --output-on-failure
```

Resultados informados pelo responsável:

- Configuração CMake concluída com sucesso.
- `jack_pugixml`, `jack_core` e `jack_cli` compilados com sucesso.
- `comparadorxml_regression_tests` compilado e vinculado com sucesso.
- CTest: 1/1 teste aprovado, 100% dos testes executados sem falhas.

O conjunto automatizado contempla quatro casos relacionados à detecção de DTD: sequência textual em comentário, sequência em CDATA, DTD efetivo na referência e DTD efetivo no XML gerado com identificador externo.

Esses resultados comprovam a execução bem-sucedida dos testes automatizados preparados para DEF-XML-02. Não representam aprovação integral do Comparador XML nem substituem a avaliação independente do Testador.

A correção DEF-XML-01 está presente no código da CLI, com nomes sugeridos distintos para os arquivos léxico e sintático. Sua verificação específica pelos cenários de exportação ainda deve ser registrada.

A nova avaliação independente do Testador permanece obrigatória.

### Encerramento da primeira rodada de correções

A lista de pendências acima foi superada parcialmente pela segunda avaliação independente, conforme o relato posterior do Testador: a nomenclatura dos exportadores foi confirmada funcionalmente e os quatro cenários de DTD foram aprovados. A avaliação integral do Comparador XML permaneceu em andamento e, posteriormente, originou a solicitação de readequação da interface registrada na seção 9.

Não foi realizado commit, tag, release ou push nesta atividade.

---

# Registro de mudanças deste handoff

## 22/11/2026 — Encerramento da fase VMCodeGenerator

- Consolidado o estado dos componentes `SymbolTable`, `VMWriter` e `VMCodeGenerator`.
- Registrados os arquivos do gerador e dos testes identificados no pacote de checkout.
- Documentada a integração da CLI, sem presumir integração da GUI.
- Registrado build de `jack_core` e `jack_cli` no checkout empacotado, com GCC 14.2.0/CMake e GUI desativada.
- Registrada comparação byte a byte dos 11 arquivos VM regenerados com os arquivos fornecidos.
- Registrados resultados relatados pelo usuário no VM Emulator desktop e no emulador online oficial.
- Registrada a decisão de encerrar a fase sem correção especulativa.
- Nenhum commit, tag, release ou push foi realizado por esta atividade.

## 24/11/2026 — Registro da integração de runtimes GCC e revisão do handoff

- Registrada a integração e validação da vinculação estática dos runtimes GCC no `jack_cli` para builds Windows com MinGW.
- Incluídos os comandos de configuração/build, evidências de link e dependências PE, testes comparativos e procedimento para futuras distribuições Windows.
- Reposicionada a seção de vinculação estática antes do registro de mudanças, preservando a sequência estrutural do documento.
- O status do comparador de XML foi marcado como não confirmado no checkout atual; o handoff anterior o registrava como pendente, mas não foi fornecido checkout ou evidência atualizada para confirmar esse estado.
- Nenhuma operação Git foi realizada.

## Histórico anterior preservado

As instruções originais de papel, limites arquiteturais e entregas deste handoff foram mantidas e ampliadas para refletir o estado e as decisões desta fase.


## 25/11/2026 — Implementação do Comparador XML e correções solicitadas pelo Testador

- Incorporado ao handoff o estado da implementação do Comparador XML, substituindo a referência anterior de estado não confirmado.
- Registrados a arquitetura, os arquivos, as decisões aprovadas, a integração no menu interativo e os resultados da primeira avaliação independente.
- Preparada a correção dos nomes sugeridos pelos exportadores léxico e sintático, evitando colisão entre `Nome_lexico.xml` e `Nome_sintatico.xml`.
- Substituída a detecção textual de DTD por identificação estrutural de `pugi::node_doctype`, preservando a rejeição de DTD efetivo e evitando falsos positivos em comentários/CDATA.
- Preparado um alvo opcional de testes de regressão para os dois defeitos; os testes não foram executados pelo Desenvolvedor.
- A implementação corrigida ainda depende de compilação no checkout Windows do responsável e de nova avaliação independente do Testador.
- Removido o handoff separado `comparador_xml_implementacao.md` do pacote de trabalho; seu conteúdo técnico foi incorporado a este documento.
- Nenhuma operação Git foi realizada.

## 25/11/2026 — Aplicação e validação automatizada das correções no Windows

- O responsável aplicou o pacote de correções ao checkout local.
- Foram removidos os documentos auxiliares `README_CORRECOES.md` e `docs/contexto/handoffs/comparador_xml_implementacao.md`, consolidando o registro técnico neste handoff.
- A configuração CMake foi concluída com `BUILD_GUI=OFF` e `BUILD_XML_COMPARATOR_REGRESSION_TESTS=ON`.
- Os alvos `jack_cli` e `comparadorxml_regression_tests` foram compilados com sucesso no Windows.
- O CTest registrou 1/1 teste aprovado, contemplando os quatro casos automatizados de regressão relacionados à detecção de DTD.
- A verificação funcional específica dos nomes sugeridos pelos exportadores permanece pendente.
- A nova avaliação independente do Testador permanece obrigatória.
- Nenhuma operação Git foi realizada.

## 9. Readequação da interface do Comparador XML — 28/11/2026

### Solicitação e comportamento aprovado

Após a segunda avaliação funcional, o Testador informou resultados positivos para as correções DEF-XML-01 e DEF-XML-02 e solicitou simplificação do fluxo da CLI. O usuário seleciona o tipo de comparação no menu e informa diretamente dois caminhos:

1. caminho do XML de referência;
2. caminho do XML gerado.

As opções independentes permanecem: opção 10 — Comparar XML léxico; opção 11 — Comparar XML sintático. Os arquivos podem estar em diretórios distintos e não precisam obedecer a uma convenção de nomes.

Foram removidos do fluxo de comparação: seleção de diretórios, busca por nome-base, listas separadas por vírgula, arquivo `.txt`, processamento em lote e busca não recursiva. A alteração não modifica o fluxo de exportação dos XML.

### Implementação

A alteração ficou restrita a `cli/src/application.cpp`, no método `Application::compararXml(bool lexico)`:

- solicita o caminho do XML de referência e o caminho do XML gerado;
- verifica existência e tipo regular dos caminhos usando `std::filesystem`;
- abre os dois arquivos em modo binário e verifica falhas de abertura/leitura;
- passa os conteúdos em memória ao `jack::xml::ComparadorXml` com o tipo léxico ou sintático correspondente;
- preserva a apresentação de equivalência, divergências, caminhos estruturais, descrições, valores esperado/gerado, total de divergências e localização de erros de parsing;
- retorna ao fluxo normal da CLI; `processarOpcao()` mantém a pausa existente antes da reapresentação do menu principal.

Não foram alterados `jack::xml::ComparadorXml`, sua interface pública, pugixml 1.16, `jack_core`, regras estruturais, segurança do parser, geradores XML, exportadores, opções do menu, integração VM ou CMake. Não foi adicionado teste automatizado específico para a interação da CLI; esses cenários permanecem para avaliação independente.

### Arquivo modificado

- `cli/src/application.cpp` — substituição do fluxo de diretórios/listas/lote por comparação individual de dois caminhos explícitos.

Nenhum outro arquivo de código foi alterado nesta readequação.

### Build técnico realizado pelo Desenvolvedor

Ambiente Linux do Desenvolvedor: GCC 14.2.0 e CMake (versão detectada pelo ambiente: 3.31.6). Com GUI desativada e testes de regressão habilitados:

```bash
cmake -S . -B build -DBUILD_GUI=OFF -DBUILD_XML_COMPARATOR_REGRESSION_TESTS=ON
cmake --build build --target jack_cli comparadorxml_regression_tests -j2
```

Resultado: configuração concluída e os alvos `jack_cli` e `comparadorxml_regression_tests` compilados e vinculados com sucesso. O CTest foi executado com `ctest --test-dir build --output-on-failure`: 1/1 teste aprovado, sem falhas. Esse resultado confirma o conjunto automatizado de regressão do comparador, mas não substitui a compilação Windows nem a avaliação independente do Testador.

Também foram realizados testes manuais de fumaça da CLI no Linux, usando diretórios e nomes de arquivos com espaços:
- opção 10: comparação léxica equivalente entre dois caminhos explícitos — resultado equivalente;
- opção 11: comparação sintática equivalente entre dois caminhos explícitos — resultado equivalente;
- caminho do XML gerado inexistente — diagnóstico de arquivo não encontrado;
- XML gerado malformado — diagnóstico de parsing com linha e coluna;
- documentos divergentes — divergência textual identificada com caminho estrutural e valores de referência/gerado;
- após a operação, a CLI reapresentou o menu principal.

Esses testes manuais não cobrem falha real de permissão de leitura nem substituem a matriz completa do Testador. Não foi executado nesta etapa teste de regressão funcional do compilador VM.

### Verificações pendentes para o Testador

1. Comparação léxica por dois caminhos explícitos.
2. Comparação sintática por dois caminhos explícitos.
3. Arquivos em diretórios diferentes e caminhos com espaços.
4. Caminho de referência inexistente e caminho do XML gerado inexistente.
5. Falha de leitura de um dos arquivos.
6. XML malformado e diagnóstico de linha/coluna.
7. Equivalência com formatação diferente e identificação de divergências.
8. Retorno ao menu principal após a comparação.
9. Regressão das funcionalidades de compilação VM.
10. Repetição dos cenários DEF-XML-01 e DEF-XML-02, conforme matriz atualizada pelo Testador.

### Estado após a avaliação independente

A readequação foi compilada pelo responsável no Windows, com `BUILD_GUI=OFF` e `BUILD_XML_COMPARATOR_REGRESSION_TESTS=ON`. Os alvos `jack_cli` e `comparadorxml_regression_tests` foram compilados com sucesso e o CTest registrou 1/1 teste aprovado.

O responsável também executou no Windows comparações reais de ArrayTest:
- léxico: `MainT_original.xml` versus `Main_lexico.xml` — documentos equivalentes;
- sintático: `Main_original.xml` versus `Main_sintatico.xml` — documentos equivalentes.

O SHA-256 informado para `build-cli\jack_cli.exe` produzido nessa compilação foi:

`872C19742D981CD3B7A4DE58C468CA3D8794C77E08D0C7D363A9D8225D0B80FE`

A avaliação independente posterior do Testador aprovou os cenários da interface, inclusive caminhos com espaços, arquivos inexistentes, diretório no lugar de arquivo, XML malformado, linha/coluna, raiz incompatível e retorno ao menu após sucesso ou erro. O Testador também repetiu os cenários DEF-XML-01 e DEF-XML-02 e os aprovou.

A regressão VM e a validação de distribuição Windows estão consolidadas na seção 10. A implementação do Comparador XML e sua avaliação independente foram encerradas. Não há pendência de código identificada para esta fase.

Não foi realizado commit, tag, release ou push.

---

## 10. Encerramento do Comparador XML e regressão independente — 28/11/2026

### 10.1. Parecer e escopo avaliado

O Testador concluiu a avaliação independente da implementação do Comparador XML, incluindo as correções DEF-XML-01 e DEF-XML-02, a interface readequada e a regressão das funcionalidades de geração VM. O Desenvolvedor analisou o relatório conclusivo e manifestou parecer favorável ao encerramento.

A interface final permanece:
- opção 10 — Comparar XML léxico;
- opção 11 — Comparar XML sintático.

Cada operação solicita exclusivamente o caminho do XML de referência e o caminho do XML gerado. Os arquivos podem estar em diretórios distintos. Não há busca automática, nomes-base, listas de classes, arquivos CSV/TXT ou comparação em lote no fluxo final.

A alteração da interface substituiu expressamente as propostas originais do Analista de comando `compare-xml`, comparação por diretórios e processamento em lote. Essas propostas não são defeitos pendentes: foram substituídas por decisão posterior do responsável pelo projeto. Códigos de saída específicos e ajuda de comando também não se aplicam ao fluxo interativo final.

### 10.2. Resultados DEF-XML-01 e DEF-XML-02

**DEF-XML-01 — colisão nos nomes dos XML exportados: APROVADO.**

O Testador confirmou no Windows 11 que as exportações léxica e sintática produzem arquivos distintos, respectivamente `Nome_lexico.xml` e `Nome_sintatico.xml`, preservando os arquivos e permitindo repetir as exportações sem erro.

**DEF-XML-02 — identificação de DTD: APROVADO.**

Foram aprovados os cenários:
- sequência `<!DOCTYPE fake>` dentro de comentário — aceita;
- sequência `<!DOCTYPE fake>` dentro de CDATA — aceita;
- DTD efetivo no XML de referência — rejeitado;
- DTD efetivo no XML gerado — rejeitado.

Os três cenários principais também foram reproduzidos no executável Windows: comentário e CDATA foram aceitos e DTD efetivo foi rejeitado com diagnóstico de DTD não suportado.

### 10.3. Readequação da interface — resultados do Testador

O Testador executou no Windows 11 e aprovou:

| Cenário | Resultado |
|---|---|
| Comparação léxica de ArrayTest | PASS |
| Comparação sintática de ArrayTest | PASS |
| Caminhos contendo espaços | PASS |
| Arquivo de referência inexistente | PASS |
| Arquivo gerado inexistente | PASS |
| Diretório informado no lugar de arquivo | PASS |
| XML malformado | PASS |
| Diagnóstico de linha e coluna | PASS |
| Raiz XML incompatível | PASS |
| Retorno ao menu após sucesso | PASS |
| Retorno ao menu após erro | PASS |
| Repetição das exportações | PASS |

O responsável pelo projeto também realizou comparações reais de ArrayTest no Windows:
- léxico: `MainT_original.xml` versus `Main_lexico.xml` — `[OK] Os documentos XML são equivalentes.`;
- sintático: `Main_original.xml` versus `Main_sintatico.xml` — `[OK] Os documentos XML são equivalentes.`.

Essas duas execuções foram realizadas pelo responsável e são distintas da avaliação independente do Testador.

### 10.4. Validação da distribuição em Windows independente

O Testador extraiu o executável e a DLL fornecida em:

`C:\Teste JackCompiler\Aplicacao`

A aplicação iniciou e executou os testes funcionais no outro computador Windows 11, sem instalação de Qt, CMake ou MinGW. O conjunto distribuído foi suficiente para o ambiente efetivamente testado.

Esta validação da distribuição não possui identificador individual próprio. O identificador W-00 corresponde especificamente à inicialização do executável no ambiente Windows independente. O identificador W-13 corresponde exclusivamente ao teste de falha de permissão de leitura do arquivo XML gerado, descrito no relatório do Testador; não identifica a validação da distribuição.

Limite: não extrapolar o resultado para todas as versões, arquiteturas ou configurações possíveis do Windows. O relatório não apresenta hash coletado no computador independente para comparação criptográfica com o executável originalmente encaminhado.

### 10.5. Regressão VM — W-12

O Testador compilou os seis projetos oficiais do Nand2Tetris Project 11 a partir de 11 arquivos Jack, utilizando o modo não interativo existente:

`jack_cli.exe --compile <diretorio> --output <diretorio>`

Todos os projetos retornaram código de saída zero e produziram a quantidade esperada de arquivos VM:

| Projeto | Arquivos Jack | Arquivos VM esperados/gerados | Resultado |
|---|---:|---:|---|
| Seven | 1 | 1/1 | PASS |
| ConvertToBin | 1 | 1/1 | PASS |
| Square | 3 | 3/3 | PASS* |
| Average | 1 | 1/1 | PASS |
| Pong | 4 | 4/4 | PASS |
| ComplexArrays | 1 | 1/1 | PASS |
| **Total** | **11** | **11/11** | **PASS** |

\* Em Square, `PASS*` refere-se exclusivamente à compilação dos três arquivos Jack e à geração dos três arquivos VM esperados. A avaliação funcional é apresentada separadamente: movimentação aprovada; redimensionamento não aprovado.

Os arquivos foram executados no VM Emulator desktop. Seven, ConvertToBin, Average, ComplexArrays e Pong apresentaram os resultados esperados. Square permitiu movimentação, mas não redimensionamento.

**Ressalva de Square:** o comportamento de redimensionamento também foi reproduzido pelo Testador com o compilador oficial do Nand2Tetris. Portanto, não há evidência para atribuir essa limitação ao JackCompiler. O resultado não foi tratado como defeito da implementação.

A conclusão do Testador foi de que não foram identificadas regressões funcionais na geração VM associadas às alterações do Comparador XML, dentro dos projetos e cenários avaliados. Isso não constitui prova de correção de todas as construções Jack.

### 10.6. Identificação do executável Windows

O responsável compilou a versão readequada no checkout Windows e informou o seguinte SHA-256 para `build-cli\\jack_cli.exe`:

`872C19742D981CD3B7A4DE58C468CA3D8794C77E08D0C7D363A9D8225D0B80FE`

O hash identifica o binário produzido naquela compilação. Não foi fornecido hash independente do arquivo executado no computador do Testador; assim, a correspondência do binário da avaliação é baseada no processo de entrega e no relatório do Testador, não em verificação criptográfica bilateral.

### 10.7. Parecer do Desenvolvedor e encerramento

Com base na implementação, nas evidências de compilação, nos testes realizados pelo responsável e no relatório conclusivo do Testador, o Desenvolvedor declarou-se favorável ao encerramento.

Estado final:
- implementação do Comparador XML: concluída;
- integração ao `jack_core` e à CLI: concluída;
- DEF-XML-01: aprovado;
- DEF-XML-02: aprovado;
- interface final opções 10 e 11: aprovada;
- tratamento de erros e caminhos: aprovado nos cenários executados;
- distribuição Windows: validada no ambiente independente testado, com a limitação de generalização registrada;
- regressão VM W-12: compilação e geração dos arquivos VM aprovadas nos seis projetos; validação funcional aprovada nos cenários avaliados de Seven, ConvertToBin, Average, ComplexArrays e Pong; em Square, movimentação aprovada e redimensionamento não aprovado;
- avaliação independente: concluída e aprovada.

Não foi identificada pendência técnica que justifique manter a implementação aberta ou realizar novas modificações de código nesta fase.

Não foram realizados commit, tag, release ou push. Essas operações permanecem condicionadas à autorização explícita do responsável pelo projeto.

---

## 11. Registro de mudanças — atualização de 28/11/2026

- Atualizada a data do handoff para 28/11/2026.
- Atualizada a seção de limitações para remover pendências do Comparador XML que foram superadas pela avaliação independente.
- Atualizado o estado da seção 9 com a compilação Windows, o CTest 1/1, as comparações reais ArrayTest e o SHA-256 do executável.
- Acrescentada a seção 10 com os resultados conclusivos DEF-XML-01, DEF-XML-02, interface, validação da distribuição Windows independente e regressão VM W-12. A distribuição não possui identificador individual de teste; W-00 identifica a inicialização no ambiente independente e W-13 identifica a falha de permissão.
- Registradas explicitamente as diferenças entre a especificação original do Analista e o escopo final aprovado pelo responsável.
- Registradas as limitações de rastreabilidade do hash do executável e de generalização do teste de distribuição Windows.
- Registrada a manifestação favorável do Desenvolvedor ao encerramento.
- Nenhuma operação Git foi realizada.

