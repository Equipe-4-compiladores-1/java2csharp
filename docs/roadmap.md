# Roadmap e Evolução do Projeto (Subconjunto Imperativo)

O planejamento do **Java2CSharp** foca na transpilação do subconjunto **imperativo e procedural** do Java para C#, contemplando tipos primitivos, rotinas/funções estáticas, estruturas de controle e gerenciamento de escopos.

---

## Status das  Funcionalidades 

### 1. Estruturas de Controle de Fluxo (`if / else`) — :material-check-all: *Concluído*

* **Objetivo:** Tradução de desvios condicionais procedurais.
* **AST & Parser:** Nó `IfStmtNode` com suporte a blocos `thenBody` e `elseBody`.
* **Análise Semântica:** Validação do tipo booleano na expressão condicional.

### 2. Análise Semântica e Checagem de Tipos — :material-check-all: *Concluído*

* **Objetivo:** Validação de regras imperativas e tipos antes da geração de código C#.
* **Tabela de Símbolos:** Gerenciamento de escopos aninhados para variáveis locais e parâmetros de funções.
* **Checagem de Tipos:** Promoção de tipos numéricos (*widening*), incluindo `byte` e `short`, e prevenção de redefinição de variáveis.

### 3. Laços de Repetição (`while` / `for`) — :material-check-all: *Concluído*

* **Objetivo:** Suportar iterações e laços procedurais.
* **Gramática:**
```java
while (condicao) { ... }
for (int i = 0; i < n; i++) { ... }
```
* **AST:** Nós `WhileStmtNode` e `ForStmtNode` com escopamento próprio de variáveis de controle.
* **Análise Semântica:** Condição do laço validada como `boolean`.

### 4. Operadores de Incremento/Decremento (`++` / `--`) — :material-check-all: *Concluído*

* **Objetivo:** Suportar as formas pós-fixa e pré-fixa como comandos, no incremento do `for` e em expressões.
* **AST:** Nó `NumericUpdateExprNode`.
* **Análise Semântica:** Rejeita o uso em variáveis não declaradas ou do tipo `boolean`/`String`.

### 5. Atribuição Composta (`+=`, `-=`, `*=`, `/=`, `%=`) — :material-check-all: *Concluído*

* **Objetivo:** Reconhecer os operadores de atribuição composta no lexer/parser e traduzi-los para C#.
* **Situação atual:** Suportados para variáveis numéricas e valores numéricos, como comandos ou em expressões.

### 6. Vetores Primitivos Unidimensionais (Arrays 1D) — :material-check-all: *Concluído*

* **Objetivo:** Suportar declaração, alocação dinâmica com `new` e leitura/escrita por índice numérico em coleções 1D.
* **Gramática & AST:** Nós `ArrayDeclNode`, `ArrayAllocNode`, `ArrayAccessNode` e `ArrayAssignNode`.
* **Análise Semântica & Codegen:** Registro de tipos vetorizados (`int[]`, `double[]`, etc.) na Tabela de Símbolos, checagem de compatibilidade na alocação, validação de índices inteiros, bloqueio de indexação em variáveis escalares e tradução direta para C#.

---

## Planejamento de Sprints (Backlog Imperativo)

| Sprint | Foco Principal | Status | Entregas / Issues Alvo |
| --- | --- | --- | --- |
| **Sprint 1** | **Fundação & E/S Procedural** | :material-check-circle: Concluída | • AST básica e padrão Visitor<br>• Tipos primitivos e variáveis locais<br>• Funções e métodos estáticos (`#1`)<br>• E/S básica (`print`, `println`, `printf`) |
| **Sprint 2** | **Desvios & Análise Semântica** | :material-check-circle: Concluída | • Bloco condicional (`if/else`) (`#2`)<br>• Operadores relacionais (`==`, `!=`, `<`, `<=`, `>`, `>=`)<br>• Tabela de Símbolos com escopos aninhados<br>• Checagem e promoção de tipos (`refactor/checagem-tipos`) |
| **Sprint 3** | **Laços de Repetição & Operadores** | :material-check-circle: Concluída | • :material-check: Laços de repetição (`while` e `for`) (`#43`)<br>• :material-check: Operadores de incremento/decremento (`++`, `--`)<br>• :material-check: Tipos primitivos `byte` e `short`<br>• :material-check: Atribuição composta (`+=`, `-=`, `*=`, `/=`, `%=`)<br>• Documentação da sprint (`#38`) |
| **Sprint 4** | **Vetores Primitivos & Ajustes** | :material-check-circle: Concluída | • :material-check: Suporte a arrays 1D de tipos primitivos (ex: `int[] arr`, `new int[n]`, `arr[i]`)<br>• :material-check: Validação semântica e geração de código C# para coleções 1D<br>• :material-check: Suíte de testes e documentação oficial da Sprint 4 |

---
