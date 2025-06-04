#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "container/destruction.h"
#include "container/hash.h"
#include "container/print.h"

struct Map;
typedef struct Map Map;

Map *map_create(
    HashFunction    key_hash,
    PrintFunction   key_print,
    DestroyFunction key_destroy,
    PrintFunction   value_print,
    DestroyFunction value_destroy
);
void *map_get(const Map *map, const void *key);
void  map_set(Map *map, void *key, void *value);
void  map_print(const Map *map, size_t padding);
void  map_destroy(Map *map);

#define map_set_(m, k, v) map_set((m), (void *)(k), (void *)(v))
#define map_get_(m, k) map_get((m), (void *)(k))
