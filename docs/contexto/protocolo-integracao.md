# Protocolo de integração dos chats especialistas

## Objetivo

Manter os trabalhos de Analista, Desenvolvedor, Testador e Revisor coerentes com o mesmo estado do JackCompiler, evitando decisões conflitantes, alterações não rastreadas e funcionalidades presumidas.

## Fonte de verdade

1. Código, CMake e testes do commit em análise.
2. Decisões explícitas aprovadas pelo responsável.
3. Documentação oficial.
4. Contexto e handoffs.
5. Conversas anteriores.

Nenhum chat deve tratar uma sugestão sua como decisão aprovada.

## Registro obrigatório ao concluir uma atividade

Cada especialista deve entregar ao Arquiteto de Documentação:

- objetivo e escopo;
- branch e commit usados como base;
- arquivos examinados;
- arquivos alterados (ou declaração explícita de nenhum);
- fatos observados e evidências;
- decisões propostas, separadas das aprovadas;
- comandos/testes executados e resultados;
- limitações, riscos e pendências;
- dependências de outros especialistas.

## Mudanças no repositório

- Não realizar commit, tag, release ou push sem autorização explícita.
- Não sobrescrever trabalho de outro especialista.
- Antes de editar, verificar `git status` e o commit atual.
- Mudanças devem ser pequenas e rastreáveis.
- Documentar comandos PowerShell para aplicar e validar mudanças.
- Se o código contradisser a documentação, registrar a divergência antes de editar.
- Não ampliar escopo sem acordo.

## Coordenação entre especialistas

- Analista: produz especificações, regras e critérios; não declara implementação existente sem evidência.
- Desenvolvedor: implementa somente escopo aprovado; registra decisões técnicas e arquivos alterados.
- Testador: deriva casos de teste de requisitos e comportamento acordados; registra ambiente, comandos e resultados.
- Revisor: avalia criticamente alterações e evidências; não altera silenciosamente decisões ou requisitos.
- Arquiteto de Documentação: mantém estado global, integra entregas, resolve conflitos por consulta ao responsável e mantém contexto/handoffs.

## Ciclo de trabalho

1. Especialista lê `docs/contexto/README.md`, `estado-atual.md`, `decisoes-e-pendencias.md` e seu handoff.
2. Confirma o estado real do Git.
3. Executa a atividade delimitada.
4. Entrega relatório de conclusão no formato acima.
5. Arquiteto integra o resultado e atualiza os registros.
6. Documentos oficiais só são atualizados após verificação e autorização aplicável.

## Conflitos

Se dois chats propuserem soluções incompatíveis:
- não escolher por conta própria;
- descrever as alternativas, impactos e evidências;
- encaminhar a decisão ao responsável pelo projeto;
- registrar a decisão aprovada.
