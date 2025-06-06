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
#include "result.h"
#include "str.h"

DEFINE_RESULT(Offset, offset, Offset)

struct ScopeHierarchy {
    Map *scopes;
    // Read-only pointer to prog. Must not be freed.
    const Program *prog;
};

static OffsetResult traverse_block(Map *scopes, Block *block, Offset base_offset, uint32_t parent_id);

// validate_identifier validates that the variable is defined before usage.
//
// - Returns error message if something isn't right.
// - Returns NULL otherwise.
static inline char *validate_identifier(Map *scopes, Identifier *ident, uint32_t current_scope_id) {
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
        return msprintf("variable undefined: %s", var_name);
    }

    return NULL;
}

// validate_expression validates expression.
//
// - Returns error message if something isn't right.
// - Returns NULL otherwise.
static inline char *validate_expression(Map *scopes, Expression *expr, uint32_t current_scope_id) {
    char *err = NULL;
    switch (expr->type) {
    case EXPRESSION_UNARY_OP:
        err = validate_expression(scopes, expr->una_op->expr, current_scope_id);

        break;
    case EXPRESSION_BINARY_OP:
        err = validate_expression(scopes, expr->bin_op->left, current_scope_id);
        if (err != NULL) {
            break;
        }
        err = validate_expression(scopes, expr->bin_op->right, current_scope_id);
        break;
    case EXPRESSION_IDENTIFIER:
        err = validate_identifier(scopes, expr->ident, current_scope_id);
        break;
    }
    return err;
}

// calculate_required_stack_size_of_this_scope calculates required stack size for variables defined in given scope.
static inline OffsetResult calculate_required_stack_size_of_this_scope(
    Map           *scopes,
    StatementList *statement_list,
    uint32_t       scope_id
) {
    StatementListNode *node = statement_list->head;
    IGNORE_INT_TO_POINTER()
    Scope *scope = map_get_(scopes, scope_id);

    Offset current_offset = 0;
    while (node != NULL) {
        switch (node->stmt->type) {
        case STATEMENT_NEW_VARIABLE:
            char *maybe_error = validate_expression(scopes, node->stmt->new_variable.expr, scope_id);
            if (maybe_error != NULL) {
                return offset_result_err(maybe_error);
            }

            const char *var_name = node->stmt->new_variable.ident->name;
            if (scope_get_variable(scope, var_name) != NULL) {
                return offset_result_err(msprintf("variable is already declared in this scope: %s", var_name));
            }
            scope_add_variable(scope, var_name, current_offset);
            current_offset += 1; // TODO: type size
            break;
        }
        node = node->next;
    }

    return offset_result_ok(current_offset);
}

// calculate_required_stack_size_of_nested_scopes calculates
// minimal required stack size for variables defined in nested scopes.
static inline OffsetResult calculate_required_stack_size_of_nested_scopes(
    Map           *scopes,
    StatementList *statement_list,
    uint32_t       scope_id,
    Offset         start_offset
) {
    char *err = NULL;

    StatementListNode *node = statement_list->head;
    Offset             max_stack_size_of_nested_scopes = 0;
    OffsetResult       nested_scope_size_res;
    while (node != NULL) {
        switch (node->stmt->type) {
        case STATEMENT_ASSIGN:
            err = validate_identifier(scopes, node->stmt->assign.ident, scope_id);
            if (err != NULL) {
                return offset_result_err(err);
            }
            break;
        case STATEMENT_IF:
            err = validate_expression(scopes, node->stmt->if_.cond, scope_id);
            if (err != NULL) {
                return offset_result_err(err);
            }

            nested_scope_size_res = traverse_block(scopes, node->stmt->if_.if_block, start_offset, scope_id);
            if (offset_result_is_err(nested_scope_size_res)) {
                return nested_scope_size_res;
            }
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, offset_result_get_val(nested_scope_size_res));
            break;
        case STATEMENT_IF_ELSE:
            err = validate_expression(scopes, node->stmt->if_else.cond, scope_id);
            if (err != NULL) {
                return offset_result_err(err);
            }

            nested_scope_size_res = traverse_block(scopes, node->stmt->if_else.if_block, start_offset, scope_id);
            if (offset_result_is_err(nested_scope_size_res)) {
                return nested_scope_size_res;
            }
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, offset_result_get_val(nested_scope_size_res));

            nested_scope_size_res = traverse_block(scopes, node->stmt->if_else.else_block, start_offset, scope_id);
            if (offset_result_is_err(nested_scope_size_res)) {
                return nested_scope_size_res;
            }
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, offset_result_get_val(nested_scope_size_res));
            break;
        case STATEMENT_WHILE:
            err = validate_expression(scopes, node->stmt->while_.cond, scope_id);
            if (err != NULL) {
                return offset_result_err(err);
            }

            nested_scope_size_res = traverse_block(scopes, node->stmt->while_.block, start_offset, scope_id);
            if (offset_result_is_err(nested_scope_size_res)) {
                return nested_scope_size_res;
            }
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, offset_result_get_val(nested_scope_size_res));
            break;
        case STATEMENT_BLOCK:
            nested_scope_size_res = traverse_block(scopes, node->stmt->block.block, start_offset, scope_id);
            if (offset_result_is_err(nested_scope_size_res)) {
                return nested_scope_size_res;
            }
            max_stack_size_of_nested_scopes = MAX(max_stack_size_of_nested_scopes, offset_result_get_val(nested_scope_size_res));
            break;
        }

        node = node->next;
    }

    return offset_result_ok(max_stack_size_of_nested_scopes);
}

// traverse_block traverses block and builds scopes: its own and of all children blocks.
//
// Returns minimal required size on stack.
static inline OffsetResult traverse_block(Map *scopes, Block *block, int32_t base_offset, uint32_t parent_id) {
    Scope *scope = scope_create(block->id, base_offset);
    map_set_(scopes, block->id, scope);
    scope->parent_id = parent_id;

    Scope *parent_scope = map_get_(scopes, parent_id);
    if (parent_scope != NULL) {
        scope_add_scope(parent_scope, block->id);
    }

    OffsetResult current_size_res = calculate_required_stack_size_of_this_scope(scopes, block->statements, block->id);
    if (offset_result_is_err(current_size_res)) {
        return current_size_res;
    }
    Offset current_size = offset_result_get_val(current_size_res);

    OffsetResult nested_size_res = calculate_required_stack_size_of_nested_scopes(
        scopes,
        block->statements,
        block->id,
        current_size
    );
    if (offset_result_is_err(nested_size_res)) {
        return nested_size_res;
    }
    Offset nested_size = offset_result_get_val(nested_size_res);

    scope->required_size = current_size + nested_size;

    return offset_result_ok(current_size + nested_size);
}

ScopeHierarchyBuildResult scope_hierarchy_build(const Program *prog) {
    Map *scopes = map_create(
        hashf_uint32,
        (PrintFunction)uint32_print,
        destroy_nop,
        (PrintFunction)scope_print,
        (DestroyFunction)scope_destroy
    );
    Scope *scope = scope_create(prog->id, 0);
    map_set_(scopes, prog->id, scope);

    OffsetResult current_size_res = calculate_required_stack_size_of_this_scope(scopes, prog->statements, prog->id);
    if (offset_result_is_err(current_size_res)) {
        return scope_hierarchy_build_result_err(offset_result_get_err(current_size_res));
    }
    Offset current_size = offset_result_get_val(current_size_res);

    OffsetResult nested_size_res = calculate_required_stack_size_of_nested_scopes(
        scopes,
        prog->statements,
        prog->id,
        current_size
    );
    if (offset_result_is_err(nested_size_res)) {
        return scope_hierarchy_build_result_err(offset_result_get_err(nested_size_res));
    }
    Offset nested_size = offset_result_get_val(nested_size_res);

    scope->required_size = current_size + nested_size;

    ScopeHierarchy *sh = malloc(sizeof(ScopeHierarchy));
    *sh = (ScopeHierarchy){
        .prog = prog,
        .scopes = scopes,
    };

    return scope_hierarchy_build_result_ok(sh);
}

static inline void _scope_hierarchy_print(const ScopeHierarchy *sh, size_t padding, uint32_t scope_id) {
    Scope *scope = map_get_(sh->scopes, scope_id);
    scope_print(scope, padding);

    print_padding(padding + 1);
    puts("scopes:");

    for (size_t i = 0; i < vec_length(scope->scopes); i++) {
        void *maybe_inner_scope_id = vec_get_at(scope->scopes, i);
        if (maybe_inner_scope_id == NULL) {
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
