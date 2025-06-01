%{
#include "gen/po.tab.h"

#include "compiler/ast/ast.h"
#include "str.h"

extern YYSTYPE yylval;
%}

%%
"if"    { return IF; }
"else"  { return ELSE; }
"while" { return WHILE; }

"("  { return L_PAREN; }
")"  { return R_PAREN; }
"{"  { return L_BRACE; }
"}"  { return R_BRACE; }
";"  { return SEMICOLON; }

":=" { return OP_WALRUS; }
"="  { return OP_ASSIGN; }

"+"  { yylval.bin_op_type = BINARY_OP_PLUS; return OP_PLUS; }
"-"  { yylval.bin_op_type = BINARY_OP_MINUS; return OP_MINUS; }
"*"  { yylval.bin_op_type = BINARY_OP_MUL; return OP_MUL; }
"/"  { yylval.bin_op_type = BINARY_OP_DIV; return OP_DIV; }

"==" { yylval.bin_op_type = BINARY_OP_EQUALS; return OP_EQUALS; }
"!=" { yylval.bin_op_type = BINARY_OP_NOT_EQUALS; return OP_NOT_EQUALS; }

[+-]?[1-9][0-9]*|0          { yylval.int_ = atoi(yytext); return LIT_INT; }
[+-]?([1-9][0-9]*[.])[0-9]+ { yylval.float_ = atof(yytext); return LIT_FLOAT; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str_ = strdup(yytext); return IDENTIFIER; }

[\n\r]+ { return EOL; }
%%
