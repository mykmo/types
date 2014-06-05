#ifndef __TYPES_H
#define __TYPES_H

typedef int (*cmp_func_t)(const void *, const void *);
typedef void (*free_func_t)(void *);

typedef void (*tree_walker_t)(void *, void *);
typedef void (*map_walker_t)(void *, void *, void *);

#endif
