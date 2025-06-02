#pragma once

#include <stddef.h>

struct IDProvider;
typedef struct IDProvider IDProvider;

IDProvider *id_provider_create(void);
size_t id_provider_get(IDProvider *idp);
void id_provider_destroy(IDProvider *idp);
