%{
    #include "gen/po.tab.h"

    #include "ast/ast.h"
    #include "std/str.h"

    extern YYSTYPE yylval;
%}

%%
"if"    { return IF; }
"else"  { return ELSE; }
"while" { return WHILE; }
"prints" { return PRINTS; }
"printi" { return PRINTI; }
"printc" { return PRINTC; }

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
"&&"  { yylval.bin_op_type = BINARY_OP_AND; return OP_AND; }
"||"  { yylval.bin_op_type = BINARY_OP_OR; return OP_OR; }

"==" { yylval.bin_op_type = BINARY_OP_EQUALS; return OP_EQUALS; }
"!=" { yylval.bin_op_type = BINARY_OP_NOT_EQUALS; return OP_NOT_EQUALS; }

"!" { yylval.una_op_type = UNARY_OP_NOT; return OP_NOT; }

[+-]?[1-9][0-9]*|0   { yylval.int_ = atoi(yytext); return LIT_INT; }
\"([^\"\n])*\"       {
                        yylval.str_ = strdup(yytext + 1);
                        yylval.str_[strlen(yylval.str_) - 1] = '\0';
                        return LIT_STR;
                     }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str_ = strdup(yytext); return IDENTIFIER; }

[\n\r]+ { return EOL; }
%%
