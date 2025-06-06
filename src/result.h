#pragma once

#include <stdbool.h>
#include <stddef.h>

#define DEFINE_RESULT(Name, name, type)                           \
    struct Name##Result {                                         \
        type  val;                                                \
        char *err;                                                \
    };                                                            \
    typedef struct Name##Result Name##Result;                     \
                                                                  \
    static inline Name##Result name##_result_ok(type val) {       \
        return (Name##Result){                                    \
            .val = val,                                           \
            .err = NULL,                                          \
        };                                                        \
    }                                                             \
                                                                  \
    static inline Name##Result name##_result_err(char *err) {     \
        return (Name##Result){                                    \
            .err = err,                                           \
        };                                                        \
    }                                                             \
                                                                  \
    static inline bool name##_result_is_ok(Name##Result res) {    \
        return res.err == NULL;                                   \
    }                                                             \
                                                                  \
    static inline bool name##_result_is_err(Name##Result res) {   \
        return res.err != NULL;                                   \
    }                                                             \
                                                                  \
    static inline type name##_result_get_val(Name##Result res) {  \
        return res.val;                                           \
    }                                                             \
                                                                  \
    static inline char *name##_result_get_err(Name##Result res) { \
        return res.err;                                           \
    }
