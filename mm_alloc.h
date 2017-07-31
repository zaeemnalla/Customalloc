#pragma once
#ifndef custom_malloc_H
#define custom_malloc_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

// maximum block size in bytes: 2^20 = 1 MiB
#define MAX_EXP 20
#define MAX_SIZE 1<<MAX_EXP

// block struct
typedef struct block {
    size_t size;                                // size in bytes
    bool free;                                  // whether block is available for allocation
    void* data;                                 // pointer to actual data
    struct block* next;                         // pointer to next block in list
    struct block* buddy;                        // pointer to buddy
    struct block* merged_buddy[MAX_EXP];        // pointer list for buddies of the merged blocks up the 'tree'
} block;

// pointer to start of list
struct block* head;

// functions to be implemented in mm_alloc.c
void* mm_malloc(size_t size);
void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);

// debug functions
void print_block(struct block* b);
void print_list();

#endif
