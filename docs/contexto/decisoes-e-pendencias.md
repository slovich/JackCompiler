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

## Correções documentais candidatas — ainda não aplicadas

1. `docs/instalacao/compilacao.md`: revisar a tabela de diretórios, que aparenta duplicar `build`/`build/` e listar diretórios de build distintos; comparar com a árvore real antes de editar.
2. `docs/tecnico/linguagem-jack.md`: corrigir a formatação da tabela de operadores, preservando os dados e a terminologia.
3. `docs/usuario/manual-gui.md`: deixar explícito que a navegação/seleção do arquivo e o acionamento do botão de seleção/análise são etapas distintas; a análise é executada pelo slot `on_pushButton_selecionar_clicked()`.
4. `docs/usuario/manual-cli.md`: distinguir seleção de arquivo individual, opção 8 (testes classificados) e opção 9 (processamento em lote).
5. `docs/tecnico/testes.md`: explicar a execução pela CLI e a ausência de integração CTest no CMake atual.
6. `docs/tecnico/arquitetura.md`: conferir descrições de dependências, fluxo GUI/XML/AST e responsabilidades dos alvos.
7. `README.md`: decidir se deve mencionar a pasta de contexto; não é obrigatório expor documentação operacional interna na página pública.

## Pendências de verificação

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