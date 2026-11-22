# Manual do Usuário --- Interface de Linha de Comando (CLI)

## 1. Apresentação

A interface de linha de comando (CLI) do JackCompiler permite utilizar
as funcionalidades do compilador diretamente pelo terminal.

A CLI oferece dois modos de utilização:

-   **Modo interativo:** apresenta um menu para seleção e visualização
    de arquivos Jack, análise léxica, análise sintática, exportação de
    XML e execução de testes individuais ou em lote.
-   **Modo não interativo:** recebe argumentos na inicialização para
    compilar um arquivo Jack ou os arquivos Jack diretamente contidos em
    um diretório, gerando código para a máquina virtual Jack (VM).

As opções do menu interativo são selecionadas durante a execução. A
geração VM é acionada pelo modo não interativo, descrito na seção 3.10.

## 2. Inicialização

Após compilar o projeto, execute o arquivo `jack_cli.exe` no terminal.

Exemplo no PowerShell:

``` powershell
.\build-cli\jack_cli.exe
```

Quando executado sem argumentos, o programa apresenta o menu interativo.
Quando iniciado com argumentos de compilação ou de ajuda, utiliza o modo
não interativo, sem apresentar o menu principal.

No modo interativo, a aplicação apresenta o menu principal:

``` text
========================================
  Jack Compiler - Menu Principal
========================================
1 - Selecionar arquivo Jack
2 - Exibir arquivo selecionado
3 - Análise léxica
4 - Análise sintática
5 - Análise completa
6 - Exportar XML léxico
7 - Exportar XML sintático
8 - Executar testes
9 - Testar arquivos Jack em lote
0 - Sair
----------------------------------------
Escolha uma opção:
```

Informe o número correspondente à operação desejada e pressione ENTER.

Entradas inválidas são rejeitadas, e o menu é apresentado novamente após
a mensagem de erro.

## 3. Funcionalidades do menu principal

### 3.1. Selecionar arquivo Jack

A opção 1 permite carregar um arquivo para análise.

O programa solicita o caminho do arquivo:

``` text
Informe o caminho do arquivo:
```

Informe o caminho desejado. É possível utilizar caminhos absolutos ou
relativos ao diretório de execução.

Exemplo:

``` text
E:\Projetos\Jack\Main.jack
```

Após a leitura bem-sucedida, o programa confirma o carregamento e
apresenta o caminho do arquivo selecionado.

**Observação:** a aplicação tenta abrir o caminho informado, sem
restringir a seleção à extensão `.jack`. Recomenda-se utilizar arquivos
de código-fonte Jack.

### 3.2. Exibir arquivo selecionado

A opção 2 apresenta no terminal o conteúdo do arquivo carregado.

É necessário selecionar um arquivo previamente. Caso nenhum arquivo
tenha sido carregado, a aplicação apresenta um aviso.

### 3.3. Análise léxica

A opção 3 executa o analisador léxico sobre o conteúdo do arquivo
selecionado.

Quando a análise é concluída com sucesso, são apresentados:

-   Resultado da análise;
-   Quantidade de tokens identificados;
-   Linha e coluna de cada token;
-   Lexema correspondente a cada token.

Em caso de falha, a aplicação apresenta a mensagem de erro retornada
pelo analisador léxico.

### 3.4. Análise sintática

A opção 4 executa o analisador sintático sobre os tokens produzidos pela
análise léxica.

Para utilizar essa funcionalidade, é necessário:

1.  Selecionar um arquivo;
2.  Executar uma análise léxica com sucesso;
3.  Selecionar a opção de análise sintática.

Caso a análise léxica ainda não tenha sido executada com sucesso, a
aplicação solicita que essa etapa seja realizada primeiro.

Em caso de falha sintática, são apresentadas a mensagem do erro e sua
localização, com linha e coluna.

### 3.5. Análise completa

A opção 5 executa automaticamente as duas etapas de análise:

1.  Análise léxica;
2.  Análise sintática.

A análise sintática somente é executada quando a análise léxica é
concluída com sucesso.

Essa opção é indicada quando se deseja verificar a validade léxica e
sintática de um arquivo em uma única operação.

### 3.6. Exportar XML léxico

A opção 6 exporta a representação XML dos tokens identificados pelo
analisador léxico.

É necessário que exista uma análise léxica válida.

O programa apresenta um destino sugerido, construído a partir do
diretório e do nome do arquivo original, substituindo sua extensão por
`.xml`.

Exemplo:

``` text
Arquivo original: E:\Projetos\Jack\Main.jack
Destino sugerido: E:\Projetos\Jack\Main.xml
```

O usuário pode:

-   Pressionar ENTER para utilizar o destino sugerido;
-   Informar outro caminho para salvar o arquivo.

Se a extensão `.xml` não estiver presente, ela será acrescentada
automaticamente.

### 3.7. Exportar XML sintático

A opção 7 exporta a representação XML da estrutura sintática reconhecida
pelo parser, construída a partir da AST.

É necessário que exista uma análise sintática válida.

O procedimento de escolha do destino é equivalente ao da exportação XML
léxica: o programa sugere um caminho baseado no arquivo original e
permite informar outro destino.

### 3.8. Executar testes

A opção 8 executa os testes automatizados de arquivos Jack organizados
em duas categorias:

-   `validos`: arquivos que devem ser aceitos;
-   `invalidos`: arquivos que devem apresentar falha léxica ou
    sintática.

Por padrão, a aplicação utiliza:

``` text
tests/inputs/
├── validos/
└── invalidos/
```

O caminho completo do diretório padrão é apresentado na tela.

O usuário pode pressionar ENTER para utilizar esse diretório ou informar
outro caminho. Caminhos relativos informados são interpretados em
relação à raiz do projeto.

O diretório informado precisa conter os dois subdiretórios, e ambos
devem possuir arquivos com extensão `.jack`.

Para cada arquivo, o programa executa a análise léxica e, quando
possível, a análise sintática. O resultado é comparado com o resultado
esperado para a categoria em que o arquivo está organizado.

Ao final, são apresentados:

-   Total de testes;
-   Testes aprovados;
-   Testes reprovados;
-   Erros de execução.

Um teste é considerado aprovado quando o resultado observado corresponde
ao esperado para sua categoria.

### 3.9. Testar arquivos Jack em lote

A opção 9 permite analisar todos os arquivos `.jack` encontrados
diretamente em um diretório informado pelo usuário.

O programa solicita:

``` text
Informe o caminho absoluto do diretório:
```

Informe um caminho absoluto válido.

O processamento não percorre subdiretórios. Somente arquivos regulares
com extensão `.jack` presentes diretamente no diretório informado são
considerados.

Os arquivos são ordenados pelo nome antes da execução.

Para cada arquivo, o programa apresenta:

-   Resultado da leitura;
-   Resultado da análise léxica;
-   Quantidade de tokens, quando disponível;
-   Resultado da análise sintática, quando executada;
-   Mensagem e localização do primeiro erro identificado, quando houver.

Se ocorrer um erro léxico, a análise sintática daquele arquivo não será
executada. O mesmo vale para erros de leitura.

Ao final, é apresentado um resumo contendo:

-   Quantidade de arquivos encontrados;
-   Quantidade de arquivos processados;
-   Quantidade de análises concluídas com sucesso;
-   Quantidade de erros léxicos;
-   Quantidade de erros sintáticos;
-   Quantidade de erros de leitura;
-   Total de tokens identificados;
-   Total de ocorrências de erros léxicos e sintáticos.

**Importante:** o processamento em lote apresenta somente o primeiro
erro léxico ou sintático identificado em cada arquivo. Outros erros
eventualmente existentes podem não ser apresentados.

### 3.10. Compilar arquivos Jack em código VM

Além das operações disponíveis no menu interativo, a CLI oferece um modo
não interativo para gerar arquivos de código VM.

A sintaxe aceita é:

``` text
jack_cli --compile|compile <arquivo.jack|diretorio> [--output|-o <diretorio>]
jack_cli --help|-h
```

As formas `--compile` e `compile` são alternativas. Da mesma forma,
`--output` e `-o` são opções equivalentes e podem ser utilizadas com
qualquer uma das formas de iniciar a compilação.

Exemplos no PowerShell, considerando a execução a partir da raiz do
projeto:

``` powershell
# Compilar um arquivo; o .vm será gravado junto ao arquivo de entrada.
.\build-cli\jack_cli.exe --compile .\Main.jack

# Compilar os arquivos .jack diretamente contidos em um diretório,
# gravando os resultados no diretório informado.
.\build-cli\jack_cli.exe --compile .\fontes --output .\saida

# Forma alternativa, utilizando compile e -o.
.\build-cli\jack_cli.exe compile .\Main.jack -o .\saida

# Exibir a ajuda do modo não interativo.
.\build-cli\jack_cli.exe --help

# Forma abreviada para exibir a ajuda.
.\build-cli\jack_cli.exe -h
```

Na invocação do modo de compilação, o primeiro argumento após o nome do
executável deve ser `--compile` ou `compile`. O argumento seguinte
identifica um arquivo `.jack` ou um diretório de entrada. Se a opção
`--output` ou `-o` for informada, ela define o diretório de destino dos
arquivos `.vm`. Quando essa opção é omitida, os arquivos gerados são
gravados no mesmo diretório do arquivo de entrada ou do diretório
processado.

As opções `--help` e `-h` solicitam a exibição da ajuda e não exigem um
arquivo ou diretório de entrada.

Quando a entrada é um diretório, a busca por arquivos `.jack` é não
recursiva: somente os arquivos diretamente contidos nele são
considerados. Subdiretórios não são percorridos.

Esse modo é independente do menu interativo. Para consultar a sintaxe
reconhecida pelo executável, utilize `--help` ou `-h`.

### 3.11. Sair

No modo interativo, a opção 0 encerra a aplicação e apresenta a mensagem
de encerramento.

## 4. Mensagens apresentadas no terminal

A CLI utiliza identificadores textuais para facilitar a interpretação
dos resultados:

| Identificador | Significado |
| --- | --- |
| `[INFO]` | Informação sobre a operação em andamento ou seus resultados. |
| `[OK]` | Operação concluída com sucesso. |
| `[AVISO]` | Situação que exige atenção, mas não necessariamente representa uma falha. |
| `[ERRO]` | Falha na operação ou entrada inválida. |

Quando o terminal oferece suporte, essas mensagens também são
apresentadas com cores.

## 5. Fluxo de utilização recomendado

Para analisar um arquivo individual, recomenda-se o seguinte fluxo:

1.  Iniciar a aplicação;
2.  Selecionar um arquivo Jack;
3.  Exibir o arquivo, se necessário;
4.  Executar a análise completa;
5.  Exportar o XML léxico ou sintático, caso desejado;
6.  Encerrar a aplicação.

Para verificar léxica e sintaticamente vários arquivos de uma vez,
utilize a opção 9. Para executar a coleção de testes organizada entre
arquivos válidos e inválidos, utilize a opção 8. Para gerar código VM de
um arquivo ou de um diretório, utilize o modo não interativo descrito na
seção 3.10.

## 6. Limitações da interface

O menu interativo disponibiliza as análises léxica e sintática, a
exportação de suas representações XML e as operações de teste descritas
neste manual. A geração de código VM é disponibilizada separadamente
pelo modo não interativo.

A existência da tabela de símbolos e de verificações utilizadas durante
a geração VM não deve ser interpretada como comprovação de uma etapa de
análise semântica completa e abrangente.

O modo não interativo documentado neste manual limita a entrada a um
arquivo `.jack` ou a um diretório. Quando recebe um diretório, o
processamento não percorre subdiretórios. Para confirmar as opções
aceitas pelo executável, consulte `jack_cli --help`.
