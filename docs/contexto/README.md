# Contexto operacional — JackCompiler

Esta pasta reúne os registros necessários à continuidade, coordenação e transferência do contexto do projeto JackCompiler entre conversas e agentes especialistas.

O documento de apresentação pública do projeto está em [`README.md`](../../README.md), na raiz do repositório. Este README tem finalidade diferente: orientar a consulta aos registros operacionais e estabelecer como eles devem ser interpretados.

## Regra de precedência

Em caso de divergência, considerar as fontes nesta ordem:

1. Código, configuração de build e testes efetivamente presentes no repositório.
2. Decisões explícitas registradas e aprovadas.
3. Documentação oficial do projeto e README principal.
4. Registros de contexto e handoffs.
5. Histórico de conversas e inferências.

Uma divergência não deve ser corrigida silenciosamente. Deve ser registrada, acompanhada da evidência disponível e encaminhada para decisão quando necessário.

## Documentos desta pasta

| Documento | Finalidade |
|---|---|
| [`estado-atual.md`](estado-atual.md) | Fotografia técnica do projeto, vinculada à referência de repositório indicada no próprio documento. |
| [`decisoes-e-pendencias.md`](decisoes-e-pendencias.md) | Registro de decisões confirmadas, pendências e divergências conhecidas. |
| [`protocolo-integracao.md`](protocolo-integracao.md) | Regras de comunicação, coordenação e integração entre as frentes de trabalho. |
| [`handoffs/`](handoffs/) | Instruções iniciais e limites de atuação dos especialistas. |

## Documentação oficial

A documentação oficial permanece em seus locais próprios. Os registros desta pasta complementam esses documentos, mas não os substituem.

- [`README.md`](../../README.md) — apresentação pública do projeto.
- [`docs/desenvolvimento/uso-de-inteligencia-artificial.md`](../desenvolvimento/uso-de-inteligencia-artificial.md)
- [`docs/instalacao/compilacao.md`](../instalacao/compilacao.md)
- [`docs/tecnico/arquitetura.md`](../tecnico/arquitetura.md)
- [`docs/tecnico/linguagem-jack.md`](../tecnico/linguagem-jack.md)
- [`docs/tecnico/testes.md`](../tecnico/testes.md)
- [`docs/usuario/manual-cli.md`](../usuario/manual-cli.md)
- [`docs/usuario/manual-gui.md`](../usuario/manual-gui.md)

Não mover, substituir ou reestruturar os documentos oficiais apenas em razão da existência desta pasta de contexto.

## Atualização dos registros

Ao atualizar documentos de contexto, identificar, sempre que aplicável:

- commit e branch de referência;
- evidências examinadas;
- arquivos afetados;
- decisões novas ou alteradas;
- pendências remanescentes.

Não apresentar uma informação como verificada se ela decorrer apenas de relato, hipótese ou histórico de conversa. Distinguir explicitamente esses níveis de evidência.

Alterações nos documentos oficiais, commits, releases ou operações no repositório dependem de autorização explícita do responsável pelo projeto.
