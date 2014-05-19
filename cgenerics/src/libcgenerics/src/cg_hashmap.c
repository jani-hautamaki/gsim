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

// malloc, free
#include <stdlib.h>
// strlen, strncpy, strcmp
#include <string.h>

#include "cgenerics/cg_hashmap.h"
#include "cg_list_internal.h"

// INTERNAL HELPERS: KEY AND VALUE MEMORY MANAGEMENT
//===================================================

static void *cg_hashmap_clone_key(cg_hashmap *map, const void *key) {
	void *rval = NULL;
	if ((map->key_clone != NULL) && (key != NULL)) {
		rval = map->key_clone(key);
	} else {
		rval = (void *) key;
	}
	return rval;
}

static void *cg_hashmap_clone_item(cg_hashmap *map, const void *itemptr) {
	void *rval = NULL;
	if ((map->item_clone != NULL) && (itemptr != NULL)) {
		rval = map->item_clone(itemptr);
	} else {
		rval = (void *) itemptr;
	} // if-else
	return rval;
}

static void cg_hashmap_free_key(cg_hashmap *map, void *key) {
	if ((key != NULL) && (map->key_free != NULL)) {
		map->key_free(key);
	}
}

static void cg_hashmap_free_item(cg_hashmap *map, void *itemptr) {
	if ((itemptr != NULL) && (map->item_free != NULL)) {
		map->item_free(itemptr);
	}
}

// INTERNAL HELPERS: NODE
//========================

static void cg_hashmap_free_entry(
    cg_hashmap *map,
    cg_list* bucket,
    cg_hashmapentry *mapentry
) {
	// If the node ptr is NULL, do nothing.
	if (mapentry == NULL) return;

	if (mapentry->keylist_entry != NULL) {
		// Remove the node from the keylist
		cg_list_remove(map->keylist, mapentry->keylist_entry);
		mapentry->keylist_entry = NULL;
	}

	if (mapentry->bucket_entry != NULL) {
		// Remove the node from the bucket
		cg_list_remove(bucket, mapentry->bucket_entry);
		mapentry->bucket_entry = NULL;
	}

	// Destroy item
	cg_hashmap_free_item(map, mapentry->itemptr);
	mapentry->itemptr = NULL;

	// Destroy key
	cg_hashmap_free_key(map, mapentry->key);
	mapentry->key = NULL;

	// Finally the node itself is freed
	free(mapentry);
}

static cg_hashmapentry *cg_hashmap_create_entry(
    cg_hashmap *map,
    cg_list *bucket
) {
	cg_hashmapentry *entry = NULL;
	int complete = 0;
	int ok = 0;

	do {
		// Allocate memory
		entry = malloc(sizeof(cg_hashmapentry));
		if (entry == NULL) break;

		// Initialize
		entry->bucket_entry = NULL;
		entry->keylist_entry = NULL;
		entry->key = NULL;
		entry->itemptr = NULL;

		// Attempt appending to the bucket.
		ok = cg_list_append(bucket, entry);
		if (!ok) break;
		entry->bucket_entry = bucket->tail;

		// Attempt appending to the keylist.
		ok = cg_list_append(map->keylist, entry);
		if (!ok) break;
		entry->keylist_entry = map->keylist->tail;

		// Construction complete
		complete = 1;
	} while(0);


	if (complete == 0) {
		// Destroy the partial node
		cg_hashmap_free_entry(map, bucket, entry);
		entry = NULL;
	}

	return entry;
}

static cg_hashmapentry *cg_hashmap_find_entry(
    cg_hashmap *map,
    cg_list *bucket,
    const void *key
) {
	// Return value
	cg_hashmapentry *rval = NULL;

	// For bucket traversal
	cg_listentry *listentry = bucket->head;

	// Find out whether an item with this key exists in the bucket
	while (listentry != NULL) {
		cg_hashmapentry *mapentry = listentry->itemptr;

		if (map->key_isequal(key, mapentry->key) == 0) {
			// Matching key found. Discontinue search.
			rval = mapentry;
			break;
		}

		// Move on to the next.
                listentry = listentry->next;
        } // while

	return rval;
}

// INTERNAL HELPERS: BUCKET
//==========================

static void cg_hashmap_free_bucket(cg_hashmap *map, cg_list *bucket) {

	// If the bucket is already freed, do nothing.
	if (bucket == NULL) return;

	// Free all nodes
	cg_listentry *e = NULL;
	while ((e = bucket->tail) != NULL) {
		cg_hashmap_free_entry(map, bucket, e->itemptr);
	}

	// Free the bucket itself
	free(bucket);
}

static cg_list *cg_hashmap_get_bucket(
    cg_hashmap *map,
    const void *key
) {
	// Compute hash value
	uint32_t hash = map->key_hash(key);

	// Determine the bucket for the hash
	uint32_t bnum = hash % ((uint32_t) map->num_buckets);

	// Retrieve the bucket
	return map->buckets[bnum];
}


// CONSTRUCTORS & DESTRUCTORS
//============================

extern cg_hashmap *cg_hashmap_create() {
	cg_hashmap *map = NULL;

	int complete = 0;
	do {
		map = malloc(sizeof(cg_hashmap));
		if (map == NULL) break;

		cg_hashmap_init(map);
		if (map->err) break;

		complete = 1;
	} while(0);

	if (!complete) {
		// Destroy the partial hashmap
		cg_hashmap_free(map);
		map = NULL;
	}

	return map;
}

extern void cg_hashmap_free(cg_hashmap *map) {
	if (map != NULL) {
		cg_hashmap_deinit(map);
	}
	free(map);
}


extern void cg_hashmap_init(cg_hashmap *map) {
	map->num_buckets = 0;
	map->buckets = NULL;
	map->keylist = NULL;

	map->key_isequal = NULL;
	map->key_free = NULL;
	map->key_hash = NULL;
	map->item_free = NULL;

	// Additional
	map->key_clone = NULL;
	map->item_clone = NULL;

	map->err = 0;

	// Attempt to create keylist and buckets

	int complete = 0;
	do {
		map->keylist = cg_list_create();
		if (map->keylist == NULL) break;

		int n = 128;
		map->num_buckets = n;
		map->buckets = malloc(n*sizeof(cg_list*));
		if (map->buckets == NULL) break;

		// Init buckets
		for (int i = 0; i < n; i++) {
			map->buckets[i] = NULL;
		}

		// Create buckets (each one is a list)
		int do_break = 0;
		for (int i = 0; i < n; i++) {
			map->buckets[i] = cg_list_create();
			if (map->buckets[i] == NULL) {
				// Init failed.
				// TODO:
				// Getting out of here would be a lot
				// easier using a goto statement.
				do_break = 1;
				break;
			}
		}
		if (do_break) break;

		// Initialization complete
		complete = 1;
	} while(0);

	if (!complete) {
		// Deinitialize partial hashmap, and mark error.
		cg_hashmap_deinit(map);
		map->err = 1;
	}
}

extern void cg_hashmap_deinit(cg_hashmap *map) {
	// free buckets

	if (map->buckets != NULL) {
		int n = map->num_buckets;
		for (int i = 0; i < n; i++) {
			// Free the bucket and mark it NULL if not already.
			cg_hashmap_free_bucket(map, map->buckets[i]);
			map->buckets[i] = NULL;
		}
		// Free the bucket array
		map->num_buckets = 0;
		free(map->buckets);
		map->buckets = NULL;
	}

	// Free the list itself.
	cg_list_free(map->keylist);
	map->keylist = NULL;
}

// ACCESS METHODS
//================

extern int cg_hashmap_put(
	cg_hashmap *map,
	const void *key,
	const void *itemptr
) {
	// When true, the operation may be carried on.
	int still_good = 1;

	// Get the bucket corresponding to the key
	cg_list *bucket = cg_hashmap_get_bucket(map, key);

	// Find if there's already an entry for the key
	cg_hashmapentry *mapentry
	    = cg_hashmap_find_entry(map, bucket, key);

	if (mapentry == NULL) {
		// Append map with a new entry that is inserted
		// into the current bucket.
		mapentry = cg_hashmap_create_entry(map, bucket);

		if (mapentry == NULL) {
			still_good = 0;
		}
	}

	if ((still_good) && (mapentry->key != key)) {
		cg_hashmap_free_key(map, mapentry->key);
		mapentry->key = cg_hashmap_clone_key(map, key);

		if ((key != NULL) && (mapentry->key == NULL)) {
			// Allocation failed
			still_good = 0;
		}
	}

	if ((still_good) && (mapentry->itemptr != itemptr)) {
		cg_hashmap_free_item(map, mapentry->itemptr);
		mapentry->itemptr = cg_hashmap_clone_item(map, itemptr);

		if ((itemptr != NULL) && (mapentry->itemptr == NULL)) {
			// Allocation failed.
			still_good = 0;
		}
	}

	if (!still_good) {
		cg_hashmap_free_entry(map, bucket, mapentry);
		mapentry = NULL;
	}

	return mapentry != NULL;
}

extern void *cg_hashmap_get(
	cg_hashmap *map,
	const void *key
) {
        // Get the bucket
        cg_list *bucket = cg_hashmap_get_bucket(map, key);

	// Find an entry corresponding to the key, or NULL.
	cg_hashmapentry *mapentry
	    = cg_hashmap_find_entry(map, bucket, key);

	// If an entry was found, return the itemptr.
	if (mapentry != NULL) {
		return mapentry->itemptr;
	}

	return NULL;
}

extern void *cg_hashmap_remove(
	cg_hashmap *map,
	const void *key
) {
	void *rval = NULL;

	// Get the bucket
        cg_list *bucket = cg_hashmap_get_bucket(map, key);

	// Find the map node, if such a key exists
	cg_hashmapentry *mapentry
	    = cg_hashmap_find_entry(map, bucket, key);

	if (mapentry != NULL) {
		// Pop the itemptr out before freeing the map node.
		rval = mapentry->itemptr;
		mapentry->itemptr = NULL;

		// Now the entry can bee freed.
		cg_hashmap_free_entry(map, bucket, mapentry);
		mapentry = NULL;
	}

	return rval;
}

extern void cg_hashmap_delete(
	cg_hashmap *map,
	const void *key
) {
	void *itemptr = cg_hashmap_remove(map, key);
	cg_hashmap_free_item(map, itemptr);
}

// OTHER METHODS
//===============

extern int cg_hashmap_get_size(
    const cg_hashmap *map
) {
	return cg_list_get_size(map->keylist);
}


// CONVENIENCE: STRING VALUES
//============================


static char *clone_str(const char *s) {
	// Compute string length (including terminating null)
	int size = strlen(s)+1;

	// Allocate and copy
	char *dest = malloc(size);
	strncpy(dest, s, size);

	return dest;
}

/**
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-param
 */
static uint32_t keyhash_str(const char *s) {

	// 32-bit 32 bit offset_basis = 2166136261
	// 32-bit FNV_prime 224 + 28 + 0x93 = 16777619

	// "The xor is performed on the low order octet (8 bits) of hash."
	uint32_t hash = 0x811C9DC5; // offset_basis

	while (*s != '\0') {
		uint32_t octet = ((uint32_t) (*s)) & 0xff;

		hash = hash ^ octet;
		hash = hash * 0x01000193; // FNV_prime

		s++;
	} // while

	return hash;
}

extern void cg_hashmap_use_string_values(cg_hashmap *map) {
	map->item_clone = (void *) clone_str;
	map->item_free = (void *) free;
}

extern void cg_hashmap_use_string_keys(cg_hashmap *map) {
	map->key_clone = (void *) clone_str;
	map->key_free = (void *) free;
	map->key_hash = (void *) keyhash_str;
	map->key_isequal = (void *) strcmp;
}

// CONVENIENCE: INT HASHMAP
//==========================

static uint32_t keyhash_int(const int *key) {
	return *key;
}

static int isequal_int(const int *a, const int *b) {
	return *a - *b;
}

extern cg_hashmap *cg_inthashmap_create() {
	cg_hashmap *map = cg_hashmap_create();

	// Initialize.
	map->key_free = free;
	map->key_isequal = (void *) isequal_int;
	map->key_hash = (void *) keyhash_int;

	return map;
}

extern void cg_inthashmap_free(cg_hashmap *map) {
	cg_hashmap_free(map);
}

extern void *cg_inthashmap_get(
    cg_hashmap *map,
    int key
) {
	return cg_hashmap_get(map, &key);
}

extern void cg_inthashmap_put(
    cg_hashmap *map,
    int key,
    const void *itemptr
) {
	// Dynamically create a copy of the key
	int *keyptr = malloc(sizeof(int));
	if (keyptr == NULL) {
		// Error. Out of memory.
		// TODO.
		return;
	}
	*keyptr = key;

	// Put it
	cg_hashmap_put(map, keyptr, itemptr);
}

extern void *cg_inthashmap_remove(
    cg_hashmap *map,
    int key
) {
	return cg_hashmap_remove(map, &key);
}

extern void cg_inthashmap_delete(
    cg_hashmap *map,
    int key
) {
	cg_hashmap_delete(map, &key);
}

