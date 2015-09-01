//******************************{begin:header}******************************//
//           cgenerics - The C Library for Generic Data Structures          //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
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

int main(void) {

	// Create a new doubly linked list.
	cg_hashmap *map = cg_inthashmap_create();

	// Configure the hashmap to use strings as values
	cg_hashmap_use_string_values(map);

	// Insert keys 10x the number of buckets.
	// to make sure that at least one bucket
	// contains two entries.
	int n = map->num_buckets * 10;
	char buffer[0x200];
	for (int i = 0; i < n; i++) {
		sprintf(buffer, "Key %d", i);
		cg_inthashmap_put(map, i, buffer);
	}

	// Display the number of keys
	printf("Keys in the map: %d\n", cg_hashmap_get_size(map));

	// Traverse map
	cg_listiterator *iter = cg_listiterator_create();
	cg_listiterator_begin(iter, map->keylist);
	while (cg_listiterator_has_next(iter)) {
		cg_hashmapentry *entry = cg_listiterator_next(iter);
		const int *key = entry->key;
		const char *s = entry->itemptr;
		printf("Key %d has value <%s>\n", *key, s);
	}

	printf("Get an item...\n");

	int key = n-10;
	const char *s = cg_inthashmap_get(map, key);

	printf("key=%d, string=\"%s\"\n", key, s);
	cg_inthashmap_delete(map, key);

	// Make sure the item really disappeared
	s = cg_inthashmap_get(map, key);
	printf("key=%d, string=\"%s\"\n", key, s);

	printf("Deinitialization\n");

	// Free the iterator
	cg_listiterator_free(iter);

	// Free the list.
	cg_inthashmap_free(map);

	return 0;
}
