# Handoff — Desenvolvedor

**Projeto:** JackCompiler  
**Repositório informado:** `slovich/JackCompiler`  
**Atualização:** 24/11/2026
**Escopo:** desenvolvimento e validação dos componentes do JackCompiler, incluindo o `VMCodeGenerator`, a investigação das ocorrências em Square e Pong e a portabilidade da CLI Windows.

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

1. Confirmar no checkout local vigente a branch, commit, working tree e conteúdo real dos arquivos antes de qualquer nova alteração.
2. Reexecutar a suíte completa de testes no ambiente do usuário e registrar comandos, contagens e resultados.
3. Confirmar se todos os 11 arquivos VM e os seis programas continuam reproduzíveis no checkout atual.
4. Não há validação funcional da geração VM pela GUI Qt registrada nesta fase.
5. A causa do comportamento anômalo do VM Emulator desktop permanece desconhecida.
6. O redimensionamento de Square permanece sem comportamento funcional observado, mas foi reproduzido com o compilador oficial e não deve ser classificado como defeito confirmado do JackCompiler.
7. Não há evidência suficiente nesta fase para afirmar cobertura integral de todas as construções Jack ou ausência de defeitos no VMCodeGenerator.
8. A compatibilidade com outras versões do emulador, do Java ou do sistema operacional não foi investigada sistematicamente.
9. O handoff recebido registra o comparador de XML como pendência. Entretanto, seu estado no checkout atual não pôde ser confirmado nesta revisão, pois foi disponibilizado apenas o handoff, sem o checkout vigente ou evidência atualizada de implementação e testes. Antes de classificar essa atividade como pendente ou concluída, verificar a branch, o commit, a working tree, os arquivos e os testes correspondentes no projeto atual.

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
