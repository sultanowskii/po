#pragma once

#include <stddef.h>

#define ID_PROVIDER_INVALID_ID 0

struct IDProvider;
typedef struct IDProvider IDProvider;

IDProvider *id_provider_create(void);
size_t      id_provider_get(IDProvider *idp);
void        id_provider_destroy(IDProvider *idp);
