# Gramática Suportada

Como o escopo deste transpilador foca no paradigma procedural e estruturado da disciplina de Compiladores, suportamos um subconjunto restrito e direto da linguagem Java. 

## :material-format-list-checks: Estruturas Suportadas

O parser está configurado para reconhecer as seguintes declarações e expressões:

### 1. Tipos de Dados Básicos
O sistema de tipos suporta mapeamento direto para os tipos equivalentes do C#.

| Java | C# gerado | Observação |
| --- | --- | --- |
| `byte` | `byte` | Inteiro de 8 bits. |
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

!!! note "Tipos `byte` e `short`"
    Variáveis e parâmetros `byte`/`short` podem ser atribuídos a tipos maiores (`short`, `int`, `long`, `float`, `double`), e em expressões aritméticas são promovidos a `int`, como no Java. Como todo literal inteiro é tratado como `int`, uma declaração como `byte b = 1;` é rejeitada pelo analisador semântico — o valor deve vir de outra variável ou parâmetro `byte`.

!!! warning "`String`"
    `String` é reconhecido pelo lexer e literais de texto (`"..."`) podem ser usados em expressões e impressões, mas ainda **não** é possível declarar variáveis ou parâmetros do tipo `String` (o tipo só aparece na assinatura do `main`).

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

### 5. Variáveis e Atribuição

Variáveis locais devem ser declaradas com valor inicial. Depois de declaradas, podem receber novos valores por atribuição simples.

```java
int contador = 0;
contador = contador + 1;
```

### 6. Estruturas de Controle de Fluxo

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

### 7. Operadores

#### Aritméticos

| Operador | Significado |
| --- | --- |
| `+` | Soma (ou concatenação, quando um dos operandos é texto) |
| `-` | Subtração |
| `*` | Multiplicação |
| `/` | Divisão |

A precedência padrão é respeitada (`*` e `/` são avaliados antes de `+` e `-`) e parênteses podem ser usados para alterar a ordem de avaliação. O tipo do resultado é promovido para o maior tipo envolvido (ex.: `int + double` resulta em `double`).

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

#### Incremento e decremento

| Operador | Significado | Equivalente |
| --- | --- | --- |
| `x++` | Incrementa a variável em 1 | `x = x + 1` |
| `x--` | Decrementa a variável em 1 | `x = x - 1` |

São aceitos na forma **pós-fixa** como comando isolado (`contador++;`) ou na parte de incremento do `for`. Só podem ser aplicados a variáveis numéricas já declaradas; usar em `boolean` ou `String` gera erro semântico. A forma pré-fixa (`++x`) e o uso dentro de expressões (`int y = x++;`) não são suportados.

#### Atribuição composta

| Operador | Equivalente | Status |
| --- | --- | --- |
| `x += y` | `x = x + y` | :material-clock-outline: Planejado |
| `x -= y` | `x = x - y` | :material-clock-outline: Planejado |
| `x *= y` | `x = x * y` | :material-clock-outline: Planejado |
| `x /= y` | `x = x / y` | :material-clock-outline: Planejado |

Os operadores de atribuição composta ainda **não** são reconhecidos pelo lexer/parser. Até sua implementação, utilize a forma expandida (`x = x + y`).
Reconhecimento de operações binárias respeitando a precedência padrão (`*`, `/` avaliados antes de `+`, `-`). Os operadores de atualização numérica aceitam incremento e decremento prefixados ou pós-fixados e atribuição composta, tanto como comandos quanto em expressões:

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

## :material-alert-circle-outline: Limitações Conhecidas

Por se tratar de um transpilador focado no paradigma imperativo, os seguintes recursos do Java **não são suportados**:

* Criação e instância de objetos (`new Objeto()`).
* Herança, interfaces e polimorfismo (`extends`, `implements`).
* Modificadores de acesso granulares em variáveis (`private`, `protected`).
* Variáveis e parâmetros do tipo `String`.
* Operadores lógicos (`&&`, `||`, `!`), módulo (`%`) e menos unário (`-x`, `-1`).
* Atribuição composta (`+=`, `-=`, `*=`, `/=`) e incremento/decremento pré-fixo (`++x`).
* `else if` encadeado, `do-while`, `switch`, `break` e `continue`.
* Declaração de variáveis sem valor inicial (`int x;`).

---
