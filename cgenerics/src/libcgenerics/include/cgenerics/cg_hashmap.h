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

#ifndef CG_HASHMAP_H
#define CG_HASHMAP_H

// uint32_t
#include <stdint.h>

// cg_listentry
#include "cg_list.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cg_hashmapentry {
	/**
	 * The key.
	 */
	void *key;

	/**
	 * The value.
	 */
	void *itemptr;

	/**
	 * Pointer to the bucket entry.
	 */
	cg_listentry *bucket_entry;

	/**
	 * Pointer to the keylist entry.
	 */
	cg_listentry *keylist_entry;
};

typedef struct cg_hashmapentry cg_hashmapentry;

/*
struct cg_hashmap_vtable {
};
typedef struct cg_hashmap_vtable cg_hashmap_vtable;
*/


struct cg_hashmap {

	/**
	 * Number of buckets
	 */
	unsigned int num_buckets;

	/**
	 * Array of buckets
	 */
	cg_list **buckets;

	/**
	 * Linked-list of keys
	 */
	cg_list *keylist;


	// FUNCTION POINTERS
	//===================

	/**
	 * Compares two keys
	 */
	int (*key_isequal)(const void *key1, const void *key2);

	/**
	 * Free a key.
	 */
	void (*key_free)(void *key);

	/**
	 * Computes a key hash value
	 */
	uint32_t (*key_hash)(const void *key);

	/**
	 * For making an identical copy of the key
	 */
	void *(*key_clone)(const void *key);

	/**
	 * For making an identical copy of the value
	 */
	void *(*item_clone)(const void *itemptr);

	/**
	 * Free an item
	 */
	void (*item_free)(void *itemptr);


	/**
	 * Error code
	 */
	int err;
};

typedef struct cg_hashmap cg_hashmap;

// CONSTRUCTORS & DESTRUCTORS
//============================

cg_hashmap *cg_hashmap_create(void);
void cg_hashmap_free(cg_hashmap *map);

void cg_hashmap_init(cg_hashmap *map);
void cg_hashmap_deinit(cg_hashmap *map);

// MANIPULATION AND RETRIEVAL
//============================

int cg_hashmap_put(cg_hashmap *map, const void *key, const void *itemptr);
void *cg_hashmap_get(cg_hashmap *map, const void *key);
void *cg_hashmap_remove(cg_hashmap *map, const void *key);
void cg_hashmap_delete(cg_hashmap *map, const void *key);

// OTHER METHODS
//===============

int cg_hashmap_get_size(const cg_hashmap *map);

// CONVENIENCE: INT HASHMAP
//==========================

cg_hashmap *cg_inthashmap_create(void);
void cg_inthashmap_free(cg_hashmap *map);

void *cg_inthashmap_get(cg_hashmap *map, int key);
void cg_inthashmap_put(cg_hashmap *map, int key, const void *itemptr);
void *cg_inthashmap_remove(cg_hashmap *map, int key);
void cg_inthashmap_delete(cg_hashmap *map, int key);


// CONVENIENCE: STR HASHMAP
//==========================

void cg_hashmap_use_string_values(cg_hashmap *map);
void cg_hashmap_use_string_keys(cg_hashmap *map);


/*

cg_hashmap *cg_strhashmap_create();
void cg_strhashmap_free(cg_hashmap *map);

void *cg_strhashmap_get(
    cg_hashmap *map,
    const char *key
);

void *cg_strhashmap_put(
    cg_hashmap *map,
    const char *key,
    const void *itemptr
);

*/


#ifdef __cplusplus
} // extern "C"
#endif

#endif
