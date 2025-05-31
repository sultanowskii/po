#include "compiler/ast/identifier.h"

#include <malloc.h>

#include "str.h"

struct Identifier {
    char *name;
};
typedef struct Identifier Identifier;

Identifier *identifier_create(const char *name) {
    Identifier *ident = malloc(sizeof(Identifier));
    *ident = (Identifier){
        .name = strdup(name),
    };
    return ident;
}

void identifier_destroy(Identifier *ident) {
    free(ident->name);
    free(ident);
}
