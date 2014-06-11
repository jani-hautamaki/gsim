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
 * Hash map sample #3: Use strings as keys
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
	cg_hashmap *map = cg_hashmap_create();

	// Configure the hashmap to use strings as keys
	cg_hashmap_use_string_keys(map);

	// Configure the hashmap to use strings as values, too.
	cg_hashmap_use_string_values(map);

	int n = map->num_buckets * 10;
	char key[0x200];
	char val[0x200];
	for (int i = 0; i < n; i++) {
		sprintf(key, "Key %d", i);
		sprintf(val, "Value for key %d", i);
		cg_hashmap_put(map, key, val);
	}

	printf("Size of the map: %d\n", cg_hashmap_get_size(map));

	// Traverse the map
	cg_listiterator *iter = cg_listiterator_create();
	cg_listiterator_begin(iter, map->keylist);
	while (cg_listiterator_has_next(iter)) {
		cg_hashmapentry *entry = cg_listiterator_next(iter);
		const char *key_ptr = entry->key;
		const char *val_ptr = entry->itemptr;
		printf("Key <%s> has value <%s>\n", key_ptr, val_ptr);
	}

	// Remove the last key inserted
	cg_hashmap_delete(map, key);

	// See if it really disappeared
	printf("key=\"%s\", string=\"%s\"\n",
	    key, (char *) cg_hashmap_get(map, key));

	// Shut down and clean up.
	printf("Deinitialization\n");

	// Free the iterator
	cg_listiterator_free(iter);
	// Free the list.
	cg_hashmap_free(map);

	return 0;
}
