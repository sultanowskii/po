#include "compiler/ast/unary_op.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/expression.h"
#include "fmt.h"

struct UnaryOp {
    UnaryOpType op;
    Expression *expr;
};

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

static inline char *una_op_type_to_str(UnaryOp *una_op) {
    switch (una_op->op) {
    case UNARY_OP_NOT:
        return "!";
    }
    return "unknown";
}

void unary_op_print(UnaryOp *una_op, size_t padding) {
    print_padding(padding);
    printf("UnaOp[%s]\n", una_op_type_to_str(una_op));

    print_padding(padding + 1);
    puts("expr:");
    expression_print(una_op->expr, padding + 2);
}
