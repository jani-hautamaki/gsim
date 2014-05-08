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

#include "cgenerics/generic_list.h"

// malloc, free, NULL
#include <stdlib.h>

extern generic_list *generic_list_create() {
	generic_list *list = NULL;
	do {
		list = malloc(sizeof(generic_list));
		if (list == NULL) break;
		generic_list_init(list);
	} while(0);

	return list;
}

extern void generic_list_free(generic_list *list) {
	if (list != NULL) {
		generic_list_deinit(list);
		free(list);
	}
}


extern void generic_list_init(generic_list *list) {
	list->head = NULL;
	list->tail = NULL;
	list->err = 0;
	list->free = NULL;
}

extern void generic_list_deinit(generic_list *list) {
	generic_list_delete_all(list);
}

// STATIC HELPERS
//================


static generic_listentry *generic_listentry_create(void *itemptr) {
	// Allocate memory
	generic_listentry *entry = malloc(sizeof(generic_listentry));

	// Initialize if the allocation succeeded
	if (entry != NULL) {
		entry->prev = NULL;
		entry->next = NULL;
		entry->itemptr = itemptr;
	}

	// Return an initialized list entry,
	// or NULL if the allocation failed.
	return entry;
}

static void generic_listentry_free(generic_listentry *entry) {
	if (entry != NULL) {
		entry->itemptr = NULL;
		entry->next = NULL;
		entry->prev = NULL;

		free(entry);
	}
}

// INTERNAL METHODS
//==================

extern void *generic_list_delete(
        generic_list *list,
        generic_listentry *entry,
	int free_itemptr
) {
	// Detach the entry from the list
	void *itemptr = entry->itemptr;
	generic_listentry *prev = entry->prev;
	generic_listentry *next = entry->next;

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
	generic_listentry_free(entry);

	// Free the item pointer, if the method is specified.
	if (free_itemptr != 0) {
		if  ((list->free != NULL) && (itemptr != NULL)) {
			list->free(itemptr);
		}
	}

	// The possibly freed itemptr is returned.
	return itemptr;
}

extern generic_listentry *generic_list_insert_after(
        generic_list *list,
        generic_listentry *entry,
        void *itemptr
) {
	generic_listentry *rval
	    = generic_listentry_create(itemptr);

	if (rval != NULL) {

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

	} else {
		// Not enough memory.
	}

	return rval;
}

extern generic_listentry *generic_list_insert_before(
        generic_list *list,
        generic_listentry *entry,
        void *itemptr
) {
	generic_listentry *rval
	    = generic_listentry_create(itemptr);

	if (rval != NULL) {

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

	} else {
		// Not enough memory.
		printf("creation failed\n");
	}

	return rval;
}

// MISCELLANEOUS
//===============

extern int generic_list_get_size(
        const generic_list *list
) {
	int size = 0;
	generic_listentry *entry = list->head;
	while (entry != NULL) {
		size++;
		entry = entry->next;
	}
	return size;
}

// CONTENT INSERTION
//===================

extern void generic_list_append(
        generic_list *list,
        void *itemptr
) {
	generic_listentry *entry
	    = generic_listentry_create(itemptr);

	if (entry == NULL) {
		// Not enough memory
		//list->err = E_NOMEM;
		return;
	}

	generic_listentry *tail = list->tail;
	if (tail != NULL) {
		tail->next = entry;
		entry->prev = tail;
	} else {
		list->head = entry;
	}

	list->tail = entry;
}

extern void generic_list_prepend(
        generic_list *list,
        void *itemptr
) {
	generic_listentry *entry
	    = generic_listentry_create(itemptr);

	if (entry == NULL) {
		// Not enough memory
		//list->err = E_NOMEM;
		return;
	}

	generic_listentry *head = list->head;
	if (head != NULL) {
		head->prev = entry;
		entry->next = head;
	} else {
		list->tail = entry;
	}

	list->head = entry;
}

// CONTENT REMOVAL
//=================

extern void generic_list_delete_first(
        generic_list *list
) {
	if (list->head != NULL) {
		generic_list_delete(list, list->head, 1);
	} else {
		// Error, list already empty
	}
}

extern void generic_list_delete_last(
        generic_list *list
) {
	if (list->tail != NULL) {
		generic_list_delete(list, list->tail, 1);
	} else {
		// Error, list already empty
	}
}

extern void *generic_list_remove_first(
        generic_list *list
) {
	void *itemptr = NULL;
	if (list->head != NULL) {
		itemptr = generic_list_delete(list, list->head, 0);
	} else {
		// Error, list already empty
	}
	return itemptr;
}

extern void *generic_list_remove_last(
        generic_list *list
) {
	void *itemptr = NULL;
	if (list->tail != NULL) {
		itemptr = generic_list_delete(list, list->tail, 0);
	} else {
		// Error, list already empty
	}
	return itemptr;
}

extern void generic_list_remove_all(
	generic_list *list
) {
	while (list->head != NULL) {
		generic_list_remove_first(list);
	}
}

extern void generic_list_delete_all(
	generic_list *list
) {
	while (list->head != NULL) {
		generic_list_delete_first(list);
	}
}


