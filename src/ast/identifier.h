#pragma once

#include <stddef.h>

struct Identifier {
    char *name;
};
typedef struct Identifier Identifier;

Identifier *identifier_create(const char *name);
void        identifier_print(Identifier *ident, size_t depth);
void        identifier_destroy(Identifier *identifier);
