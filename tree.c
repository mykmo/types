#define _GNU_SOURCE

#include <assert.h>
#include <search.h>
#include <string.h>

#include "tree.h"

void tree_init(tree_t *tree, cmp_func_t cmp)
{
	memset(tree, 0, sizeof(*tree));

	tree->cmp = cmp;
}

void tree_init_full(tree_t *tree, cmp_func_t cmp, free_func_t destroy)
{
	tree_init(tree, cmp);
	tree->destroy = destroy;
}

void tree_add(tree_t *tree, const void *key)
{
	void **node = tsearch(key, &tree->root, tree->cmp);

	assert(node);

	if (*node != key) {
		if (tree->destroy)
			tree->destroy(*node);

		*node = (void *) key;
	}
}

void *tree_find(tree_t *tree, const void *key)
{
	void **node = tfind(key, &tree->root, tree->cmp);

	return node ? *node : NULL;
}

bool tree_remove(tree_t *tree, const void *key)
{
	if (! tree->destroy)
		return ! tdelete(key, &tree->root, tree->cmp);

	void **node = tfind(key, &tree->root, tree->cmp);

	if (! node)
		return false;

	void *tmp = *node;

	tdelete(key, &tree->root, tree->cmp);
	tree->destroy(tmp);

	return true;
}

void tree_foreach(tree_t *tree, tree_walker_t walk, void *data)
{
	__extension__
	void action(const void *nodep, const VISIT which, const int depth)
	{
		if (which == postorder || which == leaf)
			walk(*(void **) nodep, data);
	}

	twalk(tree->root, action);
}

static void nothing(void *data) { (void) data; }

void tree_clean(tree_t *tree)
{
	tdestroy(tree->root, tree->destroy ? tree->destroy : nothing);
	tree->root = NULL;
}
