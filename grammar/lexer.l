%{
#include "ast.h"
#include "parser.tab.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
INT_LIT     {DIGIT}+
FLOAT_LIT   {DIGIT}+\.{DIGIT}+
STR_LIT     \"[^"]*\"
CHAR_LIT    \'[^']\'
WHITESPACE  [ \t\r\n]+

%%

{WHITESPACE}        { /* Ignora espacos */ }

"class"             { return CLASS; }
"public"            { return PUBLIC; }
"static"            { return STATIC; }
"byte"              { return TYPE_BYTE; }
"short"             { return TYPE_SHORT; }
"int"               { return TYPE_INT; }
"double"            { return TYPE_DOUBLE; } 
"float"             { return TYPE_FLOAT; }  
"char"              { return TYPE_CHAR; }  
"long"              { return TYPE_LONG; }
"boolean"           { return TYPE_BOOL; }
"String"            { return TYPE_STRING; }
"void"              { return TYPE_VOID; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"return"            { return RETURN; }
"true"              { return TRUE; }
"false"             { return FALSE; }
"System.out.println" { return PRINTLN; }
"System.out.print"   { return PRINT; }
"System.out.printf"  { return PRINTF; }
"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"<"                 { return LT; }
">"                 { return GT; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"="                 { return '='; }
";"                 { return ';'; }
","                 { return ','; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"["                 { return '['; }
"]"                 { return ']'; }
"."                 { return '.'; }

{INT_LIT}           { yylval.sval = strdup(yytext); return INT_LITERAL; }
{FLOAT_LIT}         { yylval.sval = strdup(yytext); return FLOAT_LITERAL; }
{STR_LIT}           { yylval.sval = strdup(yytext); return STRING_LITERAL; }
{CHAR_LIT}          { yylval.sval = strdup(yytext); return CHAR_LITERAL; }
{IDENTIFIER}        { yylval.sval = strdup(yytext); return IDENTIFIER; }

.                   { std::cerr << "Lexer Warning: Caractere ignorado: " << yytext << " na linha " << yylineno << std::endl; }

%%