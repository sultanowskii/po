#pragma once

#include <stddef.h>

// Forward declarations
typedef struct Expression Expression;

enum BinaryOpType {
    BINARY_OP_AND,
    BINARY_OP_OR,
    BINARY_OP_EQUALS,
    BINARY_OP_NOT_EQUALS,
    BINARY_OP_PLUS,
    BINARY_OP_MINUS,
    BINARY_OP_MUL,
    BINARY_OP_DIV,
};
typedef enum BinaryOpType BinaryOpType;

static const char *BINARY_OP_TYPE_STR[] = {
    [BINARY_OP_AND] = "&&",
    [BINARY_OP_OR] = "||",
    [BINARY_OP_EQUALS] = "==",
    [BINARY_OP_NOT_EQUALS] = "!=",
    [BINARY_OP_PLUS] = "+",
    [BINARY_OP_MINUS] = "-",
    [BINARY_OP_MUL] = "*",
    [BINARY_OP_DIV] = "/",
};

struct BinaryOp {
    BinaryOpType op;
    Expression  *left;
    Expression  *right;
};
typedef struct BinaryOp BinaryOp;

BinaryOp *binary_op_create(BinaryOpType op_type, Expression *left, Expression *right);
void      binary_op_print(BinaryOp *bin_op, size_t padding);
void      binary_op_destroy(BinaryOp *bin_op);
