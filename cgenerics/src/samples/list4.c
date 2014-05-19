//******************************{begin:header}******************************//
//           cgenerics - The C Library for Generic Data Structures          //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://code.google.com/p/gsim
//
//      Copyright (C) 2013-2014 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

/**
 *
 * Linked-list sample #4: sorting
 *
 */

#include "cgenerics/cg_list.h"
#include "cgenerics/cg_listiterator.h"

// malloc, free, rand, srand
#include <stdlib.h>
// sprintf, printf
#include <stdio.h>

void dump_list(cg_list *list) {
	cg_listiterator iter;
	cg_listiterator_begin(&iter, list);

	while (cg_listiterator_has_next(&iter)) {
		int *val_ptr = cg_listiterator_next(&iter);
		printf("%d  ", *val_ptr);
	}
	printf("\n");
}

static int compare_int(const int *a, const int *b) {
	return *a - *b;
}

int main() {

	// Create a new doubly linked list.
	cg_list *list = cg_list_create();

	// Use stdlib's free() to deallocate item pointers.
	list->free = free;

	// constant seed in order to reproduce the behaviour
	srand(0x1F17F01F);

	// Populate the list with 10 strings.
	printf("Populating the list...\n");
	for (int i = 0; i < 20; i++) {
		int *val_ptr = malloc(sizeof(int));
		*val_ptr = rand() % 1000;
		cg_list_append(list, val_ptr);
	}

	printf("The list before sorting\n");
	dump_list(list);

	printf("Sorting...\n");
	cg_list_bubblesort(list, (void *) compare_int);

	printf("Sorted list\n");
	dump_list(list);

	// Free the list.
	cg_list_free(list);

	return 0;
}
