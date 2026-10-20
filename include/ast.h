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
class AssignStmtNode;
class WhileStmtNode;
class ForStmtNode;
class UnaryStmtNode;

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
    virtual void visit(AssignStmtNode* node) = 0;
    virtual void visit(WhileStmtNode* node) = 0;
    virtual void visit(ForStmtNode* node) = 0;
    virtual void visit(UnaryStmtNode* node) = 0;
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

class StringNode : public ExprNode {
public:
    std::string value;
    StringNode(std::string val) : value(std::move(val)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class MethodCallExprNode : public ExprNode {
public:
    std::string methodName;
    std::vector<std::shared_ptr<ExprNode>> args;
    MethodCallExprNode(std::string name, std::vector<std::shared_ptr<ExprNode>> arguments)
        : methodName(std::move(name)), args(std::move(arguments)) {}
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

class MethodCallStmtNode : public StmtNode {
public:
    std::string methodName;
    std::vector<std::shared_ptr<ExprNode>> args;
    MethodCallStmtNode(std::string name, std::vector<std::shared_ptr<ExprNode>> arguments)
        : methodName(std::move(name)), args(std::move(arguments)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class PrintfStmtNode : public StmtNode {
public:
    std::vector<std::shared_ptr<ExprNode>> args;
    PrintfStmtNode(std::vector<std::shared_ptr<ExprNode>> arguments) : args(std::move(arguments)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

struct Param {
    std::string type;
    std::string name;
    Param(const std::string& t, const std::string& n) : type(t), name(n) {}
};

class WhileStmtNode : public StmtNode {
public:
    std::shared_ptr<ExprNode> condition;
    std::vector<std::shared_ptr<StmtNode>> body;
    WhileStmtNode(std::shared_ptr<ExprNode> cond, std::vector<std::shared_ptr<StmtNode>> b)
        : condition(cond), body(b) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

class ForStmtNode : public StmtNode {
public:
    std::shared_ptr<StmtNode> init; 
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StmtNode> increment;
    std::vector<std::shared_ptr<StmtNode>> body;

    ForStmtNode(std::shared_ptr<StmtNode> i, std::shared_ptr<ExprNode> cond, 
                std::shared_ptr<StmtNode> inc, std::vector<std::shared_ptr<StmtNode>> b)
        : init(i), condition(cond), increment(inc), body(b) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

class AssignStmtNode : public StmtNode {
public:
    std::string identifier;
    std::shared_ptr<ExprNode> expr;

    AssignStmtNode(const std::string& id, std::shared_ptr<ExprNode> e)
        : identifier(id), expr(e) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

class FunctionDeclNode : public ASTNode {
public:
    std::string returnType;
    std::string name;
    std::vector<Param> params;
    std::vector<std::shared_ptr<StmtNode>> body;

    FunctionDeclNode(std::string rType, std::string n, std::vector<Param> p = {})
        : returnType(std::move(rType)), name(std::move(n)), params(std::move(p)) {}

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class ClassDeclNode : public ASTNode {
public:
    std::string className;
    std::vector<std::shared_ptr<FunctionDeclNode>> functions;

    ClassDeclNode(std::string name) : className(std::move(name)) {}

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};

class UnaryStmtNode : public StmtNode {
public:
    std::string identifier;
    std::string op; // "++" ou "--"

    UnaryStmtNode(std::string id, std::string operation)
        : identifier(std::move(id)), op(std::move(operation)) {}

    void accept(ASTVisitor* visitor) override {
        visitor->visit(this);
    }
};

#endif