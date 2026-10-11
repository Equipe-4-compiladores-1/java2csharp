# Roadmap e Evolução do Projeto (Subconjunto Imperativo)

O planejamento do **Java2CSharp** foca na transpilação do subconjunto **imperativo e procedural** do Java para C#, contemplando tipos primitivos, rotinas/funções estáticas, estruturas de controle e gerenciamento de escopos.

---

## | Status das  Funcionalidades |

### 1. Estruturas de Controle de Fluxo (`if / else`) — :material-check-all: *Concluído*

* **Objetivo:** Tradução de desvios condicionais procedurais.
* **AST & Parser:** Nó `IfStmtNode` com suporte a blocos `thenBody` e `elseBody`.
* **Análise Semântica:** Validação do tipo booleano na expressão condicional.

### 2. Análise Semântica e Checagem de Tipos — :material-check-all: *Concluído*

* **Objetivo:** Validação de regras imperativas e tipos antes da geração de código C#.
* **Tabela de Símbolos:** Gerenciamento de escopos aninhados para variáveis locais e parâmetros de funções.
* **Checagem de Tipos:** Promoção de tipos numéricos (*widening*) e prevenção de redefinição de variáveis.

### 3. Laços de Repetição (`while` / `for`) — :material-clock-outline: *Próxima Etapa*

* **Objetivo:** Suportar iterações e laços procedurais.
* **Gramática Alvo:**
```java
while (condicao) { ... }
for (int i = 0; i < n; i++) { ... }

```


* **Impacto na AST:** Nós `WhileStmtNode` e `ForStmtNode` com escopamento próprio de variáveis de controle.

---

## | Planejamento de Sprints (Backlog Imperativo) |

| Sprint | Foco Principal | Status | Entregas / Issues Alvo |
| --- | --- | --- | --- |
| **Sprint 1** | **Fundação & E/S Procedural** | :material-check-circle: Concluída | • AST básica e padrão Visitor<br>
<br>• Tipos primitivos e variáveis locais<br>
<br>• Funções e métodos estáticos (`#1`)<br>
<br>• E/S básica (`print`, `println`, `printf`) |
| **Sprint 2** | **Desvios & Análise Semântica** | :material-progress-clock: **Em Andamento** | • Bloco condicional (`if/else`) (`#2`)<br>
<br>• Tabela de Símbolos com escopos aninhados<br>
<br>• Checagem e promoção de tipos (`refactor/checagem-tipos`) |
| **Sprint 3** | **Laços de Repetição & Operadores** | :material-clock-outline: Planejada | • Laços de repetição (`while` e `for`)<br>
<br>• Operadores de incremento/decremento (`++`, `--`) e atribuição composta (`+=`, `-=`) |
| **Sprint 4** | **Vetores Primitivos & Ajustes** | :material-clock-outline: Futura | • Suporte a arrays 1D de tipos primitivos (ex: `int[] arr`)<br>
<br>• Otimização e limpeza do código C# gerado |

---