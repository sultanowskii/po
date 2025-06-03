#include "container/map.h"

#include <inttypes.h>
#include <malloc.h>
#include <string.h>

#include "container/hash.h"

#define BUCKET_COUNT 64

typedef struct MapEntry MapEntry;
struct MapEntry {
    Hash      hash;
    void     *key;
    void     *value;
    MapEntry *next;
};

struct Map {
    MapEntry       *buckets[BUCKET_COUNT];
    HashFunction    hash_key;
    DestroyFunction key_destroy;
    DestroyFunction value_destroy;
};

static inline MapEntry *map_entry_create(Hash hash, void *key, void *value, MapEntry *next) {
    MapEntry *entry = malloc(sizeof(MapEntry));
    *entry = (MapEntry){
        .hash = hash,
        .key = key,
        .value = value,
        .next = next,
    };
    return entry;
}

static inline void map_entry_destroy(
    Map      *map,
    MapEntry *entry
) {
    map->key_destroy(entry->key);
    map->value_destroy(entry->value);
    free(entry);
}

Map *map_create(HashFunction hash_key, DestroyFunction key_destroy, DestroyFunction value_destroy) {
    Map *map = malloc(sizeof(Map));
    *map = (Map){
        .hash_key = hash_key,
        .key_destroy = key_destroy,
        .value_destroy = value_destroy,
    };
    memset(map->buckets, 0, sizeof(map->buckets));
    return map;
}

void map_destroy(Map *map) {
    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        MapEntry *entry = map->buckets[i];
        while (entry != NULL) {
            MapEntry *next = entry->next;
            map_entry_destroy(map, entry);
            entry = next;
        }
        map->buckets[i] = NULL;
    }
    free(map);
}

void *map_get(Map *map, void *key) {
    Hash   hash = map->hash_key(key);
    size_t bucket_index = hash % BUCKET_COUNT;

    MapEntry *entry = map->buckets[bucket_index];
    while (entry != NULL) {
        if (entry->hash == hash) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void map_set(Map *map, void *key, void *value) {
    Hash   hash = map->hash_key(key);
    size_t bucket_index = hash % BUCKET_COUNT;

    MapEntry *entry = map->buckets[bucket_index];
    if (entry == NULL) {
        map->buckets[bucket_index] = map_entry_create(hash, key, value, NULL);
        return;
    }

    MapEntry *prev = entry;
    while (entry != NULL) {
        if (entry->hash == hash) {
            entry->value = value;
            return;
        }
        prev = entry;
        entry = entry->next;
    }

    prev->next = map_entry_create(hash, key, value, entry);
}
