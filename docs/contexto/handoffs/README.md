# Handoffs dos especialistas

Esta pasta reúne os documentos de transferência que definem o ponto de partida, as responsabilidades, os limites de atuação e as entregas esperadas de cada frente especializada do JackCompiler.

O README principal, na raiz do repositório, apresenta publicamente o projeto. O [`README de contexto`](../README.md) explica a organização dos registros operacionais. Este arquivo trata especificamente do uso dos handoffs.

## Como iniciar uma frente de trabalho

Antes de executar qualquer atividade, o especialista deve consultar, nesta ordem:

1. [`../README.md`](../README.md) — orientação geral do contexto operacional.
2. [`../estado-atual.md`](../estado-atual.md) — situação técnica registrada e sua referência.
3. [`../decisoes-e-pendencias.md`](../decisoes-e-pendencias.md) — decisões confirmadas, pendências e divergências.
4. [`../protocolo-integracao.md`](../protocolo-integracao.md) — regras de coordenação e integração.
5. O handoff específico da sua frente.

O handoff define o escopo inicial, mas não substitui a verificação do estado atual do repositório. Se houver indícios de que o repositório avançou além da referência registrada, o especialista deve identificar as diferenças relevantes antes de agir.

## Handoffs disponíveis

| Documento | Frente |
|---|---|
| [`analista.md`](analista.md) | Análise e avaliação técnica. |
| [`auditoria-documental.md`](auditoria-documental.md) | Auditoria de consistência, precisão e organização documental. |
| [`desenvolvedor.md`](desenvolvedor.md) | Implementação e evolução do código. |
| [`gitkeeper.md`](gitkeeper.md) | Inspeção do estado Git, organização de alterações e preparação de commits, respeitando as autorizações recebidas. |
| [`revisor.md`](revisor.md) | Revisão técnica e avaliação de alterações. |
| [`testador.md`](testador.md) | Planejamento, execução e registro de testes. |
| [`ux-designer.md`](ux-designer.md) | Estudo e especificação da experiência de uso da interface gráfica, incluindo organização visual, navegação, fluxos de interação e feedback ao usuário. |

A descrição da tabela indica a finalidade geral de cada handoff; os limites e as instruções específicos são os definidos no respectivo documento.

## Registros complementares

Os documentos abaixo apoiam a consulta e a rastreabilidade das atividades, mas não são handoffs e não definem responsabilidades de especialistas.

| Documento | Finalidade |
|---|---|
| [`relatorio_testes_vmcodegenerator.md`](relatorio_testes_vmcodegenerator.md) | Registro dos testes e da validação do VMCodeGenerator, com resultados, proveniência das evidências e limitações do escopo validado. |

## Entrega ao concluir

Ao encerrar uma atividade, o especialista deve entregar ao Arquiteto de Documentação um relatório que identifique, conforme aplicável:

- objetivo e escopo efetivamente executados;
- arquivos examinados ou alterados;
- evidências e procedimentos utilizados;
- testes executados e respectivos resultados;
- decisões confirmadas ou propostas;
- limitações, divergências e pendências;
- ações que dependem de autorização ou de outra frente.

Não declarar como concluída uma ação que não tenha sido efetivamente executada e verificada. Não transformar hipóteses em fatos nem ampliar o escopo definido pelo handoff sem alinhamento.

## Manutenção desta pasta

Os handoffs devem permanecer coerentes com o estado e as decisões documentados em `docs/contexto/`. Quando uma instrução ficar desatualizada, registrar a divergência e propor sua revisão; não alterar silenciosamente decisões ou responsabilidades.
