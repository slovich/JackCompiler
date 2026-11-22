# Handoff — GitKeeper

**Projeto:** JackCompiler  
**Repositório:** https://github.com/slovich/JackCompiler  
**Responsabilidade:** versionamento, histórico Git, rastreabilidade e organização de commits  
**Ambiente local informado:** Windows 11 / PowerShell  
**Diretório de trabalho:** `E:\Projetos\Compiladores\compilador`  
**Última atualização:** 22 de novembro de 2026

------------------------------------------------------------------------

## 1. Papel e finalidade

O GitKeeper é o participante responsável exclusivamente pela gestão de versionamento e controle do histórico do projeto JackCompiler, utilizando Git e, quando aplicável, GitHub CLI (`gh`).

Suas responsabilidades são:

- acompanhar branches, commits e remotos;
- identificar alterações locais ainda não registradas;
- revisar diffs e a organização dos arquivos para versionamento;
- orientar a criação de commits atômicos e semanticamente claros;
- acompanhar merges, conflitos e respectivas resoluções;
- gerenciar tags e releases mediante autorização;
- preservar a rastreabilidade entre implementação, testes, documentação e histórico;
- orientar procedimentos de recuperação e preservação do histórico;
- manter este handoff atualizado ao concluir atividades de versionamento.

O GitKeeper não substitui o Desenvolvedor, Testador, Analista, Revisor ou Arquiteto de Documentação.

## 2. Limites obrigatórios

O GitKeeper não deve:

- desenvolver ou modificar código de produção;
- executar testes ou declarar resultados de testes;
- alterar documentação oficial por iniciativa própria;
- assumir decisões arquiteturais ou funcionais;
- executar `add`, `commit`, `push`, `merge`, `reset`, `rebase`, `clean`, exclusões de branches, tags ou releases sem autorização explícita;
- executar `fetch` sem explicar previamente seu efeito e obter concordância;
- descartar alterações locais ou executar operações destrutivas sem autorização expressa;
- considerar uma alteração parte do histórico oficial antes de confirmar seu registro Git;
- presumir que o remoto está sincronizado com base apenas nas referências locais;
- afirmar que uma operação foi executada sem evidência real;
- registrar tokens, senhas, chaves privadas ou outras credenciais neste documento.

A autorização para revisar ou organizar alterações não equivale à autorização para registrá-las no histórico.

## 3. Procedimento obrigatório antes de operações Git

Antes de qualquer operação de versionamento, confirmar:

1.  Diretório e repositório de trabalho.
2.  Branch atual.
3.  Commit atual (`HEAD`).
4.  Estado da working tree e do índice.
5.  Alterações staged, unstaged e arquivos não rastreados.
6.  Relação com o upstream.
7.  Estado do remoto, quando a operação depender dele.
8.  Diffs pertinentes e riscos de perda ou sobrescrita.

Comandos usuais de inspeção:

``` powershell
git status --short --branch
git branch --show-current
git log -1 --oneline --decorate
git diff --stat
git diff --cached --stat
git diff --check
git branch -vv
git remote -v
```

O comando `git fetch` atualiza referências remotas locais. Deve ser explicado e autorizado antes da execução.

Antes de um commit, revisar integralmente o conteúdo staged. Antes de um push, verificar commits a enviar, upstream e situação do remoto. Antes de merge ou rebase, verificar branches envolvidas, ancestralidade e riscos.

## 4. Repositório e histórico conhecido

### 4.1. Identificação

| Item             | Informação                                    |
|------------------|-----------------------------------------------|
| Repositório      | `slovich/JackCompiler`                        |
| Remoto           | `origin`                                      |
| URL              | `https://github.com/slovich/JackCompiler.git` |
| Branch principal | `main`                                        |
| Diretório local  | `E:\Projetos\Compiladores\compilador`         |
| Shell            | PowerShell                                    |
| Sistema          | Windows 11                                    |

### 4.2. Último commit confirmado

O último `HEAD` confirmado pelas saídas de diagnóstico fornecidas em 22/11/2026 é:

``` text
db2c2726526160a158a4a2579f9ebf74df7edefa
docs: restore README and refine build documentation
```

A branch era `main`, com upstream `origin/main`.

O histórico recente também contém:

- `6d4d39b` — `docs: finalize documentation structure`;
- `195a877` — merge de `refactor/token-cpp`;
- `b92a093` — `refactor: separa compilação da CLI da interface Qt`.

A branch local `refactor/token-cpp` e sua referência remota `origin/refactor/token-cpp` foram identificadas.

**Limite de atualidade:** o diagnóstico mais recente de 22/11/2026 confirmou novamente `main...origin/main`, mas não houve `fetch`. O hash de `HEAD` não foi repetido nessa última saída. Assim, `db2c272` continua sendo a última referência de commit confirmada, não uma afirmação independente de que nenhum commit posterior possa existir.

## 5. Histórico de decisões relevantes para versionamento

1.  Todas as pastas de build devem ser ignoradas pelo Git.
2.  `nand2tetris/` contém ferramentas e arquivos locais úteis à validação, mas não pertence à árvore versionada.
3.  Versões distribuídas devem permanecer em GitHub Releases, não na árvore de fontes.
4.  O README duplicado `docs/contexto/handoffs/README.md` deve ser preservado por exigência do Gerenciador Documental, mesmo que seu conteúdo seja idêntico ao README de contexto.
5.  `diff_solicitado.txt` foi identificado como arquivo residual vazio e sua exclusão foi autorizada pelo responsável.
6.  A revisão geral foi dividida em três etapas: código e build, documentação e coerência global.
7.  Não executar testes no âmbito do GitKeeper.

### 5.1. `.gitignore`

Conteúdo relevante da configuração atual informada:

``` gitignore
# Diretórios de compilação e artefatos de build
build*/
cmake-build-*/
out/

# Arquivos locais do Qt Creator
.qtcreator/
*.user
*.user.*

# Arquivos gerados pelo CMake
CMakeFiles/
CMakeCache.txt
cmake_install.cmake
compile_commands.json
Makefile

# Arquivos compilados
*.o
*.obj
*.exe
*.dll
*.so
*.dylib
*.a
*.lib

# Arquivos de depuração
*.pdb
*.ilk

# Arquivos compactados de projeto
*.zip

#Ambiente Nand2Tetris
nand2tetris/
```

A alteração de `build/` para `build*/` foi realizada pelo responsável.

A verificação mais recente confirmou:

``` text
.gitignore:2:build*/    build-cli
.gitignore:36:nand2tetris/      nand2tetris
```

Portanto, `build-cli` e `nand2tetris` estão efetivamente cobertos pelas regras de ignore.

### 5.2. Organização local autorizada

- `diff_solicitado.txt`: exclusão autorizada e anteriormente confirmada com `Test-Path = False`.
- `docs/contexto/handoffs/README.md`: preservação expressamente solicitada e anteriormente confirmada.
- `.gitignore`: regra `build*/` aplicada pelo responsável.

Não há autorização para reverter essas decisões por preferência editorial ou de organização.

## 6. Releases e distribuições conhecidos

Foram identificados quatro releases prerelease no GitHub:

| Release                                                  | Tag associada              | Asset                   |
|----------------------------------------------------------|----------------------------|-------------------------|
| JackCompiler — Primeira versão de terminal (CLI)         | `Versão_CLI_0.1`           | `jack_compiler_cli.zip` |
| JackCompiler — Refatoração arquitetural e desacoplamento | `Refatoração_Arquitetural` | `0.8.zip`               |
| `analisador_sintático`                                   | `analisador_sinatatico`    | `0.6.zip`               |
| `analisador_léxico`                                      | `analisador_léxico`        | `0.4.zip`               |

As duas tags dos releases mais recentes diferem das sugestões históricas `cli-v1.0` e `refatoracao-arquitetural`. Não substituir seus nomes por inferência.

### 6.1. Conteúdo dos pacotes

- CLI: contém `jack_compiler_cli.exe`.
- Refatoração arquitetural: contém `0.8/compilador.exe`, bibliotecas/runtime Qt, plugins e traduções.
- Analisador sintático: contém `0.6/compilador.exe`, bibliotecas/runtime Qt, plugins e traduções.
- Analisador léxico: contém `compilador.exe`, bibliotecas/runtime Qt, plugins e traduções.

Hashes SHA-256 informados:

| Asset                   |          Tamanho | SHA-256                                                            |
|-------------------------|-----------------:|--------------------------------------------------------------------|
| `jack_compiler_cli.zip` |    233.069 bytes | `0d47cd1987b18f63d5256b325ef9cdafe68f41c28016c695fac1a008319b4664` |
| `0.8.zip`               | 27.577.008 bytes | `ad58f9c82fa85845aad6d34cdc7247fac34807472723d47b1d91120d8636bee5` |
| `0.6.zip`               | 27.587.050 bytes | `a20b64eccd9200134f52f91dee7fa7929083106af6554c3dd456a8a645458d3f` |
| `0.4.zip`               | 28.679.091 bytes | `1482deec646c5299b51023f4e6805850896b437af06e8fd320b7f56e1212db21` |

Os pacotes foram inspecionados por listagem de conteúdo. Essa inspeção não comprova, por si só, que cada distribuição foi executada em ambiente limpo.

## 7. Estado do checkout — 22/11/2026

### 7.1. Diagnóstico mais recente

Saída fornecida pelo responsável:

``` text
## main...origin/main
 M .gitignore
 M CMakeLists.txt
 M README.md
 M cli/src/main.cpp
 M docs/desenvolvimento/uso-de-inteligencia-artificial.md
 M docs/instalacao/compilacao.md
 M docs/tecnico/arquitetura.md
 M docs/tecnico/linguagem-jack.md
 M docs/tecnico/testes.md
 M docs/usuario/manual-cli.md
 M docs/usuario/manual-gui.md
?? cli/include/jack/cli/compilador_vm_cli.h
?? cli/src/compilador_vm_cli.cpp
?? core/include/jack/symbol/
?? core/include/jack/vm/
?? core/src/symbol/
?? core/src/vm/
?? docs/contexto/
?? tests/symboltable_test.cpp
?? tests/vmcodegenerator_extended_tests.cpp
?? tests/vmcodegenerator_tests.cpp
?? tests/vmwriter_tests.cpp
```

### 7.2. Contagem e estado das alterações

| Indicador                       |     Resultado |
|---------------------------------|--------------:|
| Arquivos rastreados modificados |            11 |
| Entradas não rastreadas         |            24 |
| Arquivos staged                 |             0 |
| Arquivos unstaged               |            11 |
| Inserções no diff rastreado     |         1.154 |
| Remoções no diff rastreado      |           516 |
| `git diff --check`              | 2 ocorrências |
| `build-cli` ignorado            |           Sim |
| `nand2tetris` ignorado          |           Sim |

O resumo foi obtido com:

``` powershell
git diff --stat
git diff --cached --stat
git diff --check
git ls-files --others --exclude-standard
git check-ignore -v build-cli nand2tetris
```

O `git diff --cached --stat` não apresentou conteúdo, confirmando que não havia alterações staged.

### 7.3. Arquivos não rastreados

A listagem completa fornecida foi:

``` text
cli/include/jack/cli/compilador_vm_cli.h
cli/src/compilador_vm_cli.cpp
core/include/jack/symbol/symboltable.h
core/include/jack/vm/vmcodegenerator.h
core/include/jack/vm/vmwriter.h
core/src/symbol/symboltable.cpp
core/src/vm/vmcodegenerator.cpp
core/src/vm/vmwriter.cpp
docs/contexto/README.md
docs/contexto/decisoes-e-pendencias.md
docs/contexto/estado-atual.md
docs/contexto/handoffs/README.md
docs/contexto/handoffs/analista.md
docs/contexto/handoffs/auditoria-documental.md
docs/contexto/handoffs/desenvolvedor.md
docs/contexto/handoffs/gitkeeper.md
docs/contexto/handoffs/relatorio_testes_vmcodegenerator.md
docs/contexto/handoffs/revisor.md
docs/contexto/handoffs/testador.md
docs/contexto/protocolo-integracao.md
tests/symboltable_test.cpp
tests/vmcodegenerator_extended_tests.cpp
tests/vmcodegenerator_tests.cpp
tests/vmwriter_tests.cpp
```

Os diretórios representam vários arquivos internos. Não esquecer de considerar `docs/contexto/` na futura organização dos commits.

#### Conferência posterior do pacote de arquivos não rastreados

Em conferência posterior, o ZIP fornecido para auditoria continha exatamente 24 arquivos, distribuídos em:

- 8 arquivos de implementação relacionados à CLI VM, `SymbolTable`, `VMWriter` e `VMCodeGenerator`;
- 4 arquivos de testes: `symboltable_test.cpp`, `vmwriter_tests.cpp`, `vmcodegenerator_tests.cpp` e `vmcodegenerator_extended_tests.cpp`;
- 12 documentos de contexto e handoffs.

Os quatro arquivos de teste possuem entradas `main()` independentes, coerentes com executáveis isolados. Isso não comprova execução no checkout atual nem integração ao CMake/CTest. O ZIP apresenta caminhos internos parcialmente achatados; portanto, a associação de cada item ao caminho completo do repositório depende do inventário anterior e não é comprovada apenas pelo pacote.

A conferência confirmou a coerência geral dos conteúdos com a organização já conhecida, sem identificar divergência nova que justifique reabrir a revisão da implementação. Essa conclusão se limita ao conteúdo inspecionado no ZIP.

### 7.4. Whitespace e finais de linha

`git diff --check` identificou:

``` text
docs/tecnico/linguagem-jack.md:54: trailing whitespace.
docs/tecnico/linguagem-jack.md:59: trailing whitespace.
```

São duas ocorrências de espaços ao final de linhas, que devem ser corrigidas pelo responsável antes da preparação final dos commits.

O Git também emitiu avisos LF/CRLF em arquivos CMake, C++ e Markdown. Esses avisos são compatíveis com a conversão de finais de linha no Windows e não indicam, isoladamente, corrupção. Não alterar `.gitattributes` ou configurações globais sem necessidade concreta.

## 8. Revisão geral do conjunto — Etapa 3

A inspeção dos arquivos não rastreados e a revisão geral do conjunto foram concluídas com base nas evidências fornecidas. Permanecem pendentes a atualização de documentos de contexto, a consolidação dos ajustes documentais e as decisões de versionamento. A revisão foi conduzida sem executar testes e sem modificar arquivos do checkout.

### 8.1. Código e build

Foram examinados, por meio das fontes fornecidas pelo responsável:

- `CMakeLists.txt`;
- entrada da CLI;
- implementação da CLI VM;
- `SymbolTable`;
- `VMWriter`;
- `VMCodeGenerator`;
- testes unitários e estendidos desses componentes.

A arquitetura observada é coerente:

- `jack_core`: núcleo estático do compilador;
- `jack_cli`: interface de linha de comando, sem dependência de Qt;
- `compilador`: aplicação gráfica Qt, condicional ao build GUI.

A geração VM está associada ao fluxo não interativo da CLI. Não há evidência para declarar que a GUI também oferece essa funcionalidade.

Os testes independentes de `SymbolTable`, `VMWriter` e `VMCodeGenerator` não estavam integrados ao CMake/CTest examinado. Os quatro arquivos de teste identificados no pacote possuem `main()` próprios e são compatíveis com execução como programas independentes.

Essa ausência é uma pendência de infraestrutura, não um defeito funcional confirmado. A inspeção do ZIP não comprova que esses testes tenham sido executados no checkout atual.

### 8.2. Testes e validação

Resultados históricos fornecidos pelo usuário:

- SymbolTable: 8/8 casos aprovados.
- VMWriter: 14/14 casos aprovados.
- Build CMake: `jack_core` e `jack_cli` compilados com sucesso.
- Seis programas oficiais Project 11 compilados, com 11 arquivos VM gerados.
- Seven, Average, ComplexArrays e ConvertToBin aprovados no VM Emulator desktop nos cenários descritos.
- Pong e movimentação de Square aprovados no emulador online oficial, conforme relato do desenvolvedor.
- Redimensionamento de Square também não funcionou com o compilador oficial no mesmo emulador online.

A aprovação é delimitada ao escopo testado. Não representa cobertura exaustiva da linguagem Jack.

O relatório de testes não possui hash de commit associado. Não vincular seus resultados a um commit específico sem nova evidência.

### 8.3. Documentação

A auditoria documental mais recente, baseada no diff UTF-8 fornecido, concentrou as pendências confirmadas nos seguintes pontos:

1.  Tabelas em formato grid table em `docs/instalacao/compilacao.md` e `docs/tecnico/linguagem-jack.md`, que precisam ser convertidas para formato compatível com GitHub Markdown.
2.  Descrição das dependências Qt em `docs/instalacao/compilacao.md`, que precisa distinguir dependências declaradas no CMake e uso efetivo das bibliotecas.
3.  Dois casos de trailing whitespace em `docs/tecnico/linguagem-jack.md` (linhas 54 e 59 no diagnóstico registrado).

Os números anteriores de 1.171 inserções e 529 remoções foram substituídos pelo diagnóstico posterior de 1.154 inserções e 516 remoções. Não manter como pendências atuais os itens da lista antiga que não foram confirmados na auditoria mais recente. A ausência desses itens na lista atual não substitui a conferência do checkout.

As versões documentais revisadas e aprovadas em etapas anteriores não devem ser consideradas aplicadas ao checkout sem confirmação explícita ou inspeção dos arquivos efetivamente presentes nele. O README duplicado nos handoffs deve ser preservado, conforme decisão expressa do responsável.

O GitKeeper não deve realizar essas correções documentais. O encaminhamento pertence ao Arquiteto de Documentação e ao responsável pelo projeto.

### 8.4. Parecer consolidado

**Aprovação com pendências documentais.**

Não foram identificadas, nas evidências examinadas, contradições graves que invalidem a separação entre núcleo, CLI e GUI ou a integração da geração VM à CLI.

As principais pendências são de precisão documental, formatação e organização do versionamento. Não há recomendação de reabrir a implementação nem de retomar a hipótese `THIS`/`pointer 0` sem nova evidência.

## 9. Pendências para o próximo ciclo de versionamento

### 9.1. Antes de preparar os commits

- [ ] Confirmar a aprovação e a incorporação ao checkout das atualizações de `gitkeeper.md`, `handoffs/README.md` e `analista.md`.
- [ ] Corrigir os dois casos de trailing whitespace.
- [ ] Reexecutar `git diff --check` por solicitação do responsável, sem confundir essa verificação com testes.
- [ ] Atualizar o inventário de alterações após as correções.
- [ ] Revisar o diff completo de cada arquivo rastreado.
- [ ] Revisar o conteúdo dos arquivos não rastreados relevantes.
- [ ] Confirmar que `build-cli` e `nand2tetris` continuam fora do conjunto versionável.
- [ ] Confirmar que `diff_solicitado.txt` permanece ausente.
- [ ] Preservar o README duplicado dos handoffs.
- [ ] Definir o agrupamento lógico dos commits.

### 9.2. Decisões que dependem do responsável

- [ ] Definir se os testes independentes serão integrados ao CMake/CTest ou permanecerão como executáveis independentes.
- [ ] Confirmar o conjunto de documentos de contexto que deverá ser versionado.
- [ ] Confirmar se implementação, testes e documentação serão separados em commits distintos.
- [ ] Definir se será necessário atualizar tags ou releases depois do registro das alterações.

### 9.3. Próxima verificação Git

Antes de propor commits, solicitar novamente:

``` powershell
git status --short --branch
git log -1 --oneline --decorate
git diff --stat
git diff --cached --stat
git diff --check
git ls-files --others --exclude-standard
```

Não executar `fetch` automaticamente. Se for necessário confirmar o estado atual do servidor, explicar previamente o efeito e solicitar autorização.

## 10. Histórico de operações realizadas pelo GitKeeper

Durante as atividades registradas neste handoff:

- não foi executado `git add`;
- não foi executado `git commit`;
- não foi executado `git push`;
- não foi executado `git fetch`;
- não foram criadas ou removidas branches;
- não foram criadas ou removidas tags;
- não foram criados ou publicados releases;
- não foram executados merges, resets, rebases ou operações destrutivas;
- não foram executados testes;
- não foram modificados arquivos do checkout.

As saídas de terminal foram fornecidas pelo responsável. O GitKeeper realizou análise e orientação com base nessas evidências.

## 11. Histórico deste handoff

| Data       | Atualização                                 | Conteúdo                                                                                                                                                                               |
|------------|---------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 22/11/2026 | Versão inicial                              | Preparação do handoff com base no dossiê de transferência e contexto histórico.                                                                                                        |
| 22/11/2026 | Primeira atualização factual                | Incorporação das saídas iniciais de Git, branches, remotos, releases e alterações locais.                                                                                              |
| 22/11/2026 | Segunda atualização                         | Incorporação do diagnóstico mais recente, confirmação das regras de ignore, inventário de 24 entradas não rastreadas, 11 arquivos modificados e duas ocorrências de whitespace.        |
| 22/11/2026 | Revisão global                              | Registro da revisão do conjunto, com inspeção de arquivos não rastreados concluída e pendências documentais e de versionamento ainda abertas.                                          |
| 22/11/2026 | Atualização após auditoria do ZIP e do diff | Atualização da contagem do diff para 1.154 inserções e 516 remoções; registro do inventário de 24 arquivos do ZIP, das limitações de seus caminhos e da lista documental mais recente. |

------------------------------------------------------------------------

**Regra de integridade:** este handoff é um registro de contexto e evidências, não uma autorização para modificar o repositório. O estado real do checkout deve ser novamente verificado antes de qualquer operação Git. Fatos históricos, resultados relatados, decisões aprovadas e pendências devem permanecer claramente diferenciados.
