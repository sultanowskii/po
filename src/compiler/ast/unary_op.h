#pragma once

#include <stddef.h>

// Forward declarations
typedef struct Expression Expression;

enum UnaryOpType {
    UNARY_OP_NOT,
};
typedef enum UnaryOpType UnaryOpType;

struct UnaryOp {
    UnaryOpType op;
    Expression *expr;
};
typedef struct UnaryOp UnaryOp;

UnaryOp *unary_op_create(UnaryOpType op_type, Expression *expr);
void     unary_op_print(UnaryOp *una_op, size_t padding);
void     unary_op_destroy(UnaryOp *una_op);
