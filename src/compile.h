#pragma once

#define IGNORE_INT_TO_POINTER() _Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"")
#define IGNORE_POINTER_TO_INT() _Pragma("GCC diagnostic ignored \"-Wpointer-to-int-cast\"")
