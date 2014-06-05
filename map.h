#ifndef __MAP_H
#define __MAP_H

#include <stdbool.h>

#include "types.h"

typedef struct {
	void *root;

	cmp_func_t cmp;
	free_func_t key_destroy;
	free_func_t value_destroy;
} map_t;

void map_init(map_t *map, cmp_func_t cmp);
void map_init_full(map_t *map, cmp_func_t cmp, free_func_t key_destroy, free_func_t value_destroy);

void map_add(map_t *map, void *key, void *value);
void *map_find(map_t *map, void *key);
bool map_lookup(map_t *map, void *key, void **orig_key, void **orig_value);

bool map_remove(map_t *map, void *key);

void map_foreach(map_t *map, map_walker_t walk, void *data);

void map_clean(map_t *map);

#endif
