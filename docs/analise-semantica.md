# Análise Semântica & Checagem de Tipos

O **Analisador Semântico** (`SemanticAnalyzerVisitor`) é responsável por validar as regras de contexto e tipos da linguagem Java antes que o código C# seja gerado. Ele percorre a Árvore Sintática Abstrata (AST) utilizando o padrão *Visitor* e faz uso de uma **Tabela de Símbolos com escopos aninhados**.

---

##  | Regras de Checagem & Validação |

### 1. Tabela de Símbolos e Escopos
* **Tabela de Símbolos (`SymbolTable`)**: Mapeia identificadores para suas informações (`SymbolInfo`: tipo, categoria, inicialização e linha).
* **Escopos Aninhados**: Gerenciados via pilha de mapas (`scopes`). Novos escopos são empilhados na entrada de métodos (`FunctionDeclNode`) e blocos condicionais (`IfStmtNode`) e desempilhados na saída.
* **Redefinição de Símbolos**: Variáveis ou parâmetros com o mesmo nome no mesmo escopo resultam em erro semântico imediato.

### 2. Compatibilidade e Promoção de Tipos (*Widening Conversions*)
A atribuição ou retorno de valores permite conversões implícitas de alargamento de tipo conforme a hierarquia abaixo:

$$\text{int} \longrightarrow \text{long} \longrightarrow \text{float} \longrightarrow \text{double}$$

| Tipo Alvo (`target`) | Tipos Aceitos (`source`) |
| :--- | :--- |
| `double` | `double`, `float`, `long`, `int` |
| `float` | `float`, `double` *(literal)*, `long`, `int` |
| `long` | `long`, `int` |
| `int` | `int` |
| `boolean` | `boolean`, `bool` |
| `String` | `String`, `string` |

### 3. Validação de Expressões e Operadores
* **Operadores Aritméticos (`+`, `-`, `*`, `/`)**: Requerem operandos numéricos compatíveis. A operação promove o tipo do resultado para o maior tipo envolvido.
* **Concatenação de Strings (`+`)**: Se qualquer um dos operandos for do tipo `String`, o resultado da operação binária será `String`.
* **Condição de `if`**: A expressão condicional deve obrigatoriamente retornar o tipo `boolean`.

---

## | Mensagens de Erro Semântico |

Quando uma violação semântica é encontrada, a mensagem é emitida no `std::cerr` e a compilação é interrompida antes da fase de codegen (retornando código de saída `1`):

```text
[Erro Semantico] Variavel 'x' ja foi declarada neste escopo.
[Erro Semantico] Variavel 'y' nao declarada.
[Erro Semantico] Atribuicao incompativel para a variavel 'a'. Esperado: 'int', Recebido: 'double'.
[Erro Semantico] A condicao da instrucao 'if' deve ser do tipo boolean. Recebido: 'int'.
```