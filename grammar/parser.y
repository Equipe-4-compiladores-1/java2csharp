%{
#include "ast.h"
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

extern int yylex();
extern char* yytext;
extern int yylineno;
void yyerror(const char* s);

std::vector<std::shared_ptr<FunctionDeclNode>> programFunctions;
std::shared_ptr<ClassDeclNode> rootNode;
%}

%union {
    char* sval;
    ASTNode* node;
    StmtNode* stmt;
    ExprNode* expr;
    Param* param;
    std::vector<Param>* param_list;
    std::vector<std::shared_ptr<StmtNode>>* stmt_list;
    std::vector<std::shared_ptr<ExprNode>>* expr_list;
}

%token <sval> IDENTIFIER INT_LITERAL FLOAT_LITERAL CHAR_LITERAL STRING_LITERAL
%token TYPE_INT TYPE_DOUBLE TYPE_FLOAT TYPE_CHAR TYPE_LONG TYPE_BOOL TYPE_VOID
%token RETURN CLASS PUBLIC STATIC TYPE_STRING PRINT PRINTLN PRINTF
%token IF ELSE TRUE FALSE
%token EQ NE LT LE GT GE

%destructor { free($$); } <sval>

%type <stmt> var_decl stmt
%type <expr> expr
%type <stmt_list> stmt_list
%type <stmt_list> else_opt
%type <sval> type_specifier
%type <expr_list> expr_list printf_args_opt expr_list_opt
%type <param> param
%type <param_list> param_list param_list_opt

%left EQ NE LT LE GT GE
%left '+' '-'
%left '*' '/'

%define parse.error detailed

%start program

%%

program:
    class_decl
    ;

function_list:
    function_decl
    | function_list function_decl
    ;

type_specifier:
    TYPE_INT  { $$ = strdup("int"); }
    | TYPE_BOOL { $$ = strdup("boolean"); }
    | TYPE_VOID { $$ = strdup("void"); }
    | TYPE_DOUBLE { $$ = strdup("double"); }
    | TYPE_FLOAT  { $$ = strdup("float"); }
    | TYPE_CHAR   { $$ = strdup("char"); }
    | TYPE_LONG   { $$ = strdup("long"); }
    ;

function_decl:
    type_specifier IDENTIFIER '(' param_list_opt ')' '{' stmt_list '}' {
        auto func = std::make_shared<FunctionDeclNode>($1, $2, *$4);
        func->body = *$7;
        delete $4;
        delete $7;
        programFunctions.push_back(func);
        free($1);
        free($2);
    }
    | PUBLIC STATIC TYPE_VOID IDENTIFIER '(' TYPE_STRING '[' ']' IDENTIFIER ')' '{' stmt_list '}' {
        auto func = std::make_shared<FunctionDeclNode>("void", $4);
        func->body = *$12;
        delete $12;
        programFunctions.push_back(func);
        free($4);
        free($9);
    }
    ;
class_decl:
    CLASS IDENTIFIER '{' function_list '}' {
        auto classNode = std::make_shared<ClassDeclNode>($2);
        classNode->functions = programFunctions;
        programFunctions.clear();
        rootNode = classNode;
        free($2);
    }
    ;

stmt_list:
    /* vazio */ { $$ = new std::vector<std::shared_ptr<StmtNode>>(); }
    | stmt_list stmt {
        $1->push_back(std::shared_ptr<StmtNode>($2));
        $$ = $1;
    }
    ;

stmt:
    var_decl ';' { $$ = $1; }
    | RETURN expr ';' { $$ = new ReturnStmtNode(std::shared_ptr<ExprNode>($2)); }
    | RETURN ';' { $$ = new ReturnStmtNode(nullptr); }
    | PRINT '(' expr ')' ';' { $$ = new PrintStmtNode(std::shared_ptr<ExprNode>($3), false); }
    | PRINTLN '(' expr ')' ';' { $$ = new PrintStmtNode(std::shared_ptr<ExprNode>($3), true); }
    | PRINTF '(' STRING_LITERAL  printf_args_opt ')' ';' { 
        std::vector<std::shared_ptr<ExprNode>> args;
        
        // 1. Adicionamos a string (convertendo para StringNode)
        args.push_back(std::make_shared<StringNode>($3));
        
        // 2. Lemos a lista ($5) e copiamos TODAS as variáveis para os argumentos
        for (auto expr_ptr : *$4) {
            args.push_back(expr_ptr);
        }
        
        // 3. Passamos a lista pronta para o PrintfStmtNode
        $$ = new PrintfStmtNode(args); 
        
        // 4. Limpamos a lista temporária da memória
        delete $4;
    }
    | IDENTIFIER '(' expr_list_opt ')' ';' {
        $$ = new MethodCallStmtNode($1, *$3);
        delete $3;
        free($1);
    }
    | IDENTIFIER '.' IDENTIFIER '(' expr_list_opt ')' ';' { 
        std::string qualifiedName = std::string($1) + "." + std::string($3);
        $$ = new MethodCallStmtNode(qualifiedName, *$5);
        delete $5;
        free($1);
        free($3);
    }
    | IF '(' expr ')' '{' stmt_list '}' else_opt {
        std::vector<std::shared_ptr<StmtNode>> elseBody;
        bool hasElse = $8 != nullptr;
        if (hasElse) {
            elseBody = *$8;
            delete $8;
        }
        $$ = new IfStmtNode(std::shared_ptr<ExprNode>($3), *$6, elseBody, hasElse);
        delete $6;
    }
    ;

else_opt:
    /* vazio */ { $$ = nullptr; }
    | ELSE '{' stmt_list '}' { $$ = $3; }
    ;

var_decl:
    type_specifier IDENTIFIER '=' expr {
        $$ = new VarDeclNode($1, $2, std::shared_ptr<ExprNode>($4));
        free($1);
        free($2);
    }
    ;



expr:
    '(' expr ')' { $$ = new ParenthesizedExprNode(std::shared_ptr<ExprNode>($2)); }
    | INT_LITERAL { $$ = new LiteralNode($1); free($1); }
    | FLOAT_LITERAL   { $$ = new LiteralNode($1); free($1); } 
    | CHAR_LITERAL    { $$ = new LiteralNode($1); free($1); }
    | STRING_LITERAL { $$ = new StringNode($1); free($1); }
    | TRUE { $$ = new LiteralNode("true"); }
    | FALSE { $$ = new LiteralNode("false"); }
    | IDENTIFIER { 
        $$ = new LiteralNode($1);
        free($1);
    }
    | IDENTIFIER '(' expr_list_opt ')' {
        $$ = new MethodCallExprNode(std::string($1), *$3);
        delete $3;
        free($1);
    }
    | IDENTIFIER '.' IDENTIFIER '(' expr_list_opt ')' {
        std::string qualifiedName = std::string($1) + "." + std::string($3);
        $$ = new MethodCallExprNode(qualifiedName, *$5);
        delete $5;
        free($1);
        free($3);
    }
    | expr '+' expr { $$ = new BinaryExprNode("+", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr '-' expr { $$ = new BinaryExprNode("-", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr '*' expr { $$ = new BinaryExprNode("*", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr '/' expr { $$ = new BinaryExprNode("/", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr EQ expr { $$ = new BinaryExprNode("==", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr NE expr { $$ = new BinaryExprNode("!=", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr LT expr { $$ = new BinaryExprNode("<", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr LE expr { $$ = new BinaryExprNode("<=", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr GT expr { $$ = new BinaryExprNode(">", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    | expr GE expr { $$ = new BinaryExprNode(">=", std::shared_ptr<ExprNode>($1), std::shared_ptr<ExprNode>($3)); }
    ;

expr_list:
    expr { 
        $$ = new std::vector<std::shared_ptr<ExprNode>>(); 
        $$->push_back(std::shared_ptr<ExprNode>($1)); 
    }
    | expr_list ',' expr {
        $1->push_back(std::shared_ptr<ExprNode>($3));
        $$ = $1;
    }
    ;

expr_list_opt:
    /* vazio */ { $$ = new std::vector<std::shared_ptr<ExprNode>>(); }
    | expr_list { $$ = $1; }
    ;

param_list_opt:
    /* vazio */ { $$ = new std::vector<Param>(); }
    | param_list { $$ = $1; }
    ;

param_list:
    param { 
        $$ = new std::vector<Param>(); 
        $$->push_back(*$1); 
        delete $1;
    }
    | param_list ',' param {
        $1->push_back(*$3);
        $$ = $1;
        delete $3;
    }
    ;

param:
    type_specifier IDENTIFIER { 
        $$ = new Param($1, $2); 
        free($1);
        free($2);
    }
    ;
printf_args_opt:
    /* vazio */ { $$ = new std::vector<std::shared_ptr<ExprNode>>(); }
    | ',' expr_list { $$ = $2; }
    ;
%%

void yyerror(const char* s) {
    const char* termo = yytext;
    if (termo == nullptr || *termo == '\0' || std::strstr(s, "end of file") != nullptr) {
        termo = "fim do arquivo";
    }

    std::cerr << "[Erro Sintatico] na linha " << yylineno
              << ": termo '" << termo << "' nao pode ser compilado: "
              << s << std::endl;
}