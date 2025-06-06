#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "types/value_type.h"

struct Value {
    ValueType type;
    union {
        int32_t int_;
        char   *str;
    };
};
typedef struct Value Value;

Value *value_create_int(int32_t val);
Value *value_create_str(const char *val);
void   value_print(Value *value, size_t padding);
void  *value_destroy(Value *value);
