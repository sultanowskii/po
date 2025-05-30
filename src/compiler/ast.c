#include "compiler/ast.h"

struct StatementNewVariable {
    struct AstNode *identifier;
    struct AstNode *expression;
};

struct StatementAssign {
    struct AstNode *identifier;
    struct AstNode *expression;
};

struct StatementIf {
    struct AstNode *cond_expression;
    struct AstNode *if_block;
    struct AstNode *else_block;
};

struct StatementWhile {
    struct AstNode *cond_expression;
    struct AstNode *block;
};

struct StatementBlock {
    // TODO: linked list of statements
};

struct Statement {
    enum StatementType type;
    union {
        struct StatementNewVariable new_variable;
        struct StatementAssign assign;
        struct StatementIf if_;
        struct StatementWhile while_;
        struct StatementBlock block;
    };
};

struct ExpressionBinary {
    enum ExpressionBinaryType type;
    struct AstNode *left_expression;
    struct AstNode *right_expression;
};

struct ExpressionLiteralFloat {
    double value;
};

struct ExpressionLiteralInt {
    signed long long value;
};

struct ExpressionLiteral {
    enum ExpressionLiteralType type;
    union {
        struct ExpressionLiteralFloat float_;
        struct ExpressionLiteralInt int_;
    };
};

struct ExpressionIdentifier {
    char *name;
};

struct Expression {
    enum ExpressionType type;
    union {
        struct ExpressionBinary binary;
        struct ExpressionLiteral literal;
        struct ExpressionIdentifier identifier;
    };
};

struct AstNode {
    enum AstNodeType type;
    union {
        struct Statement statement;
        struct Expression expression;
    };
};


