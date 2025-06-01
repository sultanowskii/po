#include "compiler/ast/literal.h"

#include <malloc.h>
#include <stdio.h>

#include "fmt.h"

struct Literal {
    LiteralType type;
    union {
        double           float_;
        signed long long int_;
    };
};
typedef struct Literal Literal;

Literal *literal_create_float(double value) {
    Literal *lit = malloc(sizeof(Literal));
    *lit = (Literal){
        .type = LITERAL_FLOAT,
        .float_ = value,
    };
    return lit;
}

Literal *literal_create_int(signed long long value) {
    Literal *lit = malloc(sizeof(Literal));
    *lit = (Literal){
        .type = LITERAL_INT,
        .int_ = value,
    };
    return lit;
}

void literal_destroy(Literal *lit) {
    free(lit);
}

static inline char *literal_type_to_str(Literal *literal) {
    switch (literal->type) {
        case LITERAL_FLOAT:
            return "float";
        case LITERAL_INT:
            return "int";
    }
    return "unknown";
}

static inline char *literal_print_value(Literal *literal) {
    switch (literal->type) {
        case LITERAL_FLOAT:
            printf("%llf", literal->float_);
            break;
        case LITERAL_INT:
            printf("%lld", literal->int_);
            break;
    }
    return "unknown";
}

void literal_print(Literal *literal, size_t depth) {
    print_padding(depth);
    printf("Literal[type=%s, value=", literal_type_to_str(literal));
    literal_print_value(literal);
    puts("]");
}
