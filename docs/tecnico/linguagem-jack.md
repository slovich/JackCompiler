# Linguagem Jack: estrutura e regras reconhecidas

## 1. Objetivo e escopo

Este documento apresenta os elementos da linguagem Jack reconhecidos pela implementação atual do **JackCompiler**. Seu objetivo é servir como referência para compreender a estrutura dos arquivos `.jack`, as categorias de tokens, as declarações, as instruções e as expressões aceitas pelo analisador sintático.

A descrição considera o comportamento efetivamente implementado no projeto. Ela não deve ser interpretada como uma especificação completa de todas as etapas de compilação da linguagem: o JackCompiler realiza análise léxica, análise sintática, construção de AST e geração de XML. Não realiza análise semântica nem geração de código para a máquina virtual.

## 2. Estrutura geral de um arquivo

Na implementação atual, cada arquivo `.jack` deve conter **uma única declaração de classe**. A estrutura geral é:

```jack
class NomeDaClasse {
    // declarações de variáveis de classe

    // declarações de sub-rotinas
}
```

A classe começa pela palavra-chave `class`, seguida de um identificador e de um bloco delimitado por chaves. Dentro do bloco podem aparecer declarações de variáveis de classe e declarações de sub-rotinas.

O parser verifica se não há tokens adicionais depois do fechamento da classe. Portanto, embora a AST tenha uma estrutura capaz de armazenar uma coleção de classes, a análise atual de um arquivo aceita uma classe por vez.

## 3. Elementos léxicos

O analisador léxico percorre o código-fonte e classifica as sequências reconhecidas em cinco categorias.

| Categoria | Descrição | Exemplos |
|---|---|---|
| `Keyword` | Palavras reservadas da linguagem | `class`, `function`, `return` |
| `Symbol` | Símbolos e delimitadores | `{`, `}`, `(`, `+`, `;` |
| `IntegerConstant` | Constantes inteiras | `0`, `17`, `32767` |
| `StringConstant` | Constantes de texto entre aspas duplas | `"Olá"` |
| `Identifier` | Nomes definidos no código | `Main`, `contador`, `calcular` |

### 3.1 Palavras-chave

As palavras-chave reconhecidas pela implementação são:

```text
class constructor function method field static var
int char boolean void
true false null this
let do if else while return
```

Essas palavras são classificadas como `Keyword`, e não como identificadores.

### 3.2 Símbolos

Os símbolos reconhecidos são:

```text
{}()[].,;+-*/&|<>=~
```

Eles incluem delimitadores de blocos, parênteses, colchetes, pontuação e operadores utilizados nas expressões e instruções.

### 3.3 Identificadores

Um identificador começa com uma letra ASCII. Os caracteres seguintes podem ser letras ASCII, dígitos ou sublinhado (`_`).

Exemplos aceitos pelo analisador léxico:

```jack
Main
contador
valor2
minha_variavel
```

A classificação como identificador ocorre quando o lexema não corresponde a uma das palavras-chave reconhecidas.

### 3.4 Constantes inteiras

As constantes inteiras são formadas por um ou mais dígitos e devem estar no intervalo de **0 a 32767**, inclusive.

```jack
0
42
32767
```

Um valor fora desse intervalo produz erro léxico.

### 3.5 Constantes de texto

As constantes de texto são delimitadas por aspas duplas:

```jack
"Hello, Jack"
```

O lexema armazenado no token não inclui as aspas delimitadoras. A implementação não aceita quebras de linha dentro da constante e não implementa sequências de escape para representar aspas ou outros caracteres especiais.

### 3.6 Espaços em branco e comentários

Espaços, tabulações e quebras de linha são ignorados na separação dos tokens.

A implementação reconhece duas formas de comentário:

```jack
// comentário de linha

/* comentário
   de bloco */
```

O comentário de linha termina na quebra de linha. O comentário de bloco termina na primeira ocorrência de `*/`. Comentários de bloco não são aninhados.

Um comentário de bloco sem fechamento é reportado como erro léxico.

## 4. Tipos e declarações

A implementação reconhece os tipos primitivos `int`, `char` e `boolean`. Também aceita um identificador como nome de tipo, permitindo declarações que utilizem tipos definidos por classes.

O tipo `void` é aceito como tipo de retorno de sub-rotinas, mas não como tipo de variável ou parâmetro nas regras implementadas.

### 4.1 Declarações de variáveis de classe

As declarações de variáveis de classe utilizam os modificadores `static` ou `field`:

```jack
static int contador;
field boolean ativo;
static int x, y, z;
```

Uma declaração contém o modificador, o tipo, um ou mais nomes separados por vírgula e o ponto e vírgula final.

### 4.2 Declarações de variáveis locais

As variáveis locais são declaradas com a palavra-chave `var`:

```jack
var int quantidade;
var boolean encontrado;
var int x, y;
```

A declaração ocorre no corpo da sub-rotina, antes da sequência de instruções.

### 4.3 Declarações de sub-rotinas

O parser reconhece três categorias de sub-rotinas:

- `constructor`;
- `function`;
- `method`.

A forma geral é:

```jack
function void executar(int limite, boolean ativo) {
    var int contador;

    // instruções
}
```

A declaração contém a categoria da sub-rotina, o tipo de retorno, o identificador, a lista de parâmetros entre parênteses e o corpo delimitado por chaves.

O tipo de retorno pode ser `void`, `int`, `char`, `boolean` ou um identificador que represente um tipo definido pelo usuário.

### 4.4 Lista de parâmetros

A lista de parâmetros pode estar vazia ou conter parâmetros separados por vírgulas. Cada parâmetro é formado por um tipo e um identificador:

```jack
function void iniciar() {
    // sem parâmetros
}

function int somar(int a, int b) {
    // corpo
}
```

Os tipos aceitos para parâmetros são `int`, `char`, `boolean` ou um identificador de tipo.

## 5. Corpo de sub-rotina e instruções

O corpo de uma sub-rotina começa com `{`, pode conter declarações locais e, em seguida, apresenta uma sequência de instruções. A sequência pode ser vazia.

As instruções reconhecidas são `let`, `if`, `while`, `do` e `return`.

### 5.1 Atribuição (`let`)

A instrução `let` atribui o resultado de uma expressão a uma variável ou a uma posição de arranjo:

```jack
let total = 10;
let resultado = a + b;
let valores[i] = 25;
```

A forma geral é:

```jack
let destino = expressão;
```

O destino pode ser um identificador simples ou um acesso a arranjo.

### 5.2 Condicional (`if` e `else`)

A instrução condicional contém uma expressão entre parênteses e um bloco obrigatório para o ramo verdadeiro. O bloco `else` é opcional:

```jack
if (idade > 17) {
    let autorizado = true;
} else {
    let autorizado = false;
}
```

### 5.3 Repetição (`while`)

A instrução `while` contém uma expressão de condição entre parênteses e um bloco de instruções:

```jack
while (contador < limite) {
    let contador = contador + 1;
}
```

### 5.4 Chamada de sub-rotina (`do`)

A instrução `do` executa uma chamada de sub-rotina. A chamada pode ser direta ou qualificada:

```jack
do atualizar();
do Output.printInt(valor);
```

A forma geral da chamada é:

```jack
nomeSubRotina(argumentos)
```

ou:

```jack
qualificador.nomeSubRotina(argumentos)
```

A lista de argumentos pode estar vazia ou conter expressões separadas por vírgulas.

A instrução `do` termina com ponto e vírgula.

### 5.5 Retorno (`return`)

A instrução `return` pode aparecer sem expressão ou seguida de uma expressão:

```jack
return;
return resultado;
```

A análise sintática verifica a estrutura da instrução, mas não verifica se a presença ou ausência de uma expressão é compatível com o tipo de retorno declarado. Essa verificação pertence à análise semântica, que não está implementada no projeto.

## 6. Expressões e termos

Uma expressão é formada por um termo, seguido opcionalmente por uma sequência de operadores binários e termos:

```jack
a + b
a + b * c
(valor - 1) / divisor
```

Os operadores binários reconhecidos são:

| Operador | Representação |
|---|---|
| Símbolo | Identificação |
|---|---|
| `+` | Operador binário |
| `-` | Operador binário |
| `*` | Operador binário |
| `/` | Operador binário |
| `&` | Operador binário |
| `|` | Operador binário |
| `=` | Operador binário |
| `<` | Operador binário |
| `>` | Operador binário |

Nesta etapa, a tabela identifica os símbolos reconhecidos pelo parser, sem atribuir a eles uma semântica de execução. O parser verifica sua ocorrência na estrutura sintática e constrói os nós correspondentes na AST.

As operações binárias são encadeadas na AST da esquerda para a direita, conforme a estrutura da regra de expressão implementada.

### 6.1 Termos reconhecidos

Um termo pode ser:

- uma constante inteira;
- uma constante de texto;
- uma constante de palavra-chave;
- uma variável;
- um acesso a arranjo;
- uma chamada de sub-rotina;
- uma expressão entre parênteses;
- uma expressão com operador unário.

Exemplos:

```jack
42
"texto"
true
null
this
contador
valores[indice]
calcular(a, b)
Objeto.metodo(valor)
(a + b)
-valor
~mascara
```

### 6.2 Constantes de palavra-chave

As constantes de palavra-chave reconhecidas como termos são:

```text
true false null this
```

Elas são representadas na AST por nós específicos de constante de palavra-chave.

### 6.3 Operadores unários

O parser reconhece os operadores unários `-` e `~` antes de outro termo:

```jack
-valor
~mascara
```

A expressão resultante é representada por um nó de expressão unária.

### 6.4 Acesso a arranjos

O acesso a um elemento de arranjo é representado por um identificador seguido de uma expressão entre colchetes:

```jack
valores[indice]
```

A expressão entre colchetes determina o índice utilizado.

### 6.5 Chamadas em expressões

Além das chamadas iniciadas por `do`, o parser reconhece chamadas como termos, permitindo que apareçam em expressões:

```jack
let resultado = calcular(a, b);
let tamanho = Array.length(valores);
```

A chamada pode conter um qualificador antes do ponto ou ser uma chamada direta pelo nome da sub-rotina.

## 7. Resumo da estrutura gramatical

A tabela a seguir resume as construções reconhecidas pelo parser. Ela é uma visão estrutural, não uma gramática formal completa em notação EBNF.

| Construção | Estrutura resumida |
|---|---|
| Classe | `class` + identificador + `{` + declarações + `}` |
| Variável de classe | `static` ou `field` + tipo + nomes + `;` |
| Variável local | `var` + tipo + nomes + `;` |
| Sub-rotina | categoria + tipo de retorno + nome + parâmetros + corpo |
| Parâmetro | tipo + identificador |
| Corpo | `{` + declarações locais + instruções + `}` |
| Atribuição | `let` + destino + `=` + expressão + `;` |
| Condicional | `if` + condição + bloco + `else` opcional |
| Repetição | `while` + condição + bloco |
| Chamada | `do` + chamada + `;` |
| Retorno | `return` + expressão opcional + `;` |
| Expressão | termo + sequência opcional de operador e termo |

## 8. Limites entre análise sintática e semântica

A aceitação de um arquivo pelo parser indica que sua estrutura corresponde às regras sintáticas implementadas. Isso não garante que o programa seja semanticamente válido.

Atualmente, o JackCompiler não verifica, por exemplo:

- se uma variável foi declarada antes de ser utilizada;
- se há declarações duplicadas de nomes;
- se os tipos dos argumentos correspondem aos parâmetros de uma chamada;
- se uma operação é compatível com os tipos dos operandos;
- se o retorno é compatível com o tipo declarado pela sub-rotina;
- se uma chamada referencia uma sub-rotina existente.

Essas verificações exigiriam uma etapa de análise semântica posterior à análise sintática e à construção da AST.

## 9. Relação com a implementação

As regras descritas neste documento são reconhecidas principalmente pelos componentes:

- `core/src/lexer/analisadorlexico.cpp`: identificação de tokens, comentários, constantes, símbolos e erros léxicos;
- `core/src/parser/analisadorsintatico.cpp`: reconhecimento das declarações, instruções, expressões e termos;
- `core/include/jack/ast/`: estruturas utilizadas para representar as construções reconhecidas;
- `core/src/xml/`: geração das representações XML a partir dos tokens e da AST.

Quando houver diferença entre uma descrição genérica da linguagem Jack e o comportamento efetivo do código, este documento prioriza o comportamento da implementação atual.

---

*Documento técnico referente às regras léxicas e sintáticas atualmente reconhecidas pelo JackCompiler.*
