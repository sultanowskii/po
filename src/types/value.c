#include "types/value.h"

#include <inttypes.h>
#include <malloc.h>

#include "std/fmt.h"
#include "std/str.h"
#include "types/value_type.h"

Value *value_create_int(int32_t val) {
    Value *value = malloc(sizeof(Value));
    *value = (Value){
        .type = TYPE_INT,
        .int_ = val,
    };
    return value;
}

Value *value_create_str(const char *val) {
    Value *value = malloc(sizeof(Value));
    *value = (Value){
        .type = TYPE_STR,
        .str = strdup(val),
    };
    return value;
}

Value *value_copy(const Value *orig) {
    switch (orig->type) {
    case TYPE_INT:
        return value_create_int(orig->int_);
    case TYPE_STR:
        return value_create_str(orig->str);
    }
}

void value_destroy(Value *value) {
    switch (value->type) {
    case TYPE_STR:
        free(value->str);
        break;
    }
    free(value);
}

void value_print(Value *value, size_t padding) {
    print_padding(padding);
    printf("Value[type=%s]\n", VALUE_TYPE_STR[value->type]);

    print_padding(padding + 1);
    puts("val:");

    print_padding(padding + 2);
    switch (value->type) {
    case TYPE_INT:
        printf("%" PRId32 "\n", value->int_);
        break;
    case TYPE_STR:
        puts(value->str);
        break;
    }
}
