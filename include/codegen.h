#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <iostream>
#include <vector>
#include <memory>

class CSharpCodegenVisitor : public ASTVisitor {
private:
    bool inForHeader = false;

    int indentLevel = 0;
    void printIndent() {
        for (int i = 0; i < indentLevel; ++i) {
            std::cout << "    "; 
        }
    }

public:
    void generateProgram(const std::shared_ptr<ClassDeclNode>& root);
    void visit(ClassDeclNode* node) override;
    void visit(BinaryExprNode* node) override;
    void visit(ParenthesizedExprNode* node) override;
    void visit(LiteralNode* node) override;
    void visit(VarDeclNode* node) override;
    void visit(ReturnStmtNode* node) override;
    void visit(FunctionDeclNode* node) override;
    void visit(PrintStmtNode* node) override;
    void visit(PrintfStmtNode* node) override;
    void visit(StringNode* node) override;
    void visit(MethodCallExprNode* node) override;
    void visit(MethodCallStmtNode* node) override;
    void visit(IfStmtNode* node) override;
    void visit(AssignStmtNode* node) override;
    void visit(WhileStmtNode* node) override;
    void visit(ForStmtNode* node) override;
};

#endif
