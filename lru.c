#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

static unsigned long time_counter; //the time counter.

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
    //evict the page that is oldest (hasn't been used in a long time).

    // the page that is to be evicted
    int victim  = 0;
    // oldest time stamp tracker
    unsigned long oldest = coremap[0].time_in;

    int i;
    for (i = 0; i < memsize; i++) {
        if (coremap[i].time_in < oldest) {
                oldest = coremap[i].time_in;
                victim = i;
        }
    }

    return victim ;
}


/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {

    //assign p with a time stamp.
    coremap[p->frame >> PAGE_SHIFT].time_in = time_counter;
    time_counter++;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	//set counter to 0
	time_counter = 0;
}
