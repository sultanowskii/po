#pragma once

#include <inttypes.h>

typedef uint32_t Hash;

typedef Hash (*HashFunction)(void *);

Hash hashf_string(const void *v);
Hash hash_string(const char *v);

Hash hashf_uint32(void *v);
Hash hash_uint32(uint32_t v);
