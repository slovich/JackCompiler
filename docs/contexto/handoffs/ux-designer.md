# UX Designer --- Handoff inicial

**Projeto:** JackCompiler\
**Responsabilidade:** UX Designer --- interface gráfica e experiência do
usuário\
**Base examinada:** `mainwindow.ui`, `mainwindow.cpp`, `mainwindow.h` e
captura da aplicação fornecida pelo responsável pelo projeto.\
**Status deste documento:** levantamento inicial da interface existente.
Não constitui especificação visual aprovada nem plano de implementação.

------------------------------------------------------------------------

## 1. Papel do UX Designer no projeto

O UX Designer é responsável por estudar e especificar a experiência de
uso da interface gráfica Qt do JackCompiler, incluindo organização da
informação, navegação, estados visuais, feedback ao usuário e
responsividade.

O trabalho deve ser conduzido em discussão e aprovação contínuas com o
responsável pelo projeto. As especificações aprovadas serão encaminhadas
ao Analista, que organizará as fases de desenvolvimento, e ao
Desenvolvedor, responsável pela implementação.

### Limites de atuação

-   Não escrever, gerar ou alterar código de produção.
-   Não modificar a arquitetura ou a lógica interna de `jack_core` ou
    `jack_cli`.
-   Não assumir atribuições do Analista, Desenvolvedor, Testador,
    GitKeeper ou Gerenciador Documental.
-   Não presumir que funcionalidades previstas ou em desenvolvimento já
    estejam implementadas.
-   Não consolidar decisões de UX sem discussão e aprovação do
    responsável pelo projeto.

------------------------------------------------------------------------

## 2. Organização atual da interface

A aplicação gráfica utiliza Qt Widgets e possui uma janela principal
`MainWindow`, construída por meio de `QMainWindow`.

Conforme os arquivos examinados, a interface atualmente está organizada
em duas abas principais:

1.  **Analisador Léxico**
2.  **Analisador Sintático**

### 2.1. Aba Analisador Léxico

A aba contém duas regiões principais:

**Seleção de arquivo** - `QTreeView` para navegação por diretórios. -
`QListView` para exibição dos arquivos encontrados. - O modelo de
diretórios e arquivos é baseado em `QFileSystemModel`. - O filtro
configurado para a lista de arquivos é `*.jack`. - O diretório inicial
configurado no código é `C:/`.

**Ações** - Botão `Selecionar`. - Botão `Exportar XML`. - Os botões são
habilitados ou desabilitados conforme a disponibilidade dos dados e a
seleção de arquivo.

Na parte inferior existe um `QTabWidget` de detalhes com três páginas:

-   **Resumo:** apresenta o nome do arquivo, situação da análise, total
    de tokens e quantidades por categoria.
-   **Código-fonte:** apresenta o conteúdo do arquivo em um `QTextEdit`
    configurado como somente leitura. Há integração com o realce léxico.
-   **Lista de Tokens:** apresenta uma tabela com número, linha,
    categoria e lexema. A tabela não permite edição manual e seleciona
    linhas inteiras.

### 2.2. Aba Analisador Sintático

A aba contém um `QTabWidget` com duas páginas:

-   **XML:** apresenta o XML sintático em um `QTextEdit`.
-   **Árvore Hierárquica:** apresenta a estrutura em um `QTreeView`.

Há também um botão para exportar o XML sintático.

A aba sintática começa desabilitada e é habilitada quando a análise
sintática termina com sucesso.

------------------------------------------------------------------------

## 3. Fluxo funcional atualmente implementado

O fluxo observado em `MainWindow::on_pushButton_selecionar_clicked()` é
integrado: selecionar um arquivo não apenas o carrega, mas desencadeia
as análises léxica e sintática.

Sequência atual:

1.  O usuário navega pelos diretórios e seleciona um arquivo `.jack`.
2.  O botão `Selecionar` é habilitado quando há um arquivo selecionado.
3.  Ao acionar `Selecionar`, a aplicação limpa resultados sintáticos
    anteriores.
4.  O arquivo é aberto e seu conteúdo é lido.
5.  O `AnalisadorLexico` é executado.
6.  Em caso de sucesso léxico:
    -   os tokens são armazenados para uso pela interface;
    -   o resumo é atualizado;
    -   o código-fonte é apresentado;
    -   a tabela de tokens é preenchida;
    -   a exportação do XML léxico é habilitada.
7.  Em seguida, o `AnalisadorSintatico` é executado automaticamente com
    os tokens e o código-fonte.
8.  Em caso de sucesso sintático:
    -   a AST retornada é armazenada;
    -   o XML sintático é gerado;
    -   a visualização XML é atualizada;
    -   a árvore hierárquica é construída;
    -   a aba sintática e a exportação do XML sintático são habilitadas.
9.  Em caso de falha sintática, os resultados léxicos permanecem
    disponíveis e a aba sintática continua desabilitada.

A interface utiliza caixas de mensagem (`QMessageBox`) para comunicar
algumas situações de erro, aviso e sucesso.

A exportação léxica e a exportação sintática são ações separadas, cada
uma com diálogo para escolha do destino do arquivo.

------------------------------------------------------------------------

## 4. Elementos técnicos observados que afetam a experiência

Os pontos abaixo são constatações dos arquivos enviados, não propostas
de alteração de código.

### 4.1. Layout

O arquivo `mainwindow.ui` combina gerenciadores de layout com widgets
posicionados por geometrias explícitas. Há componentes internos de abas
com coordenadas e dimensões fixas.

Esse aspecto deverá ser considerado em qualquer avaliação futura de
redimensionamento e adaptação da interface.

### 4.2. Feedback

O código examinado utiliza `QMessageBox` para avisos, erros e
confirmações de operações. Não foi identificada, nos arquivos da
`MainWindow` enviados, uma estrutura própria de acompanhamento contínuo
de operações.

### 4.3. Execução das análises

As análises léxica e sintática são acionadas dentro do slot de seleção
de arquivo. No código examinado, não há evidência de que essas operações
sejam executadas em uma thread de trabalho separada.

A responsividade durante operações mais demoradas deverá ser avaliada
com o Desenvolvedor e o Analista quando houver uma fase específica para
isso. O UX Designer não deve prescrever detalhes de implementação
interna.

### 4.4. Seleção de arquivos

O construtor configura o diretório inicial como `C:/` e o filtro
`*.jack`. A seleção de um diretório limpa a seleção anterior da lista de
arquivos e desabilita o botão `Selecionar` até que um arquivo seja
selecionado.

No código fornecido existem duas conexões para `selectionChanged` da
seleção de arquivos que executam verificações equivalentes para
habilitar o botão `Selecionar`. Esse registro é apenas uma observação;
eventual avaliação técnica cabe ao Analista e ao Desenvolvedor.

### 4.5. Sobre

A aplicação possui uma ação de menu `Ajuda > Sobre`, que abre
`DialogSobre`. Os arquivos enviados mostram um diálogo modal com texto
informativo e botão `VOLTAR`. O diálogo utiliza dimensões fixas no
arquivo `.ui`.

------------------------------------------------------------------------

## 5. O que este levantamento não confirma

Este documento não afirma:

-   que a interface tenha sido validada em diferentes resoluções;
-   que exista suporte implementado a execução assíncrona ou
    cancelamento de operações;
-   que análise semântica ou geração de código VM estejam disponíveis na
    GUI;
-   que a interface já tenha recursos de progresso global ou progresso
    por etapa;
-   que o comportamento visual observado em uma captura seja idêntico em
    todos os estilos e temas do sistema operacional;
-   que todos os elementos da aplicação estejam descritos pelos três
    arquivos da `MainWindow` examinados.

O levantamento limita-se aos arquivos e à captura disponibilizados nesta
etapa.

------------------------------------------------------------------------

## 6. Oportunidades futuras de avaliação de UX

Os itens a seguir são temas que poderão ser estudados em fases
posteriores. Não são decisões visuais aprovadas nem instruções de
implementação.

1.  **Navegação e organização da janela:** avaliar se a organização
    atual em abas continua adequada à evolução funcional do compilador.
2.  **Hierarquia da informação:** avaliar a relação entre seleção de
    arquivos, ações disponíveis e apresentação dos resultados.
3.  **Fluxo de análise:** discutir se a seleção de arquivo continuará
    executando automaticamente todas as etapas ou se haverá ações
    distintas. Essa decisão depende de discussão com o responsável e de
    alinhamento com o Analista.
4.  **Estados da aplicação:** mapear estados inicial, arquivo carregado,
    processamento, conclusão, aviso, erro e cancelamento, conforme as
    operações efetivamente disponíveis.
5.  **Feedback de operações:** avaliar formas de comunicar processamento
    e resultados sem depender excessivamente de caixas de mensagem.
6.  **Responsividade:** avaliar redimensionamento, distribuição de
    espaço e comportamento dos componentes em diferentes dimensões de
    janela.
7.  **Evolução funcional:** quando novas etapas do compilador forem
    confirmadas pelo núcleo e pelo Desenvolvedor, estudar como
    apresentá-las sem presumir funcionalidades ainda não implementadas.

Qualquer proposta deverá preservar a separação entre interface e núcleo:
a GUI solicita operações e apresenta resultados; não incorpora lógica de
compilação.

------------------------------------------------------------------------

## 7. Relação com os demais agentes

-   **Gerenciador Documental / Arquiteto de Documentação:** registra e
    controla a documentação do projeto produzida pelos agentes. Deve
    receber este handoff e orientar sua integração ao conjunto
    documental.
-   **Analista:** recebe diretrizes aprovadas, organiza fases de
    desenvolvimento e define seus critérios de abertura e encerramento.
-   **Desenvolvedor:** implementa o código da GUI conforme as
    especificações aprovadas. Também atua na CLI, cuja interface de
    terminal não faz parte do escopo do UX Designer.
-   **Testador:** trabalha com o Desenvolvedor para verificar se as
    implementações atendem aos objetivos e critérios estabelecidos.
-   **GitKeeper:** administra o versionamento e o GitHub, podendo
    revisar alterações antes de commits e pushes.
-   **UX Designer:** estuda e especifica a experiência da GUI, valida
    propostas com o responsável e encaminha especificações aprovadas ao
    Analista.

------------------------------------------------------------------------

## 8. Estado deste handoff

Este é o documento inicial de ambientação do UX Designer. Registra
apenas a interface existente e temas que poderão ser avaliados
futuramente.

As discussões sobre reformulação visual já iniciadas devem permanecer em
caráter de discussão até que o responsável pelo projeto aprove sua
consolidação em uma especificação própria. Portanto, este handoff não
incorpora decisões visuais ainda em negociação.

**Fonte técnica examinada:** `mainwindow.ui`, `mainwindow.cpp`,
`mainwindow.h` e captura da janela principal fornecidos pelo responsável
pelo projeto.
