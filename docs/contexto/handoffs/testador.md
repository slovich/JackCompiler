# Handoff — Testador

**Projeto:** JackCompiler  
**Responsabilidade:** planejamento, execução e registro de testes, avaliação de regressões e transferência do estado de validação.  
**Última atualização:** 28/11/2026  
**Fase mais recente registrada:** avaliação funcional do Comparador XML da CLI, incluindo correções DEF-XML-01/02, readequação da interface, testes Windows W-00 a W-13 e regressão dos seis programas oficiais do Project 11.

---

## 1. Papel e limites

Planejar e executar testes do JackCompiler, registrar resultados reproduzíveis e avaliar regressões.

O Testador não deve:
- implementar ou alterar código de produção por iniciativa própria;
- modificar arquivos `.vm` gerados para contornar defeitos;
- atribuir ao JackCompiler comportamentos também reproduzidos com o compilador oficial;
- declarar aprovação além dos cenários efetivamente examinados;
- confundir testes unitários, integração, processamento em lote e validação funcional;
- reabrir investigações encerradas sem novas evidências.

Alterações de produção devem ser conduzidas pelo Desenvolvedor. Questões documentais devem ser encaminhadas ao Arquiteto de Documentação, sem transferir a ele a atualização rotineira deste handoff.

## 2. Primeiro passo ao iniciar nova fase

Antes de qualquer atividade dependente do estado atual do projeto:

1. Consultar o contexto e os registros de validação anteriores.
2. Confirmar branch, commit e estado do repositório de trabalho.
3. Examinar `docs/tecnico/testes.md`.
4. Examinar `tests/inputs/validos` e `tests/inputs/invalidos`.
5. Examinar a implementação vigente das opções 8 e 9 da CLI, se forem relevantes.
6. Confirmar os comandos, ferramentas, dependências e targets de build no commit sob teste.
7. Registrar diferenças entre o estado atual e o último handoff.

Não presumir que números de arquivos, funcionalidades, comandos ou resultados históricos permanecem atuais sem reconfirmação.

## 3. Estado inicial histórico — testes da CLI

O handoff original registrava, como estado conhecido a reconfirmar:
- 8 arquivos `.jack` em `tests/inputs/validos`;
- 9 arquivos `.jack` em `tests/inputs/invalidos`;
- opção 8 executando testes classificados e comparando observado com esperado;
- opção 9 processando arquivos `.jack` de um diretório sem classificação prévia;
- CMake sem configuração de CTest.

**Situação:** esses dados são históricos. Não foram reconfirmados no commit da fase VMCodeGenerator descrita abaixo. Não afirmar que continuam atuais.

Limites que permanecem:
- não confundir o teste funcional da opção 8 com o processamento em lote da opção 9;
- não declarar uma suíte CTest inexistente sem inspecionar o CMake vigente;
- registrar falhas reais e preservar as evidências.

## 4. Histórico de validação — componentes anteriores

### 4.1 SymbolTable — teste unitário isolado

**Escopo:** dois escopos (`class` e `subroutine`), categorias `static`, `field`, `argument`, `local`, indexação independente, reserva de `argument 0` em métodos, reinicialização, precedência, duplicidade e consulta de identificadores inexistentes.

**Comando informado:**
```powershell
& "C:\Qt\Tools\mingw1310_64\bin\g++.exe" -std=c++17 -Wall -Wextra -Wpedantic `
  -I ".\core\include" `
  ".\tests\symboltable_test.cpp" `
  # demais fontes dependentes conforme a versão do teste
```

O comando acima é apenas indicativo do compilador e das opções utilizados; não é uma linha de reprodução completa confirmada. Para reprodução, consultar o arquivo de teste e a composição de fontes vigente.

**Resultado fornecido pelo usuário:**
- 8 casos aprovados;
- 0 reprovados;
- resultado reportado: 8/8 PASS.

**Limite:** não cobre VMWriter, VMCodeGenerator, análise semântica completa nem execução de programas Project 11.

### 4.2 VMWriter — teste unitário isolado

Foi preparado `tests/vmwriter_tests.cpp` como teste isolado, compilado diretamente com a implementação de VMWriter.

**Comando efetivamente informado pelo usuário:**
```powershell
Set-Location "E:\Projetos\Compiladores\compilador"
& "C:\Qt\Tools\mingw1310_64\bin\g++.exe" `
    -std=c++17 -Wall -Wextra -Wpedantic `
    -I ".\core\include" `
    ".\tests\vmwriter_tests.cpp" `
    ".\core\src\vm\vmwriter.cpp" `
    -o ".\build-cli\vmwriter_tests.exe"
& ".\build-cli\vmwriter_tests.exe"
```

**Resultado fornecido pelo usuário:** 14 casos aprovados, 0 reprovados.

Cobertura exercitada: push/pop e formatação, nove comandos aritméticos/lógicos aceitos, rejeição de comando desconhecido, label/goto/if-goto, call/function/return, restrições de segmentos e índices, validação de nomes, quantidades negativas, finalização/idempotência, escrita após finalização e falhas de fluxo/flush.

**Limite:** a implementação observada aceita nove comandos aritméticos/lógicos. Não presumir um décimo comando.

### 4.3 Build integrado CMake — teste de integração

Ambiente informado:
- Windows 11;
- CMake 3.30.5 em `C:\Qt\Tools\CMake_64\bin\cmake.exe`;
- GCC 13.1.0 em `C:\Qt\Tools\mingw1310_64\bin\g++.exe`;
- MinGW Makefiles;
- C++17;
- diretório de trabalho `E:\Projetos\Compiladores\compilador`.

Comandos:
```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-cli -DBUILD_GUI=OFF -G "MinGW Makefiles"
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_core
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_cli
```

**Resultado fornecido pelo usuário:** `jack_core` e `jack_cli` compilados com sucesso.

**Limite:** sucesso de build não comprova correção funcional do compilador.

## 5. Fase VMCodeGenerator — integração e validação dos seis programas oficiais

### 5.1 Escopo

Validar a geração de arquivos VM pela CLI integrada e executar os programas oficiais do Nand2Tetris Project 11 no VM Emulator. A validação abrange os cenários exercitados pelos programas, não toda a linguagem Jack nem todas as combinações possíveis de construções.

### 5.2 Versão, commit e proveniência

- Repositório informado: `slovich/JackCompiler`.
- Branch principal conhecida: `main`.
- Checkout informado: `E:\Projetos\Compiladores\compilador`.
- **Commit/hash:** não fornecido nem confirmado durante esta fase.
- **Tag/release:** não fornecida nem confirmada.
- **Versão exata do código-fonte examinado:** não identificada por hash; os resultados referem-se à versão local que o usuário compilou e executou durante a validação.

Não atribuir os resultados a um commit específico sem confirmação posterior.

### 5.3 Ambiente e ferramentas

**Build e CLI:**
- Windows 11;
- CMake 3.30.5;
- MinGW GCC 13.1.0;
- MinGW Makefiles;
- C++17;
- CLI `build-cli\jack_cli.exe`.

**Emulação:**
- VM Emulator 2.5 desktop, instalado localmente no Windows 11;
- emulador VM online oficial do Nand2Tetris, utilizado posteriormente pelo desenvolvedor;
- o ambiente desktop foi relatado como executando Java 8 Update 513, JVM Client de 32 bits.

A informação sobre Java é descrição do ambiente, não diagnóstico causal.

### 5.4 Integração CLI/VM — teste de integração

A CLI recebeu integração para aceitar:
```text
jack_cli.exe --compile <arquivo.jack|diretorio> [--output <diretorio>]
jack_cli.exe compile <arquivo.jack|diretorio> [-o <diretorio>]
jack_cli.exe --help
```

Também foi informado que, sem argumentos, o menu interativo anterior permanece disponível.

**Comandos executados pelo usuário:**
```powershell
Set-Location "E:\Projetos\Compiladores\compilador"
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-cli -DBUILD_GUI=OFF -G "MinGW Makefiles"
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_cli
& ".\build-cli\jack_cli.exe" --help
& ".\build-cli\jack_cli.exe"
```

**Resultados fornecidos:**
- configuração CMake concluída;
- `jack_core` e `jack_cli` compilados;
- `--help` exibiu a sintaxe;
- sem argumentos, o menu interativo abriu e encerrou normalmente;
- foi observada corrupção visual de caracteres acentuados no terminal/help (problema de apresentação/encoding ainda não diagnosticado).

### 5.5 Compilação dos programas oficiais — teste de integração

Diretório dos fontes:
`nand2tetris\projects\11`

Programas e arquivos:
- Seven: `Main.jack`;
- ConvertToBin: `Main.jack`;
- Square: `Main.jack`, `Square.jack`, `SquareGame.jack`;
- Average: `Main.jack`;
- Pong: `Ball.jack`, `Bat.jack`, `Main.jack`, `PongGame.jack`;
- ComplexArrays: `Main.jack`.

Comando PowerShell utilizado:
```powershell
$programas = @("Seven","ConvertToBin","Square","Average","Pong","ComplexArrays")
$raiz = ".\nand2tetris\projects\11"
$saidaBase = ".\build-cli\vm-output"
New-Item -ItemType Directory -Force -Path $saidaBase | Out-Null
$resultados = foreach ($programa in $programas) {
  $entrada = Join-Path $raiz $programa
  $saida = Join-Path $saidaBase $programa
  & ".\build-cli\jack_cli.exe" --compile $entrada --output $saida
  $codigo = $LASTEXITCODE
  $arquivosVM = @(Get-ChildItem -Path $saida -Filter "*.vm" -File -ErrorAction SilentlyContinue)
  [PSCustomObject]@{
    Programa=$programa
    CodigoSaida=$codigo
    ArquivosVM=$arquivosVM.Count
    Resultado=if($codigo -eq 0){"SUCESSO"}else{"FALHA"}
    DiretorioSaida=$saida
  }
}
$resultados | Format-Table -AutoSize
```

**Resultado fornecido pelo usuário:**
- 6/6 diretórios compilados com código de saída 0;
- 11/11 arquivos VM gerados;
- Seven: 1 arquivo;
- ConvertToBin: 1;
- Square: 3;
- Average: 1;
- Pong: 4;
- ComplexArrays: 1.

### 5.6 Inspeção textual dos arquivos VM

O conteúdo dos 11 arquivos foi fornecido em `Texto colado(7).txt`, 2.472 linhas de transcript/inventário, contendo os arquivos gerados. A contagem consolidada dos arquivos VM foi de 2.396 linhas.

Foram examinados tamanho, contagem de linhas e conteúdo. A inspeção superficial não encontrou erro textual óbvio que, isoladamente, explicasse os problemas interativos. Essa inspeção não substitui execução nem comprova correção da pilha, dos segmentos ou da semântica.

### 5.7 Validação funcional — VM Emulator desktop

Os quatro programas seguintes foram executados pelo usuário no VM Emulator 2.5 desktop e tiveram os resultados observados:

**Seven**
- Saída observada: `7`.
- A chamada `Math.multiply` e a impressão por `Output.printInt` foram executadas.
- Resultado: PASS no cenário testado.

**Average**
- Entradas: quantidade `3`; números `10`, `20`, `30`.
- Saída observada: `The average is 20`.
- Resultado: PASS no cenário testado.

**ComplexArrays**
- Saídas observadas:
  - Teste 1: esperado 5, obtido 5;
  - Teste 2: esperado 40, obtido 40;
  - Teste 3: esperado 0, obtido 0;
  - Teste 4: esperado 77, obtido 77;
  - Teste 5: esperado 110, obtido 110.
- Resultado: PASS nos cinco casos apresentados pelo programa.

**ConvertToBin**
- Inicialmente, a execução sem inicializar a entrada em `RAM[8000]` não produziu a saída esperada. Isso foi tratado como preparação incompleta do teste, não como falha do compilador.
- Após definir `RAM[8000] = 8001`, o programa escreveu os 16 bits nas posições `RAM[8001]` a `RAM[8016]`.
- Representação binária de 8001: `0001111101000001`.
- Os 16 valores foram conferidos; os endereços 8001 a 8015 apareceram na captura e o usuário confirmou que o último endereço 8016 continha 0.
- Resultado: PASS após inicialização correta da entrada.

**Square — desktop**
- O quadrado foi desenhado.
- O emulador indicava visualmente o pressionamento de teclas.
- Setas não movimentaram o quadrado; X/Z não alteraram o tamanho.
- Resultado histórico: falha funcional observada no desktop. Posteriormente, a movimentação foi aprovada no emulador online oficial; portanto, não usar a observação desktop isoladamente para atribuir defeito ao compilador.

**Pong — desktop**
- A tela e a raquete foram desenhadas.
- A bola apareceu, desapareceu e voltou a aparecer em outra posição, mas não apresentou movimento contínuo observável.
- A raquete não respondeu aos comandos, embora o emulador indicasse as teclas pressionadas.
- Resultado histórico: falha funcional observada no desktop. Posteriormente, Pong foi aprovado no emulador online oficial; não atribuir a ocorrência ao compilador sem evidência adicional.

### 5.8 Validação funcional adicional — emulador online oficial

**Origem da evidência:** resultados relatados pelo desenvolvedor na atualização de encerramento da implementação. Não foram reproduzidos diretamente pelo Testador nesta conversa.

Resultados relatados:
- Pong: aprovado no emulador online oficial, incluindo movimento da bola e interação com a raquete.
- Square: movimentação aprovada nas quatro direções.
- Square: redimensionamento não aprovado.
- O código-fonte Jack original de Square foi compilado pelo compilador oficial e executado no mesmo emulador online; o redimensionamento também não funcionou.
- Assim, a limitação de redimensionamento foi reproduzida com a ferramenta oficial de referência e não deve ser classificada como defeito confirmado do JackCompiler.

### 5.9 Parecer consolidado da fase

| Programa/cenário | Resultado consolidado | Proveniência |
|---|---|---|
| Seven | PASS | Execução desktop relatada pelo usuário, com captura |
| Average | PASS | Execução desktop relatada pelo usuário, com captura |
| ComplexArrays | PASS | Execução desktop relatada pelo usuário, com captura |
| ConvertToBin | PASS | Execução desktop relatada pelo usuário, com captura e confirmação do último bit |
| Pong | PASS | Resultado no emulador online relatado pelo desenvolvedor |
| Square — movimentação | PASS | Resultado no emulador online relatado pelo desenvolvedor |
| Square — redimensionamento | Limitação reproduzida com compilador oficial | Comparação relatada pelo desenvolvedor |

A fase foi considerada concluída com aprovação funcional dentro do escopo descrito, sem garantia de ausência de defeitos.

### 5.10 VM Emulator desktop — observação ambiental

Foi relatado que:
- o emulador reconhecia visualmente o pressionamento das teclas;
- a aplicação nem sempre detectava a entrada por leitura direta do teclado;
- no KeyboardTest oficial, foi necessário manter a tecla pressionada por aproximadamente dez segundos para que o evento fosse registrado;
- Average, que utiliza entrada textual, funcionou normalmente;
- o ambiente informado usava Java 8 Update 513 e JVM Client 32 bits.

**Causa não comprovada.** Não atribuir o comportamento ao Java, ao Windows ou ao JackCompiler sem investigação independente. Não reabrir essa investigação sem novas evidências ou necessidade para uma fase futura.

## 6. Distinção entre tipos de teste

Para relatórios futuros, classificar explicitamente:

### Testes unitários
Testam componentes isolados, como SymbolTable e VMWriter, por meio de programas de teste próprios. Os resultados históricos registrados são 8/8 e 14/14, respectivamente.

### Testes de integração
Verificam a integração entre componentes e a aplicação, como build CMake, ligação do core à CLI, compilação em lote e geração dos 11 arquivos VM.

### Validação funcional
Executa os arquivos VM no emulador e confere comportamento e resultados observáveis. Os testes dos seis programas oficiais pertencem a esta categoria.

Não chamar a compilação em lote de teste unitário. Não chamar a execução de um programa oficial de teste unitário. Não afirmar que os testes isolados estão integrados ao CTest sem confirmação do CMake vigente.

## 7. Limitações e lacunas da cobertura da fase VMCodeGenerator

- Não foi fornecido commit/hash do código examinado.
- Não foi fornecida tag ou release associada aos binários.
- A execução online de Pong e Square foi relatada pelo desenvolvedor, não reproduzida diretamente pelo Testador.
- O teste de redimensionamento de Square foi comparado com o compilador oficial, mas os artefatos completos dessa comparação não foram anexados a este handoff.
- A inspeção textual dos arquivos VM não constitui prova formal de correção.
- Não foi demonstrada cobertura exaustiva de toda a linguagem Jack.
- Não foram apresentados nesta fase resultados completos de testes negativos semânticos, testes de falha de escrita/atomicidade Windows ou integração do VMCodeGenerator com uma suíte CTest.
- Não foi confirmada a execução de todos os programas oficiais no desktop e online em condições idênticas.
- O comportamento anormal de teclado no VM Emulator desktop não foi diagnosticado.

Essas lacunas delimitam a aprovação; não são automaticamente defeitos conhecidos.

## 8. Investigações encerradas e política de reabertura

### 8.1 Hipótese de perda de `THIS`/`pointer 0`

Foi levantada durante a análise dos arquivos `SquareGame.vm` e `PongGame.vm`, pois chamadas a métodos podem alterar `pointer 0`.

Os resultados posteriores no emulador online oficial — Pong funcional e movimentação de Square funcional — não sustentaram a hipótese como defeito confirmado do compilador.

**Estado:** não confirmada; investigação encerrada para esta fase. Reabrir somente diante de nova reprodução isolada ou evidência técnica.

### 8.2 Redimensionamento de Square

O comportamento não funcionou com o compilador do projeto nem com o compilador oficial, no mesmo emulador online, conforme relato do desenvolvedor.

**Estado:** limitação reproduzida com a referência oficial; não atribuir ao JackCompiler. Não solicitar alteração especulativa.

### 8.3 Teclado no VM Emulator desktop

O comportamento foi inconsistente, mas não houve diagnóstico causal.

**Estado:** observação ambiental documentada; sem atribuição de causa e sem ação corretiva contra o compilador.

## 9. Próximas atividades do Testador

Ao iniciar uma nova fase:

1. Obter o commit/hash e a branch exatos.
2. Reconfirmar os targets e comandos de build.
3. Reexaminar `docs/tecnico/testes.md`.
4. Reconfirmar os diretórios de testes válidos e inválidos.
5. Verificar a implementação atual das opções 8 e 9, se forem objeto da fase.
6. Planejar testes conforme o objetivo da fase, sem repetir automaticamente toda a bateria anterior.
7. Preservar os resultados históricos e registrar regressões somente quando reproduzidas.
8. Ao concluir, atualizar este handoff e produzir relatório de testes com evidências e proveniência.

## 10. Entregas obrigatórias ao concluir cada fase

- plano e escopo de testes;
- branch, commit/hash e versão examinados;
- ambiente e ferramentas;
- comandos completos de reprodução;
- casos e resultados individuais;
- consolidação por tipo de teste;
- evidências de falhas e regressões;
- limitações e lacunas de cobertura;
- testes não realizados e justificativas;
- recomendações e pendências priorizadas;
- origem das evidências, distinguindo testes reproduzidos pelo Testador, resultados executados pelo usuário e resultados relatados por terceiros;
- handoff atualizado;
- relatório de testes e referências às evidências correspondentes.

## 11. Regra de encerramento

O parecer deve refletir exatamente o escopo testado. Uma etapa pode ser encerrada com limitações documentadas, desde que não sejam ocultadas nem atribuídas indevidamente ao JackCompiler.

**Estado da última fase:** VMCodeGenerator — implementação declarada concluída pelo Desenvolvedor e validação funcional encerrada pelo Testador, dentro do escopo dos seis programas oficiais e dos cenários descritos, com ressalvas e lacunas explicitadas.

---

## 12. Fase Comparador XML — avaliação funcional e regressão (28/11/2026)

### 12.1 Escopo da fase

Avaliar a entrega do Comparador XML da CLI após:

- correção do defeito DEF-XML-01 (nomes distintos para exportação léxica e sintática);
- correção do defeito DEF-XML-02 (identificação de DTD efetivo, sem rejeitar ocorrências em comentário ou CDATA);
- readequação da interface para solicitar somente os caminhos do XML de referência e do XML gerado;
- validação do executável em Windows 11 sem Qt, CMake ou MinGW instalados;
- regressão da geração VM com os seis programas oficiais do Nand2Tetris Project 11;
- teste complementar de falha de permissão de leitura (W-13).

A interface avaliada:

- opção 10: Comparar XML léxico;
- opção 11: Comparar XML sintático;
- ambas solicitam apenas o caminho do XML de referência e o caminho do XML gerado;
- arquivos podem estar em diretórios distintos.

### 12.2 Pacote e proveniência

- Arquivo recebido: `jack_cli(1).zip`.
- SHA-256 do ZIP registrado durante a avaliação: `13eb2172364e208cca062400e4ac6216ae7958e93f4d9f6149fd59944ae790e3`.
- Conteúdo informado/inspecionado na entrega: `jack_cli.exe` e `libwinpthread-1.dll`.
- O pacote foi descompactado em `C:\Teste JackCompiler\Aplicacao`.
- Ambiente de teste: Windows 11, Windows PowerShell 5.1, sem instalação de Qt, CMake ou MinGW.
- O executável iniciou e executou os cenários relatados neste handoff.

**Commit/hash do código-fonte:** não fornecido nem confirmado para esta entrega. O hash do ZIP identifica o pacote distribuído, não o commit Git nem prova, por si só, a correspondência entre fontes e binário.

### 12.3 Ambiente e organização dos testes Windows

Raiz da avaliação: `C:\Teste JackCompiler`

Diretórios:

- `Aplicacao`
- `Referencias`
- `Gerados`
- `Fixtures`
- `Resultados`

Referências oficiais utilizadas:

- `C:\Teste JackCompiler\Referencias\ArrayTest`
- `C:\Teste JackCompiler\Referencias\ExpressionLessSquare`
- `C:\Teste JackCompiler\Referencias\Square`

Fontes oficiais do Project 11: `C:\nand2tetris\projects\11`

### 12.4 Comparador XML — resultados W-00 a W-11

Os resultados abaixo foram executados pelo responsável no computador Windows de teste e apresentados ao Testador por transcript/saída do PowerShell. Não os descrever como execução direta do Testador.

| Teste | Cenário | Resultado |
|---|---|---|
| W-00 | Inicialização do executável no ambiente Windows sem ferramentas de desenvolvimento | PASS — menu principal abriu |
| W-01 | Comparação léxica de ArrayTest (`MainT.xml` × `Main_lexico.xml`) | PASS — documentos equivalentes |
| W-02 | Comparação sintática de ArrayTest (`Main.xml` × `Main_sintatico.xml`) | PASS — documentos equivalentes |
| W-03 | Caminhos com espaços em `C:\Teste JackCompiler` | PASS |
| W-04 | Arquivo de referência inexistente | PASS — erro específico |
| W-05 | Arquivo gerado inexistente | PASS — erro específico |
| W-06 | Diretório informado no lugar do arquivo de referência | PASS — rejeitado como não sendo arquivo regular |
| W-07 | XML malformado | PASS — diagnóstico de tags incompatíveis, linha 1, coluna 28 |
| W-08 | Raiz XML incompatível | PASS — esperada `<class>`, referência e gerado com `<tokens>` |
| W-09 | Retorno ao menu após comparação/erro | PASS — relatado pelo responsável |
| W-10 | Repetição das exportações léxica e sintática | PASS — exportações repetidas com sucesso |
| W-11 | Comentário, CDATA e DTD efetivo | PASS — três cenários com resultados esperados |

Detalhes adicionais:

- W-01 e W-02 utilizaram caminhos em `Referencias\ArrayTest` e `Gerados\ArrayTest`.
- W-04 e W-05 verificaram separadamente arquivo de referência e arquivo gerado inexistentes.
- W-06 foi demonstrado com diretório no campo de referência. O mesmo cenário no campo do arquivo gerado não foi apresentado.
- W-07 foi demonstrado com XML malformado no campo do arquivo gerado.
- W-10 confirmou exportação com destino explicitamente indicado e repetição aceitando o destino sugerido. Os arquivos exportados ao aceitar o destino sugerido foram gravados no diretório de referências; registrar isso como detalhe do procedimento, não como falha funcional demonstrada.
- W-09 foi relatado como funcional pelo responsável; a saída apresentada mostrou o retorno ao menu após as operações.

### 12.5 W-11 — distinção entre comentário, CDATA e DTD

Foram criados cinco fixtures:

- `comentario.xml`: contém `<!DOCTYPE fake>` dentro de comentário;
- `controle_comentario.xml`: controle equivalente sem comentário;
- `cdata.xml`: contém `<!DOCTYPE fake>` dentro de CDATA;
- `controle_cdata.xml`: contém o mesmo conteúdo textual por entidades XML;
- `dtd.xml`: contém declaração DTD efetiva.

Resultados Windows:

- `comentario.xml` × `controle_comentario.xml`: equivalentes;
- `cdata.xml` × `controle_cdata.xml`: equivalentes;
- `dtd.xml` × `controle_comentario.xml`: DTD rejeitado, com diagnóstico "DTD e declarações de entidades não são suportados" e localização linha 1, coluna 1.

Resultado: 3/3 cenários principais PASS no executável Windows. Somam-se aos quatro cenários independentes já executados anteriormente na validação da correção DEF-XML-02.

### 12.6 W-12 — regressão da geração VM no Windows

#### Fontes e compilação

Raiz: `C:\nand2tetris\projects\11`

Foram encontrados 11 fontes Jack distribuídos por seis diretórios:

| Projeto | Fontes Jack | Arquivos VM esperados | Arquivos VM gerados | Código de saída |
|---|---:|---:|---:|---:|
| Seven | 1 | 1 | 1 | 0 |
| ConvertToBin | 1 | 1 | 1 | 0 |
| Square | 3 | 3 | 3 | 0 |
| Average | 1 | 1 | 1 | 0 |
| Pong | 4 | 4 | 4 | 0 |
| ComplexArrays | 1 | 1 | 1 | 0 |
| **Total** | **11** | **11** | **11** | **0 em todos** |

Executável: `C:\Teste JackCompiler\Aplicacao\jack_cli.exe`

Saídas: `C:\Teste JackCompiler\Gerados\W12\<Projeto>`

Comando utilizado para cada projeto:

```powershell
& $Exe --compile $Entrada --output $Saida
```

Relatório gerado pelo script: `C:\Teste JackCompiler\Resultados\W12_compilacao.txt`

Os seis projetos foram compilados com código de saída zero e quantidade esperada de arquivos VM. O transcript da execução foi fornecido pelo responsável.

#### Execução no VM Emulator desktop

Os programas foram carregados a partir dos diretórios `Gerados\W12`.

**Seven**

- Saída observada: `7`.
- Resultado: PASS.

**ConvertToBin**

- `RAM[8000]` foi inicializada com `8001`.
- O programa escreveu 16 bits em `RAM[8001]` a `RAM[8016]`.
- Sequência observada: `1 0 0 0 0 0 1 0 1 1 1 1 1 0 0 0`.
- A sequência corresponde à representação binária de 8001 em ordem de bit menos significativo para mais significativo.
- Resultado: PASS.

**Average**

- Entradas utilizadas: 10, 20 e 30.
- Saída observada: `Average is 20`.
- Resultado: PASS.

**ComplexArrays**

A captura apresentada pelo responsável mostrou os cinco resultados:

- teste 1: 5;
- teste 2: 40;
- teste 3: 0;
- teste 4: 77;
- teste 5: 110.

Todos corresponderam aos resultados esperados.

- Resultado: PASS (5/5 verificações internas).

**Square**

- O programa foi executado.
- O quadrado respondeu aos comandos direcionais.
- Os comandos de redimensionamento não funcionaram.
- Resultado: movimentação PASS; redimensionamento não aprovado.
- Preservar a ressalva histórica: o redimensionamento também havia sido observado como não funcional com o compilador oficial no emulador online, conforme comparação relatada anteriormente pelo Desenvolvedor. Não classificar como defeito confirmado do JackCompiler.

**Pong**

- O responsável informou que todos os testes funcionaram no VM Emulator desktop.
- Resultado: PASS, conforme relato do responsável.

#### Parecer W-12

- 6/6 diretórios compilados;
- 11/11 arquivos VM gerados;
- resultados funcionais aprovados nos cenários avaliados de Seven, ConvertToBin, Average, ComplexArrays e Pong;
- em Square, movimentação aprovada e redimensionamento não aprovado;
- a limitação de redimensionamento também foi reproduzida com o compilador oficial no emulador online, conforme relato anterior do Desenvolvedor;
- a evidência atual de Pong é execução desktop relatada pelo responsável, distinta do resultado histórico relatado pelo Desenvolvedor no emulador online.

Não foram identificadas regressões funcionais na geração VM associadas à readequação do Comparador XML, dentro do escopo avaliado.

### 12.7 W-13 — falha de permissão de leitura

O responsável criou:

- `C:\Teste JackCompiler\Fixtures\W13_Permissao\controle.xml`;
- `C:\Teste JackCompiler\Fixtures\W13_Permissao\sem_permissao.xml`.

Foi aplicada uma negação explícita de leitura ao usuário corrente usando `icacls`:

```powershell
$Arquivo = "C:\Teste JackCompiler\Fixtures\W13_Permissao\sem_permissao.xml"
$Usuario = [System.Security.Principal.WindowsIdentity]::GetCurrent().Name
icacls $Arquivo /deny "${Usuario}:(R)"
```

Durante a restrição, a opção 10 foi executada com `controle.xml` como referência e `sem_permissao.xml` como gerado.

Mensagem observada:

```text
[ERRO] Não foi possível abrir o arquivo gerado:
C:\Teste JackCompiler\Fixtures\W13_Permissao\sem_permissao.xml.
Verifique o caminho e as permissões de leitura.
```

A aplicação permaneceu funcional, retornou ao menu e encerrou normalmente. A permissão foi removida com:

```powershell
icacls $Arquivo /remove:d $Usuario
```

A leitura posterior com `Get-Content` confirmou a restauração.

Resultado: PASS. A falha de acesso foi identificada sem ser confundida com arquivo inexistente ou XML malformado.

### 12.8 Parecer consolidado da fase Comparador XML

| Área | Resultado |
|---|---|
| DEF-XML-01 — nomes distintos de exportação | PASS |
| DEF-XML-02 — identificação de DTD efetivo | PASS |
| Interface por dois caminhos explícitos | PASS |
| Tratamento de caminhos inválidos/inexistentes | PASS nos cenários executados |
| XML malformado e localização do erro | PASS |
| Raiz XML incompatível | PASS |
| Retorno ao menu | PASS |
| Execução em Windows sem ferramentas de desenvolvimento | Validada no ambiente testado |
| Regressão Project 11 — compilação | PASS, 6/6 projetos e 11/11 arquivos VM |
| Regressão Project 11 — execução VM | Resultados funcionais aprovados nos cenários avaliados de Seven, ConvertToBin, Average, ComplexArrays e Pong; em Square, movimentação aprovada e redimensionamento não aprovado |
| Falha de permissão W-13 | PASS |

**Parecer do Testador:** avaliação funcional encerrada com parecer favorável, sem defeitos novos identificados nos cenários executados. A aprovação é limitada à versão/pacote e ao escopo efetivamente testados; não constitui garantia de ausência de defeitos.

### 12.9 Limitações, lacunas e proveniência

- Não foi fornecido commit/hash Git correspondente ao executável `jack_cli.exe` distribuído em `jack_cli(1).zip`.
- O SHA-256 registrado identifica o ZIP recebido, não o código-fonte nem uma tag Git.
- Os testes W-00 a W-13 foram executados pelo responsável no computador Windows e reportados ao Testador por transcrições, saídas e captura. Não declarar que o Testador executou diretamente a aplicação Windows.
- A compilação Linux independente e os testes anteriores de núcleo/regressão da entrega foram registrados em mensagens anteriores; conservar a proveniência própria desses testes.
- W-06 foi demonstrado com diretório no campo de referência; não foi apresentado o mesmo caso no campo do arquivo gerado.
- O comportamento de redimensionamento de Square permanece uma ressalva conhecida, reproduzida anteriormente com a referência oficial conforme relato do Desenvolvedor.
- W-13 testou a permissão de leitura do arquivo gerado. Não foi demonstrada a mesma condição para o arquivo de referência.
- Não foi apresentada cobertura exaustiva da linguagem Jack, de todos os tipos de erro de sistema de arquivos ou de todas as versões/configurações do Windows.
- A avaliação não identifica um commit específico. Antes de associar o parecer a uma tag/release, obter do Desenvolvedor o hash e a confirmação de correspondência entre fontes e binário.

### 12.10 Encerramento e próximas responsabilidades

A fase de avaliação funcional do Comparador XML foi encerrada pelo Testador em 28/11/2026, com parecer favorável e ressalvas documentadas.

O Testador solicitou ao Desenvolvedor manifestação técnica sobre:

1. ciência dos resultados;
2. confirmação de correspondência entre implementação entregue e versão testada;
3. divergências conhecidas entre especificação e comportamento;
4. pendências técnicas ainda existentes;
5. manifestação sobre encerramento da implementação.

A consolidação documental do projeto e a integração dos resultados ao histórico geral são responsabilidade do Gerenciador Documental. Este handoff é atualizado pelo Testador para preservar seu próprio estado de validação.
