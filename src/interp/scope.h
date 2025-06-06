#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "types/value.h"

struct Scope;
typedef struct Scope Scope;

Scope *scope_create(void);
void   scope_print(const Scope *scope, size_t padding);
Value *scope_get_var(const Scope *scope, const char *name);
bool   scope_has_var(const Scope *scope, const char *name);
void   scope_add_var(Scope *scope, const char *name, Value *value);
char  *scope_update_var(Scope *scope, char *name, Value *value);
void   scope_destroy(Scope *scope);
