# Relatório de testes — Encerramento da fase VMCodeGenerator

**Projeto:** JackCompiler  
**Data do relatório:** 22/11/2026  
**Responsável pelo registro:** Testador  
**Escopo:** integração da geração VM à CLI e validação funcional dos seis programas oficiais do Nand2Tetris Project 11.

## 1. Parecer executivo

A fase de implementação e validação funcional do VMCodeGenerator é considerada concluída no escopo efetivamente testado. Os seis programas oficiais foram compilados e produziram 11 arquivos VM. Quatro programas foram executados com sucesso no VM Emulator desktop, e os resultados posteriores relatados pelo desenvolvedor no emulador online oficial aprovaram Pong e a movimentação de Square.

O redimensionamento de Square não funcionou, mas o mesmo comportamento foi reproduzido com o compilador oficial de referência no mesmo emulador online. Portanto, não há evidência para classificar essa ocorrência como defeito do JackCompiler.

Não se afirma ausência geral de defeitos nem cobertura exaustiva da linguagem Jack.

## 2. Ambiente e identificação da versão

- Sistema: Windows 11.
- CMake: 3.30.5.
- Compilador: MinGW GCC 13.1.0.
- Gerador: MinGW Makefiles.
- Linguagem: C++17.
- Diretório informado: `E:\Projetos\Compiladores\compilador`.
- VM Emulator desktop: versão 2.5.
- Emulador online: emulador VM oficial do Nand2Tetris.

**Commit/hash e tag/release:** não informados nesta fase. Os resultados se referem ao checkout local compilado e executado pelo usuário; não devem ser vinculados a um commit específico sem confirmação.

## 3. Testes de integração

### 3.1 Build

Comandos:
```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-cli -DBUILD_GUI=OFF -G "MinGW Makefiles"
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_core
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_cli
```

Resultado fornecido pelo usuário: `jack_core` e `jack_cli` compilados com sucesso.

### 3.2 Compilação dos seis programas oficiais

Comando utilizado:
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

Resultado fornecido pelo usuário:
- 6/6 diretórios compilados com código 0;
- 11/11 arquivos VM gerados;
- Seven: 1; ConvertToBin: 1; Square: 3; Average: 1; Pong: 4; ComplexArrays: 1.

## 4. Validação funcional — resultados

| Programa/cenário | Resultado observado | Classificação |
|---|---|---|
| Seven | Saída `7` | PASS |
| Average | Entradas 3, 10, 20, 30; saída `20` | PASS |
| ComplexArrays | Saídas 5, 40, 0, 77 e 110 | PASS |
| ConvertToBin | Entrada `RAM[8000]=8001`; 16 bits conferidos em `RAM[8001..8016]` | PASS |
| Pong — VM Emulator desktop | Bola/raquete com comportamento inconsistente; teclado não controlou a raquete | Falha observada no ambiente desktop |
| Pong — emulador online oficial | Bola e raquete funcionaram | PASS, resultado relatado pelo desenvolvedor |
| Square — VM Emulator desktop | Quadrado desenhado; setas e X/Z sem efeito | Falha observada no ambiente desktop |
| Square — emulador online oficial | Movimentação nas quatro direções | PASS, resultado relatado pelo desenvolvedor |
| Square — redimensionamento | Não funcionou também com o compilador oficial no mesmo emulador online | Limitação reproduzida com referência oficial |

### 4.1 Seven

O usuário executou `Main.vm` no VM Emulator desktop. A saída observada foi `7`. A captura mostrou a função `Main.main`, a chamada `Math.multiply 2` e a chamada `Output.printInt 1`.

### 4.2 Average

Entradas: quantidade 3; valores 10, 20 e 30. Saída observada: `The average is 20`.

### 4.3 ComplexArrays

O programa apresentou cinco comparações:
- esperado 5, obtido 5;
- esperado 40, obtido 40;
- esperado 0, obtido 0;
- esperado 77, obtido 77;
- esperado 110, obtido 110.

### 4.4 ConvertToBin

O programa exige inicialização da entrada em `RAM[8000]`. Após definir esse endereço como 8001 e executar novamente, os bits foram escritos em `RAM[8001]` a `RAM[8016]`. A sequência binária é `0001111101000001`, armazenada por endereço do bit menos significativo ao mais significativo. O usuário confirmou que o último endereço continha 0.

### 4.5 Square e Pong

No VM Emulator desktop, os dois programas apresentaram falhas de interação. Posteriormente, o desenvolvedor informou que:
- Pong funcionou no emulador online oficial, incluindo bola e raquete;
- Square movimentou-se nas quatro direções no emulador online oficial;
- o redimensionamento de Square também falhou quando o código-fonte original foi compilado pelo compilador oficial e executado no mesmo emulador online.

Os resultados online são **relatados pelo desenvolvedor**, não reproduzidos diretamente pelo Testador. A comparação do redimensionamento com o compilador oficial impede atribuir essa limitação ao JackCompiler com as evidências disponíveis.

## 5. Observação sobre o VM Emulator desktop

O usuário relatou que o emulador visualmente reconhecia o pressionamento de teclas, mas os programas nem sempre detectavam as entradas. No KeyboardTest oficial, foi necessário manter uma tecla pressionada por aproximadamente dez segundos. Average, que utiliza entrada textual, funcionou normalmente.

O ambiente foi descrito como Java 8 Update 513, JVM Client de 32 bits. A causa não foi comprovada; não atribuir o comportamento ao Java, Windows ou JackCompiler.

## 6. Classificação e limites

- **Testes unitários históricos:** SymbolTable 8/8 e VMWriter 14/14, resultados fornecidos pelo usuário em fases anteriores.
- **Testes de integração:** build CMake e compilação dos seis diretórios, 11 arquivos VM.
- **Validação funcional:** execução dos programas oficiais nos emuladores, com a proveniência indicada em cada caso.

Não houve nesta fase comprovação de cobertura exaustiva da linguagem Jack, testes completos de todos os cenários semânticos, nem reprodução direta pelo Testador dos resultados online relatados pelo desenvolvedor. Commit/hash não foi fornecido.

## 7. Hipóteses e investigações

A hipótese de perda de contexto `THIS`/`pointer 0` foi levantada ao examinar Square e Pong. Os resultados posteriores no emulador online não sustentaram essa hipótese como defeito confirmado. Não solicitar correção especulativa nem reabrir a investigação sem nova reprodução isolada ou evidência.

## 8. Conclusão

A implementação do VMCodeGenerator pode avançar para as próximas etapas do projeto, considerando os seis programas oficiais compilados e os cenários funcionais descritos.

A aprovação é delimitada ao escopo testado. As falhas observadas no VM Emulator desktop e a limitação de redimensionamento de Square permanecem documentadas, sem atribuição indevida ao JackCompiler.

**Parecer:** etapa encerrada com aprovação funcional no escopo validado e ressalvas explicitadas.
