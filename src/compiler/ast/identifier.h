#pragma once

struct Identifier;
typedef struct Identifier Identifier;

Identifier *identifier_create(const char *name);
void        identifier_destroy(Identifier *identifier);
