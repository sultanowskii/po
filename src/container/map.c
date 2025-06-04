#include "container/map.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "container/destruction.h"
#include "container/hash.h"
#include "container/print.h"
#include "fmt.h"

#define BUCKET_COUNT 64

typedef struct MapEntry MapEntry;
struct MapEntry {
    Hash      hash;
    void     *key;
    void     *value;
    MapEntry *next;
};

struct Map {
    PrintFunction   key_print;
    DestroyFunction key_destroy;
    DestroyFunction value_destroy;
    PrintFunction   value_print;
    HashFunction    key_hash;
    MapEntry       *buckets[BUCKET_COUNT];
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

Map *map_create(
    HashFunction    key_hash,
    PrintFunction   key_print,
    DestroyFunction key_destroy,
    PrintFunction   value_print,
    DestroyFunction value_destroy
) {
    Map *map = malloc(sizeof(Map));
    *map = (Map){
        .key_hash = key_hash,
        .key_print = key_print,
        .key_destroy = key_destroy,
        .value_print = value_print,
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

void *map_get(const Map *map, const void *key) {
    Hash   hash = map->key_hash(key);
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
    Hash   hash = map->key_hash(key);
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

void map_print(const Map *map, size_t padding) {
    print_padding(padding);
    puts("Map");

    print_padding(padding + 1);
    puts("data:");

    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        MapEntry *entry = map->buckets[i];

        while (entry != NULL) {
            print_padding(padding + 2);
            puts("Entry");

            print_padding(padding + 3);
            puts("key:");
            map->key_print(entry->key, padding + 4);

            print_padding(padding + 3);
            puts("value:");
            map->value_print(entry->value, padding + 4);

            entry = entry->next;
        }
    }
}
