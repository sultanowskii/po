#pragma once

#include <inttypes.h>

#include "container/destruction.h"
#include "container/hash.h"

struct Map;
typedef struct Map Map;

Map  *map_create(HashFunction hash_key, DestroyFunction key_destroy, DestroyFunction value_destroy);
void *map_get(const Map *map, void *key);
void  map_set(Map *map, void *key, void *value);
void  map_destroy(Map *map);

#define map_set_(m, k, v) map_set((m), (void *)(k), (void *)(v))
#define map_get_(m, k) map_get((m), (void *)(k))
