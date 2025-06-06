#pragma once

enum ValueType {
    TYPE_INT,
    TYPE_STR,
};
typedef enum ValueType ValueType;

static const char *VALUE_TYPE_STR[] = {
    [TYPE_INT] = "int",
    [TYPE_STR] = "str",
};
