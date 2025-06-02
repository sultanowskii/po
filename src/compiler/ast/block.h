#pragma once

#include <stddef.h>

#include "compiler/id_provider.h"

// Forward declarations
typedef struct StatementList StatementList;

struct Block {
    size_t         id;
    StatementList *statements;
};
typedef struct Block Block;

Block *block_create(IDProvider *idp, StatementList *statement_list);
void   block_print(Block *block, size_t padding);
void   block_destroy(Block *block);
