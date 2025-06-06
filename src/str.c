#include "str.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *strdup(const char *s) {
    size_t length = strlen(s);
    char  *copy = malloc(length + 1);
    strncpy(copy, s, length);
    copy[length] = '\0';
    return copy;
}

char *msprintf(const char *format, ...) {
    va_list args_to_determine_size;
    va_start(args_to_determine_size, format);
    size_t formatted_size = vsnprintf(NULL, 0, format, args_to_determine_size);
    va_end(args_to_determine_size);

    va_list args_to_format;
    va_start(args_to_format, format);
    char *s = malloc(formatted_size + 1);
    vsprintf(s, format, args_to_format);
    va_end(args_to_format);

    return s;
}
