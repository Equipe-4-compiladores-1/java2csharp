# Gramática Suportada

Como o escopo deste transpilador foca no paradigma procedural e estruturado da disciplina de Compiladores, suportamos um subconjunto restrito e direto da linguagem Java. 

## :material-format-list-checks: Estruturas Suportadas

O parser está configurado para reconhecer as seguintes declarações e expressões:

### 1. Tipos de Dados Básicos
O sistema de tipos suporta mapeamento direto para os tipos equivalentes do C#.

| Java | C# gerado | Observação |
| --- | --- | --- |
| `byte` | `byte` / `sbyte` | Inteiro de 8 bits. Variáveis escalares são geradas como `byte`; vetores são gerados como `sbyte[]` para preservar o intervalo com sinal do Java. |
| `short` | `short` | Inteiro de 16 bits. |
| `int` | `int` | Tipo padrão dos literais inteiros (`10`, `42`). |
| `long` | `long` | |
| `float` | `float` | |
| `double` | `double` | Tipo padrão dos literais com ponto (`3.14`). |
| `boolean` | `bool` | Literais `true` e `false`. |
| `char` | `char` | Literais entre aspas simples (`'a'`). |
| `void` | `void` | Apenas como tipo de retorno de métodos. |

```java
byte copiarByte(byte valor) {
    byte copia = valor;
    return copia;
}

int somar(byte a, short b) {
    return a + b; // byte e short são promovidos a int
}
```

!!! note "Diferença entre `byte` e `byte[]` no C# gerado"
    O tipo Java `byte` é representado como `byte` nas declarações escalares e parâmetros atuais do código C# gerado. Já os elementos de vetores usam `sbyte[]`, porque `sbyte` corresponde ao inteiro de 8 bits com sinal do Java (`-128` a `127`), enquanto `byte` corresponde ao intervalo sem sinal (`0` a `255`) no C#.

    Essa conversão vetorial é aplicada pelo codegen em `byte[]`; não representa uma conversão automática adicional dos elementos escalares.

#### Comportamento Semântico de Tipos Vetorizados
Quando um tipo é declarado como vetor unidimensional (`T[]`, como `int[]` ou `double[]`), a **Tabela de Símbolos** registra a variável preservando o modificador de dimensão no identificador de tipo (`"int[]"`, `"double[]"`):

* **Invariância na alocação e atribuição:** Diferente das conversões implícitas de alargamento (*widening*) aplicadas a variáveis escalares (onde um `double` aceita um `int`), tipos vetorizados exigem correspondência exata de tipo base na alocação e atribuição (ex.: `double[] v = new int[5];` é rejeitado pelo analisador semântico).
* **Extração do tipo base na indexação:** Ao ler ou atribuir um elemento via índice (`arr[i]`), o analisador verifica se a variável possui sufixo de dimensão `[]` (rejeitando indexação de variáveis escalares), exige que o índice seja compatível com `int` e resolve o tipo da expressão para o tipo base do elemento (`T`).
* **Restrição de operadores escalares:** Operadores aritméticos (`+`, `-`, `*`, `/`) e de atualização numérica (`++`, `--`, `+=`) não podem ser aplicados diretamente sobre a referência do vetor (`arr++` ou `a + b`). Os elementos podem ser usados em expressões e receber atribuições simples, como `arr[i] = arr[i] + 1`; atualizações indexadas diretas ainda não são suportadas.

!!! note "Tipos `byte` e `short`"
    Variáveis e parâmetros `byte`/`short` podem ser atribuídos a tipos maiores (`short`, `int`, `long`, `float`, `double`), e em expressões aritméticas são promovidos a `int`, como no Java. Como todo literal inteiro é tratado como `int`, uma declaração como `byte b = 1;` é rejeitada pelo analisador semântico — o valor deve vir de outra variável ou parâmetro `byte`.

!!! warning "`String`"
    `String` é reconhecido pelo lexer e pode ser usado em variáveis, parâmetros escalares e vetores unidimensionais. Literais de texto (`"..."`) podem ser usados em expressões e impressões. Entretanto, ao passar uma String como parâmetro de um método, o transpilador ainda não faz todas as verificações realizadas para os tipos básicos.

### 2. Declaração de Classes e Main
O ponto de entrada do programa exige uma classe base e o método estático por padrão.
```java
class Principal {
    public static void main(String[] args) { ... }
}
```

No C# a classe vira uma `public static class` dentro do namespace `TranspiledProgram`, e o `main` passa a ser `public static void Main(string[] args)`.

### 3. Funções e Chamadas de Método

O transpilador suporta a declaração de múltiplas funções procedurais e invocações de métodos estáticos com ou sem parâmetros, tanto operando como expressões ou comandos na mesma linha.

```java
// Declaração
int somar(int a, int b) { ... }
void inicializar() { return; }

// Invocação
int resultado = somar(a, b) + 10;
inicializar();
```

### 4. Impressão no Terminal

Funções nativas de impressão são mapeadas para os métodos da classe `Console` do C#.

* `System.out.print(...)` :material-arrow-right: `Console.Write(...)`
* `System.out.println(...)` :material-arrow-right: `Console.WriteLine(...)`
* `System.out.printf(...)` :material-arrow-right: `Console.Write(...)`, adaptando `%d`, `%f` etc. para os índices de formatação do C# (`{0}`, `{1}`, ...).

Comentários de uma linha (`//`) e de bloco (`/* ... */`) são ignorados pelo lexer e não aparecem no C# gerado.

### 5. Variáveis e Atribuição

Variáveis locais podem ser declaradas com ou sem valor inicial. Quando não recebem um valor na declaração, devem ser inicializadas por atribuição antes do uso.

```java
int contador = 0;
contador = contador + 1;

double resultado;
resultado = 3.5;
```

### 6. Estruturas de Dados

O transpilador oferece suporte a **coleções contíguas em memória** por meio de **vetores unidimensionais (Arrays 1D)** de tipos primitivos (`int[]`, `double[]`, `boolean[]`, `char[]`, etc.) e de `String` (`String[]`).

#### Declaração e Alocação Dinâmica (`new`)
Um vetor pode ser declarado apenas com sua assinatura de tipo (para alocação posterior) ou já inicializado com o operador `new` e uma expressão inteira que define seu tamanho:

```java
// Declaração seguida de alocação
int[] arr;
arr = new int[5];

// Declaração com alocação imediata e tamanho dinâmico
int tamanho = 10;
double[] notas = new double[tamanho];
```

#### Acesso e Modificação por Índice

Os elementos são lidos e modificados utilizando colchetes `[ ]` com qualquer expressão aritmética que resulte em `int`:

```java
arr[0] = 10;
arr[i + 1] = arr[i] * 2;
int valor = arr[0];
```

!!! warning "Limitações Procedurais de Vetores"
Nesta versão, o suporte a coleções em memória possui as seguintes restrições:

* **Apenas uma dimensão (1D):** Matrizes ou arrays multidimensionais (`int[][]`, `new int[2][2]`) não são suportados.
* **Estilo de declaração Java:** Os colchetes devem vir junto ao tipo (`int[] arr`), e não após o identificador (`int arr[]`).
* **Sem inicialização literal em linha:** Expressões como `int[] arr = {1, 2, 3};` não são suportadas; utilize sempre `new tipo[tamanho]`.

### 7. Estruturas de Controle de Fluxo

Cada bloco (`if`, `else`, `while`, `for`) abre um **escopo próprio**: variáveis declaradas dentro dele não são visíveis fora. As condições precisam ser expressões do tipo `boolean`, caso contrário o analisador semântico emite um erro. As chaves `{ }` são obrigatórias em todos os blocos.

#### `if` / `else`

```java
if (nota >= 5) {
    System.out.println("Aprovado");
} else {
    System.out.println("Reprovado");
}
```

O `else` é opcional e os desvios podem ser aninhados dentro dos blocos. A forma encadeada `else if (...)` ainda não é suportada; use um `if` dentro do bloco `else`.

#### `while`

```java
int n = 3;
while (n > 0) {
    System.out.printf("n = %d\n", n);
    n--;
}
```

#### `for`

O cabeçalho aceita uma declaração ou atribuição na inicialização, uma condição e um incremento (`i++`, `i--` ou atribuição). Qualquer uma das três partes pode ser omitida. A variável declarada na inicialização pertence ao escopo do laço.

```java
for (int i = 0; i < 3; i++) {
    System.out.printf("Crescente: %d\n", i);
}

for (int j = 3; j > 0; j--) {
    System.out.printf("Decrescente: %d\n", j);
}
```

#### `break` / `continue`

Os comandos `break` e `continue` são suportados dentro de blocos `while` e `for`:

```java
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue;
    }
    if (i > 7) {
        break;
    }
}
```

`break` encerra o laço atual e `continue` passa imediatamente para a próxima iteração. Ambos são rejeitados pelo analisador semântico quando aparecem fora de um laço.

??? example "Exemplo combinado e código C# gerado"

    === "Java (Entrada)"

        ```java
        class Exemplo {
            public static void main(String[] args) {
                int soma = 0;
                for (int i = 1; i <= 5; i++) {
                    if (i == 3) {
                        System.out.println("Meio do laco");
                    } else {
                        soma = soma + i;
                    }
                }
                int n = 3;
                while (n > 0) {
                    n--;
                }
                System.out.printf("Soma: %d\n", soma);
            }
        }
        ```

    === "C# (Saída Gerada)"

        ```csharp
        using System;

        namespace TranspiledProgram {
            public static class Exemplo {
                public static void Main(string[] args) {
                    int soma = 0;
                    for (int i = 1; i <= 5; i++) {
                        if (i == 3) {
                            Console.WriteLine("Meio do laco");
                        } else {
                            soma = soma + i;
                        }
                    }
                    int n = 3;
                    while (n > 0) {
                        n--;
                    }
                    Console.Write("Soma: {0}\n", soma);
                }

            }
        }
        ```

### 8. Operadores

#### Aritméticos

| Operador | Significado |
| --- | --- |
| `+` | Soma (ou concatenação, quando um dos operandos é texto) |
| `-` | Subtração |
| `-x` | Menos unário (inverte o sinal de `x`) |
| `*` | Multiplicação |
| `/` | Divisão |
| `%` | Resto da divisão |

A precedência padrão é respeitada (`*`, `/` e `%` são avaliados antes de `+` e `-`), e o menos unário tem precedência maior que os operadores binários. Parênteses podem ser usados para alterar a ordem de avaliação. O tipo do resultado é promovido para o maior tipo envolvido (ex.: `int + double` resulta em `double`). Os operandos de `%` e do menos unário devem ser numéricos.

#### Relacionais

| Operador | Significado |
| --- | --- |
| `==` | Igual a |
| `!=` | Diferente de |
| `<` | Menor que |
| `<=` | Menor ou igual a |
| `>` | Maior que |
| `>=` | Maior ou igual a |

Os operandos precisam ter tipos compatíveis (ex.: comparar `int` com `boolean` gera erro semântico) e o resultado é sempre `boolean`, podendo ser usado como condição de `if`, `while` e `for`. Os operadores relacionais têm precedência menor que os aritméticos (`a + 1 > b` equivale a `(a + 1) > b`).

#### Lógicos

| Operador | Significado |
| --- | --- |
| `!` | Negação lógica |
| `&&` | E lógico |
| `||` | Ou lógico |

`!` tem maior precedência que `&&`, e `&&` tem maior precedência que `||`. Os operandos devem ser do tipo `boolean`; o resultado de cada operação também é `boolean`. Parênteses podem ser usados para explicitar ou alterar a ordem:

```java
if (ativo && (!bloqueado || administrador)) {
    System.out.println("Acesso permitido");
}
```

#### Incremento e decremento

| Operador | Significado | Equivalente |
| --- | --- | --- |
| `x++` | Incrementa a variável em 1 | `x = x + 1` |
| `x--` | Decrementa a variável em 1 | `x = x - 1` |
| `++x` | Incrementa a variável em 1 | `x = x + 1` |
| `--x` | Decrementa a variável em 1 | `x = x - 1` |

São aceitos nas formas pós-fixa e pré-fixa, como comandos isolados, na parte de incremento do `for` e dentro de expressões. Só podem ser aplicados a variáveis numéricas já declaradas; usar em `boolean` ou `String` gera erro semântico.

#### Atribuição composta

| Operador | Equivalente | Status |
| --- | --- | --- |
| `x += y` | `x = x + y` | :material-check-all: Suportado |
| `x -= y` | `x = x - y` | :material-check-all: Suportado |
| `x *= y` | `x = x * y` | :material-check-all: Suportado |
| `x /= y` | `x = x / y` | :material-check-all: Suportado |
| `x %= y` | `x = x % y` | :material-check-all: Suportado |

Os operadores de atualização numérica são reconhecidos pelo lexer/parser e aceitam incremento e decremento pré-fixados ou pós-fixados e atribuição composta, tanto como comandos quanto em expressões:

```java
valor++;
--valor;
valor += 2;
valor -= 1;
valor *= 3;
valor /= 2;
valor %= 5;
int anterior = valor++;
int atual = ++valor;
int soma = (valor += 2);
```

Os alvos devem ser variáveis numéricas declaradas, e os valores das atribuições compostas também devem ser numéricos.

!!! warning "Atualizações em elementos de vetores"
    Nesta versão, a sintaxe de atualização numérica aceita o nome da variável diretamente (`valor++`, `valor += 2`, `++valor`). Elementos indexados podem ser lidos e receber atribuições simples, como `arr[i] = arr[i] + 1`, mas a forma direta `arr[i]++` ou `arr[i] += 2` ainda não é suportada pelo parser.

## :material-alert-circle-outline: Limitações Conhecidas

Por se tratar de um transpilador focado no paradigma imperativo, os seguintes recursos do Java **não são suportados**:

* Criação e instância de objetos (`new Objeto()`).
* Herança, interfaces e polimorfismo (`extends`, `implements`).
* Modificadores de acesso granulares em variáveis (`private`, `protected`).
* Parâmetros comuns do tipo `String` ainda não possuem validação completa equivalente à dos tipos primitivos; literais de String e o parâmetro `String[] args` do `main` são tratados pelo transpilador.
* `else if` encadeado, `do-while` e `switch`.
* Arrays multidimensionais (`int[][]`), inicialização literal com chaves (`{1, 2, 3}`) e sintaxe de colchetes após o nome da variável (`int arr[]`).

---
