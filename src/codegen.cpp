#include "codegen.h"
#include <sstream>

void CSharpCodegenVisitor::generateProgram(const std::shared_ptr<ClassDeclNode>& root) {
    if (!root) return;

    std::cout << "using System;\n\n";
    std::cout << "namespace TranspiledProgram {\n";

    // Chama o visitante para processar o nó da classe
    root->accept(this);

    std::cout << "}";
}

void CSharpCodegenVisitor::visit(ClassDeclNode* node) {
    // Usa o nome dinâmico da classe que veio do código Java
    std::cout << "    public static class " << node->className << " {\n";

    // Visita cada função dentro da classe
    for (const auto& func : node->functions) {
        func->accept(this);
    }

    std::cout << "    }\n";
}


void CSharpCodegenVisitor::visit(VarDeclNode* node) {
    std::string type = (node->type == "boolean") ? "bool" : node->type;
    std::cout << "            " << type << " " << node->name << " = ";
    if (node->initExpr) {
        node->initExpr->accept(this);
    }
    std::cout << ";\n";
}

void CSharpCodegenVisitor::visit(ReturnStmtNode* node) {
    std::cout << "            return ";
    if (node->expr) {
        node->expr->accept(this);
    }
    std::cout << ";\n";
}

void CSharpCodegenVisitor::visit(BinaryExprNode* node) {
    node->left->accept(this);
    std::cout << " " << node->op << " ";
    node->right->accept(this);
}

void CSharpCodegenVisitor::visit(ParenthesizedExprNode* node) {
    std::cout << "(";
    if (node->expr) node->expr->accept(this);
    std::cout << ")";
}

void CSharpCodegenVisitor::visit(LiteralNode* node) {
    std::cout << node->value;
}
void CSharpCodegenVisitor::visit(PrintStmtNode* node) {
    if (node->isNewLine) {
        std::cout << "            Console.WriteLine(";
    } else {
        std::cout << "            Console.Write(";
    }
    
    // Visita a expressão que está dentro dos parênteses
    if (node->expr) {
        node->expr->accept(this);
    }
    
    std::cout << ");\n";
}

void CSharpCodegenVisitor::visit(StringNode* node) {
    std::cout << node->value;
}

void CSharpCodegenVisitor::visit(PrintfStmtNode* node) {
    std::cout << "            Console.Write(";
    
    if (!node->args.empty()) {
        auto formatNode = std::dynamic_pointer_cast<StringNode>(node->args[0]);
        
        if (formatNode) {
            std::string fmt = formatNode->value;
            size_t pos = 0;
            int argIndex = 0;
            
            // Procura por QUALQUER '%' na string
            while ((pos = fmt.find("%", pos)) != std::string::npos) {
                // Verifica se não é um "%%" (que serve para imprimir o próprio símbolo de porcentagem)
                if (pos + 1 < fmt.length() && fmt[pos + 1] != '%') {
                    std::string replacement = "{" + std::to_string(argIndex) + "}";
                    // Substitui 2 caracteres (o % e a letra seguinte, como 'f' ou 'd')
                    fmt.replace(pos, 2, replacement);
                    
                    pos += replacement.length();
                    argIndex++;
                } else {
                    pos += 2; // Pula o "%%"
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

void CSharpCodegenVisitor::visit(FunctionDeclNode* node) {
    // Se o nome no Java for "main", mudamos para "Main" no C#. Senão, mantemos o original.
    std::string methodName = (node->name == "main") ? "Main" : node->name;

    // Escreve a assinatura usando a nossa nova variável 'methodName'
    std::cout << "        public static " << node->returnType << " " << methodName << "(";
    
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

    // Visita o corpo da função
    for (auto stmt : node->body) {
        if (stmt) {
            stmt->accept(this);
        }
    }

    std::cout << "        }\n\n";
}

void CSharpCodegenVisitor::visit(MethodCallExprNode* node) {
    // Atribui o nome do metodo e abre parênteses
    std::cout << node->methodName << "(";   

    // Itera sobre os args, visitando cada expr para gerar o codigo
    for (size_t i = 0; i < node->args.size(); ++i) {
        node->args[i]->accept(this);
        if (i < node->args.size() - 1) {
            std::cout << ", ";
        }
    }

    // Fecha parênteses (sem ponto e virgula, pois é uma expr)
    std::cout << ")";
}

void CSharpCodegenVisitor::visit(MethodCallStmtNode* node) {
    std::cout << "            ";

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

    std::cout << ");\n";
}

void CSharpCodegenVisitor::visit(IfStmtNode* node) {
    std::cout << "            if (";
    node->condition->accept(this);
    std::cout << ") {\n";

    auto generateBody = [this](const std::vector<std::shared_ptr<StmtNode>>& body) {
        std::ostringstream bodyOutput;
        std::streambuf* previousBuffer = std::cout.rdbuf(bodyOutput.rdbuf());
        for (const auto& stmt : body) {
            if (stmt) stmt->accept(this);
        }
        std::cout.rdbuf(previousBuffer);

        std::istringstream lines(bodyOutput.str());
        std::string line;
        while (std::getline(lines, line)) {
            std::cout << "    " << line << "\n";
        }
    };

    generateBody(node->thenBody);

    std::cout << "            }";
    if (node->hasElse) {
        std::cout << " else {\n";
        generateBody(node->elseBody);
        std::cout << "            }";
    }
    std::cout << "\n";
}