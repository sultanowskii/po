#include "compiler/scope_hierarchy.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

#include "compile.h"
#include "compiler/ast/binary_op.h"
#include "compiler/ast/block.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/program.h"
#include "compiler/ast/unary_op.h"
#include "compiler/id_provider.h"
#include "compiler/scope.h"
#include "container/map.h"
#include "container/vec.h"
#include "fmt.h"
#include "math.h"
#include "str.h"

struct ScopeHierarchy {
    Map *scopes;
    // Read-only pointer to prog. Must not be freed.
    const Program *prog;
};

static int32_t traverse_block(Map *scopes, Block *block, int32_t base_offset, uint32_t parent_id);

static inline void validate_identifier(Map *scopes, Identifier *ident, uint32_t current_scope_id) {
    printf("validate_identifier(%s, %" PRIu32 ")\n", ident->name, current_scope_id);
    const char *var_name = ident->name;

    uint32_t scope_id = current_scope_id;
    while (scope_id != ID_PROVIDER_INVALID_ID) {
        IGNORE_INT_TO_POINTER()
        Scope *tmp = map_get_(scopes, scope_id);
        if (scope_get_variable(tmp, var_name) != NULL) {
            break;
        }
        scope_id = tmp->parent_id;
    }

    if (scope_id == ID_PROVIDER_INVALID_ID) {
        // TODO: error: variable undefined
        return;
    }
}

static inline void validate_expression(Map *scopes, Expression *expr, uint32_t current_scope_id) {
    printf("validate_expression(some-expr, %" PRIu32 ")\n", current_scope_id);
    switch (expr->type) {
    case EXPRESSION_UNARY_OP:
        validate_expression(scopes, expr->una_op->expr, current_scope_id);
        break;
    case EXPRESSION_BINARY_OP:
        validate_expression(scopes, expr->bin_op->left, current_scope_id);
        validate_expression(scopes, expr->bin_op->right, current_scope_id);
        break;
    case EXPRESSION_LITERAL:
        break;
    case EXPRESSION_IDENTIFIER:
        validate_identifier(scopes, expr->ident, current_scope_id);
        break;
    }
}

// calculate_required_stack_size_of_this_scope calculates required stack size for variables defined in given scope.
static inline int32_t calculate_required_stack_size_of_this_scope(
    Map           *scopes,
    StatementList *statement_list,
    uint32_t       scope_id
) {
    printf("calculate_required_stack_size_of_this_scope(%" PRIu32 ")\n", scope_id);
    StatementListNode *node = statement_list->head;
    IGNORE_INT_TO_POINTER()
    Scope *scope = map_get_(scopes, scope_id);

    int32_t current_offset = 0;
    while (node != NULL) {
        switch (node->stmt->type) {
        case STATEMENT_NEW_VARIABLE:
            validate_expression(scopes, node->stmt->new_variable.expr, scope_id);

            const char *var_name = node->stmt->new_variable.ident->name;
            if (scope_get_variable(scope, var_name) != NULL) {
                // TODO: error: variable is already declared in this scope
                return -100500;
            }
            scope_add_variable(scope, var_name, current_offset);
            current_offset += 1; // TODO: type size
            break;
        }
        node = node->next;
    }

    return current_offset;
}

// calculate_required_stack_size_of_nested_scopes calculates
// minimal required stack size for variables defined in nested scopes.
static inline int32_t calculate_required_stack_size_of_nested_scopes(
    Map           *scopes,
    StatementList *statement_list,
    uint32_t       scope_id,
    int32_t        start_offset
) {
    printf("calculate_required_stack_size_of_nested_scopes(%" PRIu32 ")\n", scope_id);
    StatementListNode *node = statement_list->head;
    int32_t            max_stack_size_of_nested_scopes = 0;
    int32_t            nested_scope_size = 0;
    while (node != NULL) {
        switch (node->stmt->type) {
        case STATEMENT_ASSIGN:
            validate_identifier(scopes, node->stmt->assign.ident, scope_id);
            break;
        case STATEMENT_IF:
            validate_expression(scopes, node->stmt->if_.cond, scope_id);

            nested_scope_size = traverse_block(scopes, node->stmt->if_.if_block, start_offset, scope_id);
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, nested_scope_size);
            break;
        case STATEMENT_IF_ELSE:
            validate_expression(scopes, node->stmt->if_else.cond, scope_id);

            nested_scope_size = traverse_block(scopes, node->stmt->if_else.if_block, start_offset, scope_id);
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, nested_scope_size);

            nested_scope_size = traverse_block(scopes, node->stmt->if_else.else_block, start_offset, scope_id);
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, nested_scope_size);
            break;
        case STATEMENT_WHILE:
            validate_expression(scopes, node->stmt->while_.cond, scope_id);

            nested_scope_size = traverse_block(scopes, node->stmt->while_.block, start_offset, scope_id);
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, nested_scope_size);
            break;
        case STATEMENT_BLOCK:
            nested_scope_size = traverse_block(scopes, node->stmt->block.block, start_offset, scope_id);
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, nested_scope_size);
            break;
        }

        node = node->next;
    }
}

// traverse_block traverses block and builds scopes: its own and of all children blocks.
//
// Returns minimal required size on stack.
static inline int32_t traverse_block(Map *scopes, Block *block, int32_t base_offset, uint32_t parent_id) {
    Scope *scope = scope_create(block->id, base_offset);
    map_set_(scopes, block->id, scope);
    scope->parent_id = parent_id;

    Scope *parent_scope = map_get_(scopes, parent_id);
    if (parent_scope != NULL) {
        scope_add_scope(parent_scope, block->id);
    }

    printf("traverse_block(%" PRIu32 ")\n", block->id);

    int32_t current_size = calculate_required_stack_size_of_this_scope(scopes, block->statements, block->id);
    int32_t nested_size = calculate_required_stack_size_of_nested_scopes(
        scopes,
        block->statements,
        block->id,
        current_size
    );

    scope->required_size = current_size + nested_size;

    return current_size + nested_size;
}

ScopeHierarchy *scope_hierarchy_build(const Program *prog) {
    Map *scopes = map_create(
        hashf_uint32,
        (PrintFunction)uint32_print,
        destroy_nop,
        (PrintFunction)scope_print,
        (DestroyFunction)scope_destroy
    );
    Scope *scope = scope_create(prog->id, 0);
    map_set_(scopes, prog->id, scope);

    int32_t current_size = calculate_required_stack_size_of_this_scope(scopes, prog->statements, prog->id);
    int32_t nested_size = calculate_required_stack_size_of_nested_scopes(
        scopes,
        prog->statements,
        prog->id,
        current_size
    );

    scope->required_size = current_size + nested_size;

    ScopeHierarchy *sh = malloc(sizeof(ScopeHierarchy));
    *sh = (ScopeHierarchy){
        .prog = prog,
        .scopes = scopes,
    };
    return sh;
}

static inline void _scope_hierarchy_print(const ScopeHierarchy *sh, size_t padding, uint32_t scope_id) {
    Scope *scope = map_get_(sh->scopes, scope_id);
    scope_print(scope, padding);

    print_padding(padding + 1);
    puts("scopes:");

    for (size_t i = 0; i < vec_length(scope->scopes); i++) {
        void *maybe_inner_scope_id = vec_get_at(scope->scopes, i);
        if (maybe_inner_scope_id == NULL) {
            puts("null :(");
            continue;
        }
        IGNORE_POINTER_TO_INT()
        _scope_hierarchy_print(sh, padding + 2, (uint32_t)maybe_inner_scope_id);
    }
}

void scope_hierarchy_print(const ScopeHierarchy *sh, size_t padding) {
    print_padding(padding);
    puts("ScopeHierarchy");

    print_padding(padding + 1);
    puts("data:");

    _scope_hierarchy_print(sh, padding + 2, sh->prog->id);
}

void scope_hierarchy_destroy(ScopeHierarchy *sh) {
    map_destroy(sh->scopes);
    free(sh);
}
