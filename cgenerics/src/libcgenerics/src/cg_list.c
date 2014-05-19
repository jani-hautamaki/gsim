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

#include "cgenerics/cg_list.h"

// malloc, free, NULL
#include <stdlib.h>

extern cg_list *cg_list_create() {
	cg_list *list = NULL;
	do {
		list = malloc(sizeof(cg_list));
		if (list == NULL) break;
		cg_list_init(list);
	} while(0);

	return list;
}

extern void cg_list_init(cg_list *list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->err = 0;
	list->free = NULL;
}

extern void cg_list_free(cg_list *list) {
	if (list != NULL) {
		cg_list_deinit(list);
		free(list);
	}
}

extern void cg_list_deinit(cg_list *list) {
	cg_list_delete_all(list);
}

// STATIC HELPERS
//================


/**
 * Allocate a list-entry, and initialize it with the itemptr.
 * Returns an initialized list-entry, or NULL if out of memory.
 */
static cg_listentry *cg_listentry_create(void *itemptr) {
	cg_listentry *entry = NULL;

	do {
		// Allocate
		entry = malloc(sizeof(cg_listentry));
		if (entry == NULL) break;

		// Initialize if the allocation succeeded
		entry->prev = NULL;
		entry->next = NULL;
		entry->itemptr = itemptr;
	} while(0);

	// Return an initialized list entry,
	// or NULL if the allocation failed.
	return entry;
}

/**
 * Free the list-entry.
 */
static void cg_listentry_free(cg_listentry *entry) {
	if (entry != NULL) {
		entry->itemptr = NULL;
		entry->next = NULL;
		entry->prev = NULL;

		free(entry);
	}
}

// INTERNAL METHODS
//==================

/**
 * Remove list-entry and return the itemptr.
 * For internal use only.
 */
extern void *cg_list_remove(
    cg_list *list,
    cg_listentry *entry
) {
	// Detach the entry from the list
	void *itemptr = entry->itemptr;
	cg_listentry *prev = entry->prev;
	cg_listentry *next = entry->next;

	if (next != NULL) {
		next->prev = prev;
	}

	if (prev != NULL) {
		prev->next = next;
	}

	// Maintain the list
	if (list->tail == entry) {
		list->tail = entry->prev;
	}
	if (list->head == entry) {
		list->head = entry->next;
	}

	// Deinitialize the detached entry
	cg_listentry_free(entry);

	// Maintain the number of entries.
	list->size--;

	return itemptr;
}


/**
 * Remove list-entry and free itemptr.
 * For internal use only.
 */
extern void cg_list_delete(
    cg_list *list,
    cg_listentry *entry
) {

	// Detach the entry from the list
	void *itemptr = cg_list_remove(list, entry);
	if  ((list->free != NULL) && (itemptr != NULL)) {
		list->free(itemptr);
	}
}

/**
 * Creates and inserts a new list-entry after the given one.
 * Returns the new list-entry, or NULL if out of memory.
 */
extern cg_listentry *cg_list_insert_after(
    cg_list *list,
    cg_listentry *entry,
    void *itemptr
) {
	cg_listentry *rval = NULL;

	do {
		// If out of memory, return NULL.
		rval = cg_listentry_create(itemptr);
		if (rval == NULL) break;

		// Move the succeeding entry forwards
		rval->next = entry->next;
		// Insert inbetween
		entry->next = rval;
		rval->prev = entry;

		if (rval->next == NULL) {
			// This is the list's new tail.
			list->tail = rval;
		} else {
			rval->next->prev = rval;
		}

		// Maintain the number of entries.
		list->size++;

	} while(0);


	return rval;
}

/**
 * Creates and inserts a new list-entry before the given one.
 * Returns the new list-entry, or NULL if out of memory.
 */
extern cg_listentry *cg_list_insert_before(
        cg_list *list,
        cg_listentry *entry,
        void *itemptr
) {
	cg_listentry *rval = NULL;

	do {
		// If out of memory, return NULL.
	    	rval = cg_listentry_create(itemptr);
		if (rval == NULL) break;

		// Move the preceding entry backwards
		rval->prev = entry->prev;
		// Insert inbetween
		entry->prev = rval;
		rval->next = entry;

		if (rval->prev == NULL) {
			// This is the list's new head
			list->head = rval;
		} else {
			rval->prev->next = rval;
		}

		// Maintain the number of entries.
		list->size++;

	} while(0);

	return rval;
}

// MISCELLANEOUS
//===============

extern int cg_list_get_size(
    const cg_list *list
) {
	return list->size;
}

// CONTENT INSERTION
//===================

/**
 * Appends an item to the end of the list.
 * Returns true if success, false if out of memory.
 */
extern int cg_list_append(
    cg_list *list,
    void *itemptr
) {
	cg_listentry *entry = NULL;

	do {
		entry = cg_listentry_create(itemptr);
		if (entry == NULL) break; // malloc failed, abort.

		cg_listentry *tail = list->tail;
		if (tail != NULL) {
			tail->next = entry;
			entry->prev = tail;
		} else {
			list->head = entry;
		}

		list->tail = entry;

		// Maintain the number of entries.
		list->size++;

	} while(0);

	return entry != NULL;
}

/**
 * Prependss an item to the beginning of the list.
 * Returns true if success, false if out of memory.
 */
extern int cg_list_prepend(
    cg_list *list,
    void *itemptr
) {

	cg_listentry *entry = NULL;
	do {
		entry = cg_listentry_create(itemptr);
		if (entry == NULL) break; // malloc failed, abort.

		cg_listentry *head = list->head;
		if (head != NULL) {
			head->prev = entry;
			entry->next = head;
		} else {
			list->tail = entry;
		}

		list->head = entry;

		// Maintain the number of entries.
		list->size++;

	} while(0);

	return entry != NULL;
}

// CONTENT REMOVAL
//=================

extern void cg_list_delete_first(
    cg_list *list
) {
	if (list->head != NULL) {
		cg_list_delete(list, list->head);
	} else {
		// Error, list already empty
	}
}

extern void cg_list_delete_last(
    cg_list *list
) {
	if (list->tail != NULL) {
		cg_list_delete(list, list->tail);
	} else {
		// Error, list already empty
	}
}

extern void *cg_list_remove_first(
    cg_list *list
) {
	void *itemptr = NULL;
	if (list->head != NULL) {
		itemptr = cg_list_remove(list, list->head);
	} else {
		// Error, list already empty
	}
	return itemptr;
}

extern void *cg_list_remove_last(
    cg_list *list
) {
	void *itemptr = NULL;
	if (list->tail != NULL) {
		itemptr = cg_list_remove(list, list->tail);
	} else {
		// Error, list already empty
	}
	return itemptr;
}

extern void cg_list_remove_all(
    cg_list *list
) {
	while (list->head != NULL) {
		cg_list_remove_first(list);
	}
}

extern void cg_list_delete_all(
    cg_list *list
) {
	while (list->head != NULL) {
		cg_list_delete_first(list);
	}
}

// SORTING
//=========

/**
 * Swap listentries and maintains head/tail
 */
extern void cg_list_swap(
    cg_list *list,
    cg_listentry *e1,
    cg_listentry *e2
) {

	cg_listentry *e1prev = e1->prev;
	cg_listentry *e1next = e1->next;

	cg_listentry *e2prev = e2->prev;
	cg_listentry *e2next = e2->next;

	if (e1next == e2) {
		e1->prev = e2;
		e2->next = e1;
	} else {
		e1->prev = e2prev;
		e2->next = e1next;

		if (e2prev != NULL) {
			e2prev->next = e1;
		} else {
			list->head = e1;
		}

		if (e1next != NULL) {
			e1next->prev = e2;
		} else {
			list->tail = e2;
		}
	}

	if (e2next == e1) {
		e1->next = e2;
		e2->prev = e1;
	} else {
		e1->next = e2next;
		e2->prev = e1prev;

		if (e2next != NULL) {
			e2next->prev = e1;
		} else {
			list->tail = e1;
		}
		if (e1prev != NULL) {
			e1prev->next = e2;
		} else {
			list->head = e2;
		}
	}
}

extern void cg_list_bubblesort(
    cg_list *list,
    int (*compare)(const void *itemptr1, const void *itemptr2)
) {
	cg_listentry *e = list->head;
	while (e != NULL) {
		cg_listentry *smallest = e;
		cg_listentry *cur = e->next;
		while (cur != NULL) {
			if (compare(smallest->itemptr, cur->itemptr) > 0) {
				// cur is smaller
				smallest = cur;
			}
			cur = cur->next;
		} // while

		if (smallest != e) {
			cg_list_swap(list, e, smallest);
			e = smallest->next;
		} else {
			e = e->next;
		}
	} // while
}

