#include "container/print.h"

#include <inttypes.h>
#include <stdio.h>

#include "std/fmt.h"

void uint32_print(uint32_t v, size_t padding) {
    print_padding(padding);
    printf("%" PRIu32 "\n", v);
}

void string_print(const char *v, size_t padding) {
    print_padding(padding);
    puts(v);
}
