#pragma once

#include "compiler/ast/expression.h"

struct BinaryOp;
typedef struct BinaryOp BinaryOp;

enum BinaryOpType {
    BINARY_OP_EQUALS,
    BINARY_OP_NOT_EQUALS,
    BINARY_OP_PLUS,
    BINARY_OP_MINUS,
    BINARY_OP_MUL,
    BINARY_OP_DIV,
};
typedef enum BinaryOpType BinaryOpType;

BinaryOp *binary_op_create(enum ExpressionBinaryOpType op_type, Expression *left_expression, Expression *right_expression);
void binary_op_destroy(BinaryOp *binary_op);
