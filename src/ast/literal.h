#pragma once

#include <inttypes.h>
#include <stddef.h>

enum LiteralType {
    LITERAL_INT,
    LITERAL_STR,
};
typedef enum LiteralType LiteralType;

static const char *LITERAL_TYPE_STR[] = {
    [LITERAL_INT] = "int",
    [LITERAL_STR] = "str",
};

struct Literal {
    LiteralType type;
    union {
        int32_t int_;
        char   *str;
    };
};
typedef struct Literal Literal;

Literal *literal_create_int(int32_t value);
Literal *literal_create_str(const char *value);
void     literal_print(Literal *literal, size_t depth);
void     literal_destroy(Literal *literal);
