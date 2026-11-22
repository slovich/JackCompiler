# Manual do Usuário --- Interface Gráfica (GUI)

## 1. Apresentação

A interface gráfica do JackCompiler permite selecionar arquivos de
código-fonte Jack, executar as análises léxica e sintática e consultar
os resultados em diferentes formas de apresentação.

A janela principal organiza as funcionalidades em duas áreas:
**Analisador Léxico** e **Analisador Sintático**. A primeira reúne a
seleção do arquivo, o resumo da análise, a visualização do código-fonte
e a listagem de tokens. A segunda apresenta o XML sintático gerado e sua
estrutura hierárquica.

Este manual descreve o funcionamento e a organização visual da interface
em seu estado atual. A geração de código VM é um fluxo documentado no
manual da CLI e não deve ser confundida com as funcionalidades gráficas
descritas aqui.

## 2. Janela principal

Ao iniciar a aplicação, é apresentada a janela principal, identificada
pelo título **Jack Compiler**.

A interface possui duas abas principais:

-   **Analisador Léxico**: área inicial de trabalho, utilizada para
    localizar e analisar arquivos Jack e consultar os resultados da
    análise léxica.
-   **Analisador Sintático**: área destinada à visualização do XML
    produzido a partir da análise sintática e de sua representação em
    árvore.

A aba **Analisador Sintático** permanece desabilitada até que um arquivo
seja processado e a análise sintática seja concluída com sucesso.

## 3. Analisador Léxico

### 3.1. Seleção de arquivo

Na parte superior da aba **Analisador Léxico**, a seção **Seleção de
Arquivo** apresenta dois painéis:

-   **Navegação por diretórios**: permite percorrer a estrutura de
    diretórios do sistema de arquivos.
-   **Lista de arquivos**: apresenta os arquivos com extensão `.jack`
    existentes no diretório selecionado.

Ao iniciar, a navegação é posicionada em `C:/`. Para localizar um
arquivo:

1.  Navegue pela árvore de diretórios até encontrar a pasta desejada.
2.  Selecione a pasta.
3.  Consulte a lista de arquivos apresentada ao lado.
4.  Selecione o arquivo `.jack` que deseja analisar.
5.  Clique em **Selecionar**.

O botão **Selecionar** fica habilitado quando há um arquivo selecionado
na lista. Ao trocar de diretório, a seleção anterior é limpa e o botão
volta a ficar desabilitado até que outro arquivo seja selecionado.

### 3.2. Carregamento e execução das análises

Ao clicar em **Selecionar**, a aplicação:

1.  Abre e lê o arquivo escolhido;
2.  Executa a análise léxica;
3.  Se a análise léxica for bem-sucedida, apresenta os tokens e atualiza
    o resumo e a visualização do código-fonte;
4.  Em seguida, executa automaticamente a análise sintática;
5.  Se a análise sintática for bem-sucedida, gera o XML sintático,
    atualiza sua visualização em árvore e habilita a aba **Analisador
    Sintático**.

Portanto, na GUI, as duas análises são executadas automaticamente em
sequência. Não é necessário acionar separadamente um comando para
iniciar a análise sintática.

Se ocorrer um erro léxico, a aplicação apresenta uma mensagem de aviso
com o erro identificado e interrompe o processamento daquele arquivo. Se
a análise léxica for concluída, mas a análise sintática falhar, a
aplicação informa que a etapa léxica foi bem-sucedida, apresenta a
quantidade de tokens e exibe a mensagem do erro sintático. Nesse caso, a
aba sintática permanece desabilitada.

### 3.3. Seção Ações

Ao lado dos painéis de seleção, a seção **Ações** apresenta o botão
**Selecionar**, descrito anteriormente, e o botão **Exportar XML**.

O botão **Exportar XML** fica disponível após uma análise léxica
bem-sucedida. Ele permite salvar a representação XML dos tokens
produzidos pelo analisador léxico.

### 3.4. Aba interna Resumo

A área inferior da aba **Analisador Léxico** possui três abas internas.
A primeira é **Resumo**.

Após uma análise léxica bem-sucedida, são apresentados:

-   Nome do arquivo analisado;
-   Situação da análise;
-   Total de tokens;
-   Quantidade de palavras-chave;
-   Quantidade de símbolos;
-   Quantidade de constantes inteiras;
-   Quantidade de constantes de texto;
-   Quantidade de identificadores.

O resumo apresenta a distribuição dos tokens por categoria, permitindo
uma consulta rápida ao resultado da análise léxica.

### 3.5. Aba interna Código-fonte

A aba **Código-fonte** exibe o conteúdo do arquivo analisado.

O texto é apresentado em modo somente leitura, não sendo possível editar
o código diretamente pela interface. O componente aplica realce léxico
com base nos tokens produzidos pelo analisador.

As categorias de tokens são diferenciadas visualmente:

| Categoria | Apresentação |
|---|---|
| Palavras-chave | Azul escuro e negrito |
| Símbolos | Roxo |
| Constantes inteiras | Verde escuro |
| Constantes de texto | Marrom-avermelhado |
| Identificadores | Azul-petróleo |

O realce é aplicado às ocorrências identificadas na análise léxica.
Assim, a coloração não é uma edição do código nem uma análise
independente: ela utiliza os tokens já produzidos pelo analisador.

### 3.6. Aba interna Lista de Tokens

A aba **Lista de Tokens** apresenta os tokens identificados em uma
tabela.

A tabela contém quatro colunas:

| Coluna | Conteúdo |
|---|---|
| Nº | Posição do token na listagem, iniciando em 1 |
| Linha | Linha do código-fonte em que o token foi identificado |
| Categoria | Tipo do token |
| Lexema | Texto correspondente ao token |

As categorias exibidas são `keyword`, `symbol`, `integerConstant`,
`stringConstant` e `identifier`.

A tabela é somente para consulta: seus dados não podem ser editados pela
interface. A seleção é feita por linha, com apenas uma linha
selecionável por vez.

## 4. Analisador Sintático

A aba principal **Analisador Sintático** é habilitada somente quando a
análise sintática do arquivo selecionado é concluída com sucesso.

Ela apresenta duas abas internas:

### 4.1. Aba XML

A aba **XML** exibe a representação XML gerada a partir da estrutura
sintática reconhecida pelo parser.

O conteúdo é apresentado em uma área de texto com realce visual. As tags
XML são destacadas em cinza, enquanto os conteúdos dos elementos que
representam tokens Jack utilizam a mesma diferenciação de cores adotada
na visualização do código-fonte:

| Categoria | Apresentação |
|---|---|
| Tags XML | Cinza |
| Palavras-chave | Azul escuro e negrito |
| Símbolos | Roxo |
| Constantes inteiras | Verde escuro |
| Constantes de texto | Marrom-avermelhado |
| Identificadores | Azul-petróleo |

O realce facilita a leitura da estrutura XML e a identificação dos
valores associados aos diferentes tipos de tokens.

### 4.2. Aba Árvore Hierárquica

A aba **Árvore Hierárquica** apresenta uma representação em árvore do
XML sintático.

Cada elemento XML é representado por um nó, e os elementos internos são
organizados como filhos de seus respectivos elementos-pai. Os conteúdos
textuais são apresentados como nós subordinados ao elemento
correspondente.

A árvore é expandida automaticamente após sua geração, permitindo
visualizar a estrutura hierárquica produzida pelo parser.

### 4.3. Exportar XML sintático

Na seção **Ações** da aba **Analisador Sintático**, o botão **Exportar
XML** permite salvar o XML sintático gerado.

O botão somente fica habilitado quando existe um resultado sintático
válido.

Ao acioná-lo, a aplicação abre uma janela para escolha do destino do
arquivo. O nome sugerido é formado pelo nome-base do arquivo Jack
analisado, acrescido do sufixo `_sintatico` e da extensão `.xml`.

Exemplo:

``` text
Arquivo analisado: Main.jack
Nome sugerido: Main_sintatico.xml
```

O usuário pode escolher outro nome ou diretório. Caso a extensão `.xml`
não seja informada, a aplicação acrescenta essa extensão
automaticamente. O cancelamento da janela de salvamento interrompe a
exportação sem gravar o arquivo.

## 5. Menu superior

A janela principal possui uma barra de menus com duas opções:

### 5.1. Arquivo

O menu **Arquivo** contém a opção **Sair**, que encerra a janela
principal e finaliza a utilização da interface gráfica.

### 5.2. Ajuda

O menu **Ajuda** contém a opção **Sobre**, que abre uma janela com
informações de identificação do projeto.

## 6. Janela Sobre

A janela **Sobre** apresenta informações institucionais e de contexto da
origem acadêmica do projeto. Seu conteúdo atual identifica:

-   Projeto Jack Compiler;
-   Desenvolvimento de compilador em etapas;
-   Disciplina Compiladores, do curso de Engenharia da Computação da
    Universidade Federal do Maranhão (UFMA);
-   Docente responsável pela disciplina;
-   Discente responsável pelo projeto;
-   Local e ano indicados na identificação do projeto.

O botão **VOLTAR** fecha a janela e retorna à janela principal.

## 7. Evolução planejada da interface

A organização visual descrita neste manual corresponde à implementação
atual da GUI. As melhorias abaixo representam objetivos de evolução e
não devem ser interpretadas como funcionalidades já implementadas:

-   Melhorar a responsividade, para que os componentes se adaptem de
    maneira mais adequada a diferentes dimensões de janela e resoluções;
-   Reorganizar o layout e a distribuição dos controles;
-   Aprimorar a apresentação visual e a consistência dos elementos da
    interface;
-   Melhorar a experiência de uso (UX), tornando os fluxos de seleção,
    análise e consulta dos resultados mais claros e intuitivos.
