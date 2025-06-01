#include "compiler/ast/expression.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/binary_op.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/literal.h"
#include "compiler/ast/unary_op.h"
#include "fmt.h"

struct Expression {
    ExpressionType type;
    union {
        UnaryOp    *una_op;
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

Expression *expression_create_unary_op(UnaryOp *unary_op) {
    Expression *expr = expression_create_empty();
    *expr = (Expression){
        .type = EXPRESSION_UNARY_OP,
        .una_op = unary_op,
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
    case EXPRESSION_UNARY_OP:
        unary_op_destroy(expression->una_op);
        break;
    case EXPRESSION_BINARY_OP:
        binary_op_destroy(expression->bin_op);
        break;
    }
    free(expression);
}

static inline char *expression_type_to_str(Expression *expr) {
    switch (expr->type) {
    case EXPRESSION_LITERAL:
        return "literal";
    case EXPRESSION_IDENTIFIER:
        return "identifier";
    case EXPRESSION_UNARY_OP:
        return "unary_op";
    case EXPRESSION_BINARY_OP:
        return "binary_op";
    }
    return "unknown";
}

void expression_print(Expression *expression, size_t padding) {
    print_padding(padding);
    printf("Expression[type=%s]\n", expression_type_to_str(expression));

    print_padding(padding + 1);
    puts("value:");
    switch (expression->type) {
    case EXPRESSION_LITERAL:
        literal_print(expression->literal, padding + 2);
        break;
    case EXPRESSION_IDENTIFIER:
        identifier_print(expression->ident, padding + 2);
        break;
    case EXPRESSION_UNARY_OP:
        unary_op_print(expression->una_op, padding + 2);
        break;
    case EXPRESSION_BINARY_OP:
        binary_op_print(expression->bin_op, padding + 2);
        break;
    }
}
