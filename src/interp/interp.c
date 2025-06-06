#include "interp/interp.h"

#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"
#include "container/map.h"
#include "container/stack.h"
#include "id_provider.h"
#include "interp/scope.h"
#include "std/result.h"
#include "std/str.h"

DEFINE_RESULT(Value, value, Value *)

ValueResult interpret_expression(Stack *scopes, Expression *expr);
char       *interpret_block(Stack *scopes, Block *block);

Scope *find_variable_scope(const Stack *scopes, const char *name) {
    StackNode *node = scopes->head;
    while (node != NULL) {
        Scope *scope = node->val;
        if (scope_get_var(scope, name) != NULL) {
            return scope;
        }
        node = node->next;
    }
    return NULL;
}

ValueResult interpret_unary_op(Stack *scopes, UnaryOp *una_op) {
    ValueResult res = interpret_expression(scopes, una_op->expr);
    if (value_result_is_err(res)) {
        return res;
    }

    Value *val = value_result_get_val(res);
    if (val->type == TYPE_STR) {
        return value_result_err(
            msprintf(
                "unary operation '%s' is not applicable to type %s",
                UNARY_OP_TYPE_STR[una_op->op],
                VALUE_TYPE_STR[TYPE_STR]
            )
        );
    }

    switch (una_op->op) {
    case UNARY_OP_NOT:
        switch (val->type) {
        case TYPE_INT:
            val->int_ = !val->int_;
            return value_result_ok(val);
        }
        break;
    }

    return value_result_err(msprintf("unary operation: unexpected state"));
}

static inline bool as_bool(const Value *value) {
    switch (value->type) {
    case TYPE_INT:
        return value->int_ != 0;
    case TYPE_STR:
        return *value->str != '\0';
    }
    return false;
}

// the types are expected to be equal
static inline bool equal(Value *a, Value *b) {
    switch (a->type) {
    case TYPE_INT:
        return a->int_ == b->int_;
    case TYPE_STR:
        return strcmp(a->str, b->str);
    }
    return false;
}

ValueResult interpret_binary_op(Stack *scopes, BinaryOp *bin_op) {
    ValueResult res_left = interpret_expression(scopes, bin_op->left);
    if (value_result_is_err(res_left)) {
        return res_left;
    }

    ValueResult res_right = interpret_expression(scopes, bin_op->right);
    if (value_result_is_err(res_right)) {
        return res_right;
    }

    Value *left = value_result_get_val(res_left);
    Value *right = value_result_get_val(res_right);

    if (left->type != right->type) {
        goto ERR_MISMATCH;
    }

    switch (bin_op->op) {
    case BINARY_OP_PLUS:
    case BINARY_OP_MINUS:
    case BINARY_OP_MUL:
    case BINARY_OP_DIV:
        if (left->type == TYPE_STR || right->type == TYPE_STR) {
            goto ERR_MISMATCH;
        }
        break;
    }

    switch (bin_op->op) {
    case BINARY_OP_AND:
        return value_result_ok(value_create_int(as_bool(left) && as_bool(right)));
    case BINARY_OP_OR:
        return value_result_ok(value_create_int(as_bool(left) || as_bool(right)));
    case BINARY_OP_EQUALS:
        return value_result_ok(value_create_int(equal(left, right)));
    case BINARY_OP_NOT_EQUALS:
        return value_result_ok(value_create_int(!equal(left, right)));
    case BINARY_OP_PLUS:
        switch (left->type) {
        case TYPE_INT:
            return value_result_ok(value_create_int(left->int_ + right->int_));
        }
        break;
    case BINARY_OP_MINUS:
        switch (left->type) {
        case TYPE_INT:
            return value_result_ok(value_create_int(left->int_ - right->int_));
        }
        break;
    case BINARY_OP_MUL:
        switch (left->type) {
        case TYPE_INT:
            return value_result_ok(value_create_int(left->int_ * right->int_));
        }
        break;
    case BINARY_OP_DIV:
        switch (left->type) {
        case TYPE_INT:
            if (right->int_ == 0) {
                return value_result_err(msprintf("division by zero"));
            }
            return value_result_ok(value_create_int(left->int_ / right->int_));
        }
        break;
    }

ERR_UNEXPECTED_STATE:
    return value_result_err(msprintf("binary operation: unexpected state"));

ERR_MISMATCH:
    return value_result_err(
        msprintf(
            "binary operation '%s' is not applicable between types '%s' and %s",
            BINARY_OP_TYPE_STR[bin_op->op],
            VALUE_TYPE_STR[left->type],
            VALUE_TYPE_STR[right->type]
        )
    );
}

ValueResult interpret_literal(Stack *scopes, Literal *lit) {
    switch (lit->type) {
    case LITERAL_INT:
        return value_result_ok(value_create_int(lit->int_));
    case LITERAL_STR:
        return value_result_ok(value_create_str(lit->str));
    }
    return value_result_err(msprintf("literal: unexpected state"));
}

ValueResult interpret_identifier(Stack *scopes, Identifier *ident) {
    Scope *scope = find_variable_scope(scopes, ident->name);
    if (scope == NULL) {
        return value_result_err(msprintf("variable undefined: %s", ident->name));
    }
    Value *value = scope_get_var(scope, ident->name);
    if (value == NULL) {
        return value_result_err(msprintf("variable undefined: %s", ident->name));
    }
    return value_result_ok(value);
}

ValueResult interpret_expression(Stack *scopes, Expression *expr) {
    switch (expr->type) {
    case EXPRESSION_UNARY_OP:
        return interpret_unary_op(scopes, expr->una_op);
    case EXPRESSION_BINARY_OP:
        return interpret_binary_op(scopes, expr->bin_op);
    case EXPRESSION_LITERAL:
        return interpret_literal(scopes, expr->literal);
    case EXPRESSION_IDENTIFIER:
        return interpret_identifier(scopes, expr->ident);
    }
    return value_result_err(msprintf("unexpected expression type"));
}

char *interpret_statement(Stack *scopes, Statement *stmt) {
    Scope      *scope = stack_top(scopes);
    ValueResult res;
    Value      *value = NULL;
    char       *err = NULL;

    switch (stmt->type) {
    case STATEMENT_NEW_VARIABLE:
        res = interpret_expression(scopes, stmt->new_variable.expr);
        if (value_result_is_err(res)) {
            return value_result_get_err(res);
        }
        value = value_result_get_val(res);
        scope_add_var(scope, stmt->new_variable.ident->name, value);
        break;
    case STATEMENT_ASSIGN:
        Scope *target_scope = find_variable_scope(scopes, stmt->assign.ident->name);
        if (target_scope == NULL) {
            return msprintf("variable undefined: %s", stmt->assign.ident->name);
        }

        res = interpret_expression(scopes, stmt->new_variable.expr);
        if (value_result_is_err(res)) {
            return value_result_get_err(res);
        }
        value = value_result_get_val(res);

        err = scope_update_var(target_scope, stmt->assign.ident->name, value);
        if (err != NULL) {
            return err;
        }
        break;
    case STATEMENT_IF:
        res = interpret_expression(scopes, stmt->if_.cond);
        if (value_result_is_err(res)) {
            return value_result_get_err(res);
        }
        value = value_result_get_val(res);

        if (value->int_) {
            err = interpret_block(scopes, stmt->if_.if_block);
            if (err != NULL) {
                return err;
            }
        }
        break;
    case STATEMENT_IF_ELSE:
        res = interpret_expression(scopes, stmt->if_else.cond);
        if (value_result_is_err(res)) {
            return value_result_get_err(res);
        }
        value = value_result_get_val(res);

        if (value->int_) {
            err = interpret_block(scopes, stmt->if_else.if_block);
            if (err != NULL) {
                return err;
            }
        } else {
            err = interpret_block(scopes, stmt->if_else.else_block);
            if (err != NULL) {
                return err;
            }
        }
        break;
    case STATEMENT_WHILE:
        bool ok = true;
        do {
            ValueResult cond_res = interpret_expression(scopes, stmt->while_.cond);
            if (value_result_is_err(res)) {
                return value_result_get_err(res);
            }
            Value *cond_value = value_result_get_val(res);
            if (!cond_value->int_) {
                break;
            }

            err = interpret_block(scopes, stmt->while_.block);
            if (err != NULL) {
                return err;
            }
        } while (ok);
        break;
    case STATEMENT_BLOCK:
        err = interpret_block(scopes, stmt->block.block);
        if (err != NULL) {
            return err;
        }
        break;
    }
}

char *interpret_block(Stack *scopes, Block *block) {
    char *err = NULL;
    stack_push(scopes, scope_create());

    StatementListNode *node = block->statements->head;

    while (node != NULL) {
        err = interpret_statement(scopes, node->stmt);
        if (err != NULL) {
            goto RET;
        }
        node = node->next;
    }
RET:
    stack_pop(scopes);
    return err;
}

char *interpret(Program *prog) {
    char  *err = NULL;
    Stack *scopes = stack_create((PrintFunction)scope_print, (DestroyFunction)scope_destroy);
    stack_push(scopes, scope_create());

    StatementListNode *node = prog->statements->head;

    while (node != NULL) {
        err = interpret_statement(scopes, node->stmt);
        if (err != NULL) {
            goto RET;
        }
        node = node->next;
    }

RET:
    stack_destroy(scopes);
    return err;
}
