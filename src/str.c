#include "str.h"

#include <malloc.h>
#include <string.h>

char *strdup(const char *s) {
    size_t length = strlen(s);
    char  *copy = malloc(length + 1);
    strncpy(copy, s, length);
    copy[length] = '\0';
    return copy;
}
