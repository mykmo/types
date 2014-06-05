#ifndef __TREE_H
#define __TREE_H

#include <stdbool.h>

#include "types.h"

typedef struct {
	void 		*root;

	cmp_func_t 	cmp;
	free_func_t	destroy;
} tree_t;

void tree_init(tree_t *tree, cmp_func_t cmp);
void tree_init_full(tree_t *tree, cmp_func_t cmp, free_func_t destroy);

void tree_add(tree_t *tree, const void *key);
void *tree_find(tree_t *tree, const void *key);
bool tree_remove(tree_t *tree, const void *key);

void tree_foreach(tree_t *tree, tree_walker_t walk, void *data);

void tree_clean(tree_t *tree);

#endif
