#include "interp/scope.h"

#include <malloc.h>

#include "container/hash.h"
#include "container/map.h"
#include "container/print.h"
#include "interp/value.h"
#include "std/fmt.h"
#include "std/str.h"

struct Scope {
    // map[const char *name, Value]
    Map *vars;
};

Scope *scope_create(void) {
    Scope *scope = malloc(sizeof(Scope));
    *scope = (Scope){
        .vars = map_create(hashf_string, (PrintFunction)string_print, free, (PrintFunction)value_print, (DestroyFunction)value_destroy),
    };
    return scope;
}

void scope_destroy(Scope *scope) {
    map_destroy(scope->vars);
    free(scope);
}

Value *scope_get_var(const Scope *scope, const char *name) {
    return map_get(scope->vars, name);
}

void scope_add_var(Scope *scope, const char *name, Value *value) {
    map_set(scope->vars, strdup(name), value);
}

char *scope_update_var(Scope *scope, char *name, Value *value) {
    Value *current_value = map_get(scope->vars, name);

    if (current_value == NULL) {
        return msprintf("undefined variable: %s", name);
    }

    if (current_value->type != value->type) {
        return msprintf(
            "type mismatch while trying to update variable: current: %s, new: %s",
            name,
            VALUE_TYPE_STR[current_value->type],
            VALUE_TYPE_STR[value->type]
        );
    }

    map_set(scope->vars, name, value);
}

void scope_print(const Scope *scope, size_t padding) {
    print_padding(padding);
    puts("Scope");

    print_padding(padding + 1);
    puts("vars:");

    map_print(scope->vars, padding + 2);
}
