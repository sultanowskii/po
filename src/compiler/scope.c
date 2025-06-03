#include "compiler/scope.h"

#include <inttypes.h>
#include <malloc.h>
#include <stddef.h>

#include "compiler/id_provider.h"
#include "container/destruction.h"
#include "container/hash.h"
#include "container/map.h"
#include "container/vec.h"

Scope *scope_create(size_t id, int32_t base_offset) {
    Scope *scope = malloc(sizeof(Scope));
    *scope = (Scope){
        .id = id,
        .base_offset = base_offset,
        .vars = map_create(hashf_string, free, destroy_nop),
        .parent_id = ID_PROVIDER_INVALID_ID,
        .scopes = vec_create(),
    };
    return scope;
}

void scope_destroy(Scope *scope) {
    map_destroy(scope->vars);
    vec_destroy(scope->scopes);
    free(scope);
}
