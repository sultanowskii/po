#include "compiler/scope.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "compile.h"
#include "compiler/id_provider.h"
#include "container/destruction.h"
#include "container/hash.h"
#include "container/map.h"
#include "container/print.h"
#include "container/vec.h"
#include "fmt.h"
#include "string.h"

VarInfo *var_info_create(Offset offset, bool defined) {
    VarInfo *vi = malloc(sizeof(VarInfo));
    *vi = (VarInfo){
        .offset = offset,
        .defined = defined,
    };
    return vi;
}

void var_info_destroy(VarInfo *vi) {
    free(vi);
}

void var_info_print(const VarInfo *vi, size_t padding) {
    print_padding(padding);
    printf("VarInfo[offset=%" PRId32 ", defined=%d]\n", vi->offset, vi->defined);
}

void var_name_print(const char *name, size_t padding) {
    print_padding(padding);
    puts(name);
}

Scope *scope_create(size_t id, int32_t base_offset) {
    Scope *scope = malloc(sizeof(Scope));
    *scope = (Scope){
        .id = id,
        .base_offset = base_offset,
        .vars = map_create(
            hashf_string,
            (PrintFunction)var_name_print,
            free,
            (PrintFunction)var_info_print,
            (DestroyFunction)var_info_destroy
        ),
        .parent_id = ID_PROVIDER_INVALID_ID,
        .scopes = vec_create((PrintFunction)uint32_print, destroy_nop),
    };
    return scope;
}

void scope_destroy(Scope *scope) {
    map_destroy(scope->vars);
    vec_destroy(scope->scopes);
    free(scope);
}

void scope_add_variable(Scope *scope, const char *name, Offset offset) {
    VarInfo *vi = var_info_create(offset, false);
    IGNORE_INT_TO_POINTER()
    map_set(scope->vars, strdup(name), vi);
}

VarInfo *scope_get_variable(Scope *scope, const char *name) {
    IGNORE_INT_TO_POINTER()
    return map_get(scope->vars, name);
}

void scope_add_scope(Scope *scope, uint32_t inner_scope_id) {
    vec_push_back(scope->scopes, (void *)inner_scope_id);
}

void scope_print(const Scope *scope, size_t padding) {
    print_padding(padding);
    printf("Scope[id=%zu, parent_id=%zu, size=%zu, base_offset=%" PRId32 "]\n", scope->id, scope->parent_id, scope->required_size, scope->base_offset);

    print_padding(padding + 1);
    puts("variables:");
    map_print(scope->vars, padding + 2);
}
