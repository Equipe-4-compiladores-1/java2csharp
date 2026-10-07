#include "semantic_analyzer.h"

bool SemanticAnalyzerVisitor::analyze(const std::shared_ptr<ClassDeclNode>& root) {
    if (root) root->accept(this);
    return !hasErrors;
}

void SemanticAnalyzerVisitor::visit(ClassDeclNode* node) {
    for (auto& func : node->functions) {
        func->accept(this);
    }
}

void SemanticAnalyzerVisitor::visit(FunctionDeclNode* node) {
    symTable.enterScope();

    for (const auto& param : node->params) {
        if (!symTable.defineSymbol(param.name, param.type)) {
            std::cerr << "[Erro Semantico]: Parâmetro '" << param.name
                      << "' redefinido na função '" << node->name << "'.\n";
            hasErrors = true;
        }
    }

    for (auto& stmt : node->body) {
        if (stmt) stmt->accept(this);
    }

    symTable.exitScope();
}

void SemanticAnalyzerVisitor::visit(VarDeclNode* node) {
    if (node->initExpr) {
        node->initExpr->accept(this);
    }
    
    if (!symTable.defineSymbol(node->name, node->type)) {
        std::cerr << "[Erro Semantico]: Variável '" << node->name 
                  << "' já foi declarada neste escopo.\n";
        hasErrors = true;
    }
}

void SemanticAnalyzerVisitor::visit(LiteralNode* node) {
    if (node->value == "true" || node->value == "false") {
        return;
    }

    if (!node->value.empty() && isalpha(node->value[0])) {
        SymbolInfo info;
        if (!symTable.resolveSymbol(node->value, info)) {
            std::cerr << "[Erro Semantico]: Uso de variável não declarada '" << node->value << "'.\n";
            hasErrors = true;
        }
    }
}

void SemanticAnalyzerVisitor::visit(BinaryExprNode* node) {
    if (node->left) node->left->accept(this);
    if (node->right) node->right->accept(this);
}
void SemanticAnalyzerVisitor::visit(ReturnStmtNode* node) {
    if (node->expr) node->expr->accept(this);
}
void SemanticAnalyzerVisitor::visit(PrintStmtNode* node) {
    if (node->expr) node->expr->accept(this);
}
void SemanticAnalyzerVisitor::visit(PrintfStmtNode* node) {
    for (auto& arg : node->args) arg->accept(this);
}
void SemanticAnalyzerVisitor::visit(MethodCallExprNode* node) {
    for (auto& arg : node->args) arg->accept(this);
}
void SemanticAnalyzerVisitor::visit(MethodCallStmtNode* node) {
    for (auto& arg : node->args) arg->accept(this);
}
void SemanticAnalyzerVisitor::visit(StringNode* node) {}

std::string SemanticAnalyzerVisitor::inferExprType(ExprNode* node) {
    if (auto literal = dynamic_cast<LiteralNode*>(node)) {
        if (literal->value == "true" || literal->value == "false") return "boolean";

        SymbolInfo info;
        if (symTable.resolveSymbol(literal->value, info)) return info.type;
        if (literal->value.find('.') != std::string::npos) return "double";
        return "int";
    }

    if (dynamic_cast<StringNode*>(node)) return "String";

    if (auto binary = dynamic_cast<BinaryExprNode*>(node)) {
        if (binary->op == "==" || binary->op == "!=" || binary->op == "<" ||
            binary->op == "<=" || binary->op == ">" || binary->op == ">=") {
            return "boolean";
        }
        return inferExprType(binary->left.get());
    }

    return "unknown";
}

void SemanticAnalyzerVisitor::visit(IfStmtNode* node) {
    if (node->condition) {
        node->condition->accept(this);
        if (inferExprType(node->condition.get()) != "boolean") {
            std::cerr << "[Erro Semantico]: A condição do if deve ser do tipo boolean.\n";
            hasErrors = true;
        }
    }

    symTable.enterScope();
    for (auto& stmt : node->thenBody) {
        if (stmt) stmt->accept(this);
    }
    symTable.exitScope();

    if (node->hasElse) {
        symTable.enterScope();
        for (auto& stmt : node->elseBody) {
            if (stmt) stmt->accept(this);
        }
        symTable.exitScope();
    }
}
