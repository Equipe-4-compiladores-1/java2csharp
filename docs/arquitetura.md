# Arquitetura do Projeto

O **Java2CSharp** segue a arquitetura clássica de compiladores de back-end, processando o código-fonte em etapas sequenciais muito bem definidas. O foco é manter o desacoplamento absoluto entre as regras gramaticais e a geração do código C#.

## :material-layers: Pipeline de Compilação

O fluxo de dados desde a leitura do arquivo `.java` até a geração do `.cs` ocorre em seis fases principais:

1. **Análise Léxica (Lex / Flex):** Lê a string de texto puro do arquivo fonte e agrupa os caracteres em unidades de significado chamadas de *Tokens* (ex: `IDENTIFIER`, `INT_LITERAL`, `TYPE_INT`).
2. **Análise Sintática (Yacc / Bison):** Consome os tokens gerados pelo Lexer e valida se a sequência forma sentenças válidas segundo as regras da gramática do nosso subconjunto Java.
3. **Construção da AST:** Durante a validação sintática, o Bison instancia nós C++ em memória (`ExprNode`, `StmtNode`, `ClassDeclNode`), formando a Árvore Sintática Abstrata (AST) que representa o programa.
4. **Análise Semântica:** O `SemanticAnalyzerVisitor` percorre a AST usando a Tabela de Símbolos para validar escopos e tipos. Se algum erro for encontrado, a compilação é interrompida antes da otimização e da geração de código.
5. **Otimização da AST:** Após a análise semântica, o passe `foldConstants` avalia expressões aritméticas, relacionais e lógicas compostas por constantes. Chamadas de método não são executadas e operações inválidas, como divisão por zero, não são calculadas.
6. **Geração de Código (Visitor Pattern):** A árvore otimizada é percorrida por um `CSharpCodegenVisitor`. Cada nó visitado emite sua própria string C# formatada, compondo o arquivo final.

## :material-file-tree: Árvore Sintática Abstrata (AST)

A AST é o coração do transpilador. Em vez de emitir código C# imediatamente assim que uma regra é reconhecida no Bison, criamos nós hierárquicos em C++17. 

Nós principais implementados em `ast.h`:
* **Declarações:** `ClassDeclNode`, `FunctionDeclNode`, `VarDeclNode`, `ArrayDeclNode`.
* **Alocação e acesso:** `ArrayAllocNode`, `ArrayAccessNode`, `ArrayAssignNode`.
* **Comandos (Statements):** `PrintStmtNode`, `PrintfStmtNode`, `ReturnStmtNode`, `MethodCallStmtNode`, `AssignStmtNode`, `UnaryStmtNode`, `BreakStmtNode`, `ContinueStmtNode`.
* **Controle de Fluxo:** `IfStmtNode`, `WhileStmtNode`, `ForStmtNode`.
* **Expressões (Expressions):** `BinaryExprNode`, `UnaryExprNode`, `ParenthesizedExprNode`, `LiteralNode`, `StringNode`, `MethodCallExprNode`, `NumericUpdateExprNode`.

!!! info "Por que usar o Padrão Visitor?"
    O padrão Visitor (`ASTVisitor`) permite adicionar operações sobre a AST sem alterar suas classes. A geração de C# fica em `codegen.cpp` e a validação semântica em `semantic_analyzer.cpp`. A otimização é um passe separado, implementado em `optimizer.cpp`;

---
