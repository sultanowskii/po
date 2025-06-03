#pragma once

#include <inttypes.h>

#include "container/hash.h"

typedef void (*DestroyFunction)(void *);

struct Map;
typedef struct Map Map;

Map  *map_create(HashFunction hash_key, DestroyFunction key_destroy, DestroyFunction value_destroy);
void *map_get(Map *map, void *key);
void  map_set(Map *map, void *key, void *value);
void  map_destroy(Map *map);
