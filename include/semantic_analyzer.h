#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h"
#include <iostream>

class SemanticAnalyzerVisitor : public ASTVisitor {
private:
    SymbolTable symTable;
    bool hasErrors = false;

public: 
    bool analyze(const std::shared_ptr<ClassDeclNode>& root);

    void visit(ClassDeclNode* node) override;
    void visit(FunctionDeclNode* node) override;
    void visit(VarDeclNode* node) override;
    void visit(LiteralNode* node) override;

    void visit(BinaryExprNode* node) override;
    void visit(ParenthesizedExprNode* node) override;
    void visit(ReturnStmtNode* node) override;
    void visit(PrintStmtNode* node) override;
    void visit(PrintfStmtNode* node) override;
    void visit(StringNode* node) override;
    void visit(MethodCallExprNode* node) override;
    void visit(MethodCallStmtNode* node) override;
    void visit(IfStmtNode* node) override;

private:
    std::string inferExprType(ExprNode* node);
};

#endif