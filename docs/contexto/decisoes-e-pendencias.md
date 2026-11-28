# Decisões, fatos e pendências

## Decisões e regras de coordenação

1. A documentação oficial existente será preservada em seus caminhos atuais.
2. `docs/contexto/` será uma estrutura complementar, não substitutiva.
3. O código e os testes atuais prevalecem sobre descrições documentais divergentes.
4. Não presumir implementação de análise semântica. A geração de código VM possui implementação e validação funcional registradas, conforme o encerramento da etapa VMCodeGenerator. Distinguir a existência dos componentes, sua integração e o alcance efetivamente validado.
5. Não alterar documentos oficiais sem apresentar diagnóstico e proposta.
6. Não realizar commits, tags ou releases sem autorização explícita.
7. Toda mudança proposta deve vir acompanhada de comandos PowerShell seguros para aplicação e validação.
8. Chats especialistas devem registrar evidências, arquivos afetados, verificações e pendências.
9. A frente UX Designer é responsável pelo estudo e pela especificação da experiência de uso da GUI, incluindo organização visual, navegação, fluxos de interação, estados visuais, feedback e responsividade. Suas propostas não constituem decisões aprovadas nem autorizam implementação. Após aprovação do responsável pelo projeto, as especificações são encaminhadas ao Analista para formalização dos requisitos e critérios de aceitação, antes de serem conduzidas às etapas de desenvolvimento.

## Fatos confirmados na auditoria inicial

- Branch `main`, commit `db2c2726526160a158a4a2579f9ebf74df7edefa`, sincronizada com `origin/main` na coleta.
- Working tree limpa na coleta.
- Sete documentos em `docs/`, mais README na raiz.
- CMake define `jack_core`, `jack_cli` e, com `BUILD_GUI=ON`, `compilador`.
- Qt está vinculado ao alvo GUI; CLI e core não recebem essa dependência no CMake examinado.
- `JACK_PROJECT_ROOT` é definido para `jack_cli` e usado pela opção 8.
- Não há configuração CTest no CMake examinado.
- O inventário de testes contém 8 arquivos válidos e 9 inválidos.
- Opção 8 executa testes classificados; opção 9 executa processamento em lote.
- A GUI executa lexer antes do parser e só chega ao parser se o lexer passar.
- A árvore sintática da GUI é construída a partir do XML sintático.
- O parser rejeita tokens extras após a classe.
- Expressões binárias são construídas com associatividade à esquerda, sem níveis de precedência separados na rotina examinada.

## Pendências documentais remanescentes da auditoria inicial

1. `docs/instalacao/compilacao.md`: revisar a tabela de diretórios, que aparenta duplicar `build`/`build/` e listar diretórios de build distintos; comparar com a árvore real antes de editar.
2. `docs/tecnico/linguagem-jack.md`: corrigir a formatação da tabela de operadores, preservando os dados e a terminologia.
3. `docs/usuario/manual-gui.md`: deixar explícito que a navegação/seleção do arquivo e o acionamento do botão de seleção/análise são etapas distintas; a análise é executada pelo slot `on_pushButton_selecionar_clicked()`.
4. `docs/usuario/manual-cli.md`: a distinção entre seleção individual, opção 8 e opção 9 foi preservada; a revisão de 28/11 acrescenta as opções 10/11 e a comparação XML.
5. `docs/tecnico/testes.md`: atualizado para distinguir os testes independentes não integrados ao CTest do alvo opcional `comparadorxml_regression_tests`, habilitado por `BUILD_XML_COMPARATOR_REGRESSION_TESTS`.
6. `docs/tecnico/arquitetura.md`: atualizado para incluir o Comparador XML, pugixml 1.16, o alvo `jack_pugixml`, a integração CLI e os testes opcionais.
7. `README.md`: atualizado com o Comparador XML e os resultados recentes. Continua pendente apenas decidir se a página pública deve mencionar a pasta interna `docs/contexto/`.

## Pendências de verificação herdadas da auditoria inicial

> Os itens abaixo foram registrados na auditoria de 20/11/2026 e não foram
> reavaliados como parte do encerramento do Comparador XML em 28/11/2026.
> Não devem ser confundidos com pendências de implementação dessa etapa.

- Confirmar o trecho final do fluxo GUI após a linha de sucesso sintático, especialmente a apresentação de falha sintática e mensagens finais.
- Confirmar a parte final do resumo apresentado pela opção 9, pois a extração fornecida terminou antes do fim do método.
- Se uma revisão técnica detalhada exigir, examinar o código integral e os testes correspondentes das afirmações sobre lexer, parser e regras da linguagem.
- Antes de cada trabalho especialista, atualizar branch, commit e working tree; o estado registrado aqui é uma fotografia de 20/11/2026, não uma garantia do estado futuro.
- Validar a estrutura atual de `docs/contexto/` no repositório: ela não apareceu na listagem enviada na última coleta, embora tenha sido informada anteriormente como criada.

## Não assumir

- Não afirmar que o projeto possui testes CTest.
- Não afirmar que a árvore GUI percorre diretamente a AST.
- Não tratar a opção 9 como a mesma rotina da opção 8.
- Não afirmar que o caminho padrão dos testes é relativo ao diretório corrente; é derivado de `JACK_PROJECT_ROOT`.
- Não afirmar que a análise semântica está implementada sem evidência específica.
- Não confundir a implementação e validação do VMWriter e VMCodeGenerator com prova de cobertura exaustiva da linguagem Jack.
- Não classificar as limitações reproduzidas com o compilador oficial ou as inconsistências do VM Emulator desktop como defeitos confirmados do JackCompiler.

### VMWriter — esclarecimento sobre comandos aritméticos/lógicos

**Decisão:** a implementação do VMWriter aceita os nove comandos
aritméticos/lógicos previstos na linguagem VM:

`add`, `sub`, `neg`, `eq`, `gt`, `lt`, `and`, `or` e `not`.

A conferência do documento acadêmico "Próxima Etapa — Geração
de Código VM", seção "Geração de código — Etapa 1: expressões
e comandos", esclareceu que o enunciado apresenta operadores
da linguagem Jack, não uma exigência de dez comandos VM.

Multiplicação e divisão devem ser traduzidas em chamadas ao
sistema operacional:

- `*` → `call Math.multiply 2`
- `/` → `call Math.divide 2`

Portanto, não há evidência de requisito para um décimo comando
aritmético/lógico no VMWriter.

**Validação:** 14/14 cenários aprovados.

**Pendência:** nenhuma relativa à quantidade de comandos
aritméticos/lógicos.

### VMCodeGenerator e integração CLI/VM — etapa encerrada

**Decisão:** considerar concluída a etapa de implementação e validação funcional do VMCodeGenerator, com aprovação restrita aos programas e cenários efetivamente testados.

Resultados registrados no parecer final do Testador:

- SymbolTable: 8/8 cenários aprovados.
- VMWriter: 14/14 cenários aprovados.
- Seis programas oficiais do Project 11 compilados com sucesso: Seven, ConvertToBin, Square, Average, Pong e ComplexArrays.
- Seis programas com funcionalidades principais validadas no escopo definido.
- Seis diretórios processados.
- Onze arquivos VM gerados.
- 2.396 linhas de código VM submetidas à inspeção textual.
- Integração CLI/VM validada nos cenários apresentados.

**Ressalvas preservadas:**

1. O redimensionamento do Square não funcionou e apresentou o mesmo comportamento com o compilador oficial no mesmo emulador. Não há evidência suficiente para atribuir a ocorrência ao JackCompiler.
2. O VM Emulator desktop apresentou inconsistências na interação por teclado, sem causa determinada.
3. A hipótese de perda de contexto de `pointer 0` em chamadas encadeadas permanece como registro histórico, sem defeito confirmado.

Os resultados do emulador online foram relatados pelo desenvolvedor e não reproduzidos pessoalmente pelo Testador nesta conversa.

**Limite da aprovação:** os resultados não representam prova exaustiva de correção para todas as construções da linguagem Jack, nem garantem ausência de regressões em programas não testados.

**Encaminhamento:** etapa encerrada. Não solicitar alterações especulativas no código com base nas ocorrências acima. Novas intervenções devem depender de evidências de defeito ou de novos requisitos.


## Etapa Comparador XML — encerramento técnico em 28/11/2026

**Estado:** implementação e avaliação funcional encerradas, conforme
handoffs atualizados do Desenvolvedor e do Testador.

### Decisões finais preservadas

1. O Comparador XML integra o `jack_core`, sem dependência de Qt e sem
   acesso próprio a arquivos ou terminal.
2. A implementação utiliza pugixml 1.16, encapsulada no alvo estático
   `jack_pugixml` e vinculada privadamente ao `jack_core`.
3. A CLI oferece comparação léxica na opção 10 e sintática na opção 11.
   Cada operação solicita diretamente o caminho do XML de referência e
   o caminho do XML gerado; os arquivos podem estar em diretórios
   diferentes.
4. A comparação considera estrutura, hierarquia, ordem dos elementos,
   atributos e conteúdo textual. A ordem textual dos atributos não é
   significativa. Somente whitespace estrutural entre elementos pode
   ser ignorado; o conteúdo textual terminal é preservado integralmente.
5. Comentários, declaração XML e instruções de processamento são
   desconsiderados. DTD efetivo é rejeitado; não há resolução de
   recursos externos.
6. Os exportadores sugerem nomes distintos: `_lexico.xml` e
   `_sintatico.xml`.
7. As propostas anteriores de comando não interativo `compare-xml`,
   comparação por diretórios, busca automática e processamento em lote
   foram substituídas pela interface interativa aprovada. Não devem ser
   reabertas como pendências sem novo requisito do responsável.
8. A opção CMake `BUILD_XML_COMPARATOR_REGRESSION_TESTS` permanece
   desativada por padrão e habilita quatro casos automatizados de
   regressão registrados no CTest.

### Evidências de encerramento

- DEF-XML-01 e DEF-XML-02 aprovados.
- Testes funcionais das opções 10 e 11 aprovados nos cenários
  documentados pelo Testador.
- Teste automatizado de regressão: CTest 1/1 aprovado, reunindo quatro
  casos de DTD.
- Distribuição Windows validada no ambiente independente Windows 11
  testado, sem Qt, CMake ou MinGW instalados, com `jack_cli.exe` e
  `libwinpthread-1.dll`.
- Regressão VM: seis projetos Project 11 compilados, 11/11 arquivos VM
  gerados e resultados funcionais positivos nos cenários observados.
- Ressalva de Square preservada: movimentação funcional, redimensionamento
  não funcional também com o compilador oficial no emulador comparado.
- W-13 aprovou a identificação de falha real de permissão de leitura do
  XML gerado; a condição equivalente no arquivo de referência não foi
  demonstrada.

### Pendência de integração e publicação

Não há pendência de código identificada para esta fase segundo o
parecer do Desenvolvedor. A etapa documental agora inclui atualizar
README, arquitetura, manual da CLI, testes e instruções de compilação,
além deste estado e registro de decisões.

O handoff do Desenvolvedor informa que não foram realizados commit,
tag, release ou push para a etapa. O GitKeeper deve confirmar a situação
real do checkout, revisar os diffs e o staging e organizar a publicação
em commits isolados. A conclusão técnica não deve ser confundida com
publicação no GitHub.


### Revisão documental de 28/11/2026

Foram atualizados nesta revisão:

- `README.md`;
- `docs/usuario/manual-cli.md`;
- `docs/tecnico/arquitetura.md`;
- `docs/tecnico/testes.md`;
- `docs/instalacao/compilacao.md`;
- `docs/contexto/estado-atual.md`;
- `docs/contexto/decisoes-e-pendencias.md`.

A revisão incorpora a conclusão do Comparador XML, os resultados de
avaliação independente, a regressão VM W-12, a validação de distribuição
Windows e o procedimento de runtime estático da CLI. Os documentos de
handoff do Desenvolvedor e do Testador fornecidos no pacote foram
preservados como registros de evidência, sem reescrita nesta atualização.

A documentação preparada não comprova commit, tag, release ou push.
Essas operações dependem da conferência do checkout e do procedimento
de autorização do GitKeeper.
