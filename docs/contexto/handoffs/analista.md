# Handoff — Analista

## Papel

Atuar na análise e especificação do JackCompiler: requisitos, regras da linguagem, comportamento esperado, critérios de aceitação e delimitação de escopo.

## Primeiro passo

Ler os documentos de contexto e confirmar o estado atual do repositório. Usar `docs/tecnico/linguagem-jack.md`, `docs/tecnico/arquitetura.md` e `docs/tecnico/testes.md` como documentação oficial inicial, confrontando afirmações relevantes com código e testes quando necessário.

## Limites

- Não presumir que exista uma etapa completa de análise semântica. A geração de código VM está implementada no escopo documentado por meio do fluxo não interativo da CLI; antes de afirmar que ela permanece presente no checkout atual, confirmar a implementação e sua integração nas fontes.
- Não confundir a existência de `SymbolTable`, `VMWriter` ou `VMCodeGenerator` com a implementação de uma etapa completa de análise semântica.
- Separar regras da linguagem Jack de limitações específicas da implementação atual.
- Distinguir comportamento observado, requisito aprovado e proposta futura.
- Não alterar código.
- Não alterar documentação oficial sem autorização.
- Não transformar inferência em decisão.

## Entregas

- especificação ou análise com escopo e premissas;
- regras e requisitos identificados;
- critérios de aceitação verificáveis;
- casos-limite e ambiguidades;
- evidências e referências aos arquivos;
- decisões que dependem do responsável;
- handoff de retorno ao Arquiteto de Documentação.
