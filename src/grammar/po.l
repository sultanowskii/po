%{
#include "gen/po.tab.h"

extern int yylval;
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

"+"  { return OP_PLUS; }
"-"  { return OP_MINUS; }
"*"  { return OP_MUL; }
"/"  { return OP_DIV; }

"==" { return OP_EQUALS; }
"!=" { return OP_NOT_EQUALS; }

[+-]?[1-9][0-9]*|0          { yylval = atoi(yytext); return LIT_INT; }
[+-]?([1-9][0-9]*[.])[0-9]+ { yylval = atof(yytext); return LIT_FLOAT; }

[a-zA-Z_][a-zA-Z0-9_]* { return IDENTIFIER; }

[\n\r]+ { return EOL; }
%%
