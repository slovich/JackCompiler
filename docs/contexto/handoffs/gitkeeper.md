# Handoff — GitKeeper

**Projeto:** JackCompiler  
**Repositório:** https://github.com/slovich/JackCompiler  
**Responsabilidade:** versionamento, histórico Git, rastreabilidade e organização de commits  
**Ambiente local informado:** Windows 11 / PowerShell  
**Diretório de trabalho:** `E:\Projetos\Compiladores\compilador`  
**Última atualização:** 22 de novembro de 2026 (após publicação da CLI 0.3)

------------------------------------------------------------------------

## 1. Papel e finalidade

O GitKeeper é o participante responsável exclusivamente pela gestão de
versionamento e controle do histórico do projeto JackCompiler,
utilizando Git e, quando aplicável, GitHub CLI (`gh`).

Suas responsabilidades são:

-   acompanhar branches, commits e remotos;
-   identificar alterações locais ainda não registradas;
-   revisar diffs e a organização dos arquivos para versionamento;
-   orientar a criação de commits atômicos e semanticamente claros;
-   acompanhar merges, conflitos e respectivas resoluções;
-   gerenciar tags e releases mediante autorização;
-   preservar a rastreabilidade entre implementação, testes,
    documentação e histórico;
-   orientar procedimentos de recuperação e preservação do histórico;
-   manter este handoff atualizado ao concluir atividades de
    versionamento.

O GitKeeper não substitui o Desenvolvedor, Testador, Analista, Revisor
ou Arquiteto de Documentação.

## 2. Limites obrigatórios

O GitKeeper não deve:

-   desenvolver ou modificar código de produção;
-   executar testes ou declarar resultados de testes;
-   alterar documentação oficial por iniciativa própria;
-   assumir decisões arquiteturais ou funcionais;
-   executar `add`, `commit`, `push`, `merge`, `reset`, `rebase`,
    `clean`, exclusões de branches, tags ou releases sem autorização
    explícita;
-   executar `fetch` sem explicar previamente seu efeito e obter
    concordância;
-   descartar alterações locais ou executar operações destrutivas sem
    autorização expressa;
-   considerar uma alteração parte do histórico oficial antes de
    confirmar seu registro Git;
-   presumir que o remoto está sincronizado com base apenas nas
    referências locais;
-   afirmar que uma operação foi executada sem evidência real;
-   registrar tokens, senhas, chaves privadas ou outras credenciais
    neste documento.

A autorização para revisar ou organizar alterações não equivale à
autorização para registrá-las no histórico.

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

O comando `git fetch` atualiza referências remotas locais. Deve ser
explicado e autorizado antes da execução.

Antes de um commit, revisar integralmente o conteúdo staged. Antes de um
push, verificar commits a enviar, upstream e situação do remoto. Antes
de merge ou rebase, verificar branches envolvidas, ancestralidade e
riscos.

## 4. Repositório e estado Git confirmado

### 4.1. Identificação

| Item | Informação |
|---|---|
| Repositório | `slovich/JackCompiler` |
| Remoto | `origin` |
| URL | `https://github.com/slovich/JackCompiler.git` |
| Branch principal | `main` |
| Upstream | `origin/main` |
| Diretório local | `E:\Projetos\Compiladores\compilador` |
| Shell | PowerShell |
| Sistema | Windows 11 |

### 4.2. Estado final confirmado em 22/11/2026

Após a publicação dos cinco commits, o responsável forneceu as seguintes
saídas:

``` text
PS E:\Projetos\Compiladores\compilador> git status --short --branch
## main...origin/main

PS E:\Projetos\Compiladores\compilador> git log -1 --oneline
229cfaa (HEAD -> main, origin/main, origin/HEAD) chore: atualizar regras de exclusão do Git
```

Conclusões sustentadas pelas saídas:

-   branch atual: `main`;
-   `HEAD`: `229cfaa`;
-   `origin/main` e `origin/HEAD` apontam para o mesmo commit;
-   working tree limpa;
-   índice sem alterações staged;
-   `main` sincronizada com a referência local de upstream
    `origin/main`.

O push foi realizado pelo responsável e o GitHub confirmou:

``` text
To https://github.com/slovich/JackCompiler.git
   db2c272..229cfaa  main -> main
```

Não foi executado `fetch` neste ciclo. A confirmação da publicação
decorre da resposta do `git push` e da saída posterior de
`git status`/`git log`, não de uma atualização independente por `fetch`.

### 4.3. Cinco commits publicados

Os cinco commits foram criados localmente, revisados por grupos e
publicados em `origin/main`, nesta ordem:

| Ordem | Hash | Mensagem | Arquivos | Inserções | Remoções |
|---:|---|---|---:|---:|---:|
| 1 | `5d05fb7` | `feat: adicionar geração de código VM à CLI` | 10 | 1.652 | 2 |
| 2 | `31ee380` | `test: adicionar testes da tabela de símbolos e dos componentes VM` | 4 | 857 | 0 |
| 3 | `8ecf543` | `docs: atualizar documentação do compilador e da geração VM` | 8 | 1.091 | 475 |
| 4 | `20285b3` | `docs: adicionar contexto do projeto e orientações para agentes` | 12 | 1.779 | 0 |
| 5 | `229cfaa` | `chore: atualizar regras de exclusão do Git` | 1 | 6 | 3 |

Todas as mensagens seguem o padrão Conventional Commits, com descrições
em português do Brasil.

#### Escopo de cada commit

1.  **Implementação da geração VM na CLI:** `CMakeLists.txt`, entrada da
    CLI, interface e implementação `CompiladorVMCLI`, `SymbolTable`,
    `VMWriter` e `VMCodeGenerator`.
2.  **Testes:** `tests/symboltable_test.cpp`,
    `tests/vmcodegenerator_extended_tests.cpp`,
    `tests/vmcodegenerator_tests.cpp` e `tests/vmwriter_tests.cpp`.
3.  **Documentação oficial:** `README.md` e os sete documentos em
    `docs/desenvolvimento/`, `docs/instalacao/`, `docs/tecnico/` e
    `docs/usuario/`.
4.  **Contexto e handoffs:** os 12 arquivos de `docs/contexto/`,
    incluindo README, decisões e pendências, estado atual, protocolo de
    integração e handoffs.
5.  **Regras de exclusão:** `.gitignore`.

O agrupamento foi revisado antes de cada commit. Cada etapa de staging e
cada criação de commit dependeram de autorização explícita do
responsável.

### 4.4. Push e sincronização

O responsável executou:

``` powershell
git push origin main
```

O servidor confirmou a atualização de `main`, de `db2c272` para
`229cfaa`. Em seguida, `git status --short --branch` não indicou commits
à frente ou atrás, e `git log -1 --oneline` mostrou `HEAD`,
`origin/main` e `origin/HEAD` no mesmo hash.

**Estado final: os cinco commits estão publicados e a branch está
sincronizada conforme a verificação fornecida.**

## 5. Histórico de decisões relevantes para versionamento

1.  Todas as pastas de build devem ser ignoradas pelo Git.
2.  `nand2tetris/` contém ferramentas e arquivos locais úteis à
    validação, mas não pertence à árvore versionada.
3.  Versões distribuídas devem permanecer em GitHub Releases, não na
    árvore de fontes.
4.  O README duplicado `docs/contexto/handoffs/README.md` deve ser
    preservado por exigência do Gerenciador Documental, mesmo que seu
    conteúdo seja idêntico ao README de contexto.
5.  `diff_solicitado.txt` foi identificado como arquivo residual vazio e
    sua exclusão foi autorizada pelo responsável.
6.  A revisão geral foi dividida em três etapas: código e build,
    documentação e coerência global.
7.  Não executar testes no âmbito do GitKeeper.
8.  Para o ciclo de versionamento concluído em 22/11/2026, foram
    aprovadas as decisões:
    -   manter os testes independentes, sem integração ao CMake/CTest
        neste ciclo;
    -   versionar integralmente os 12 arquivos de `docs/contexto/`;
    -   separar implementação, testes, documentação oficial,
        contexto/handoffs e `.gitignore` em cinco commits;
    -   preservar as Releases e tags históricas, sem atualizá-las neste
        ciclo.
9.  As mensagens dos commits devem seguir Conventional Commits e usar
    descrições em português do Brasil.
10. A convenção de versões das distribuições é: versões ímpares para a
    CLI e versões pares para a GUI. A CLI segue 0.1, 0.3, 0.5...; a GUI
    segue 0.2, 0.4, 0.6...

### 5.1. `.gitignore`

Conteúdo final informado e incluído no commit `229cfaa`:

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

A alteração de `build/` para `build*/` foi realizada pelo responsável. A
verificação anterior por `git check-ignore -v build-cli nand2tetris`
confirmou que ambas as regras cobrem esses caminhos.

O comentário `#Ambiente Nand2Tetris` foi mantido como estava no arquivo
revisado; não foi objeto de ajuste editorial pelo GitKeeper.

### 5.2. Organização local confirmada antes do commit

-   `diff_solicitado.txt`: exclusão autorizada e anteriormente
    confirmada com `Test-Path = False`.
-   `docs/contexto/handoffs/README.md`: preservado e incluído no commit
    de contexto.
-   `.gitignore`: regra `build*/` e exclusão de `nand2tetris/` incluídas
    no quinto commit.

## 6. Releases e distribuições conhecidos

Foram identificados quatro releases prerelease no GitHub:

| Release | Tag associada | Asset |
|---|---|---|
| JackCompiler — Primeira versão de terminal (CLI) | `Versão_CLI_0.1` | `jack_compiler_cli.zip` |
| JackCompiler — Refatoração arquitetural e desacoplamento | `Refatoração_Arquitetural` | `0.8.zip` |
| `analisador_sintático` | `analisador_sinatatico` | `0.6.zip` |
| `analisador_léxico` | `analisador_léxico` | `0.4.zip` |

As duas tags dos releases mais recentes diferem das sugestões históricas
`cli-v1.0` e `refatoracao-arquitetural`. Não substituir seus nomes por
inferência.

### 6.1. Conteúdo dos pacotes

-   CLI: contém `jack_compiler_cli.exe`.
-   Refatoração arquitetural: contém `0.8/compilador.exe`,
    bibliotecas/runtime Qt, plugins e traduções.
-   Analisador sintático: contém `0.6/compilador.exe`,
    bibliotecas/runtime Qt, plugins e traduções.
-   Analisador léxico: contém `compilador.exe`, bibliotecas/runtime Qt,
    plugins e traduções.

Hashes SHA-256 informados:

| Asset | Tamanho | SHA-256 |
|---|---:|---|
| `jack_compiler_cli.zip` | 233.069 bytes | `0d47cd1987b18f63d5256b325ef9cdafe68f41c28016c695fac1a008319b4664` |
| `0.8.zip` | 27.577.008 bytes | `ad58f9c82fa85845aad6d34cdc7247fac34807472723d47b1d91120d8636bee5` |
| `0.6.zip` | 27.587.050 bytes | `a20b64eccd9200134f52f91dee7fa7929083106af6554c3dd456a8a645458d3f` |
| `0.4.zip` | 28.679.091 bytes | `1482deec646c5299b51023f4e6805850896b437af06e8fd320b7f56e1212db21` |

Os pacotes foram inspecionados por listagem de conteúdo. Essa inspeção
não comprova, por si só, que cada distribuição foi executada em ambiente
limpo.

### 6.2. Convenção de versionamento das distribuições

Em 22/11/2026, o responsável definiu:

- CLI: versões ímpares (0.1, 0.3, 0.5...).
- GUI: versões pares (0.2, 0.4, 0.6...).

A convenção não altera nem renomeia Releases históricas.

### 6.3. Release CLI 0.3 — publicada em 22/11/2026

A nova Release foi criada e verificada pelo responsável:

| Campo | Valor |
|---|---|
| Título | `JackCompiler — CLI 0.3 com geração de código VM` |
| Tag | `Versão_CLI_0.3` |
| Commit-alvo | `229cfaa9537c65659d064552c459e59cd75fdb07` |
| Tipo | Pre-release |
| Asset | `jack_compiler_cli.zip` |
| Tamanho | 432.002 bytes |
| SHA-256 | `38cacc60cf95cd0628c2e2eb65bb9900333e62167666fa451a62a163523d3065` |

O ZIP foi inspecionado localmente e continha somente
`jack_compiler_cli.exe`. O responsável confirmou que o executável foi
gerado a partir do commit `229cfaa`. Uma primeira inspeção encontrou
`jack_cli.exe`; o nome interno foi corrigido antes da publicação.

A tag foi criada localmente e publicada pelo responsável:

```text
git tag Versão_CLI_0.3 229cfaa9537c65659d064552c459e59cd75fdb07
git push origin Versão_CLI_0.3
```

A consulta remota confirmou:

```text
229cfaa9537c65659d064552c459e59cd75fdb07 refs/tags/Versão_CLI_0.3
```

A GitHub Release foi criada como prerelease, com target explícito
`229cfaa`. `gh release view` confirmou título, tag, commit-alvo, tipo,
asset enviado e digest. O digest publicado pelo GitHub coincide com o
SHA-256 calculado localmente.

URL:
https://github.com/slovich/JackCompiler/releases/tag/Vers%C3%A3o_CLI_0.3

A Release CLI 0.1 e suas tags/assets históricos foram preservados.

**Limpeza local pendente de confirmação:** foi orientada a exclusão de
`build-cli/jack_compiler_cli.zip`, mas não foi recebida saída confirmando
a execução nem o resultado de `Test-Path`. Não afirmar que o ZIP local
foi removido até nova verificação.

### 6.4. Situação das tags remotas após a publicação

`git ls-remote --tags origin` mostrou as quatro tags históricas. Após a
publicação, uma consulta específica confirmou também
`Versão_CLI_0.3` no commit `229cfaa`. Não foi executado `fetch`.

## 7. Revisão técnica e documental --- histórico relevante

A revisão geral do conjunto foi concluída antes da preparação dos
commits. O GitKeeper atuou apenas na inspeção de fontes, diffs,
inventário e organização para versionamento; não executou testes nem
alterou código ou documentação oficial.

### 7.1. Código e build

Foram examinados, por meio das fontes fornecidas pelo responsável:

-   `CMakeLists.txt`;
-   entrada da CLI;
-   implementação da CLI VM;
-   `SymbolTable`;
-   `VMWriter`;
-   `VMCodeGenerator`;
-   testes unitários e estendidos desses componentes.

A arquitetura observada foi considerada coerente:

-   `jack_core`: núcleo estático do compilador;
-   `jack_cli`: interface de linha de comando, sem dependência de Qt;
-   `compilador`: aplicação gráfica Qt, condicional ao build GUI.

A geração VM está associada ao fluxo não interativo da CLI. Não há
evidência para declarar que a GUI também oferece essa funcionalidade.

Os testes independentes de `SymbolTable`, `VMWriter` e `VMCodeGenerator`
não estavam integrados ao CMake/CTest examinado. Os quatro arquivos de
teste possuem `main()` próprios e são compatíveis com execução como
programas independentes.

A decisão aprovada para o ciclo foi manter os testes independentes, sem
integração ao CMake/CTest. Isso é uma decisão de organização, não uma
afirmação de que a cobertura seja exaustiva.

### 7.2. Testes e validação --- resultados históricos relatados

Resultados fornecidos pelo usuário:

-   SymbolTable: 8/8 casos aprovados.
-   VMWriter: 14/14 casos aprovados.
-   Build CMake: `jack_core` e `jack_cli` compilados com sucesso.
-   Seis programas oficiais Project 11 compilados, com 11 arquivos VM
    gerados.
-   Seven, Average, ComplexArrays e ConvertToBin aprovados no VM
    Emulator desktop nos cenários descritos.
-   Pong e movimentação de Square aprovados no emulador online oficial,
    conforme relato do desenvolvedor.
-   Redimensionamento de Square também não funcionou com o compilador
    oficial no mesmo emulador online.

A aprovação é delimitada ao escopo testado. Não representa cobertura
exaustiva da linguagem Jack.

O relatório de testes não possui hash de commit associado. Não vincular
seus resultados a um commit específico sem nova evidência.

O GitKeeper não executou testes durante o ciclo de versionamento e não
deve apresentar os resultados acima como execução própria.

### 7.3. Documentação oficial e contexto

Os documentos oficiais revisados foram aprovados nos seguintes escopos:

-   README principal: aprovado;
-   arquitetura: bloco 2, versão revisada aprovada;
-   testes: bloco 3, versão revisada aprovada;
-   linguagem Jack: versão final V3 aprovada após ajustes na seção 5.5 e
    conversão de tabelas;
-   compilação: versão V3 aprovada;
-   manual CLI: versão V3 aprovada, com ressalva documental não
    impeditiva sobre a confirmação de caminho absoluto na opção 9;
-   manual GUI: versão V2 aprovada no escopo atual;
-   documentação sobre uso de IA: aprovada;
-   `docs/contexto/`: registros, decisões, estado, protocolo e handoffs
    revisados e aprovados.

O Gerenciador Documental determinou a preservação de
`docs/contexto/handoffs/README.md`. O arquivo foi mantido e incluído no
quarto commit.

O relatório `relatorio_testes_vmcodegenerator.md` foi classificado como
registro complementar, não como handoff, e sua organização foi refletida
no índice de handoffs.

### 7.4. Whitespace intencional em documentos de contexto

Durante a preparação do quarto commit, `git diff --cached --check`
apontou 14 ocorrências de trailing whitespace em linhas de metadados nos
arquivos:

-   `docs/contexto/handoffs/desenvolvedor.md` --- linhas 3, 4 e 5;
-   `docs/contexto/handoffs/gitkeeper.md` --- linhas 3, 4, 5, 6 e 7;
-   `docs/contexto/handoffs/relatorio_testes_vmcodegenerator.md` ---
    linhas 3, 4 e 5;
-   `docs/contexto/handoffs/testador.md` --- linhas 3, 4 e 5.

O Arquiteto de Documentação emitiu parecer expresso:

1.  preservar os 14 espaços, pois não são erros editoriais;
2.  não solicitar correções aos responsáveis;
3.  não alterar o conteúdo dos quatro arquivos;
4.  manter o staging sem novo `git add`;
5.  aceitar que `git diff --cached --check` aponte as 14 ocorrências.

O staging foi aprovado com essa ressalva e o quarto commit foi criado
sem modificar os documentos. Essas ocorrências são uma exceção
documental intencional, não uma pendência de correção.

O `git diff --check` executado após a publicação retornou sem saída
porque a working tree estava limpa; isso não significa que os espaços
tenham sido removidos dos documentos versionados.

### 7.5. Parecer consolidado

A revisão do conjunto e a preparação do versionamento foram concluídas.
Os cinco commits foram criados e publicados, com separação entre
implementação, testes, documentação oficial, contexto/handoffs e regras
de ignore.

Não há pendência de versionamento identificada no encerramento deste
ciclo. Isso não equivale a uma declaração de ausência de defeitos no
compilador, nem substitui futuras revisões técnicas, testes ou decisões
de produto.

## 8. Estado do checkout e referências após a Release CLI 0.3

### 8.1. Estado local após a atualização deste handoff

Em 22/11/2026, após a substituição do arquivo pela versão revisada, o
responsável forneceu as seguintes evidências:

```text
PS E:\Projetos\Compiladores\compilador> git status --short --branch
## main...origin/main
 M docs/contexto/handoffs/gitkeeper.md

PS E:\Projetos\Compiladores\compilador> git log -1 --oneline --decorate
229cfaa (HEAD -> main, tag: Versão_CLI_0.3, origin/main, origin/HEAD) chore: atualizar regras de exclusão do Git

PS E:\Projetos\Compiladores\compilador> git diff --stat
 docs/contexto/handoffs/gitkeeper.md | 717 +++++++++++++++++++++++-------------
 1 file changed, 455 insertions(+), 262 deletions(-)

PS E:\Projetos\Compiladores\compilador> git diff --cached --stat
[sem saída]

PS E:\Projetos\Compiladores\compilador> git diff --check
[sem saída]
```

Conclusões sustentadas por essas saídas:

- branch atual: `main`;
- `HEAD`: `229cfaa`, também identificado pela tag `Versão_CLI_0.3`;
- `origin/main` e `origin/HEAD` apontam para `229cfaa` nas referências
  locais exibidas;
- somente `docs/contexto/handoffs/gitkeeper.md` aparece modificado;
- não há alterações staged;
- o diff contém 455 inserções e 262 remoções;
- `git diff --check` não apontou erros de whitespace.

O Git emitiu aviso de que LF será convertido para CRLF quando tocar
novamente o arquivo. Esse aviso, isoladamente, não indica corrupção nem
falha na verificação de whitespace.

A publicação da CLI 0.3 apontou explicitamente para `229cfaa`, não para o
conteúdo modificado da working tree. O diff documental ainda não foi
registrado em commit nem publicado.

### 8.2. Referências remotas confirmadas

Após a publicação, `git ls-remote --tags origin "Versão_CLI_0.3"`
retornou:

```text
229cfaa9537c65659d064552c459e59cd75fdb07 refs/tags/Versão_CLI_0.3
```

`gh release view` confirmou o asset e o digest remoto. Não foi executado
`fetch` neste ciclo.

## 9. Pendências e recomendações para um próximo ciclo

### 9.1. Pendências imediatas

1. Confirmar a limpeza local do pacote publicado:
   `build-cli/jack_compiler_cli.zip`. A exclusão foi orientada, mas não
   há confirmação de execução neste registro.
2. Concluir a revisão do diff de `docs/contexto/handoffs/gitkeeper.md`.
   A inspeção mais recente confirmou que esse é o único arquivo
   modificado, sem alterações staged e sem apontamentos de
   `git diff --check`.
3. Solicitar autorização explícita antes de executar `git add` e `git
   commit`. A mensagem prevista é
   `docs: atualizar handoff do GitKeeper após release CLI 0.3`.
4. Antes do staging, obter nova fotografia do estado Git e revisar o
   conteúdo que será incluído no índice.

### 9.2. Decisões já tomadas e que não devem ser reabertas sem novo motivo

-   testes permanecem independentes, sem integração CMake/CTest neste
    ciclo;
-   os 12 arquivos de `docs/contexto/` foram versionados;
-   implementação, testes, documentação oficial, contexto/handoffs e
    `.gitignore` foram separados em cinco commits;
-   Releases e tags históricas foram preservadas;
-   a CLI usa versões ímpares e a GUI versões pares;
-   CLI 0.3 foi publicada como prerelease na tag `Versão_CLI_0.3`;
-   mensagens de commit usam descrições em português do Brasil;
-   os 14 espaços em branco intencionais nos metadados foram
    preservados.

### 9.3. Próxima verificação Git

Antes de qualquer nova operação, solicitar uma nova fotografia do
repositório:

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

Não executar `fetch` automaticamente. Se for necessário confirmar o
estado atual do servidor, explicar previamente o efeito e solicitar
autorização.

Qualquer nova operação de escrita --- `add`, `commit`, `push`, merge,
rebase, reset, alteração de tags ou Releases --- continua dependendo de
autorização explícita.

## 10. Histórico de operações realizadas pelo GitKeeper neste ciclo

O GitKeeper orientou a preparação, conferência e sequência de
versionamento. O responsável executou os comandos no PowerShell e
forneceu as saídas.

Operações confirmadas no ciclo anterior:

- `git add`: staging seletivo dos cinco grupos;
- `git commit`: cinco commits criados após autorização individual;
- `git push origin main`: publicação dos cinco commits;
- comandos de inspeção Git: executados pelo responsável.

Operações confirmadas para a CLI 0.3:

- `git tag Versão_CLI_0.3 229cfaa...`: criação da tag local;
- `git push origin Versão_CLI_0.3`: publicação da tag;
- `gh release create`: criação da Release prerelease, com target
  `229cfaa` e asset `jack_compiler_cli.zip`;
- `gh release view`: confirmação dos metadados e digest do asset;
- `git ls-remote --tags origin "Versão_CLI_0.3"`: confirmação da tag
  remota.

Não foram executados neste ciclo:

- `git fetch`;
- merges ou rebases;
- resets, cleans ou outras operações destrutivas;
- criação ou exclusão de branches;
- exclusão ou atualização de tags/releases existentes;
- testes pelo GitKeeper;
- alterações diretas do GitKeeper nos arquivos do checkout.

A limpeza do ZIP local foi orientada, mas permanece sem confirmação.

## 11. Histórico deste handoff

| Data | Atualização | Conteúdo |
|---|---|---|
| 22/11/2026 | Versão inicial | Preparação do handoff com base no dossiê de transferência e contexto histórico. |
| 22/11/2026 | Atualizações de auditoria | Incorporação das saídas iniciais de Git, branches, remotos, releases, alterações locais, revisão de código/build e auditoria documental. |
| 22/11/2026 | Encerramento do versionamento | Registro dos cinco commits, escopos, hashes, mensagens em português, ressalva dos 14 espaços intencionais, push confirmado e sincronização de `main` com `origin/main`. |
| 22/11/2026 | Publicação da CLI 0.3 | Registro da convenção de versões CLI/GUI, tag `Versão_CLI_0.3`, Release prerelease, asset, digest e pendência de limpeza local. |

------------------------------------------------------------------------

**Regra de integridade:** este handoff é um registro de contexto e
evidências, não uma autorização para modificar o repositório. O estado
real do checkout deve ser novamente verificado antes de qualquer
operação Git. Fatos históricos, resultados relatados, decisões aprovadas
e pendências devem permanecer claramente diferenciados.
