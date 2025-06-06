// Prologue
%{
    #include <stdio.h>
    #include <inttypes.h>

    #include "ast/ast.h"
    // Stuff from Flex that Bison needs to know about:
    extern int yylex();
    extern FILE *yyin;

    void yyerror(IDProvider *idp, Program **prog, const char *s);

    int yydebug = 1;
    #define YYDEBUG 1
%}

// Required includes for all generated files
// Without this, generated header won't #include ast.h.
%code requires {
    #include "ast/ast.h"
}

// Enabling verbose errors
%define parse.error verbose

// Add yyparse() param - the program itself.
%parse-param {IDProvider *idp} {Program **program}

// YYSTYPE definition. Is used in Lexer (yylval) and here
%union {
    UnaryOpType una_op_type;
    BinaryOpType bin_op_type;
    Expression *expr;
    Program *program;
    StatementList *stmt_list;
    Statement *stmt;
    Block *block;

    int32_t int_;
    char *str_;
}

// Tokens
%token IF
%token ELSE
%token WHILE

%token L_PAREN
%token R_PAREN
%token L_BRACE
%token R_BRACE
%token SEMICOLON

%token <bin_op_type> OP_WALRUS
%token <bin_op_type> OP_ASSIGN

%token <bin_op_type> OP_PLUS
%token <bin_op_type> OP_MINUS
%token <bin_op_type> OP_MUL
%token <bin_op_type> OP_DIV
%token <bin_op_type> OP_AND
%token <bin_op_type> OP_OR
%token <bin_op_type> OP_EQUALS
%token <bin_op_type> OP_NOT_EQUALS

%token <una_op_type> OP_NOT

%token <int_> LIT_INT

%token <str_> IDENTIFIER

%token EOL

// Types of non-terminal tokens.
%type <expr> expression expression_logic_term expression_compared_term expression_term expression_factor
%type <stmt> statement statement_new_variable statement_assign statement_cond statement_while
%type <stmt_list> statement_list non_empty_statement_list
%type <block> block

%%
prog
    : delimiter_optional statement_list YYEOF { *program = program_create(idp, $2); }
    ;

statement_list
    :                                    { $$ = statement_list_create(); }
    | non_empty_statement_list delimiter
    | non_empty_statement_list
    ;

non_empty_statement_list
    : statement                                    { $$ = statement_list_create(); statement_list_add_statement($$, $1); }
    | non_empty_statement_list delimiter statement { statement_list_add_statement($1, $3); }
    ;

delimiter
    : EOL
    | SEMICOLON
    | delimiter EOL
    | delimiter SEMICOLON
    ;

delimiter_optional
    :
    | delimiter
    ;

statement
    : statement_new_variable
    | statement_assign
    | statement_cond
    | statement_while
    | block                  { $$ = statement_create_block($1); }
    ;

statement_new_variable
    : IDENTIFIER OP_WALRUS expression { $$ = statement_create_new_variable(identifier_create($1), $3); }
    ;

statement_assign
    : IDENTIFIER OP_ASSIGN expression { $$ = statement_create_assign(identifier_create($1), $3); }
    ;

statement_cond
    : IF L_PAREN expression R_PAREN block            { $$ = statement_create_if($3, $5); }
    | IF L_PAREN expression R_PAREN block ELSE block { $$ = statement_create_if_else($3, $5, $7); }
    ;

statement_while
    : WHILE L_PAREN expression R_PAREN block { $$ = statement_create_while($3, $5); }
    ;

expression
    : expression_logic_term
    | expression OP_AND expression_logic_term { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    | expression OP_OR expression_logic_term  { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    ;

expression_logic_term
    : expression_compared_term
    | expression_logic_term OP_EQUALS expression_compared_term     { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    | expression_logic_term OP_NOT_EQUALS expression_compared_term { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    ;

expression_compared_term
    : expression_term
    | expression_compared_term OP_PLUS expression_term  { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    | expression_compared_term OP_MINUS expression_term { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    ;

expression_term
    : expression_factor
    | expression_term OP_MUL expression_factor { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    | expression_term OP_DIV expression_factor { $$ = expression_create_binary_op(binary_op_create($2, $1, $3)); }
    ;

expression_factor
    : LIT_INT                    { $$ = expression_create_literal(literal_create_int($1)); }
    | IDENTIFIER                 { $$ = expression_create_identifier(identifier_create($1)); }
    | OP_NOT expression_factor   { $$ = expression_create_unary_op(unary_op_create($1, $2)); }
    | L_PAREN expression R_PAREN { $$ = $2; }
    ;

block
    : L_BRACE delimiter_optional statement_list R_BRACE { $$ = block_create(idp, $3); }
    ;
%%

void yyerror(IDProvider *idp, Program **prog, const char *s) {
    fprintf(stderr, "error: %s\n", s);
}
