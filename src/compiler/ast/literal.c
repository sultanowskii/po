#include "compiler/ast/literal.h"

#include <malloc.h>

struct Literal {
    LiteralType type;
    union {
        double float_;
        signed long long int_;
    };
};
typedef struct Literal Literal;


Literal *literal_create_float(double value) {
    Literal *lit = malloc(sizeof(Literal));
    *lit = (Literal) {
        .type = LITERAL_FLOAT,
        .float_ = value,
    };
    return lit;
}

Literal *literal_create_int(signed long long value) {
    Literal *lit = malloc(sizeof(Literal));
    *lit = (Literal) {
        .type = LITERAL_INT,
        .int_ = value,
    };
    return lit;
}

void literal_destroy(Literal *lit) {
    free(lit);
}
