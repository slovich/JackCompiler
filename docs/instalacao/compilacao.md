# Compilação do JackCompiler

Este documento apresenta os requisitos e os procedimentos para
configurar e compilar o JackCompiler no Windows, contemplando tanto a
versão de linha de comando (CLI) quanto a interface gráfica (GUI).

## 1. Requisitos

O projeto utiliza C++17 e CMake para configuração e gerenciamento da
compilação.

### 1.1. Ferramentas e versões

| Ferramenta | Versão utilizada |
|---|---|
| C++ | C++17 |
| CMake | 3.30.5 |
| Qt | 6.12.0 |
| MinGW-w64 | 13.1.0 |
| Sistema operacional | Windows 11 |

O arquivo `CMakeLists.txt` estabelece o CMake 3.16 como versão mínima.

O projeto também prevê compatibilidade com Qt 5 ou Qt 6 para a interface
gráfica. Entretanto, o ambiente de desenvolvimento e validação
documentado utiliza Qt 6.12.0.

### 1.2. Estrutura do ambiente

No ambiente utilizado durante o desenvolvimento, as ferramentas estão
instaladas nos seguintes diretórios:

-   CMake: `C:\Qt\Tools\CMake_64\bin`
-   Qt: `C:\Qt\6.12.0\mingw_64`
-   MinGW: `C:\Qt\Tools\mingw1310_64\bin`

**Observação:** o CMake não está disponível globalmente pelo `PATH` do
Windows nesse ambiente. Por isso, os comandos apresentados utilizam o
caminho completo do executável.

## 2. Obtenção do código-fonte

Clone o repositório:

``` powershell
git clone https://github.com/slovich/JackCompiler.git
```

Entre no diretório do projeto:

``` powershell
cd JackCompiler
```

## 3. Compilação da versão CLI

A versão CLI não depende do Qt. Isso é possível porque o núcleo do
compilador (`jack_core`) é implementado em C++ independente da interface
gráfica. A geração de código VM integrada à CLI faz parte desse núcleo e
não exige uma dependência gráfica adicional.

Para configurar uma compilação somente da CLI, utilize `BUILD_GUI=OFF`.

### 3.1. Configuração

Execute:

``` powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-cli -DBUILD_GUI=OFF -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

Esse comando configura o projeto para utilizar o MinGW 13.1.0 e gera os
arquivos necessários à compilação no diretório `build-cli`.

### 3.2. Compilação

Execute:

``` powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_cli
```

O executável será gerado em:

``` text
build-cli/jack_cli.exe
```

### 3.3. Execução

Para executar o programa:

``` powershell
.\build-cli\jack_cli.exe
```

A CLI pode ser compilada e executada sem a instalação do Qt.

O Comparador XML utiliza a cópia de pugixml 1.16 mantida em
`third_party/pugixml/`, integrada pelo CMake. Não é necessário instalar
essa biblioteca separadamente para compilar o projeto.

### 3.4. Testes de regressão do Comparador XML

O projeto disponibiliza um conjunto opcional de testes de regressão do
Comparador XML. Para habilitá-lo, configure o CMake com
`BUILD_XML_COMPARATOR_REGRESSION_TESTS=ON`:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-cli -DBUILD_GUI=OFF -DBUILD_XML_COMPARATOR_REGRESSION_TESTS=ON -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

Compile a CLI e o alvo de regressão:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_cli comparadorxml_regression_tests
```

Execute o teste registrado no CTest:

```powershell
& "C:\Qt\Tools\CMake_64\bin\ctest.exe" --test-dir build-cli --output-on-failure
```

A opção está desativada por padrão. Quando habilitada, o projeto cria
`comparadorxml_regression_tests` e registra no CTest um teste que reúne
quatro casos relacionados à distinção entre `<!DOCTYPE` em comentário
ou CDATA e uma declaração DTD efetiva. O resultado histórico de 1/1
teste aprovado refere-se a esse conjunto específico, não à execução de
toda a suíte do projeto.

### 3.5. Distribuição da CLI no Windows com MinGW

Em builds Windows com compilador GNU/MinGW, o CMake aplica
`-static-libgcc` e `-static-libstdc++` somente ao alvo `jack_cli`.
Essas opções eliminam as dependências externas `libgcc_s_seh-1.dll` e
`libstdc++-6.dll` nos executáveis em que a inspeção da tabela de
importação confirmar essa condição.

A dependência `libwinpthread-1.dll` permanece externa e deve acompanhar
o executável na distribuição. Utilize a DLL correspondente à mesma
instalação/toolchain MinGW usada para compilar o programa. Não utilize
`-static` genérico como substituto dessas opções.

Para validar um pacote antes de distribuí-lo:

1. configure um diretório de build limpo ou independente com Windows,
   MinGW/GNU e `BUILD_GUI=OFF`;
2. compile o alvo `jack_cli`;
3. confirme em
   `CMakeFiles\jack_cli.dir\link.txt` a presença de
   `-static-libgcc` e `-static-libstdc++`;
4. inspecione as dependências do executável com `objdump -p` e confirme
   a ausência de `libgcc_s_seh-1.dll` e `libstdc++-6.dll`;
5. inclua `libwinpthread-1.dll` da mesma toolchain;
6. execute `jack_cli.exe --help` e uma compilação real;
7. teste o pacote em outro Windows sem MinGW instalado ou disponível
   no `PATH`.

Exemplo de inspeção:

```powershell
& "C:\Qt\Tools\mingw1310_64\bin\objdump.exe" -p `
    .\build-cli\jack_cli.exe |
    Select-String "DLL Name"
```

A validação registrada em 28/11/2026 executou o pacote com
`jack_cli.exe` e `libwinpthread-1.dll` em outro computador Windows 11,
sem instalação de Qt, CMake ou MinGW. O resultado foi positivo no
ambiente testado, mas não deve ser generalizado para todas as versões,
arquiteturas ou configurações do Windows.

## 4. Compilação da interface gráfica (GUI)

A configuração da GUI exige os componentes Qt Widgets e Qt Xml, conforme
as dependências declaradas no CMake. A interface utiliza Qt Widgets,
enquanto a leitura dos documentos XML examinada na arquitetura utiliza
recursos do Qt Core. Para compilar essa versão, é necessário possuir uma
instalação compatível do Qt e um compilador correspondente.

### 4.1. Utilizando o Qt Creator

A forma recomendada de configurar e compilar a interface gráfica é por
meio do Qt Creator.

No ambiente de desenvolvimento utilizado, a configuração é:

-   Kit: Qt 6.12.0 MinGW 64-bit;
-   Compilador: MinGW 13.1.0;
-   Configuração de compilação: Debug.

Após abrir o projeto no Qt Creator:

1.  Selecione o kit Qt 6.12.0 MinGW 64-bit.
2.  Aguarde a configuração do CMake.
3.  Selecione o alvo `compilador`.
4.  Execute a compilação.

O executável será gerado no diretório de compilação configurado pelo Qt
Creator.

### 4.2. Configuração pelo terminal

Também é possível configurar a compilação gráfica diretamente pelo
CMake, desde que o Qt e o compilador estejam corretamente configurados.

Exemplo de configuração:

``` powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-gui -DBUILD_GUI=ON -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

Para compilar:

``` powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-gui --target compilador
```

O parâmetro `CMAKE_PREFIX_PATH` informa ao CMake onde localizar a
instalação do Qt.

## 5. Compilação dos dois executáveis

Quando `BUILD_GUI` está habilitado, o projeto disponibiliza os alvos
`jack_cli` e `compilador`. Ambos utilizam o núcleo compartilhado
`jack_core`; a CLI permanece independente do Qt, enquanto o alvo
`compilador` corresponde à interface gráfica.

A configuração padrão do projeto utiliza `BUILD_GUI=ON`.

Para configurar e compilar ambos:

``` powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

``` powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build
```

## 6. Distribuição da interface gráfica

A compilação da GUI gera um executável que depende das bibliotecas do
Qt. Portanto, copiar apenas o arquivo `compilador.exe` para outro
computador geralmente não é suficiente.

Para preparar uma distribuição no Windows, utilize a ferramenta
`windeployqt`, disponibilizada pela instalação do Qt.

Exemplo:

``` powershell
& "C:\Qt\6.12.0\mingw_64\bin\windeployqt.exe" --dir "C:\caminho\para\distribuicao" "C:\caminho\para\compilador.exe"
```

A ferramenta identifica e copia as bibliotecas e os plugins necessários
do Qt para o diretório de distribuição.

Dependendo do ambiente de destino, também poderá ser necessário
disponibilizar as bibliotecas de execução do MinGW.

A distribuição deve ser testada em um computador que não possua o
ambiente de desenvolvimento instalado.

## 7. Organização dos diretórios de compilação

Recomenda-se utilizar diretórios separados para cada configuração de
compilação:

| Diretório | Finalidade |
|---|---|
| `build-cli` | Compilação independente da CLI |
| `build-gui` | Compilação da GUI |
| `build` | Compilação dos dois executáveis |

Cada um desses diretórios contém os arquivos gerados pelo CMake para sua
respectiva configuração. Os diretórios de compilação são gerados
automaticamente e não fazem parte do código-fonte do projeto.

## 8. Observações

-   A CLI não exige Qt, pois depende apenas do núcleo do compilador.
-   A configuração da GUI exige Qt Widgets e Qt Xml, conforme as
    dependências declaradas no CMake. A interface utiliza Qt Widgets,
    enquanto a leitura dos documentos XML examinada na arquitetura
    utiliza recursos do Qt Core.
-   O projeto utiliza C++17.
-   O CMake gerencia a configuração e a geração dos arquivos de
    compilação.
-   A compatibilidade com Qt 5 está prevista na configuração do projeto,
    mas não foi validada no ambiente documentado.
-   As versões e os caminhos apresentados descrevem o ambiente de
    desenvolvimento registrado; em outra instalação, ajuste os caminhos
    do CMake, Qt e MinGW conforme necessário.
-   A compilação em modo Debug é utilizada no ambiente de
    desenvolvimento. Para distribuição, recomenda-se avaliar uma
    compilação em modo Release.
