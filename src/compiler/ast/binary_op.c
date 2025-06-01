#include "compiler/ast/binary_op.h"

#include <malloc.h>
#include <stdio.h>

#include "compiler/ast/expression.h"
#include "fmt.h"

BinaryOp *binary_op_create(BinaryOpType op_type, Expression *left, Expression *right) {
    BinaryOp *bin_op = malloc(sizeof(BinaryOp));
    *bin_op = (BinaryOp){
        .op = op_type,
        .left = left,
        .right = right,
    };
    return bin_op;
}

void binary_op_destroy(BinaryOp *bin_op) {
    expression_destroy(bin_op->left);
    expression_destroy(bin_op->right);
    free(bin_op);
}

static inline char *bin_op_type_to_str(BinaryOp *bin_op) {
    switch (bin_op->op) {
    case BINARY_OP_AND:
        return "&&";
    case BINARY_OP_OR:
        return "||";
    case BINARY_OP_EQUALS:
        return "==";
    case BINARY_OP_NOT_EQUALS:
        return "!=";
    case BINARY_OP_PLUS:
        return "+";
    case BINARY_OP_MINUS:
        return "-";
    case BINARY_OP_MUL:
        return "*";
    case BINARY_OP_DIV:
        return "/";
    }
    return "unknown";
}

void binary_op_print(BinaryOp *bin_op, size_t padding) {
    print_padding(padding);
    printf("BinOp[%s]\n", bin_op_type_to_str(bin_op));

    print_padding(padding + 1);
    puts("left:");
    expression_print(bin_op->left, padding + 2);

    print_padding(padding + 1);
    puts("right:");
    expression_print(bin_op->right, padding + 2);
}
