#include "codegen.h"
#include <sstream>
#include <iostream>

// =========================================================================
// 1. ESTRUTURA DO PROGRAMA (Top-Level)
// =========================================================================

namespace {
std::string toCSharpType(const std::string& javaType) {
    if (javaType == "boolean") return "bool";
    if (javaType == "byte") return "sbyte";
    return javaType;
}
}

void CSharpCodegenVisitor::generateProgram(const std::shared_ptr<ClassDeclNode>& root) {
    if (!root) return;

    std::cout << "using System;\n\n";
    std::cout << "namespace TranspiledProgram {\n";

    indentLevel = 1; // Entramos no namespace, nível 1 para a classe
    root->accept(this);

    std::cout << "}\n";
}

void CSharpCodegenVisitor::visit(ClassDeclNode* node) {
    printIndent();
    std::cout << "public static class " << node->className << " {\n";

    indentLevel++; // Entramos na classe, nível 2 para as funções
    for (const auto& func : node->functions) {
        func->accept(this);
    }
    indentLevel--;

    printIndent();
    std::cout << "}\n";
}

void CSharpCodegenVisitor::visit(FunctionDeclNode* node) {
    printIndent();
    
    // Se o nome no Java for "main", mudamos para "Main" no C#. Senão, mantemos o original.
    std::string methodName = (node->name == "main") ? "Main" : node->name;

    std::cout << "public static " << node->returnType << " " << methodName << "(";
    
    // Tratamento especial para os parâmetros do main
    if (node->name == "main") {
        std::cout << "string[] args";
    } else {
        // Imprime a lista de parâmetros dinâmica
        for (size_t i = 0; i < node->params.size(); ++i) {
            std::cout << node->params[i].type << " " << node->params[i].name;
            if (i < node->params.size() - 1) {
                std::cout << ", ";
            }
        }
    }
    
    std::cout << ") {\n";

    indentLevel++; // Entramos na função, nível 3+ para os comandos
    for (auto stmt : node->body) {
        if (stmt) {
            stmt->accept(this);
        }
    }
    indentLevel--;

    printIndent();
    std::cout << "}\n\n";
}

// =========================================================================
// 2. COMANDOS (Statements)
// =========================================================================

void CSharpCodegenVisitor::visit(VarDeclNode* node) {
    if (!inForHeader) printIndent(); 
    
    std::string type = (node->type == "boolean") ? "bool" : node->type;
    std::cout << type << " " << node->name << " = ";
    
    if (node->initExpr) node->initExpr->accept(this);
    
    if (!inForHeader) std::cout << ";\n";
}

void CSharpCodegenVisitor::visit(AssignStmtNode* node) {
    if (!inForHeader) printIndent(); 
    
    std::cout << node->identifier << " = ";
    if (node->expr) node->expr->accept(this);
    
    if (!inForHeader) std::cout << ";\n";
}

void CSharpCodegenVisitor::visit(ReturnStmtNode* node) {
    printIndent();
    std::cout << "return ";
    if (node->expr) {
        node->expr->accept(this);
    }
    std::cout << ";\n";
}

void CSharpCodegenVisitor::visit(IfStmtNode* node) {
    printIndent();
    std::cout << "if (";
    node->condition->accept(this);
    std::cout << ") {\n";

    // Visita o bloco THEN com indentação aumentada
    indentLevel++;
    for (const auto& stmt : node->thenBody) {
        if (stmt) stmt->accept(this);
    }
    indentLevel--;

    printIndent();
    std::cout << "}";

    // Visita o bloco ELSE, se existir
    if (node->hasElse) {
        std::cout << " else {\n";
        indentLevel++;
        for (const auto& stmt : node->elseBody) {
            if (stmt) stmt->accept(this);
        }
        indentLevel--;
        
        printIndent();
        std::cout << "}\n";
    } else {
        std::cout << "\n";
    }

void CSharpCodegenVisitor::visit(WhileStmtNode* node) {
    printIndent();
    std::cout << "while (";
    if (node->condition) node->condition->accept(this);
    std::cout << ") {\n";

    indentLevel++; 
    for (auto stmt : node->body) {
        if (stmt) stmt->accept(this);
    }
    indentLevel--; 

    printIndent();
    std::cout << "}\n";
}

void CSharpCodegenVisitor::visit(ForStmtNode* node) {
    printIndent();
    std::cout << "for (";
    
    inForHeader = true;
    if (node->init) node->init->accept(this);
    std::cout << "; ";
    if (node->condition) node->condition->accept(this);
    std::cout << "; ";
    if (node->increment) node->increment->accept(this);
    inForHeader = false;

    std::cout << ") {\n";

    indentLevel++; 
    for (auto stmt : node->body) {
        if (stmt) stmt->accept(this);
    }
    indentLevel--; 

    printIndent();
    std::cout << "}\n";
}

// =========================================================================
// 3. ENTRADA E SAÍDA (I/O e Method Calls Stmts)
// =========================================================================

void CSharpCodegenVisitor::visit(PrintStmtNode* node) {
    printIndent();
    if (node->isNewLine) {
        std::cout << "Console.WriteLine(";
    } else {
        std::cout << "Console.Write(";
    }
    
    if (node->expr) {
        node->expr->accept(this);
    }
    std::cout << ");\n";
}

void CSharpCodegenVisitor::visit(PrintfStmtNode* node) {
    if (!inForHeader) printIndent();
    std::cout << "Console.Write(";
    
    if (!node->args.empty()) {
        auto formatNode = std::dynamic_pointer_cast<StringNode>(node->args[0]);
        
        if (formatNode) {
            std::string fmt = formatNode->value;
            size_t pos = 0;
            int argIndex = 0;
            
            while ((pos = fmt.find("%", pos)) != std::string::npos) {
                if (pos + 1 < fmt.length() && fmt[pos + 1] != '%') {
                    std::string replacement = "{" + std::to_string(argIndex) + "}";
                    fmt.replace(pos, 2, replacement);
                    pos += replacement.length();
                    argIndex++;
                } else {
                    pos += 2; 
                }
            }
            std::cout << fmt;
        } else {
            node->args[0]->accept(this);
        }
        
        for (size_t i = 1; i < node->args.size(); ++i) {
            std::cout << ", ";
            node->args[i]->accept(this);
        }
    }
    std::cout << ");\n";
}

void CSharpCodegenVisitor::visit(MethodCallStmtNode* node) {
    if (!inForHeader) printIndent();

    std::string name = node->methodName;
    if (name == "System.out.println") name = "Console.WriteLine";
    else if (name == "System.out.print") name = "Console.Write";

    std::cout << name << "(";

    for (size_t i = 0; i < node->args.size(); ++i) {
        node->args[i]->accept(this);
        if (i < node->args.size() - 1) {
            std::cout << ", ";
        }
    }

    if (!inForHeader) std::cout << ");\n";
    else std::cout << ")";
}

// =========================================================================
// 4. EXPRESSÕES (Expressões Inline - Sem Indentação Própria)
// =========================================================================

void CSharpCodegenVisitor::visit(BinaryExprNode* node) {
    std::cout << "(";
    node->left->accept(this);
    std::cout << " " << node->op << " ";
    node->right->accept(this);
    std::cout << ")";
}

void CSharpCodegenVisitor::visit(LiteralNode* node) {
    std::cout << node->value;
}

void CSharpCodegenVisitor::visit(StringNode* node) {
    std::cout << node->value;
}

void CSharpCodegenVisitor::visit(MethodCallExprNode* node) {
    std::cout << node->methodName << "(";   

    for (size_t i = 0; i < node->args.size(); ++i) {
        node->args[i]->accept(this);
        if (i < node->args.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")";
}

void CSharpCodegenVisitor::visit(ParenthesizedExprNode* node) {
    std::cout << "(";
    if (node->expr) node->expr->accept(this);
    std::cout << ")";
}