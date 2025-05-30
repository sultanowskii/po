%{
#include <stdio.h>

// Stuff from Flex that Bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);
%}

%token IF
%token ELSE
%token WHILE

%token L_PAREN
%token R_PAREN
%token L_BRACE
%token R_BRACE
%token SEMICOLON

%token OP_WALRUS
%token OP_ASSIGN

%token OP_PLUS
%token OP_MINUS
%token OP_MUL
%token OP_DIV

%token OP_EQUALS
%token OP_NOT_EQUALS

%token LIT_INT
%token LIT_FLOAT

%token IDENTIFIER

%token EOL

%%
prog
    : statement_list YYEOF
    ;

statement_list
    : { puts("EMPTY STATEMENT LIST"); }
    | non_empty_statement_list
    ;

non_empty_statement_list
    : statement
    | non_empty_statement_list EOL statement
    | non_empty_statement_list SEMICOLON statement
    | non_empty_statement_list SEMICOLON EOL statement
    ;

statement
    : statement_new_variable
    | statement_assign
    | statement_if
    | statement_while
    | block
    ;

statement_new_variable
    : IDENTIFIER OP_WALRUS expression
    ;

statement_assign
    : IDENTIFIER OP_ASSIGN expression
    ;

statement_if
    : IF L_PAREN expression R_PAREN block
    | IF L_PAREN expression R_PAREN block ELSE block
    ;

statement_while
    : WHILE L_PAREN expression R_PAREN block
    ;

expression
    : expression_compared_term
    | expression OP_EQUALS expression_compared_term
    | expression OP_NOT_EQUALS expression_compared_term
    ;

expression_compared_term
    : expression_term
    | expression_compared_term OP_PLUS expression_term
    | expression_compared_term OP_MINUS expression_term
    ;

expression_term
    : expression_factor
    | expression_term OP_MUL expression_factor
    | expression_term OP_DIV expression_factor
    ;

expression_factor
    : LIT_FLOAT
    | LIT_INT
    | IDENTIFIER
    | L_PAREN expression R_PAREN
    ;

block
    : L_BRACE statement_list R_BRACE
    ;
%%

int main(int argc, char **argv) {
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}