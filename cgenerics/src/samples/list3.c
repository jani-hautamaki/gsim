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
 * Linked-list sample #3: removing elements using an iterator.
 *
 */

#include "cgenerics/generic_list.h"
#include "cgenerics/generic_listiterator.h"

// malloc, free
#include <stdlib.h>
// sprintf, printf
#include <stdio.h>

int main() {

	// Create a new doubly linked list.
	generic_list *list = generic_list_create();

	// Use stdlib's free() to deallocate item pointers.
	list->free = free;

	// Populate the list with 10 strings.
	printf("Populating the list...\n");
	for (int i = 0; i < 10; i++) {
		char *s = malloc(128);
		sprintf(s, "This is item #%d", i);
		generic_list_append(list, s);
	}

	// Compute the list's size
	printf("List size: %d\n", generic_list_get_size(list));

	// Create an iterator
	generic_listiterator *iter = generic_listiterator_create();
	// Create an index counter
	int i;

	// Reset iterator position.
	generic_listiterator_begin(iter, list);

	// Remove every other element using the iterator.
	i = 0;
	while (generic_listiterator_has_next(iter)) {

		// Move backward
		generic_listiterator_next(iter);

		// Triggered at positions 1, 3, 5, ...
		// So the removed elements will be 0, 2, 4, ...
		if (i & 1) {
			generic_listiterator_remove_previous(iter);
		}

		// Update item counter
		i++;
	}

	// Reset iterator position.
	generic_listiterator_begin(iter, list);

	// Display the list's contents
	i = 0;
	while (generic_listiterator_has_next(iter)) {
		// Pick the next itemptr
		char *s = generic_listiterator_next(iter);

		// Output the item
		printf("Item %d: \"%s\"\n", i, s);

		// Update item counter
		i++;
	}

	// Free the iterator
	generic_listiterator_free(iter);

	// Free the list.
	generic_list_free(list);

	return 0;
}
