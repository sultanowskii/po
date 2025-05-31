#include "compiler/ast/expression.h"

#include <malloc.h>

#include "compiler/ast/binary_op.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/literal.h"

struct Expression {
    ExpressionType type;
    union {
        BinaryOp   *bin_op;
        Literal    *literal;
        Identifier *ident;
    };
};

static inline Expression *expression_create_empty(void) {
    return malloc(sizeof(Expression));
}

Expression *expression_create_literal(Literal *literal) {
    Expression *expr = expression_create_empty();
    *expr = (Expression){
        .type = EXPRESSION_LITERAL,
        .literal = literal,
    };
    return expr;
}

Expression *expression_create_identifier(Identifier *identifier) {
    Expression *expr = expression_create_empty();
    *expr = (Expression){
        .type = EXPRESSION_IDENTIFIER,
        .ident = identifier,
    };
    return expr;
}

Expression *expression_create_binary_op(BinaryOp *binary_op) {
    Expression *expr = expression_create_empty();
    *expr = (Expression){
        .type = EXPRESSION_BINARY_OP,
        .bin_op = binary_op,
    };
    return expr;
}

void expression_destroy(Expression *expression) {
    switch (expression->type) {
        case EXPRESSION_LITERAL:
            literal_destroy(expression->literal);
            break;
        case EXPRESSION_IDENTIFIER:
            identifier_destroy(expression->ident);
            break;
        case EXPRESSION_BINARY_OP:
            binary_op_destroy(expression->bin_op);
            break;
    }
    free(expression);
}
