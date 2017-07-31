#include "mm_alloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

void split_block(block* tmp, size_t size);
block* start_of_block(void *ptr);
void merge_block(block* tmp);

/** mm_malloc - allocate a block of at least size bytes **/
void*
mm_malloc(size_t size) {

    if ( (size==0) || (size>MAX_SIZE) )
        return NULL;

    if (head==NULL){
        void* start = sbrk(0);
        sbrk(MAX_SIZE);
        memset(start, 0, MAX_SIZE);
        head = (block*) start;
        head->size = MAX_SIZE;
        head->free = true;
        head->data = (char*)head + sizeof(block);
        head->next = NULL;
    }

    // looking for a block
    size_t block_size = size + sizeof(struct block);
    block* tmp = NULL;
    for(tmp=head; tmp!=NULL; tmp=tmp->next)
      if( (tmp->free) && (tmp->size>=block_size) )
        break;

    // block found
    if (tmp!=NULL){
        split_block(tmp, block_size);
        tmp->free = false;
        return tmp->data;
    }
    // no blocks available (ie. MAX_SIZE occupied, or no space of size available)
    else
        return NULL;

}

/** mm_free - marks a data block as free **/
void
mm_free(void* ptr) {

    if (ptr==NULL)
        return;

    block* tmp = start_of_block(ptr);
    tmp->free = true;
    merge_block(tmp);

}

/** mm_realloc - changes the data block to have the specified size **/
void*
mm_realloc(void* ptr, size_t size) {

    if (ptr==NULL)
        return mm_malloc(size);

    block* tmp = start_of_block(ptr);
    block* new = mm_malloc(size);

    // malloc succeeded (i.e. a block was allocated)
    if (new!=NULL){
        memcpy( new, ptr, sizeof(tmp->data) );
        mm_free(ptr);
        return new;
    }
    return NULL;

}

/*------------------------------------*\
|            DEBUG FUNCTIONS           |
\*------------------------------------*/

/** print_block - prints the metadata of a block **/
void
print_block(struct block* b) {
  if(!b) {
    printf("NULL block\n");
  }
  else {
    int i = 0;
    printf("Strt = %lx\n",(unsigned long) b);
    printf("Size = %lu\n",b->size);
    printf("Free = %s\n",(b->free)?"true":"false");
    printf("Data = %lx\n",(unsigned long) b->data);
    printf("Next = %lx\n",(unsigned long) b->next);
    printf("Buddy = %lx\n",(unsigned long) b->buddy);
    printf("Merged Buddies = ");
    while(b->merged_buddy[i] && i < MAX_EXP) {
        printf("%lx, ",(unsigned long) b->merged_buddy[i]);
        i++;
    }
    printf("\n\n");
  }
}

/** print_list - prints the metadata of all blocks **/
void
print_list() {
    struct block* curr = head;
    printf("--HEAP--\n");
    if(!head) printf("EMPTY\n");
    while(curr) {
        print_block(curr);
        curr = curr->next;
    }
    printf("--END--\n");
}

/*------------------------------------*\
|            EXTRA FUNCTIONS           |
\*------------------------------------*/

/** split_block - continously splits a block until obtaining one of the correct size **/
void
split_block(block *tmp, size_t size){

    if (tmp->size==size)
        return;
    else{

        // finding the correct power of 2 that the block size should be
        int pwr;
        for (pwr=0; pwr<=MAX_EXP; pwr++){
            if ( 1<<pwr >= size )
                break;
        }

        // splitting
        while ( tmp->size != 1<<pwr ){
        //split
            block* split = (block*) (tmp->data + sizeof(void*) + tmp->size/2);
            split->size = tmp->size/2;
            split->free = true;
			split->data = (char*)split + sizeof(block);
            split->next = tmp->next;

        //tmp
            tmp->size = tmp->size/2;
			tmp->data = (char*)tmp + sizeof(block);
            tmp->next = split;
        }

		tmp->next->buddy = tmp;
		tmp->buddy = tmp->next;

    }

}

/** start_of_block - gets the memory location of the start of the block that ptr (which is actually the data attribute of a block) belongs to **/
block* start_of_block(void *ptr){
    return (block*) (char*)(ptr - sizeof(block));
}

/** merge_block - continuously joins as many neighbour blocks (must be same size and both free) as it can **/
void merge_block(block* tmp){

	while( (tmp->buddy!=NULL) && (tmp->buddy->free) ){
		    tmp->size = tmp->buddy->size*2;
			tmp->data = (char*)tmp + sizeof(block);
			if (tmp->next==tmp->buddy)
		    	tmp->next = tmp->next->next;
			tmp->buddy = tmp->next;
			if (tmp->next!=NULL){
				tmp->buddy->buddy = tmp;
				if (tmp->next->next!=NULL)
					tmp->next->next->buddy=NULL;
			}
			head = tmp;
    }

}
