/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_GEN_PO_TAB_H_INCLUDED
# define YY_YY_SRC_GEN_PO_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 16 "src/grammar/po.y"

    #include "ast/ast.h"

#line 53 "src/gen/po.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    WHILE = 260,                   /* WHILE  */
    PRINTS = 261,                  /* PRINTS  */
    PRINTI = 262,                  /* PRINTI  */
    PRINTC = 263,                  /* PRINTC  */
    L_PAREN = 264,                 /* L_PAREN  */
    R_PAREN = 265,                 /* R_PAREN  */
    L_BRACE = 266,                 /* L_BRACE  */
    R_BRACE = 267,                 /* R_BRACE  */
    SEMICOLON = 268,               /* SEMICOLON  */
    OP_WALRUS = 269,               /* OP_WALRUS  */
    OP_ASSIGN = 270,               /* OP_ASSIGN  */
    OP_PLUS = 271,                 /* OP_PLUS  */
    OP_MINUS = 272,                /* OP_MINUS  */
    OP_MUL = 273,                  /* OP_MUL  */
    OP_DIV = 274,                  /* OP_DIV  */
    OP_AND = 275,                  /* OP_AND  */
    OP_OR = 276,                   /* OP_OR  */
    OP_EQUALS = 277,               /* OP_EQUALS  */
    OP_NOT_EQUALS = 278,           /* OP_NOT_EQUALS  */
    OP_NOT = 279,                  /* OP_NOT  */
    LIT_INT = 280,                 /* LIT_INT  */
    LIT_STR = 281,                 /* LIT_STR  */
    IDENTIFIER = 282,              /* IDENTIFIER  */
    EOL = 283                      /* EOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "src/grammar/po.y"

    UnaryOpType una_op_type;
    BinaryOpType bin_op_type;
    Expression *expr;
    Program *program;
    StatementList *stmt_list;
    Statement *stmt;
    Block *block;

    int32_t int_;
    char *str_;

#line 111 "src/gen/po.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (IDProvider *idp, Program **program);


#endif /* !YY_YY_SRC_GEN_PO_TAB_H_INCLUDED  */
