# Gramática Suportada

Como o escopo deste transpilador foca no paradigma procedural e estruturado da disciplina de Compiladores, suportamos um subconjunto restrito e direto da linguagem Java. 

## :material-format-list-checks: Estruturas Suportadas

O parser está configurado para reconhecer as seguintes declarações e expressões:

### 1. Tipos de Dados Básicos
O sistema de tipos suporta mapeamento direto.
* `byte`, `short`, `int`, `long`, `float`, `double`, `boolean`, `char`, `void`, `String`

### 2. Declaração de Classes e Main
O ponto de entrada do programa exige uma classe base e o método estático por padrão.
```java
class Principal {
    public static void main(String[] args) { ... }
}
```

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
* `System.out.printf(...)` :material-arrow-right: Implementação que adapta `%d`, `%f` etc. para formatação de índices do C# como `{0}`, `{1}`.

### 5. Operações Matemáticas

Reconhecimento de operações binárias respeitando a precedência padrão (`*`, `/` avaliados antes de `+`, `-`).

## :material-alert-circle-outline: Limitações Conhecidas

Por se tratar de um transpilador focado no paradigma imperativo, os seguintes recursos do Java **não são suportados**:

* Criação e instância de objetos (`new Objeto()`).
* Herança, interfaces e polimorfismo (`extends`, `implements`).
* Modificadores de acesso granulares em variáveis (`private`, `protected`).

---