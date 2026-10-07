#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

class BinaryExprNode;
class ParenthesizedExprNode;
class LiteralNode;
class VarDeclNode;
class ReturnStmtNode;
class FunctionDeclNode;
class ClassDeclNode;
class PrintStmtNode;
class PrintfStmtNode;
class StringNode;
class MethodCallExprNode;
class MethodCallStmtNode;
class IfStmtNode;

// Padrão Visitor para percorrer a AST
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(ClassDeclNode* node) = 0;
    virtual void visit(BinaryExprNode* node) = 0;
    virtual void visit(ParenthesizedExprNode* node) = 0;
    virtual void visit(LiteralNode* node) = 0;
    virtual void visit(VarDeclNode* node) = 0;
    virtual void visit(ReturnStmtNode* node) = 0;
    virtual void visit(PrintStmtNode* node) = 0;
    virtual void visit(FunctionDeclNode* node) = 0;
    virtual void visit(PrintfStmtNode* node) = 0;
    virtual void visit(StringNode* node) = 0;
    virtual void visit(MethodCallExprNode* node) = 0;
    virtual void visit(MethodCallStmtNode* node) = 0;
    virtual void visit(IfStmtNode* node) = 0;
};

// Classe Base
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* visitor) = 0;
};

// --- EXPRESSÕES ---
class ExprNode : public ASTNode {};

class ParenthesizedExprNode : public ExprNode {
public:
    std::shared_ptr<ExprNode> expr;

    explicit ParenthesizedExprNode(std::shared_ptr<ExprNode> expr)
        : expr(std::move(expr)) {}

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class LiteralNode : public ExprNode {
public:
    std::string value;
    LiteralNode(std::string val) : value(std::move(val)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class BinaryExprNode : public ExprNode {
public:
    std::string op;
    std::shared_ptr<ExprNode> left;
    std::shared_ptr<ExprNode> right;

    BinaryExprNode(std::string op, std::shared_ptr<ExprNode> l, std::shared_ptr<ExprNode> r)
        : op(std::move(op)), left(std::move(l)), right(std::move(r)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

// --- COMANDOS (STATEMENTS) ---
class StmtNode : public ASTNode {};

class IfStmtNode : public StmtNode {
public:
    std::shared_ptr<ExprNode> condition;
    std::vector<std::shared_ptr<StmtNode>> thenBody;
    std::vector<std::shared_ptr<StmtNode>> elseBody;
    bool hasElse;

    IfStmtNode(std::shared_ptr<ExprNode> condition,
               std::vector<std::shared_ptr<StmtNode>> thenBody,
               std::vector<std::shared_ptr<StmtNode>> elseBody = {},
               bool hasElse = false)
        : condition(std::move(condition)),
          thenBody(std::move(thenBody)),
          elseBody(std::move(elseBody)),
          hasElse(hasElse) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

class VarDeclNode : public StmtNode {
public:
    std::string type;
    std::string name;
    std::shared_ptr<ExprNode> initExpr;

    VarDeclNode(std::string type, std::string name, std::shared_ptr<ExprNode> init = nullptr)
        : type(std::move(type)), name(std::move(name)), initExpr(std::move(init)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class ReturnStmtNode : public StmtNode {
public:
    std::shared_ptr<ExprNode> expr;
    ReturnStmtNode(std::shared_ptr<ExprNode> e) : expr(std::move(e)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class PrintStmtNode : public StmtNode {
public:
    std::shared_ptr<ExprNode> expr;
    bool isNewLine; // true para println, false para print

    PrintStmtNode(std::shared_ptr<ExprNode> e, bool nl) : expr(e), isNewLine(nl) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

struct Param {
    std::string type;
    std::string name;
    Param(const std::string& t, const std::string& n) : type(t), name(n) {}
};


// --- FUNÇÃO ---
class FunctionDeclNode : public ASTNode {
public:
    std::string returnType;
    std::string name;
    std::vector<Param> params;
    std::vector<std::shared_ptr<StmtNode>> body;

    FunctionDeclNode(const std::string& type, const std::string& id, std::vector<Param> p = {}) 
        : returnType(type), name(id), params(p) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};


class ClassDeclNode : public ASTNode {
public:
    std::string className;
    
    std::vector<std::shared_ptr<FunctionDeclNode>> functions;

    
    ClassDeclNode(const std::string& name) : className(name) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
    virtual ~ClassDeclNode() = default;
};

// Nó para textos ("exemplo")
class StringNode : public ExprNode {
public:
    std::string value;
    StringNode(const std::string& v) : value(v) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

// Nó para o printf
class PrintfStmtNode : public StmtNode {
public:
    std::vector<std::shared_ptr<ExprNode>> args;
    PrintfStmtNode(std::vector<std::shared_ptr<ExprNode>> a) : args(a) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class MethodCallExprNode : public ExprNode {
public: 
    std::string methodName;
    std::vector<std::shared_ptr<ExprNode>> args;

    MethodCallExprNode(std::string name, std::vector<std::shared_ptr<ExprNode>> a)
        : methodName(std::move(name)), args(std::move(a)) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

class MethodCallStmtNode : public StmtNode {
public:
    std::string methodName;
    std::vector<std::shared_ptr<ExprNode>> args;

    MethodCallStmtNode(std::string name, std::vector<std::shared_ptr<ExprNode>> a)
        : methodName(std::move(name)), args(std::move(a)) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

#endif