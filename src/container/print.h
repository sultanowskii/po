#pragma once

#include <inttypes.h>
#include <stddef.h>

typedef void (*PrintFunction)(const void *val, size_t padding);

void uint32_print(uint32_t v, size_t padding);
