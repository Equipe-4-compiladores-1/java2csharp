# Transpilador Java :material-arrow-right: C&#35;

<p align="left">
  <img src="https://img.shields.io/badge/Linguagem-C%2B%2B17-blue.svg" />
  <img src="https://img.shields.io/badge/Tools-Flex%20%7C%20Bison-orange.svg" />
  <img src="https://img.shields.io/badge/Build-CMake-green.svg" />
  <img src="https://img.shields.io/badge/Disciplina-Compiladores%201-purple.svg" />
</p>

O **Java2CSharp** é um transpilador desenvolvido para converter um subconjunto **procedural/imperativo** da linguagem **Java** diretamente para código **C#** válido e executável.

!!! info "Projeto Acadêmico"
    Este projeto foi construído para a disciplina de **Compiladores 1**. O foco é aplicar os conceitos de análise léxica, análise sintática, construção de AST e padrão Visitor para geração de código.

---

## :material-bullseye: Objetivos do Projeto

<div class="grid cards" markdown>

-   :material-swap-horizontal: **Tradução Procedural**

    ---

    Mapeamento direto de variáveis, tipos primitivos, coleções em memória (Arrays 1D), expressões aritméticas/relacionais, desvios, laços e funções.

-   :material-cube-outline: **Encapsulamento C#**

    ---

    Geração automática de uma classe wrapper estática (`public static class Program`) para adaptar funções Java ao C#.

-   :material-sitemap: **AST Robusta**

    ---

    Estrutura de dados em C++ baseada em **Visitor Pattern**, mantendo o parsing totalmente desacoplado da geração de código.

-   :material-shield-check: **Validação Semântica**

    ---

    Tabela de Símbolos com escopos aninhados e checagem de tipos (escalares e vetorizados) antes da geração do código C#.

</div>

---

## :material-check-decagram: Capacidades Atuais

* **Tipos primitivos:** `byte`, `short`, `int`, `long`, `float`, `double`, `boolean` e `char`, com promoção implícita de tipos numéricos (*widening*).
* **Coleções de dados em memória (Arrays 1D):** declaração de vetores unidimensionais (`int[] arr`), alocação dinâmica com `new` (`new int[tamanho]`) e leitura/modificação por índices numéricos (`arr[i]`), integrados de forma nativa às iterações conjuntas com laços `for` e `while`.
* **Funções estáticas:** declaração de múltiplos métodos com parâmetros e retorno, e chamadas como comando ou dentro de expressões.
* **Desvios condicionais:** `if` / `else`, inclusive aninhados.
* **Laços de repetição:** `while` e `for` (com declaração da variável de controle no cabeçalho e percurso sequencial sobre vetores).
* **Operadores:** aritméticos (`+`, `-`, `*`, `/`, `%`) e menos unário (`-x`), relacionais (`==`, `!=`, `<`, `<=`, `>`, `>=`), lógicos (`&&`, `||`, `!`), atribuição composta (`+=`, `-=`, `*=`, `/=`, `%=`) e incremento/decremento (`++`, `--`).
* **Escopos de bloco:** cada método, bloco `if`/`else`, `while` e `for` abre um escopo próprio; variáveis declaradas dentro dele não são visíveis fora.
* **Validação semântica de tipos:** variáveis não declaradas ou redefinidas, atribuições e retornos incompatíveis, alocações de array com tipos divergentes, indexação de variáveis escalares e condições não booleanas são rejeitadas antes da geração de código.
* **Entrada e saída:** `System.out.print`, `println` e `printf` mapeados para `Console.Write` / `Console.WriteLine`.

Detalhes de cada construção estão em **[Gramática & Tokens](gramatica.md)** e as regras de validação em **[Análise Semântica](analise-semantica.md)**.

---

## :material-code-json: Exemplo de Transpilação

Veja abaixo a conversão que o compilador realiza:

=== "Java (Entrada)"

    ```java
    class Program {
        public static void main(String[] args) {
            int resultado = somar(10, 20);
        }

        int somar(int a, int b) {
            return a + b;
        }
    }
    ```

=== "C# (Saída Gerada)"

    ```csharp
    using System;

    namespace TranspiledProgram {
        public static class Program {
            public static void Main(string[] args) {
                int resultado = somar(10, 20);
            }

            public static int somar(int a, int b) {
                return a + b;
            }

        }
    }
    ```

---

## :material-rocket-launch: Próximos Passos

??? tip "Como testar localmente?"
    Consulte nosso **[Guia de Compilação](guia-de-uso.md)** para aprender a compilar o projeto com o CMake e rodar os testes automatizados.

---
