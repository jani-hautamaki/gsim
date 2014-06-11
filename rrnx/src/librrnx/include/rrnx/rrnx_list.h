//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

#ifndef RRNX_LIST_H
#define RRNX_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// DATA STRUCTURES
//============================================================================

struct rrnx_list_item {
        /** Pointer to the next item, or NULL. */
        struct rrnx_list_item *next;

        /** Pointer to the previous item, or NULL. */
        struct rrnx_list_item *prev;

        /** Payload data or NULL. */
        void *data;
};

typedef struct rrnx_list_item rrnx_list_item;

/**
 * Generic linked-list data structure
 */
struct rrnx_list {
        /** Pointer to the first (head) item, or NULL. */
        rrnx_list_item *first;
        /** Pointer to the last (tail) item, or NULL. */
        rrnx_list_item *last;
	/** Pointer to the destructor function */
	void (*destructor)(void*);
};

typedef struct rrnx_list rrnx_list;


//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

rrnx_list *rrnx_list_alloc(void);
void rrnx_list_free(rrnx_list *list);

//============================================================================
// METHODS: MANIPULATION
//============================================================================

rrnx_list_item *rrnx_list_append(rrnx_list *list, void* data);
void rrnx_list_remove(rrnx_list *list, rrnx_list_item *item);


// Iteration and accessing are not neccessary....

// TODO: opaque list iterator?

/*
// Return an iterator
rrnx_list_iter *rrnx_list_iterator(rrnx_list *list);
rrnx_list_iter *rrnx_list_next(rrnx_list_iter *iter);
int rrnx_list_has_next(const rrnx_list_iter *iter);
void *rrnx_list_data(rrnx_list_iter *iter);
*/

#ifdef __cplusplus
} // extern "C"
#endif

#endif
