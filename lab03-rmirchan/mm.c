/*
 * mm-explicit.c - Explicit allocator with a first-fit
 * free list with binary segregation).
 *
 *

 *
 * Structure of each block:
 * allocated : | Header(4) | Payload | Footer(4)|
 * freed     : | Header(4) | Prev_ptr(8) | Next_ptr(8) | Footer(4) |
 * Since each block must be able to hold 2 8 byte pointers in
 * its payload, the minimum block size is 24.
 *
 * The free list works by having one global pointer to the head
 * free block. Each pointer is located one space to the left of the ptr
 * to the next block in the list, and holds the address of the previous block
 * in the list.
 *
 * There is a small segregation factor of the free list because
 * there is a seperate free list for very large blocks.
 * This ensure that if the user wants to allocate a large size  block,
 * its lookup time will be less because it only has to look at the
 * large-block free-list, which will be inherently shorter.
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif


/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define SIZE_PTR(p)  ((size_t*)(((char*)(p)) - SIZE_T_SIZE))

/* Basic constants and macros, taken from APP:CS */

#define WSIZE 4
#define DSIZE 8     /* Word and header/footer size (bytes) */
#define CHUNKSIZE  (1<<7)  /* Extend heap by this amount (bytes) */
#define MAX(x, y) ((x) > (y)? (x) : (y))
#define HEADER_SIZE 24 /* minimum block size (with pointers) */

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* defines the cutoff for the large part of the seg free list */
#define BIG_SIZE 10000 * HEADER_SIZE

/* Given a pointer to a free block, compute
 address of next and prebvious blocks. The next block is stored one
 memory location down the heap. */
#define NEXT_FREE_BLK(bp) (*((char **)(bp) + 1))
#define PREV_FREE_BLK(bp) (*(char **)(bp))

static void *extend_heap(size_t w);
static void *coalesce(void * block_ptr);
static void *find_fit(size_t size);
static void place(void *b, size_t size);
static void insert_free(void * b, size_t size);
static void remove_free(void * b);

/* GLOBAL variables total 24 bytes for 3 pointers */
/* global heap pointer that will point to the first memory block */
static char * heap_pointer;

/* global pointer that points to the head of the free list */
static char * free_head;

/* global pointer that points to the head of the large size free list */
static char * big_free_head;

/*
 * mm_init - Called when a new trace starts.
 */
int mm_init(void) {
    /* initalize the heap pointer to our allocated memory */
    heap_pointer = mem_sbrk(4 * WSIZE);

    /* If the inital allocation fails, return -1 */
    if (heap_pointer == (void *)-1) {
      return -1;
    }

    /* initialize both list head pointers to 0*/
    free_head = 0;
    big_free_head = 0;

    /* Create the start of the heap before prologue. Alignment padding */
    PUT(heap_pointer, 0);
    /* Create the prologue header (4 bytes) */
    PUT(heap_pointer + WSIZE, PACK(2 * WSIZE, 1));
    /* Prologue footer (4 bytes) */
    PUT (heap_pointer + (WSIZE * 2), PACK(2 * WSIZE, 1));
    /* Epilogue (4 bytes) */
    PUT (heap_pointer + (WSIZE * 3), PACK(0, 1));

    /* Set the global heap_pointer to the first memory block.
       In this case, it points to the prologue. */
    heap_pointer += ALIGNMENT;

    /* We will need to extend the heap here later. */
    char* extended = extend_heap(CHUNKSIZE / (WSIZE * 6));
    if (extended == NULL) {
        return -1;
    }
    /* Ensure that our heap was created successfully. */
    mm_checkheap(2);

    return 0;

}

/*
 * extend_heap - increases the memory allocation of the heap
*/

static void *extend_heap(size_t w) {
    size_t s;
    /* Need and even number of spaces so we can allocate bytes. */
    if (w % 2 == 0) {
        s = w * WSIZE;
    }
    else {
      s = (w + 1) * WSIZE;
    }

    /* Must be at least 24 bytes */
    if (s < HEADER_SIZE) {
        s = HEADER_SIZE;
    }
    /* Check the memset pointer and return the block pointer. */
    char * block_ptr;
    if ((long)(block_ptr = mem_sbrk(s)) == -1) {
          return NULL;
    }

    /* Free block header/footer (after old epilogue) */
    /* new block header */
    PUT(HDRP(block_ptr), PACK(s, 0));
    /* new block footer */
    PUT(FTRP(block_ptr), PACK(s, 0));
    /* New epilogue */
    PUT(HDRP(NEXT_BLKP(block_ptr)), PACK(0, 1));

    /* We may need to coalesce the block that was just extended
       This ensure we add it to our free list */
    return coalesce(block_ptr);

}

/*
 * malloc - Allocate a block by incrementing the brk pointer.
 *      Always allocate a block whose size is a multiple of the alignment.
 */
void *malloc(size_t size) {
    /* mm_checkheap(1); */
    if (size == 0) {
        return NULL;
    }

    /* fix for overhead and alignment */
    size = MAX(ALIGN(size) + ALIGNMENT,  HEADER_SIZE);

    /* Find the first free fit using our find function and place */
    char * ptr = find_fit(size);
    if (ptr != NULL) {
        place(ptr, size);
        return ptr;
    }

    else {
        /* We need to allocate more space using extend_heap */
        void * extend = extend_heap(MAX(size, CHUNKSIZE) / WSIZE);
        if (!extend) {
            /* not enough memory */
            return NULL;
        }
        else {
            place(extend, size);
            return extend;
        }
    }

  return NULL;

}

/*
 * free - We don't know how to free a block.  So we ignore this call.
 *      Computers have big memories; surely it won't be a problem.
 */
void free(void *ptr) {
    // printf("free\n");
    if (ptr == NULL) {
        return;
    }

    /* get the size of current block */
    size_t size = GET_SIZE(HDRP(ptr));

    /* set the header and footer flags */
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    /* coalesce with adjecent blocks */
    coalesce(ptr);

}

/*
 * realloc - Change the size of the block by mallocing a new block,
 *      copying its data, and freeing the old block.
 */
void *realloc(void *oldptr, size_t size) {
    size_t oldsize;
    void *newptr;

    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0) {
        free(oldptr);
        return 0;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if(oldptr == NULL) {
        return malloc(size);
    }
    newptr = malloc(size);

    /* If realloc() fails the original block is left untouched  */
    if(!newptr) {
        return 0;
    }

    /* Copy the old data. */
    oldsize = *SIZE_PTR(oldptr);
    if(size < oldsize) oldsize = size;
    memcpy(newptr, oldptr, oldsize);

    /* Free the old block. */
    free(oldptr);

    return newptr;
}

/*
 * calloc - Allocate the block and set it to zero.
 */
void *calloc (size_t nmemb, size_t size) {
    size_t bytes = nmemb * size;
    void *newptr;

    newptr = malloc(bytes);
    memset(newptr, 0, bytes);

    return newptr;
}

/*
 * mm_checkheap - Checks the heap for various corrrect
  * features
 */
void mm_checkheap(int verbose) {
    /* Checks epilogue and prologue Blocks */

    /* Checks prologue */
    if (!GET_ALLOC(HDRP(heap_pointer))) {
        fprintf(stderr, "prologue not allocated");
        exit(1);
    }

    if (!GET_ALLOC(mem_heap_hi() - 3)) {
        fprintf(stderr, "epilogue not allocated");
        exit(1);
    }

    /* Check address alignments */

    /* Check each block's header/footer */

    /* loop through all blocks, check the size, header footer matching */
    char * b;
    int free_blocks = 0;
    for (b = NEXT_BLKP(heap_pointer); GET_SIZE(HDRP(b)) > 0; b = NEXT_BLKP(b)) {
        char * next = NEXT_BLKP(b);
        /* check if the payload exceeds the last thing in the heap */
        if (b > (char*)mem_heap_hi() || b < (char*)mem_heap_lo()) {
            fprintf(stderr, "memory out of bounds of heap");
            exit(1);
        }
        /* check for min size, header, footer equality */
        if (GET_SIZE(HDRP(b)) < HEADER_SIZE) {
            fprintf(stderr, "size of block too small\n");
            exit(1);
        }
        /* check for address alignment */
         if ((size_t)(HDRP(b)) % 8 == 0) {
            fprintf(stderr, "blocks not aligned\n");
            exit(1);
        }
        if (GET_SIZE(HDRP(b)) != GET_SIZE(FTRP(b)) ||
            GET_ALLOC(HDRP(b)) != GET_ALLOC(FTRP(b))) {
            fprintf(stderr, "header and footer don't match\n");
            exit(1);

        }
        /* check for missing coalesces */
        if (!GET_ALLOC(HDRP(b)) && !GET_ALLOC(HDRP(next))) {
            fprintf(stderr, "Missing coalesces \n");
            exit(1);
        }

        /* add to our free block counter if needed */
        if (!GET_ALLOC(HDRP(b))) {
          free_blocks++;
        }

    }

    /* Check the small free list */
    int list_free_blocks = 0;
    char * fp;
    for(fp = free_head; fp != 0; fp = NEXT_FREE_BLK(fp)) {
        list_free_blocks += 1;
        /* check to make sure pointers are in bounds */
        if (fp > (char*)mem_heap_hi() || fp < (char*)mem_heap_lo()) {
            fprintf(stderr, "list pointer out of bounds of heap");
            exit(1);
        }

        /* check for pointer bi-directionality matiching */
        if(NEXT_FREE_BLK(fp) != 0) {
            char * b = NEXT_FREE_BLK(fp);
            char * b_prev = PREV_FREE_BLK(b);
            if (fp != b_prev) {
                fprintf(stderr, "next/prev pointers not consistent\n");
                exit(1);
            }
        }
        if (GET_SIZE(HDRP(fp)) > BIG_SIZE) {
              fprintf(stderr, "free block too large for small list");
              exit(1);
        }


    }
    /* checks the big free list */
    for(fp = big_free_head; fp != 0; fp = NEXT_FREE_BLK(fp)) {
        list_free_blocks += 1;
        /* check to make sure pointers are in bounds */
        if (fp > (char*)mem_heap_hi() || fp < (char*)mem_heap_lo()) {
            fprintf(stderr, "free list pointer out of heap bounds");
            exit(1);
        }

        /* check for pointer bi-directionality matiching */
        if(NEXT_FREE_BLK(fp) != 0) {
            char * b = NEXT_FREE_BLK(fp);
            char * b_prev = PREV_FREE_BLK(b);
            if (fp != b_prev) {
                fprintf(stderr, "next/prev pointers not consistent\n");
                exit(1);
            }
        }
        if (GET_SIZE(HDRP(fp)) < BIG_SIZE) {
              fprintf(stderr, "free block too small for big list");
              exit(1);
        }


    }

    /* Checks if the number of free blocks in the heap and in our lists are
       the same */
    if(free_blocks != list_free_blocks) {
          fprintf(stderr, "number of free blocks doesn't match list\n");
          printf("%d free_blocks and %d list free blocks\n\n", free_blocks,
                                                          list_free_blocks);
          exit(1);
    }

}


/*
 * Coalesces the blocks around the given plock pointer b
*/
static void * coalesce(void * b) {

  /* Check if the previous and next blocks are free */
  size_t prev = GET_ALLOC(FTRP(PREV_BLKP(b)));
  size_t next = GET_ALLOC(HDRP(NEXT_BLKP(b)));


  /* Both surrounding blocks are allocated, no need to coalesce */
  if (prev && next) {
    /* Get the size of the current block */
      size_t size = GET_SIZE(HDRP(b));
      insert_free(b, size);
      return b;
  }
  else if (prev && !next) {
    /* Get the size of the current block */
      size_t size = GET_SIZE(HDRP(b));
     /* add the size of the next block */
      size += GET_SIZE(HDRP(NEXT_BLKP(b)));
      /* remove the next block from the free list*/
      remove_free(NEXT_BLKP(b));
      /* Use the current header and pack it with the new size */
      PUT(HDRP(b), PACK(size, 0));
      /* Macros automatically adjust footer */
      PUT(FTRP(b), PACK(size, 0));

      /* update the free list */
      insert_free(b, size);
  }
  /* Coalesce with the previous block */
  else if (!prev && next) {

      /* Change the size */
      /* Get the size of the current block */
      size_t size = GET_SIZE(HDRP(b));
      size += GET_SIZE(HDRP(PREV_BLKP(b)));
      /* Go back to the previous block */
      b = PREV_BLKP(b);

      /* Remove the  previous block from the free list */
      remove_free(b);

      /* Changes the header of the previous block */
      PUT(HDRP(b), PACK(size, 0));
      /* change the new footer of the previous block */
      PUT(FTRP(b), PACK(size, 0));

      /* update the free list */
      insert_free(b, size);
  }

  else {

      /* Coalesce with both adjacent blocks */
      /* Get the size of the current block */
      size_t size = GET_SIZE(HDRP(b));
      size += GET_SIZE(HDRP(PREV_BLKP(b))) + GET_SIZE(FTRP(NEXT_BLKP(b)));
      /* remove_block(b); */

      /* Remove the  previous block from the free list */
      remove_free(PREV_BLKP(b));
      /* remove the next block from the free list */
      remove_free(NEXT_BLKP(b));

      /* Change the previous header and next footer */
      PUT(HDRP(PREV_BLKP(b)), PACK(size, 0));
      PUT(FTRP(NEXT_BLKP(b)), PACK(size, 0));
      b = PREV_BLKP(b);

      /* update the free list */
      insert_free(b, size);


  }

    return b;
}


/*
 * find_fit traverses the heap from the start and returns a pointer to the
 * first block of sufficient size that fits the memory size
 */

static void * find_fit(size_t size) {

    /* chose which seg list to traverse besed on size */
    char * bp;
    if (size > BIG_SIZE) {
        bp = big_free_head;
    }
    else {
        bp = free_head;
    }


    if (bp == 0) {
        return NULL;
    }

    /* Loop the free list pointer */
    while (bp != 0) {
        if (size <= GET_SIZE(HDRP(bp))) {
            return bp;
        }

        bp = NEXT_FREE_BLK(bp);
    }

    // printf("find finish null \n");
    return NULL;


  }

/*
 * allocates a block of size to b's location
 */
void place (void * b, size_t asize) {
    size_t full_size = GET_SIZE(HDRP(b));

    if ((full_size - asize) >= HEADER_SIZE) {

        /* If enough space, split block */
        PUT(HDRP(b), PACK(asize, 1));
        PUT(FTRP(b), PACK(asize, 1));
        /* Explicit, so remove block from free list */
        remove_free(b);
        /* move the pointer to the next block */
        b = NEXT_BLKP(b);
        PUT(HDRP(b), PACK(full_size - asize, 0));
        PUT(FTRP(b), PACK(full_size - asize, 0));
        coalesce(b);
  }
  else
  {
      /* Otherwise, don't split block and just allocate */
      remove_free(b);
      PUT(HDRP(b), PACK(full_size, 1));
      PUT(FTRP(b), PACK(full_size, 1));
  }

}

/*
 * insert_free - inserts b at head of free blocks list
 */
static void insert_free(void * b, size_t size)
{
      /* if we have a small insert size, use free_head */
      if (size <= BIG_SIZE) {
          if (free_head == 0 ) {
              // printf("insert into empty\n");
              free_head = b;
              NEXT_FREE_BLK(b) = 0;
              PREV_FREE_BLK(b) = 0;
          }

          else {
            // printf("insert into front\n");
            PREV_FREE_BLK(free_head) = b;
            PREV_FREE_BLK(b) = 0;
            NEXT_FREE_BLK(b) = free_head;

            /* set the new head ptr */
            free_head = b;
          }
     }
     /* if we have a big insert size, use the big list.
        It has the same process as the small size list */
     else {
         if (big_free_head == 0) {
             big_free_head = b;
             NEXT_FREE_BLK(b) = 0;
             PREV_FREE_BLK(b) = 0;
         }

         else {
           PREV_FREE_BLK(big_free_head) = b;
           PREV_FREE_BLK(b) = 0;
           NEXT_FREE_BLK(b) = big_free_head;
           big_free_head = b;
         }
    }

}

/*
 * fremove_free - removes the block at b from the free list
 */

static void remove_free(void * b) {
    size_t size = GET_SIZE(HDRP(b));
    /* remove from the small list if size less than the cutoff */
    if (size <= BIG_SIZE) {
        /* list empty */
        if (free_head == 0) {
            /* printf("no free blocks to allocate small \n" ); */
            return;
        }
        /* list only has one element */
        else if (PREV_FREE_BLK(b) == 0 && NEXT_FREE_BLK(b) == 0) {
            free_head = 0;
            // printf("one element list, remove\n");
        }
        /* Remove the head of the list*/
        else if (PREV_FREE_BLK(b) == 0) {
            PREV_FREE_BLK(NEXT_FREE_BLK(b)) = 0;
            free_head = NEXT_FREE_BLK(b);
        }
        /* remove tail of list */
        else if (NEXT_FREE_BLK(b) == 0) {
            /* removing the last point in the list */
            NEXT_FREE_BLK(PREV_FREE_BLK(b)) = 0;
        }
        /* take from middle of list */
        else {
            // adjust the previous and next
            PREV_FREE_BLK(NEXT_FREE_BLK(b)) = PREV_FREE_BLK(b);
            NEXT_FREE_BLK(PREV_FREE_BLK(b)) = NEXT_FREE_BLK(b);

        }
    }
    else {
        /* do the same thing with the big size free list */
        if (big_free_head == 0) {
            /* printf("no free blocks to allocate \n" ); */
            return;
        }
        else if (PREV_FREE_BLK(b) == 0 && NEXT_FREE_BLK(b) == 0) {
            big_free_head = 0;
        }
        else if (PREV_FREE_BLK(b) == 0) {
            PREV_FREE_BLK(NEXT_FREE_BLK(b)) = 0;
            big_free_head = NEXT_FREE_BLK(b);
        }
        else if (NEXT_FREE_BLK(b) == 0) {
            NEXT_FREE_BLK(PREV_FREE_BLK(b)) = 0;
        }
        else {
            PREV_FREE_BLK(NEXT_FREE_BLK(b)) = PREV_FREE_BLK(b);
            NEXT_FREE_BLK(PREV_FREE_BLK(b)) = NEXT_FREE_BLK(b);
        }

    }
}
