/*----------------------------------------------
 * Author:Minolta Ndlovu 
 * Date: 04/23/2023
 * Description: A program that implements malloc
 * based on a free list and first fit.
 ---------------------------------------------*/
#include <unistd.h>
#include <stddef.h>


struct chunk {
  int size; // size of the chunk
  int used; // size of the used part of the chunk
  struct chunk *next; // pointer to the next chunk
};

struct chunk *flist = NULL; 

/*
* A function that allocates memory.
* @param size The size of the memory to allocate.
* @return A pointer to the allocated memory.
*/
void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  struct chunk *current = flist;
  struct chunk *previous = NULL;

  while (current) { // loop to find a chunk that is big enough and free
    if (current->size >= size) {
      current->used = size;
      if (previous) {
        previous->next = current->next; 
      } else {
        flist = current->next; 
      }
      return (void *)(current + 1);
    }
    previous = current;
    current = current->next;
  }

  size_t total_size = sizeof(struct chunk) + size; 
  struct chunk *new_chunk = (struct chunk *)sbrk(total_size); // sbrk allocates memory from the heap

  if (new_chunk == (void *)-1) {
    return NULL;
  }

  new_chunk->size = size;
  new_chunk->used = size;
  new_chunk->next = NULL;

  return (void *)(new_chunk + 1); // return the address of the chunk after the chunk header
}

/*
* A function that frees memory.
* @param ptr A pointer to the memory to free.
*/
void free(void *ptr) {
  if (!ptr) { // if ptr is NULL, do nothing
    return;
  }

  struct chunk *chunk_to_free = (struct chunk *)ptr - 1; // get the address of the chunk header
  chunk_to_free->next = flist; // add the chunk to the free list
  flist = chunk_to_free; // update the free list
}
