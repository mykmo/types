#define _GNU_SOURCE

#include <assert.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

typedef struct {
	void *key;
	void *value;
} map_entry_t;

static inline void *map_entry_new(void *key, void *value)
{
	map_entry_t *entry = malloc(sizeof(map_entry_t));

	assert(entry);

	entry->key = key;
	entry->value = value;

	return entry;
}

static inline void map_entry_free(map_t *map, map_entry_t *entry)
{
	if (map->key_destroy)
		map->key_destroy(entry->key);
	
	if (map->value_destroy)
		map->value_destroy(entry->value);

	free(entry);
}

static void *call_tfunc(map_t *map, void *data, void *(*func)())
{
	__extension__
	int compare(map_entry_t *a, map_entry_t *b)
	{
		return map->cmp(a->key, b->key);
	}

	return func(data, &map->root, compare);
}

void map_init(map_t *map, cmp_func_t cmp)
{
	memset(map, 0, sizeof(*map));

	map->cmp = cmp;
}

void map_init_full(map_t *map, cmp_func_t cmp, free_func_t key_destroy, free_func_t value_destroy)
{
	map_init(map, cmp);

	map->key_destroy = key_destroy;
	map->value_destroy = value_destroy;
}

bool map_add(map_t *map, void *key, void *value)
{
	bool is_new_item = true;

	map_entry_t entry = { key, NULL };

	void **node = call_tfunc(map, &entry, tsearch);

	assert(node);

	if (*node != &entry) {
		map_entry_free(map, *node);
		is_new_item = false;
	}

	*node = map_entry_new(key, value);

	return is_new_item;
}

void *map_find(map_t *map, void *key)
{
	void *value = NULL;

	map_lookup(map, key, NULL, &value);

	return value;
}

bool map_lookup(map_t *map, void *key, void **orig_key, void **orig_value)
{
	map_entry_t entry = { key, NULL };

	void **node = call_tfunc(map, &entry, tfind);

	if (! node)
		return false;

	map_entry_t *real = *node;

	if (orig_key)
		*orig_key = real->key;

	if (orig_value)
		*orig_value = real->value;

	return true;
}

bool map_remove(map_t *map, void *key)
{
	map_entry_t entry = { key, NULL };

	void **node = call_tfunc(map, &entry, tfind);

	if (! node)
		return false;

	void *tmp = *node;

	call_tfunc(map, &entry, tdelete);
	map_entry_free(map, tmp);

	return true;
}


void map_foreach(map_t *map, map_walker_t walk, void *data)
{
	__extension__
	void action(const void *nodep, const VISIT which, const int depth)
	{
		map_entry_t *entry = *(void **) nodep;

		if (which == postorder || which == leaf)
			walk(entry->key, entry->value, data);
	}

	twalk(map->root, action);
}

void map_clean(map_t *map)
{
	__extension__
	void free_node(void *node)
	{
		map_entry_free(map, node);
	}

	tdestroy(map->root, free_node);
	map->root = NULL;
}
