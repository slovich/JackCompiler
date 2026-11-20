# Uso de Inteligência Artificial no Desenvolvimento

## 1. Introdução

O desenvolvimento do JackCompiler contou com o auxílio de ferramentas de Inteligência Artificial (IA), utilizadas como instrumentos de apoio à programação, análise de código, investigação de problemas técnicos e elaboração da documentação.

A principal ferramenta utilizada foi o **ChatGPT, da OpenAI**, com o modelo de linguagem GPT-6.

A IA participou de diferentes etapas do desenvolvimento, desde discussões sobre arquitetura e organização do código até a implementação de funcionalidades, depuração e revisão de decisões técnicas.

Sua utilização ocorreu de maneira colaborativa e incremental, mantendo o desenvolvedor responsável pela condução do projeto e pelas decisões finais.

## 2. Contribuições da IA

As principais atividades em que a IA foi utilizada estão descritas a seguir.

### 2.1. Arquitetura e organização do projeto

A IA auxiliou na discussão e definição da estrutura dos componentes, na identificação de responsabilidades e na organização dos módulos.

Entre as atividades apoiadas estão:

- Organização das classes e dos arquivos-fonte.
- Separação das responsabilidades entre os componentes do compilador.
- Desacoplamento dos analisadores das dependências do Qt.
- Estruturação do núcleo independente de processamento (`core`).
- Integração do núcleo com as interfaces gráfica (`GUI`) e de linha de comando (`CLI`).
- Configuração do processo de compilação utilizando CMake.

### 2.2. Desenvolvimento do analisador léxico

A IA auxiliou na implementação e no aprimoramento dos mecanismos responsáveis pelo reconhecimento dos elementos léxicos da linguagem Jack.

As contribuições envolveram a identificação de palavras-chave, símbolos, identificadores, constantes inteiras e constantes de texto, além da investigação de situações de erro durante a leitura do código-fonte.

### 2.3. Desenvolvimento do analisador sintático

A IA apoiou a implementação do analisador sintático baseado na estratégia de descida recursiva, com procedimentos associados às regras gramaticais da linguagem Jack.

Também contribuiu para a estruturação dos nós da árvore sintática abstrata (AST), o tratamento de expressões e declarações e a identificação de erros sintáticos.

### 2.4. Geração de representações XML

A IA auxiliou na implementação dos mecanismos de geração das representações XML produzidas pelo compilador.

Foram trabalhadas duas representações distintas:

- **XML léxico:** representação dos tokens identificados pelo analisador léxico.
- **XML sintático:** representação hierárquica da estrutura sintática reconhecida pelo analisador, construída a partir da AST.

### 2.5. Desenvolvimento das interfaces

A IA apoiou a integração dos componentes do núcleo com as interfaces de execução.

Na interface gráfica, as atividades envolveram a integração dos analisadores, a visualização dos resultados, a representação hierárquica da AST, o realce léxico e a exportação dos documentos XML.

Na interface de linha de comando, o apoio envolveu a organização do menu, a implementação dos fluxos de análise e exportação, a execução automatizada de testes e o processamento de arquivos Jack em lote.

### 2.6. Testes, depuração e validação

A IA foi utilizada para analisar mensagens de erro, investigar comportamentos inesperados e propor correções.

Também auxiliou na elaboração e organização de casos de teste, incluindo arquivos Jack válidos e inválidos, além da definição dos critérios utilizados na execução automatizada.

As alterações propostas foram submetidas à compilação e à execução pelo desenvolvedor, que verificou os resultados no ambiente do projeto.

### 2.7. Controle de versão e documentação

A IA também foi utilizada como apoio às atividades de manutenção do repositório, incluindo:

- Organização de commits e elaboração de suas mensagens.
- Discussão sobre estratégias de integração entre branches.
- Investigação de problemas relacionados ao Git e ao GitHub.
- Organização da documentação técnica e dos manuais de utilização.

## 3. Metodologia de utilização

A utilização da IA ocorreu por meio de um processo de desenvolvimento incremental. Em vez de delegar a implementação integral da aplicação, o desenvolvedor conduziu o trabalho por funcionalidades e etapas, utilizando a IA como ferramenta de consulta, análise e apoio à implementação.

O fluxo de trabalho adotado envolveu:

1. Definição da funcionalidade e de seus requisitos.
2. Discussão das possíveis abordagens técnicas e das alterações necessárias.
3. Apresentação de orientações, trechos de código ou procedimentos pela IA.
4. Avaliação e aplicação das alterações pelo desenvolvedor.
5. Compilação e execução da aplicação para verificar o comportamento implementado.
6. Apresentação de erros ou resultados à IA para análise e investigação, quando necessário.
7. Correção e validação das alterações.
8. Registro das modificações no controle de versão.

Esse processo permitiu acompanhar a evolução do projeto de maneira gradual, mantendo a compreensão do código e das decisões técnicas como parte do desenvolvimento.

## 4. Responsabilidades e limitações

A IA atuou como ferramenta auxiliar, não como substituta do desenvolvedor.

A definição dos objetivos, a condução das etapas, a avaliação das propostas, a aplicação das modificações, a execução dos testes e a administração do repositório permaneceram sob responsabilidade do desenvolvedor.

As sugestões produzidas pela IA não foram consideradas automaticamente corretas. Códigos, procedimentos e decisões sugeridos precisaram ser avaliados no contexto do projeto e, quando aplicável, verificados por meio de compilação e testes.

A utilização da IA também não elimina a necessidade de compreensão dos componentes implementados. O desenvolvedor permaneceu responsável por compreender a arquitetura, avaliar as soluções adotadas e verificar sua adequação aos requisitos do compilador.

## 5. Considerações finais

A utilização de Inteligência Artificial contribuiu para apoiar o desenvolvimento do JackCompiler em diferentes dimensões, incluindo implementação, organização arquitetural, depuração, testes e documentação.

O processo permitiu explorar ferramentas modernas de assistência à programação em um projeto concreto de desenvolvimento de software, mantendo a participação ativa do desenvolvedor nas decisões e na validação dos resultados.

A experiência também demonstrou a importância da utilização crítica dessas ferramentas: seus resultados podem acelerar atividades de desenvolvimento, mas precisam ser avaliados, compreendidos e verificados antes de serem incorporados ao projeto.