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
 * Linked-list sample #2: inserting elements using an iterator.
 *
 */

#include "cgenerics/cg_list.h"
#include "cgenerics/cg_listiterator.h"

// malloc, free
#include <stdlib.h>
// sprintf, printf
#include <stdio.h>

int main() {

	// Create a new doubly linked list.
	cg_list *list = cg_list_create();

	// Use stdlib's free() to deallocate item pointers.
	list->free = free;

	// Populate the list with 10 strings.
	printf("Populating the list...\n");
	for (int i = 0; i < 10; i++) {
		char *s = malloc(128);
		sprintf(s, "This is item #%d", i);
		cg_list_append(list, s);
	}

	// Compute the list's size
	printf("List size: %d\n", cg_list_get_size(list));

	// Create an iterator
	cg_listiterator *iter = cg_listiterator_create();
	// Create an index counter
	int i;

	// Reset iterator position.
	// NOTE: The traversal is done in reverse order.
	cg_listiterator_end(iter, list);

	// Insert items in an alternating manner using the iterator.
	i = 0;
	while (cg_listiterator_has_previous(iter)) {

		// Move backward
		cg_listiterator_previous(iter);

		// Insert a new string
		char *s = malloc(128);
		sprintf(s, "This is insertion #%d", i);
		cg_listiterator_insert_after(iter, s);

		// Update item counter
		i++;
	}

	// Reset iterator position.
	cg_listiterator_begin(iter, list);

	// Display the list's contents
	i = 0;
	while (cg_listiterator_has_next(iter)) {
		// Pick the next itemptr
		char *s = cg_listiterator_next(iter);

		// Output the item
		printf("Item %d: \"%s\"\n", i, s);

		// Update item counter
		i++;
	}

	// Free the iterator
	cg_listiterator_free(iter);

	// Free the list.
	cg_list_free(list);

	return 0;
}
