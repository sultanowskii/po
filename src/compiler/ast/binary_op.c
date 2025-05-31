#include "compiler/ast/binary_op.h"

#include <malloc.h>

#include "compiler/ast/expression.h"

struct BinaryOp {
    BinaryOpType op;
    Expression  *left;
    Expression  *right;
};

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
