#pragma once

#include <stddef.h>

struct Literal;
typedef struct Literal Literal;

enum LiteralType {
    LITERAL_INT,
};
typedef enum LiteralType LiteralType;

Literal *literal_create_int(signed long long value);
void     literal_print(Literal *literal, size_t depth);
void     literal_destroy(Literal *literal);
