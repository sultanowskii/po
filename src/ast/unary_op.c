#include "ast/unary_op.h"

#include <malloc.h>
#include <stdio.h>

#include "ast/expression.h"
#include "std/fmt.h"

UnaryOp *unary_op_create(UnaryOpType op_type, Expression *expr) {
    UnaryOp *una_op = malloc(sizeof(UnaryOp));
    *una_op = (UnaryOp){
        .op = op_type,
        .expr = expr,
    };
    return una_op;
}

void unary_op_destroy(UnaryOp *una_op) {
    expression_destroy(una_op->expr);
    free(una_op);
}

void unary_op_print(UnaryOp *una_op, size_t padding) {
    print_padding(padding);
    printf("UnaOp[%s]\n", UNARY_OP_TYPE_STR[una_op->op]);

    print_padding(padding + 1);
    puts("expr:");
    expression_print(una_op->expr, padding + 2);
}
