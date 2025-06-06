#pragma once

#include <stddef.h>

#include "ast/statement.h"

typedef struct StatementListNode StatementListNode;
struct StatementListNode {
    Statement         *stmt;
    StatementListNode *next;
};

struct StatementList {
    size_t             length;
    StatementListNode *head;
};
typedef struct StatementList StatementList;

StatementList *statement_list_create(void);
void           statement_list_add_statement(StatementList *list, Statement *statement);
void           statement_list_print(StatementList *list, size_t padding);
void           statement_list_destroy(StatementList *list);
