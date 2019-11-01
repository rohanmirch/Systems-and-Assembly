#include "smallobj.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


// When defined, this symbol causes freed objects to be overwritten so that
// it's easier to detect access-after-free bugs.
#undef OVERWRITE_MEM


// Forward-declare the chunk_t struct type.
typedef struct chunk_t chunk_t;


/*!
 * This struct is used by the small-object allocator to represent a pool of
 * objects that are all some small fixed size.  The pool scales up and down
 * based on the number of objects that are in use; this is done by allocating
 * multiple "chunks" (chunks are allocated using malloc()), each which can hold
 * some number of objects of the specified size.  Chunks are maintained in a
 * singly linked list.  When a chunk is no longer in use, it is removed from the
 * list of chunks, and the chunk's memory is released from the pool using
 * free().
 */
struct smallobj_pool_t {
    // The size of objects in this small-object pool.
    int objsize;

    // The total number of objects allocated per chunk.
    int objects_per_chunk;

    // The list of chunks in the small-object pool.
    chunk_t *chunk_list;
};


/*!
 * This struct represents one chunk of objects in a small-object pool.  The
 * pool's "objects_per_chunk" value specifies how many objects fit in each
 * chunk.  The last member "mem" is an array of size
 * pool->objsize * pool->objects_per_chunk bytes.
 */
struct chunk_t {
    // The pool that the chunk is from.
    smallobj_pool_t *pool;

    // Index of the next available object in the chunk.
    int next_available;

    // How many small objects in this chunk have been released.
    int num_freed;

    // A pointer to the next chunk in the pool, or NULL if this is the last
    // chunk in the pool.
    chunk_t *next_chunk;

    // The start of the memory area for this chunk.
    char mem[];
};


// === GENERAL POOL MANAGEMENT FUNCTIONS =======================================


// Forward-declare this:
chunk_t *init_new_chunk(smallobj_pool_t *pool);


/*!
 * Given a pool, returns how much memory each chunk in the pool provides for
 * allocations.  In other words, this is the size of the "mem" member at the
 * end of chunks in a given pool.
 */
size_t chunk_mem_size(smallobj_pool_t *pool) {
    return pool->objects_per_chunk * pool->objsize;
}


/*!
 * Initialize a small-object pool of objects of the specified size.  The
 * initial pool will have no chunks allocated in it.
 *
 * If malloc() cannot allocate the small-object pool (unlikely) then this
 * function will report an error and then abort().
 */
smallobj_pool_t * make_so_pool(size_t objsize, int objects_per_chunk) {
    assert(objsize >= sizeof(intptr_t));

    // Allocate an empty pool.
    smallobj_pool_t *pool = malloc(sizeof(smallobj_pool_t));
    if (pool == NULL) {
        fprintf(stderr, "ERROR:  Couldn't allocate small-object pool");
        abort();
    }

    pool->objsize = objsize;
    pool->objects_per_chunk = objects_per_chunk;

    // HW5 optimization:  pre-allocate one chunk in the pool, so that the
    // performance graphs look a bit cleaner.
    // pool->chunk_list = init_new_chunk(pool);
    pool->chunk_list = NULL;

    return pool;
}


/*!
 * Release a small-object pool.  Any pointers to objects in this pool are
 * no longer valid after release.
 */
void release_so_pool(smallobj_pool_t *pool) {
    assert(pool != NULL);

    // Iterate through all chunks and free() each one.
    chunk_t *chunk = pool->chunk_list;
    while (chunk != NULL) {
        chunk_t *next = chunk->next_chunk;
        free(chunk);
        chunk = next;
    }

    // Free the pool itself.
    free(pool);
}

/*!
 * Reports the total size of the small-object pool in bytes, including the size
 * of the pool structure, and the sizes of all chunks allocated for the pool.
 * This value will never be zero, since the pool data structure always occupies
 * some number of bytes.
 */
size_t total_pool_size(smallobj_pool_t *pool) {
    assert(pool != NULL);

    size_t size = sizeof(smallobj_pool_t);
    chunk_t *chunk = pool->chunk_list;
    while (chunk != NULL) {
        size += sizeof(chunk_t) + chunk_mem_size(pool);
        chunk = chunk->next_chunk;
    }

    return size;
}


// === ALLOCATION FUNCTIONS ====================================================


/*!
 * This helper function initializes a new chunk for use in a small-object pool.
 * The function calls malloc() to allocate the chunk's memory, and then it
 * performs any necessary initialization on the chunk's contents to prepare for
 * small-object allocations.
 */
chunk_t *init_new_chunk(smallobj_pool_t *pool) {
    size_t chunk_size = sizeof(chunk_t) + chunk_mem_size(pool);
    chunk_t *chunk = malloc(chunk_size);
    if (chunk == NULL) {
        fprintf(stderr, "ERROR:  Couldn't allocate chunk for pool %p", pool);
        abort();
    }

    chunk->pool = pool;
    chunk->next_available = 0;
    chunk->num_freed = 0;
    chunk->next_chunk = NULL;

    return chunk;
}


/*!
 * This helper function searches through the small-object pool for a chunk that
 * has room for another allocation.  If all chunks are full, a new chunk will
 * be initialized and added to the pool.
 */
chunk_t * get_nonfull_chunk(smallobj_pool_t *pool) {
    chunk_t *chunk = pool->chunk_list;
    chunk_t *prev = NULL;

    // Try to find a chunk that has available space.
    while (chunk != NULL) {
        if (chunk->next_available < pool->objects_per_chunk)
            break;

        prev = chunk;
        chunk = chunk->next_chunk;
    }

    // If all chunks are full, we need to allocate a new chunk.
    if (chunk == NULL) {
        chunk = init_new_chunk(pool);

        if (prev != NULL)
            prev->next_chunk = chunk;
        else
            pool->chunk_list = chunk;
    }

    return chunk;
}


/*!
 * Given a non-full chunk from a pool, this helper function returns a newly
 * allocated object from the chunk.  The helper also performs any bookkeeping
 * necessary to record that the object has been allocated.
 */
void * alloc_object_from_chunk(chunk_t *chunk) {
    assert(chunk != NULL);

    // Make sure the chunk isn't already full.
    assert(chunk->next_available < chunk->pool->objects_per_chunk);

    // Allocate a new object from the chunk.
    void *obj = chunk->mem + chunk->next_available * chunk->pool->objsize;
    chunk->next_available++;
    return obj;
}


/*!
 * Allocate a small object from the specified pool.  The pool itself knows what
 * size objects it produces, so this function does not take a size argument.
 *
 * This function always returns a non-NULL value, which is not very realistic,
 * but certainly keeps life simple.  If a chunk-allocation failure occurs, the
 * function will print an error and then abort().
 */
void * so_alloc(smallobj_pool_t *pool) {
    assert(pool != NULL);

    // Find or allocate a chunk that has available space.
    chunk_t *chunk = get_nonfull_chunk(pool);

    // The chunk should be able to hold a new allocation.
    assert(chunk != NULL);
    assert(chunk->next_available < pool->objects_per_chunk);

    // Allocate a new object from the chunk.
    return alloc_object_from_chunk(chunk);
}


// === DEALLOCATION FUNCTIONS ==================================================


/*!
 * Returns true if the specified pointer falls within the specified chunk,
 * false otherwise.
 */
bool is_object_in_chunk(void *obj, chunk_t *chunk) {
    return obj >= (void *) chunk->mem &&
           obj < (void *) (chunk->mem + chunk_mem_size(chunk->pool));
}


/*! Records that the specified object in the chunk has been freed. */
void free_object_in_chunk(void *obj, chunk_t *chunk) {
    assert(obj != NULL);
    assert(chunk != NULL);
    assert(is_object_in_chunk(obj, chunk));

    // Record that the object has been freed.
    chunk->num_freed++;
}


/*!
 * Returns true if the chunk no longer holds any allocated objects; false
 * otherwise.
 */
bool is_chunk_empty(chunk_t *chunk) {
    return chunk->num_freed == chunk->pool->objects_per_chunk;
}


/*!
 * Release a small object back to the specified pool.
 *
 * It is an error to pass in a pointer to an object that is not from this pool.
 * this kind of error is usually detectable and will result in an error message
 * being printed and the program aborted.
 *
 * It is also an error to free the same object twice in a row.  This kind of
 * error is not detectable and may cause all manner of chaos to ensue.
 */
void so_free(smallobj_pool_t *pool, void *obj) {

#ifdef OVERWRITE_MEM
    // Overwrite the object that was just released, so that access-after-free
    // bugs are more obvious.
    memset(obj, 0xEE, pool->objsize);
#endif

    // Find the chunk that corresponds to the memory being freed.  Also keep
    // track of the "previous chunk" so that we can remove completely empty
    // chunks.

    chunk_t *chunk = pool->chunk_list;
    chunk_t *prev = NULL;

    while (chunk != NULL) {
        if (is_object_in_chunk(obj, chunk))
            break;     // Found the chunk containing the allocation.

        prev = chunk;
        chunk = chunk->next_chunk;
    }

    if (chunk == NULL) {
        fprintf(stderr, "ERROR:  Pointer %p isn't from pool %p", obj, pool);
        abort();
    }

    free_object_in_chunk(obj, chunk);

    if (is_chunk_empty(chunk)) {
        // This chunk has been completely freed.  Remove it from the list.
        // fprintf(stderr, "FREEING CHUNK\n");
        if (prev == NULL)
            pool->chunk_list = chunk->next_chunk;
        else
            prev->next_chunk = chunk->next_chunk;

#ifdef OVERWRITE_MEM
        // Overwrite the chunk so that access-after-free bugs are more obvious.
        memset(chunk, 0xEF, sizeof(chunk_t) + chunk_mem_size(pool));
#endif

        free(chunk);
    }

    /*
    if (pool->chunk_list == NULL) {
        // HW5 optimization:  pre-allocate one chunk in the pool, so that the
        // performance graphs look a bit cleaner.
        pool->chunk_list = init_new_chunk(pool);
    }
    */
}
