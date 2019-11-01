#ifndef SMALLOBJ_H
#define SMALLOBJ_H

#include <stdlib.h>

// The type that represents a small-object pool.  The contents of this type are
// defined in the .c file so that it is an opaque type to users.
typedef struct smallobj_pool_t smallobj_pool_t;

// Initialize a small-object pool of objects of the specified size.
smallobj_pool_t * make_so_pool(size_t objsize, int objects_per_chunk);

// Release a small-object pool.  Any pointers to objects in this pool are
// no longer valid after release.
void release_so_pool(smallobj_pool_t *pool);

// Returns the total size of the small-object pool in bytes.
size_t total_pool_size(smallobj_pool_t *pool);

// Allocate a small object from the specified pool.
void * so_alloc(smallobj_pool_t *pool);

// Release a small object back to the specified pool.
void so_free(smallobj_pool_t *pool, void *obj);

#endif // SMALLOBJ_H
