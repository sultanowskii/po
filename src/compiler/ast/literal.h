#pragma once

#include <stddef.h>
#include <inttypes.h>

struct Literal {
    LiteralType type;
    union {
        int32_t int_;
    };
};
typedef struct Literal Literal;

enum LiteralType {
    LITERAL_INT,
};
typedef enum LiteralType LiteralType;

Literal *literal_create_int(int32_t value);
void     literal_print(Literal *literal, size_t depth);
void     literal_destroy(Literal *literal);
