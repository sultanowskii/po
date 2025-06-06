# Проект (Разработка компиляторов)

Студент: `Султанов Артур Радикович` (`367553`), группа `P3313`

## Описание

Po - язык, имеющий синтаксические свойства с Python и Golang.

Поддерживаемые типы: int, str

На данный момент разработан интерпретатор языка.

Ссылка на репозиторий: [https://github.com/sultanowskii/po](https://github.com/sultanowskii/po)

Сборка проекта:

```bash
git clone https://github.com/sultanowskii/po
cd po
make
```

Использование:

```bash
./poi run FILENAME # "запуск" (интерпретация) программы
./poi ast FILENAME # "отрисовка" AST в консоли
```

## Грамматика и более подробное описание

Определение токенов лексера:

```lex
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
```

Грамматика:

```bison
// Prologue
%{
    #include <stdio.h>
    #include <inttypes.h>

    #include "ast/ast.h"
    // Stuff from Flex that Bison needs to know about:
    extern int yylex();
    extern FILE *yyin;

    void yyerror(IDProvider *idp, Program **prog, const char *s);
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
%token PRINTS
%token PRINTI
%token PRINTC

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
%token <str_> LIT_STR

%token <str_> IDENTIFIER

%token EOL

// Types of non-terminal tokens.
%type <expr> expression expression_logic_term expression_compared_term expression_term expression_factor
%type <stmt> statement statement_new_variable statement_assign statement_cond statement_while statement_print
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
    | statement_print
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

statement_print
    : PRINTC L_PAREN expression R_PAREN { $$ = statement_create_print_char($3); }
    | PRINTI L_PAREN expression R_PAREN { $$ = statement_create_print_int($3); }
    | PRINTS L_PAREN expression R_PAREN { $$ = statement_create_print_str($3); }
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
    | LIT_STR                    { $$ = expression_create_literal(literal_create_str($1)); }
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
```

Кейворды:

- `if`/`else` - `else` является опциональным
- `while`
- `printi`, `printc`, `prints` - псевдофункции для вывода числа, символа и строки

Бинарные операции:

- `==`, `!=` - для сравнения
- `&&`, `||` - логические И, ИЛИ
- `+`, `-`, `/`, `*` - арифметика

Унарные операции:

- `!` - логическое НЕ

Для определения новой переменной используется walrus-оператор (`:=`), для присвоения нового значения - привычное `=`, по аналогии с Golang.

Statement-ы разделяются либо с помощью `;` (точка с запятой), либо переходом на новую строку.

Блоки statement-ов выделяются с помощью `{}` (фигурные скобки). Все блоки (включая те, что идут с `if`/`else`, `while`) являются отдельными областями видимости переменных. Демонстрация:

```po
q := 1
prints("Outside"); printi(q)
{
    q = q + 10
    q := 2
    prints("Inside"); printi(q)
}
prints("Outside again"); printi(q)
```

Вывод:

```bash
$ ./poi run examples/scopes.po
Outside                
1
Inside
2
Outside again
11
```

Строковые и числовые литералы задаются в привычном формате:

```po
number := 123456
string := "some text"
```

## Про реализацию

C, Flex, Bison.

Код разбит по "пакетам":

```text
ast/      # AST-ноды
compiler/ # наброски компилятора (нерабочие)
gen/      # сгенерированные C-файлы лексера и парсера
grammar/  # грамматика (.y, .l)
interp/   # код интерпретатора
std/      # вспомогательные функции для работы со строками, выводом
types/    # типы
```

## Примеры программ

Простая арифметика:

```po
c := (3 + 1) - (4 * 8)
printi(c)
```

```bash
$ ./poi run examples/math.po
-28
```

Показ встроенных функций вывода:

```po
printc(65); printc("B"); prints("CDE")
printi(123456)
```

```bash
$ ./poi run examples/print.po
ABCDE
123456
```

N-ое число Фибоначчи:

```po
n := 10

if (n == 0 || n == 1) {
    printi(n)
} else {
    cntr := 2
    sum := 1
    prev := 0
    curr := 1

    while (cntr != n) {
        sum = sum + curr
        tmp := curr + prev
        prev = curr
        curr = tmp
        cntr = cntr + 1
    }

    printi(sum)
}
```

```bash
$ ./poi run examples/fibonacci.po
55
```

Попытка определить уже существующую в этой области видимости переменную:

```po
a := 123

if (a == 123) {
    b := 456
    b := 789
}
```

```bash
$ ./poi run examples/already_defined.po
variable is already defined in the scope: b
```

Попытка присвоить переменной значение другого типа:

```po
variable := 2 + 2 * 2
variable = "asd"

prints(variable)
```

```bash
$ ./poi run examples/type_mismatch.po
type mismatch while trying to update variable 'variable': current: int, new: str
```
