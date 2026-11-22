# Handoff — Auditoria documental inicial

## Objetivo

Registrar o diagnóstico documental e técnico realizado em 20/11/2026 para orientar a revisão dos documentos oficiais sem substituir a auditoria por suposições.

## Base

- Repositório: `slovich/JackCompiler`.
- Branch: `main`.
- Commit: `db2c2726526160a158a4a2579f9ebf74df7edefa`.
- Working tree: limpa na coleta.
- `origin/main` coincidia com o commit local na coleta.

## Documentos inventariados

Sete arquivos em `docs/`:
- desenvolvimento/uso-de-inteligencia-artificial.md
- instalacao/compilacao.md
- tecnico/arquitetura.md
- tecnico/linguagem-jack.md
- tecnico/testes.md
- usuario/manual-cli.md
- usuario/manual-gui.md

README principal na raiz.

## Achados técnicos

Consultar `../estado-atual.md` para o estado detalhado. Principais pontos:
- CMake declara `jack_core`, `jack_cli` e GUI opcional `compilador`.
- Qt está ligado ao alvo GUI.
- Não há CTest configurado no CMake examinado.
- `JACK_PROJECT_ROOT` é definido para a CLI e consumido pela opção 8.
- Opção 8 executa testes classificados em `validos` e `invalidos`.
- Opção 9 executa processamento em lote com caminho absoluto.
- Inventário: 8 válidos e 9 inválidos.
- GUI executa lexer antes do parser.
- Árvore sintática da GUI é construída a partir do XML.
- Parser constrói `ProgramNode`, rejeita tokens extras após a classe e a rotina de expressão observada associa à esquerda sem níveis separados de precedência.

## Correções candidatas

1. Revisar tabela de diretórios em `docs/instalacao/compilacao.md`.
2. Corrigir tabela de operadores em `docs/tecnico/linguagem-jack.md`.
3. Precisar fluxo de seleção/análise em `docs/usuario/manual-gui.md`.
4. Diferenciar opções 8 e 9 em `docs/usuario/manual-cli.md`.
5. Revisar afirmações de testes e CTest em `docs/tecnico/testes.md`.
6. Conferir dependências e fluxo XML/AST em `docs/tecnico/arquitetura.md`.
7. Avaliar se o README deve mencionar a documentação de contexto.

## Limitações da coleta final

A extração enviada do método GUI terminou após o trecho de sucesso sintático e não mostrou o tratamento final da falha sintática. A extração da opção 9 também terminou antes do final do método de resumo. Esses trechos devem ser consultados se a revisão dos manuais depender deles.

## Situação

Nenhum documento oficial foi alterado nesta auditoria. Os achados são diagnóstico e propostas, não autorização para editar.
