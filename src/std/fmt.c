#include "std/fmt.h"

#include <stdio.h>

void print_padding(size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf(" ");
    }
}
