#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h"
#include <string>
#include <iostream>

class SemanticAnalyzerVisitor : public ASTVisitor {
private:
    SymbolTable symbolTable;
    std::string currentExprType;
    std::string currentFunctionReturnType;
    bool hasSemanticError = false;

    bool areTypesCompatible(const std::string& targetType, const std::string& sourceType);
    std::string getPromotedType(const std::string& type1, const std::string& type2, const std::string& op);

    void reportError(const std::string& message);

public: 
    SemanticAnalyzerVisitor() = default;

    bool hasErrors() const { return hasSemanticError; }
    std::string getCurrentExprType() const { return currentExprType; }

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
};

#endif