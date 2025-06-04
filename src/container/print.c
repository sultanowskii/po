#include "container/print.h"

#include <inttypes.h>
#include <stdio.h>

void uint32_print(uint32_t v) {
    printf("%" PRIu32 "\n", v);
}
