#include "ast/literal.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

#include "std/fmt.h"
#include "std/str.h"

Literal *literal_create_int(int32_t value) {
    Literal *lit = malloc(sizeof(Literal));
    *lit = (Literal){
        .type = LITERAL_INT,
        .int_ = value,
    };
    return lit;
}

Literal *literal_create_str(const char *value) {
    Literal *lit = malloc(sizeof(Literal));
    *lit = (Literal){
        .type = LITERAL_STR,
        .str = strdup(value),
    };
    return lit;
}

void literal_destroy(Literal *literal) {
    switch (literal->type) {
    case LITERAL_STR:
        free(literal->str);
        break;
    }
    free(literal);
}

static inline char *literal_print_value(Literal *literal) {
    return "unknown";
}

void literal_print(Literal *literal, size_t padding) {
    print_padding(padding);
    printf("Literal[type=%s]\n", LITERAL_TYPE_STR[literal->type]);

    print_padding(padding + 1);
    puts("value:");

    print_padding(padding + 2);
    switch (literal->type) {
    case LITERAL_INT:
        printf("%lld\n", literal->int_);
        break;
    case LITERAL_STR:
        printf("'%s'\n", literal->str);
        break;
    }
}
