#include "ast/identifier.h"

#include <malloc.h>
#include <stdio.h>

#include "std/fmt.h"
#include "std/str.h"

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

void identifier_print(Identifier *ident, size_t depth) {
    print_padding(depth);
    printf("Identifier[name=%s]\n", ident->name);
}
