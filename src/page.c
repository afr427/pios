#include "page.h"
#include <stddef.h>

#define PAGE_SIZE 0x200000 // 2MB page size
#define TOTAL_PAGES 128


struct ppage physical_page_array[TOTAL_PAGES]; // 128 pages, each 2mb in length covers 256 megs of memory
					       //
struct ppage *free_list_head = NULL;
				  
void init_pfa_list(void) {

    for (int i = 0; i < TOTAL_PAGES; i++) {

        physical_page_array[i].physical_addr = (void *)(i * PAGE_SIZE);
        physical_page_array[i].prev = (i > 0) ? &physical_page_array[i-1] : NULL;
        physical_page_array[i].next = (i < TOTAL_PAGES-1) ? &physical_page_array[i+1] : NULL;

    }

    free_list_head = &physical_page_array[0];

}

struct ppage *allocate_physical_pages(unsigned int npages) {
	
	if (npages == 0 || free_list_head == NULL) {

        	return NULL;

   	}

    struct ppage *allocated_head = free_list_head;
    struct ppage *current = allocated_head;

    for (unsigned int i = 1; i < npages; i++) {

        if (current->next == NULL) {

            return NULL;

        }

        current = current->next;

    }

    free_list_head = current->next;

    if (free_list_head) {

        free_list_head->prev = NULL;

    }
    
    current->next = NULL;

    return allocated_head;

}

void free_physical_pages(struct ppage *ppage_list) {
	
	if (ppage_list == NULL) {

        	return;

   	}

    struct ppage *last = ppage_list;

    while (last->next != NULL) {

        last = last->next;

    }

    last->next = free_list_head;

    if (free_list_head) {

        free_list_head->prev = last;

    }

    free_list_head = ppage_list;
    ppage_list->prev = NULL;

}
