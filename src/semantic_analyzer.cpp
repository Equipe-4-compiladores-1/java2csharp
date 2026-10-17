#include "semantic_analyzer.h"
#include <cctype>

void SemanticAnalyzerVisitor::reportError(const std::string& message) {
    std::cerr << "[Erro Semantico] " << message << std::endl;
    hasSemanticError = true;
}

bool SemanticAnalyzerVisitor::areTypesCompatible(const std::string& targetType, const std::string& sourceType) {
    std::string normTarget = (targetType == "bool") ? "boolean" : targetType;
    std::string normSource = (sourceType == "bool") ? "boolean" : sourceType;

    if (normTarget == "string") normTarget = "String";
    if (normSource == "string") normSource = "String";

    if (normTarget == normSource) return true;

    if (normTarget == "double" && (normSource == "float" || normSource == "long" || normSource == "int")) return true;
    if (normTarget == "float" && (normSource == "double" || normSource == "long" || normSource == "int")) return true;
    if (normTarget == "long" && normSource == "int") return true;
    
    return false;
}

std::string SemanticAnalyzerVisitor::getPromotedType(const std::string& type1, const std::string& type2, const std::string& op) {
    std::string t1 = (type1 == "bool") ? "boolean" : type1;
    std::string t2 = (type2 == "bool") ? "boolean" : type2;

    if (op == "+" && (t1 == "String" || t2 == "String")) {
        return "String";
    }

    if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=" || op == "&&" || op == "||") {
        if (areTypesCompatible(t1, t2) || areTypesCompatible(t2, t1)) {
            return "boolean";
        }
        return "unknown";
    }

    if (t1 == "double" || t2 == "double") return "double";
    if (t1 == "float" || t2 == "float") return "float";
    if (t1 == "long" || t2 == "long") return "long";
    if (t1 == "int" && t2 == "int") return "int";

    return "unknown";
}

void SemanticAnalyzerVisitor::visit(ClassDeclNode* node) {
    for (const auto& func : node->functions) {
        SymbolInfo info{func->name, func->returnType, SymbolKind::FUNCTION, true};
        if (!symbolTable.defineSymbol(func->name, info)) {
            reportError("Metodo '" + func->name + "' ja declarado na classe '" + node->className + "'.");
        }
    }

    for (const auto& func : node->functions) {
        func->accept(this);
    }
}

void SemanticAnalyzerVisitor::visit(FunctionDeclNode* node) {
    symbolTable.enterScope();
    currentFunctionReturnType = node->returnType;

    for (const auto& param : node->params) {
        SymbolInfo info{param.name, param.type, SymbolKind::PARAMETER, true};
        if (!symbolTable.defineSymbol(param.name, info)) {
            reportError("Variavel '" + param.name + "' ja foi declarada neste escopo.");
        }
    }

    for (const auto& stmt : node->body) {
        if (stmt) stmt->accept(this);
    }

    symbolTable.exitScope();
}

void SemanticAnalyzerVisitor::visit(VarDeclNode* node) {
    std::string initType;
    if (node->initExpr) {
        node->initExpr->accept(this);
        initType = currentExprType;

        if (initType != "unknown" && !areTypesCompatible(node->type, initType)) {
            reportError("Atribuicao incompativel para a variavel '" + node->name +
                        "'. Esperado: '" + node->type + "', Recebido: '" + initType + "'.");
        }
    }

    SymbolInfo info{node->name, node->type, SymbolKind::VARIABLE, node->initExpr != nullptr};
    if (!symbolTable.defineSymbol(node->name, info)) {
        reportError("Variavel '" + node->name + "' ja foi declarada neste escopo.");
    }
}

void SemanticAnalyzerVisitor::visit(ReturnStmtNode* node) {
    if (node->expr) {
        node->expr->accept(this);
        if (currentFunctionReturnType == "void") {
            reportError("Metodo 'void' nao pode retornar valor.");
        } else if (currentExprType != "unknown" && !areTypesCompatible(currentFunctionReturnType, currentExprType)) {
            reportError("Retorno incompativel no metodo. Esperado: '" +
                        currentFunctionReturnType + "', Recebido: '" + currentExprType + "'.");
        }
    } else {
        if (currentFunctionReturnType != "void") {
            reportError("Metodo com retorno '" + currentFunctionReturnType + "' deve retornar um valor.");
        }
    }
}

void SemanticAnalyzerVisitor::visit(PrintStmtNode* node) {
    if (node->expr) {
        node->expr->accept(this);
    }
}

void SemanticAnalyzerVisitor::visit(PrintfStmtNode* node) {
    for (const auto& arg : node->args) {
        if (arg) arg->accept(this);
    }
}

void SemanticAnalyzerVisitor::visit(MethodCallStmtNode* node) {
    SymbolInfo info;
    if (!symbolTable.resolveSymbol(node->methodName, info)) {
        if (node->methodName.find('.') == std::string::npos) {
            reportError("Chamada a metodo nao declarado '" + node->methodName + "'.");
        }
    }

    for (const auto& arg : node->args) {
        if (arg) arg->accept(this);
    }
}

void SemanticAnalyzerVisitor::visit(MethodCallExprNode* node) {
    SymbolInfo info;
    if (symbolTable.resolveSymbol(node->methodName, info)) {
        currentExprType = info.type;
    } else {
        if (node->methodName.find('.') == std::string::npos) {
            reportError("Chamada a metodo nao declarado '" + node->methodName + "'.");
        }
        currentExprType = "unknown";
    }

    for (const auto& arg : node->args) {
        if (arg) arg->accept(this);
    }
}

void SemanticAnalyzerVisitor::visit(LiteralNode* node) {
    const std::string& val = node->value;

    SymbolInfo info;
    if (symbolTable.resolveSymbol(val, info)) {
        currentExprType = info.type;
        return;
    }

    if (!val.empty() && (std::isdigit(val[0]) || val[0] == '-')) {
        if (val.find('.') != std::string::npos) {
            currentExprType = "double";
        } else {
            currentExprType = "int";
        }
    } else if (val == "true" || val == "false") {
        currentExprType = "boolean";
    } else if (!val.empty() && (val[0] == '"' || val[0] == '\'')) {
        currentExprType = (val[0] == '"') ? "String" : "char";
    } else {
        if (!val.empty() && (std::isalpha(val[0]) || val[0] == '_')) {
            reportError("Variavel '" + val + "' nao declarada.");
        }
        currentExprType = "unknown";
    }
}

void SemanticAnalyzerVisitor::visit(StringNode* node) {
    currentExprType = "String";
}

void SemanticAnalyzerVisitor::visit(BinaryExprNode* node) {
    node->left->accept(this);
    std::string leftType = currentExprType;

    node->right->accept(this);
    std::string rightType = currentExprType;

    if (leftType == "unknown" || rightType == "unknown") {
        currentExprType = "unknown";
        return;
    }

    std::string resultType = getPromotedType(leftType, rightType, node->op);
    if (resultType == "unknown") {
        reportError("Operacao '" + node->op + "' invalida entre os tipos '" + leftType + "' e '" + rightType + "'.");
    }

    currentExprType = resultType;
}

void SemanticAnalyzerVisitor::visit(IfStmtNode* node) {
    if (node->condition) {
        node->condition->accept(this);
        if (currentExprType != "unknown" && currentExprType != "boolean" && currentExprType != "bool") {
            reportError("A condicao da instrucao 'if' deve ser do tipo boolean. Recebido: '" + currentExprType + "'.");
        }
    }

    symbolTable.enterScope();
    for (const auto& stmt : node->thenBody) {
        if (stmt) stmt->accept(this);
    }
    symbolTable.exitScope();

    if (node->hasElse) {
        symbolTable.enterScope();
        for (const auto& stmt : node->elseBody) {
            if (stmt) stmt->accept(this);
        }
        symbolTable.exitScope();
    }
}

void SemanticAnalyzerVisitor::visit(AssignStmtNode* node) {
    SymbolInfo info;
    if (!symbolTable.resolveSymbol(node->identifier, info)) {
        reportError("Atribuicao a variavel nao declarada '" + node->identifier + "'.");
        return;
    }

    if (node->expr) {
        node->expr->accept(this);
        std::string rightType = currentExprType;

        if (rightType != "unknown" && !areTypesCompatible(info.type, rightType)) {
            reportError("Atribuicao incompativel para a variavel '" + node->identifier +
                        "'. Esperado: '" + info.type + "', Recebido: '" + rightType + "'.");
        }
    }
}

void SemanticAnalyzerVisitor::visit(WhileStmtNode* node) {
    if (node->condition) {
        node->condition->accept(this);
        if (currentExprType != "unknown" && currentExprType != "boolean" && currentExprType != "bool") {
            reportError("A condicao do 'while' deve ser do tipo boolean. Recebido: '" + currentExprType + "'.");
        }
    }

    symbolTable.enterScope();
    for (auto stmt : node->body) {
        if (stmt) stmt->accept(this);
    }
    symbolTable.exitScope();
}

void SemanticAnalyzerVisitor::visit(ForStmtNode* node) {
    symbolTable.enterScope();

    if (node->init) node->init->accept(this);

    if (node->condition) {
        node->condition->accept(this);
        if (currentExprType != "unknown" && currentExprType != "boolean" && currentExprType != "bool") {
            reportError("A condicao do 'for' deve ser do tipo boolean. Recebido: '" + currentExprType + "'.");
        }
    }

    if (node->increment) node->increment->accept(this);

    for (auto stmt : node->body) {
        if (stmt) stmt->accept(this);
    }

    symbolTable.exitScope();
}