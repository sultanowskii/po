#include "compiler/ast/block.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/statement_list.h"
#include "fmt.h"

struct Block {
    StatementList *statements;
};

Block *block_create(StatementList *statement_list) {
    Block *block = malloc(sizeof(Block));
    block->statements = statement_list;
    return block;
}

void block_destroy(Block *block) {
    statement_list_destroy(block->statements);
    free(block);
}

void block_print(Block *block, size_t padding) {
    print_padding(padding);
    puts("Block");

    print_padding(padding + 1);
    puts("statements:");
    statement_list_print(block->statements, padding + 2);
}
