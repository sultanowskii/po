#include "compiler/ast/block.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/statement_list.h"
#include "compiler/id_provider.h"
#include "fmt.h"

Block *block_create(IDProvider *idp, StatementList *statement_list) {
    Block *block = malloc(sizeof(Block));
    block->statements = statement_list;
    block->id = id_provider_get(idp);
    return block;
}

void block_destroy(Block *block) {
    statement_list_destroy(block->statements);
    free(block);
}

void block_print(Block *block, size_t padding) {
    print_padding(padding);
    printf("Block[id=%zu]\n", block->id);

    print_padding(padding + 1);
    puts("statements:");
    statement_list_print(block->statements, padding + 2);
}
