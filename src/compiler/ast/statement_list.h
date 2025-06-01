#pragma once

#include "compiler/ast/statement.h"

struct StatementList;
typedef struct StatementList StatementList;

StatementList *statement_list_create(void);
void           statement_list_add_statement(StatementList *list, Statement *statement);
void           statement_list_destroy(StatementList *list);
