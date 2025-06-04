#include "container/hash.h"

#include <inttypes.h>
#include <stddef.h>

#include "compile.h"

const uint32_t FNV1A32_OFFSET_BASIS = 0x811c9dc5;
const uint32_t FNV1A32_PRIME = 0x01000193;

// FNV1a32
// Source: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1_hash
Hash hash_string(const char *str) {
    Hash hash = FNV1A32_OFFSET_BASIS;

    size_t index = 0;
    while (str[index] != '\0') {
        hash = hash ^ str[index];
        hash = hash * FNV1A32_PRIME;
        index++;
    }

    return hash;
}

Hash hashf_string(const void *str) {
    return hash_string(str);
}

Hash hash_uint32(uint32_t num) {
    return (Hash)num;
}

Hash hashf_uint32(const void *v) {
    IGNORE_POINTER_TO_INT()
    return hash_uint32((uint32_t)v);
}
