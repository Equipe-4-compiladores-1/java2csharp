%{
#include "ast.h"
#include "parser.tab.hpp"
#include <cstdlib>
%}

%option noyywrap
%option yylineno

/* --- Definições de Expressões Regulares --- */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
INT_LIT     {DIGIT}+
FLOAT_LIT   {DIGIT}+\.{DIGIT}+
STR_LIT     \"[^"]*\"
CHAR_LIT    \'[^']\'
WHITESPACE  [ \t\r\n]+

%%

{WHITESPACE}        { /* Ignora espaços em branco */ }

/* --- Palavras Reservadas (Orientação a Objetos e Modificadores) --- */
"class"             { return CLASS; }
"public"            { return PUBLIC; }
"static"            { return STATIC; }

/* --- Tipos de Dados --- */
"int"               { return TYPE_INT; }
"double"            { return TYPE_DOUBLE; } 
"float"             { return TYPE_FLOAT; }  
"char"              { return TYPE_CHAR; }  
"long"              { return TYPE_LONG; }
"boolean"           { return TYPE_BOOL; }
"String"            { return TYPE_STRING; }
"void"              { return TYPE_VOID; }

/* --- Estruturas de Controle de Fluxo --- */
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"return"            { return RETURN; }

/* --- Valores Literais Booleanos --- */
"true"              { return TRUE; }
"false"             { return FALSE; }

/* --- Funções de Entrada / Saída --- */
"System.out.println" { return PRINTLN; }
"System.out.print"   { return PRINT; }
"System.out.printf"  { return PRINTF; }

/* --- Operadores Relacionais --- */
"=="                { return EQ; }
"!="                { return NE; }
"<="                { return LE; }
">="                { return GE; }
"<"                 { return LT; }
">"                 { return GT; }

/* --- Operadores Aritméticos e Atribuição --- */
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"="                 { return '='; }

/* --- Pontuação e Delimitadores --- */
";"                 { return ';'; }
","                 { return ','; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"["                 { return '['; }
"]"