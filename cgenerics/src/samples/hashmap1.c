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
 * Hash map sample #1: basic usage.
 *
 */

#include "cgenerics/cg_list.h"
#include "cgenerics/cg_listiterator.h"
#include "cgenerics/cg_hashmap.h"

// malloc, free
#include <stdlib.h>
// sprintf, printf
#include <stdio.h>
// strcmp
#include <string.h>

uint32_t strhash(const char *s) {
	uint32_t h = 2166136261;
	while (*s != '\0') {
		h = (h * 16777619) ^ (*s);
		s++;
	}

	return h;
}

int main() {

	// Create a new doubly linked list.
	cg_hashmap *map = cg_hashmap_create();

	// Conversion "const char * -> const void *" is implicit.
	// Therefore the function signature should be considered
	// also implicitly valid.
	map->key_hash = (void *) strhash;
	map->key_isequal = (void *) strcmp;
	map->key_free = (void *) free;
	map->item_free = free;

	// Populate the list with 10 strings.
	printf("Populating the map...\n");
	for (int i = 0; i < 10; i++) {
		char *key = malloc(128);
		char *value = malloc(127);

		sprintf(key, "%02d", 1001+i);
		sprintf(value, "This is item #%d", i+1);
		printf("CALLING PUT...\n");
		cg_hashmap_put(map, key, value);
	}

	// Create an iterator starting from the beginning of the list.
	cg_listiterator *iter = cg_listiterator_create();

	// Traverse the hash table in the insertion order.
	cg_listiterator_begin(iter, map->keylist);
	int i = 0;
	while (cg_listiterator_has_next(iter)) {
		// Pick the next itemptr
		cg_hashmapentry *entry
		    = cg_listiterator_next(iter);

		// Output the item
		printf("Item %d: key=\"%s\", value: \"%s\"\n",
		    i, (char *) entry->key, (char *) entry->itemptr);

		// Retrieval..
		void *itemptr = cg_hashmap_get(map, entry->key);
		printf("get(): %s\n", (char *) itemptr);

		// Update item counter
		i++;
	}

	// Free the iterator
	cg_listiterator_free(iter);

	// Free the list.
	cg_hashmap_free(map);

	return 0;
}
