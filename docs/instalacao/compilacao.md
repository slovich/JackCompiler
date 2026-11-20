# Compilação do JackCompiler

Este documento apresenta os requisitos e os procedimentos para configurar e compilar o JackCompiler no Windows, contemplando tanto a versão de linha de comando (CLI) quanto a interface gráfica (GUI).

## 1. Requisitos

O projeto utiliza C++17 e CMake para configuração e gerenciamento da compilação.

### 1.1. Ferramentas e versões

| Ferramenta | Versão utilizada |
|---|---|
| C++ | C++17 |
| CMake | 3.30.5 |
| Qt | 6.12.0 |
| MinGW-w64 | 13.1.0 |
| Sistema operacional | Windows 11 |

O arquivo `CMakeLists.txt` estabelece o CMake 3.16 como versão mínima.

O projeto também prevê compatibilidade com Qt 5 ou Qt 6 para a interface gráfica. Entretanto, o ambiente de desenvolvimento e validação documentado utiliza Qt 6.12.0.

### 1.2. Estrutura do ambiente

No ambiente utilizado durante o desenvolvimento, as ferramentas estão instaladas nos seguintes diretórios:

- CMake: `C:\Qt\Tools\CMake_64\bin`
- Qt: `C:\Qt\6.12.0\mingw_64`
- MinGW: `C:\Qt\Tools\mingw1310_64\bin`

**Observação:** o CMake não está disponível globalmente pelo `PATH` do Windows nesse ambiente. Por isso, os comandos apresentados utilizam o caminho completo do executável.

## 2. Obtenção do código-fonte

Clone o repositório:

```powershell
git clone https://github.com/slovich/JackCompiler.git
```

Entre no diretório do projeto:

```powershell
cd JackCompiler
```

## 3. Compilação da versão CLI

A versão CLI não depende do Qt. Isso é possível porque o núcleo do compilador (`jack_core`) é implementado em C++ independente da interface gráfica.

Para configurar uma compilação somente da CLI, utilize `BUILD_GUI=OFF`.

### 3.1. Configuração

Execute:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-cli -DBUILD_GUI=OFF -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

Esse comando configura o projeto para utilizar o MinGW 13.1.0 e gera os arquivos necessários à compilação no diretório `build-cli`.

### 3.2. Compilação

Execute:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-cli --target jack_cli
```

O executável será gerado em:

```text
build-cli/jack_cli.exe
```

### 3.3. Execução

Para executar o programa:

```powershell
.\build-cli\jack_cli.exe
```

A CLI pode ser compilada e executada sem a instalação do Qt.

## 4. Compilação da interface gráfica (GUI)

A interface gráfica utiliza Qt Widgets e Qt Xml. Para compilar essa versão, é necessário possuir uma instalação compatível do Qt e um compilador correspondente.

### 4.1. Utilizando o Qt Creator

A forma recomendada de configurar e compilar a interface gráfica é por meio do Qt Creator.

No ambiente de desenvolvimento utilizado, a configuração é:

- Kit: Qt 6.12.0 MinGW 64-bit;
- Compilador: MinGW 13.1.0;
- Configuração de compilação: Debug.

Após abrir o projeto no Qt Creator:

1. Selecione o kit Qt 6.12.0 MinGW 64-bit.
2. Aguarde a configuração do CMake.
3. Selecione o alvo `compilador`.
4. Execute a compilação.

O executável será gerado no diretório de compilação configurado pelo Qt Creator.

### 4.2. Configuração pelo terminal

Também é possível configurar a compilação gráfica diretamente pelo CMake, desde que o Qt e o compilador estejam corretamente configurados.

Exemplo de configuração:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build-gui -DBUILD_GUI=ON -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

Para compilar:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build-gui --target compilador
```

O parâmetro `CMAKE_PREFIX_PATH` informa ao CMake onde localizar a instalação do Qt.

## 5. Compilação dos dois executáveis

Quando `BUILD_GUI` está habilitado, o projeto disponibiliza os alvos `jack_cli` e `compilador`.

A configuração padrão do projeto utiliza `BUILD_GUI=ON`.

Para configurar e compilar ambos:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/mingw_64" -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe"
```

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build
```

## 6. Distribuição da interface gráfica

A compilação da GUI gera um executável que depende das bibliotecas do Qt. Portanto, copiar apenas o arquivo `compilador.exe` para outro computador geralmente não é suficiente.

Para preparar uma distribuição no Windows, utilize a ferramenta `windeployqt`, disponibilizada pela instalação do Qt.

Exemplo:

```powershell
& "C:\Qt\6.12.0\mingw_64\bin\windeployqt.exe" --dir "C:\caminho\para\distribuicao" "C:\caminho\para\compilador.exe"
```

A ferramenta identifica e copia as bibliotecas e os plugins necessários do Qt para o diretório de distribuição.

Dependendo do ambiente de destino, também poderá ser necessário disponibilizar as bibliotecas de execução do MinGW.

A distribuição deve ser testada em um computador que não possua o ambiente de desenvolvimento instalado.

## 7. Organização dos diretórios de compilação

Recomenda-se utilizar diretórios separados para cada configuração de compilação:

| Diretório | Finalidade |
|---|---|
| `build-cli` | Compilação independente da CLI |
| `build-gui` | Compilação da GUI |
| `build` | Compilação dos dois executáveis |
| `build/` | Diretório de arquivos gerados pelo CMake |

Os diretórios de compilação são gerados automaticamente pelo CMake e não fazem parte do código-fonte do projeto.

## 8. Observações

- A CLI não exige Qt, pois depende apenas do núcleo do compilador.
- A GUI exige Qt Widgets e Qt Xml.
- O projeto utiliza C++17.
- O CMake gerencia a configuração e a geração dos arquivos de compilação.
- A compatibilidade com Qt 5 está prevista na configuração do projeto, mas não foi validada no ambiente documentado.
- A compilação em modo Debug é utilizada no ambiente de desenvolvimento. Para distribuição, recomenda-se avaliar uma compilação em modo Release.
