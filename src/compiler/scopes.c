#include "compiler/scopes.h"

#include <inttypes.h>

#include "compiler/ast/binary_op.h"
#include "compiler/ast/block.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/program.h"
#include "compiler/ast/unary_op.h"
#include "compiler/id_provider.h"
#include "compiler/scope.h"
#include "container/map.h"
#include "math.h"
#include "str.h"

#define IGNORE_INT_TO_POINTER() _Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"")
#define map_set_(m, k, v) map_set((m), (void *)(k), (void *)(v))
#define map_get_(m, k) map_get((m), (void *)(k))

static int32_t traverse_block(Map *scopes, Block *block, int32_t base_offset, uint32_t parent_id);

static inline void validate_identifier(Map *scopes, Identifier *ident, uint32_t current_scope_id) {
    const char *var_name = ident->name;

    uint32_t scope_id = current_scope_id;
    while (scope_id != ID_PROVIDER_INVALID_ID) {
        IGNORE_INT_TO_POINTER()
        Scope *tmp = map_get_(scopes, scope_id);
        if (map_get_(tmp->vars, var_name) != NULL) {
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
    }
}

// calculate_required_stack_size_of_this_scope calculates required stack size for variables defined in given scope.
static inline int32_t calculate_required_stack_size_of_this_scope(
    Map           *scopes,
    StatementList *statement_list,
    uint32_t       scope_id
) {
    StatementListNode *node = statement_list->head;
    Scope             *scope = map_get_(scopes, scope_id);

    int32_t current_offset = 0;
    while (node != NULL) {
        switch (node->stmt->type) {
        case STATEMENT_NEW_VARIABLE:
            validate_expression(scopes, node->stmt->new_variable.expr, scope_id);

            const char *var_name = node->stmt->new_variable.ident->name;
            if (map_get_(scope->vars, var_name) != NULL) {
                // TODO: error: variable is already declared in this scope
                return -100500;
            }
            IGNORE_INT_TO_POINTER()
            map_set_(scope->vars, strdup(var_name), current_offset);
            current_offset += 1; // TODO: type size
            break;
        }
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
    StatementListNode *node = statement_list->head;
    int32_t            max_stack_size_of_nested_scopes = 0;
    int32_t            nested_scope_size = 0;
    while (node != NULL) {
        switch (node->stmt->type) {
        case STATEMENT_ASSIGN:
            validate_identifier(scopes, node->stmt->new_variable.ident, scope_id);
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

    int32_t current_size = calculate_required_stack_size_of_this_scope(scopes, block->statements, block->id);
    int32_t nested_size = calculate_required_stack_size_of_nested_scopes(scopes, block->statements, block->id, current_size);

    scope->required_size = current_size + nested_size;

    return current_size + nested_size;
}

Map *scopes_build(Program *prog) {
    Map   *scopes = map_create(hashf_uint32, destroy_nop, (DestroyFunction)scope_destroy);
    Scope *scope = scope_create(prog->id, 0);
    map_set_(scopes, prog->id, scope);

    int32_t current_size = calculate_required_stack_size_of_this_scope(scopes, prog->statements, prog->id);
    int32_t nested_size = calculate_required_stack_size_of_nested_scopes(scopes, prog->statements, prog->id, current_size);

    scope->required_size = current_size + nested_size;

    return scopes;
}

void destroy_scopes(Map *scopes) {
    map_destroy(scopes);
}
