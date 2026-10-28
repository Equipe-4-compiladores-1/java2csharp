# Análise Semântica & Checagem de Tipos

O **Analisador Semântico** (`SemanticAnalyzerVisitor`) é responsável por validar as regras de contexto e tipos da linguagem Java antes que o código C# seja gerado. Ele percorre a Árvore Sintática Abstrata (AST) utilizando o padrão *Visitor* e faz uso de uma **Tabela de Símbolos com escopos aninhados**.

---

##  | Regras de Checagem & Validação |

### 1. Tabela de Símbolos e Escopos
* **Tabela de Símbolos (`SymbolTable`)**: Mapeia identificadores para suas informações (`SymbolInfo`: tipo, categoria, inicialização e linha).
* **Escopos Aninhados**: Gerenciados via pilha de mapas (`scopes`). Novos escopos são empilhados na entrada de métodos (`FunctionDeclNode`), blocos condicionais (`IfStmtNode`, um escopo para o `then` e outro para o `else`) e laços (`WhileStmtNode`, `ForStmtNode`) e desempilhados na saída. No `for`, a variável declarada na inicialização pertence ao escopo do laço.
* **Redefinição de Símbolos**: Variáveis ou parâmetros com o mesmo nome no mesmo escopo resultam em erro semântico imediato.

### 2. Compatibilidade e Promoção de Tipos (*Widening Conversions*)
A atribuição ou retorno de valores permite conversões implícitas de alargamento de tipo conforme a hierarquia abaixo:

$$\text{byte} \longrightarrow \text{short} \longrightarrow \text{int} \longrightarrow \text{long} \longrightarrow \text{float} \longrightarrow \text{double}$$

| Tipo Alvo (`target`) | Tipos Aceitos (`source`) |
| :--- | :--- |
| `byte` | `byte` |
| `short` | `short`, `byte` |
| `int` | `int`, `short`, `byte` |
| `long` | `long`, `int`, `short`, `byte` |
| `double` | `double`, `float`, `long`, `int`, `short`, `byte` |
| `float` | `float`, `double` *(literal)*, `long`, `int`, `short`, `byte` |
| `boolean` | `boolean`, `bool` |
| `String` | `String`, `string` |

Em expressões aritméticas, `byte` e `short` são promovidos a `int`, como em Java.

### 3. Validação de Expressões e Operadores
* **Operadores Aritméticos (`+`, `-`, `*`, `/`)**: Requerem operandos numéricos compatíveis. A operação promove o tipo do resultado para o maior tipo envolvido.
* **Concatenação de Strings (`+`)**: Se qualquer um dos operandos for do tipo `String`, o resultado da operação binária será `String`.
* **Operadores Relacionais (`==`, `!=`, `<`, `<=`, `>`, `>=`)**: Requerem operandos de tipos compatíveis e resultam em `boolean`.
* **Condições de `if`, `while` e `for`**: A expressão condicional deve obrigatoriamente retornar o tipo `boolean`.
* **Incremento/Decremento (`++`, `--`)**: A variável deve estar declarada e não pode ser do tipo `boolean` ou `String`.
* **Chamadas e Retornos**: Métodos chamados devem estar declarados na classe; o valor retornado deve ser compatível com o tipo de retorno do método, e métodos `void` não podem retornar valor.

---

## | Mensagens de Erro Semântico |

Quando uma violação semântica é encontrada, a mensagem é emitida no `std::cerr` e a compilação é interrompida antes da fase de codegen (retornando código de saída `1`):

```text
[Erro Semantico] Variavel 'x' ja foi declarada neste escopo.
[Erro Semantico] Variavel 'y' nao declarada.
[Erro Semantico] Atribuicao incompativel para a variavel 'a'. Esperado: 'int', Recebido: 'double'.
[Erro Semantico] A condicao da instrucao 'if' deve ser do tipo boolean. Recebido: 'int'.
[Erro Semantico] A condicao do 'while' deve ser do tipo boolean. Recebido: 'int'.
[Erro Semantico] Operacao '+' invalida entre os tipos 'int' e 'boolean'.
[Erro Semantico] Operacao '++' invalida para a variavel 'b' do tipo 'boolean'.
[Erro Semantico] Chamada a metodo nao declarado 'calcular'.
```