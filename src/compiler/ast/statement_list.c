#include "compiler/ast/statement_list.h"

#include <malloc.h>
#include <stddef.h>

typedef struct StatementListNode StatementListNode;
struct StatementListNode {
    Statement         *stmt;
    StatementListNode *next;
};

struct StatementList {
    size_t             length;
    StatementListNode *head;
};

static inline StatementListNode *statement_list_node_create(Statement *stmt, StatementListNode *next) {
    StatementListNode *node = malloc(sizeof(StatementListNode));
    node->stmt = stmt;
    node->next = next;
    return node;
}

static inline void statement_list_node_destroy(StatementListNode *node) {
    statement_destroy(node->stmt);
    free(node);
}

StatementList *statement_list_create(void) {
    StatementList *sl = malloc(sizeof(StatementList));
    sl->length = 0;
    sl->head = NULL;
}

void statement_list_add_statement(StatementList *list, Statement *statement) {
    StatementListNode *new_node = statement_list_node_create(statement, NULL);
    StatementListNode *node = list->head;

    if (node == NULL) {
        list->head = new_node;
        list->length = 1;
        return;
    }

    while (node->next != NULL) {
        node = node->next;
    }

    node->next = new_node;
    list->length++;
}

void statement_list_destroy(StatementList *list) {
    StatementListNode *node = list->head;
    while (node != NULL) {
        StatementListNode *next = node->next;
        statement_list_node_destroy(node);
        node = next;
    }
    free(list);
}
