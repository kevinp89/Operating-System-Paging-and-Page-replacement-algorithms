#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#define MAXLINE 256

extern int memsize;

extern int debug;

extern struct frame *coremap;

extern char *tracefile; // Tracefile name, OPT needs to read before replay

static unsigned long *trace_vaddr_list; // An array of every vaddr in the tracefile
static unsigned long *frame_vaddr_list; // An array of each frame in memory's vaddr
static unsigned long line_count; // Number of lines in the tracefile, length of trace_vaddr_list.
static unsigned long line_index; // Current index in the trace_vaddr_list.

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {
	int victim = 0; // The victim frame to be returned.
	int max = -1; // Current maximum next_occurence.

	int i;
	for (i = 0; i < memsize; i++) {

		// If there is no next occurence, just return
		if (coremap[i].next_occurence == 0) {
			return i;
		}

		// If the next_occurence is higher than current max, new victim
		if (coremap[i].next_occurence >= max) {
			max = coremap[i].next_occurence;
			victim = i;
		}
	}
	return victim;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {
	// Get the frame number of the page.
	int i;
	int frame = (p->frame >> PAGE_SHIFT);

	// Set the current frame vaddr to the current vaddr in trace
	frame_vaddr_list[frame] = trace_vaddr_list[line_index];

	// Increment line index
	line_index++;

	// Reset the next occurence for this frame.
	coremap[frame].next_occurence = 0;

	// Find the next occurence of frame
	for (i = line_index; i < line_count; i++) {
		if (frame_vaddr_list[frame] == trace_vaddr_list[i]) {
			coremap[frame].next_occurence = i;
			break;
		}
	}
	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {
	FILE *fp; // Tracefile pointer
	if ((fp = fopen(tracefile, "r")) == NULL) {
		perror("Error opening tracefile. Abort.");
		exit(1);
	}
	
	// Get the number of lines/memory accesses in the trace file.
	int lines = 0;
	char temp[MAXLINE];
	while ((fgets(temp, MAXLINE, fp)) != NULL) {
		if (temp[0] != '=') {
			lines++;
		}
	}
	
	// Reset the file to the beginning.
	fseek(fp, 0, SEEK_SET);

	// Dynamically allocate space for the trace list and frame list.
	trace_vaddr_list = malloc(lines * sizeof(unsigned long));
	frame_vaddr_list = malloc(memsize * sizeof(unsigned long));

	// Read the actual virtual addresses into the array
	int indx = 0;
	char buf[MAXLINE];
	char type;
	addr_t virt_addr;
	while(fgets(buf, MAXLINE, fp) != NULL){
		if(buf[0] != '='){
			sscanf(buf, "%c %lx", &type, &virt_addr);
			trace_vaddr_list[indx] = virt_addr;
			indx++;
		}
	}

	// Initialize frame_vaddr_list.
	int i;
	for (i = 0; i < memsize; i++) {
		frame_vaddr_list[i] = 0;
	}

	line_index = 0;
	line_count = lines;

	// Close file, with error checking ofcourse.
	if (fclose(fp) != 0) {
		perror("Error closing tracefile.");
		exit(1);
	}
}
