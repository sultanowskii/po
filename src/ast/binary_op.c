#include "ast/binary_op.h"

#include <malloc.h>
#include <stdio.h>

#include "ast/expression.h"
#include "std/fmt.h"

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

void binary_op_print(BinaryOp *bin_op, size_t padding) {
    print_padding(padding);
    printf("BinOp[%s]\n", BINARY_OP_TYPE_STR[bin_op->op]);

    print_padding(padding + 1);
    puts("left:");
    expression_print(bin_op->left, padding + 2);

    print_padding(padding + 1);
    puts("right:");
    expression_print(bin_op->right, padding + 2);
}
