#pragma once

#include <stddef.h>

// Forward declarations
typedef struct StatementList StatementList;

struct Block {
    StatementList *statements;
};
typedef struct Block Block;

Block *block_create(StatementList *statement_list);
void   block_print(Block *block, size_t padding);
void   block_destroy(Block *block);
