#include "compiler/ast/id_provider.h"

#include <malloc.h>
#include <stddef.h>

struct IDProvider {
    size_t next_id;
};

IDProvider *id_provider_create(void) {
    IDProvider *idp = malloc(sizeof(IDProvider));
    idp->next_id = 1;
    return idp;
}

void id_provider_destroy(IDProvider *idp) {
    free(idp);
}

size_t id_provider_get(IDProvider *idp) {
    size_t current = idp->next_id;
    idp->next_id++;
    return current;
}
