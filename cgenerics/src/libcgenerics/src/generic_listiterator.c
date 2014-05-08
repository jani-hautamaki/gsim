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

#include "generic_listiterator.h"
#include "generic_list_internal.h"

// malloc, free, NULL
#include <stdlib.h>

// CONSTRUCTION & DESTRUCTION
//============================

extern generic_listiterator *generic_listiterator_create() {
	generic_listiterator *iter
	    = malloc(sizeof(generic_listiterator));

	if (iter != NULL) {
		// Initialize
		iter->list = NULL;
		iter->prev = NULL;
		iter->cur = NULL;
		iter->next = NULL;
	} else {
		// Not enough memory
	}

	return iter;
}

extern void generic_listiterator_free(generic_listiterator *iter) {
	if (iter != NULL) {
		free(iter);
	}
}

extern generic_listiterator *generic_listiterator_from_begin(
        generic_list *list
) {
	generic_listiterator *iter = generic_listiterator_create();
	if (iter != NULL) {
		generic_listiterator_set_head(iter, list);
	} else {
		// Not enough memory
	}
	return iter;
}

extern generic_listiterator *generic_listiterator_from_end(
        generic_list *list
) {
	generic_listiterator *iter = generic_listiterator_create();
	if (iter != NULL) {
		generic_listiterator_set_tail(iter, list);
	} else {
		// Not enough memory
	}
	return iter;
}

// INITIALIZATION
//================

extern void generic_listiterator_set_head(
        generic_listiterator *iter,
        generic_list *list
) {
	iter->list = list;
	iter->prev = NULL;
	iter->cur = NULL;
	iter->next = list->head;
}


extern void generic_listiterator_set_tail(
        generic_listiterator *iter,
        generic_list *list
) {
	iter->list = list;
	iter->prev = list->tail;
	iter->cur = NULL;
	iter->next = NULL;
}

// ACCESS METHODS
//================

extern int generic_listiterator_has_current(
        const generic_listiterator *iter
) {
	return iter->cur != NULL;
}

extern void *generic_listiterator_get_current(
        generic_listiterator *iter
) {
	void *rval = NULL;

	if (iter->cur != NULL) {
		rval = iter->cur->itemptr;
	} else {
		// Error, no current entry.
	}

	return rval;
}

// TRAVERSAL
//===========

extern int generic_listiterator_has_next(
        const generic_listiterator *iter
) {
	return iter->next != NULL;
}

extern int generic_listiterator_has_previous(
        const generic_listiterator *iter
) {
	return iter->prev != NULL;
}

extern void *generic_listiterator_next(
        generic_listiterator *iter
) {
	void *rval = NULL;

	generic_listentry *next = iter->next;
	if (next != NULL) {
		// Return value
		rval = next->itemptr;
		// Move forward
		iter->prev = next->prev;
		iter->cur = next;
		iter->next = next->next;

	} else {
		// Error, no next available.
	}

	return rval;
}

extern void *generic_listiterator_previous(
        generic_listiterator *iter
) {
	void *rval = NULL;

	generic_listentry *prev = iter->prev;
	if (prev != NULL) {
		// Return value
		rval = prev->itemptr;
		// Move backward
		iter->next = prev->next;
		iter->cur = prev;
		iter->prev = prev->prev;

	} else {
		// Error, no previous available.
	}

	return rval;
}


// LIST MANIPULATION: INSERTION
//==============================

extern void *generic_listiterator_set_current(
        generic_listiterator *iter,
        void *itemptr
) {
	generic_listentry *cur = iter->cur;
	void *rval = NULL;

	if (cur != NULL) {
		rval = cur->itemptr;
		cur->itemptr = itemptr;
	} else {
		// Error, no current
	}

	return rval;
}

extern void generic_listiterator_replace_current(
        generic_listiterator *iter,
        void *itemptr
) {
	void *origptr = generic_listiterator_set_current(iter, itemptr);

	void (*free_itemptr)(void*);
	free_itemptr = iter->list->free;
	if ((free_itemptr != NULL) && (origptr != NULL)) {
		free_itemptr(origptr);
	}
}

extern void generic_listiterator_insert_after(
        generic_listiterator *iter,
        void *itemptr
) {
	if (iter->cur != NULL) {
		// Has current.
		generic_list_insert_after(
		    iter->list, iter->cur, itemptr);

		// Refresh next
		iter->next = iter->cur->next;

	} else if (iter->next != NULL) {
		// cur==NULL && next!=NULL.
		// 1) We are at the beginning of a list (prev==NULL),
		// 2) We are inbetween entries (prev!=NULL)

		// The new entry becomes the new head
		generic_list_insert_before(
		    iter->list, iter->next, itemptr);

		// Refresh next
		iter->next = iter->next->prev;
	} else if (iter->prev != NULL) {
		// prev!=NULL && cur==NULL && next==NULL
		// We are at the end of a list.

		// The new entry becomes the new tail.
		generic_list_insert_after(
		    iter->list, iter->prev, itemptr);

		// Refresh next
		iter->next = iter->prev->next;
	} else {
		// The list is empty.
		generic_list_append(iter->list, itemptr);
		iter->next = iter->list->head;
	}
}

extern void generic_listiterator_insert_before(
        generic_listiterator *iter,
        void *itemptr
) {
	if (iter->cur != NULL) {
		// Has current.
		generic_list_insert_before(
		    iter->list, iter->cur, itemptr);

		// Refresh previous
		iter->prev = iter->cur->prev;

	} else if (iter->prev != NULL) {
		// prev!=NULL && cur==NULL.
		// 1) We are at the end of a list (next==NULL),
		// 2) We are inbetween entries (next!=NULL)

		// The new entry becomes the new head
		generic_list_insert_after(
		    iter->list, iter->prev, itemptr);

		// Refresh next
		iter->prev = iter->prev->next;
	} else if (iter->next != NULL) {
		// prev==NULL && cur==NULL && next!=NULL
		// We are at the beginning of a list.

		// The new entry becomes the new head.
		generic_list_insert_before(
		    iter->list, iter->next, itemptr);

		// Refresh next
		iter->prev = iter->next->prev;
	} else {
		// The list is empty.
		generic_list_prepend(iter->list, itemptr);
		iter->prev = iter->list->tail;
	}
}





// LIST MANIPULATION: DELETION
//=============================

extern void generic_listiterator_delete_current(
        generic_listiterator *iter
) {
	if (iter->cur != NULL) {
		generic_list_delete(iter->list, iter->cur, 1);
		iter->cur = NULL;
	} else {
		// Error. No current entry.
	}
}

extern void generic_listiterator_delete_next(
        generic_listiterator *iter
) {
	generic_listentry *next = iter->next;
	if (next != NULL) {
		// Update next
		iter->next = next->next;
		// Remove entry
		generic_list_delete(iter->list, next, 1);
	} else {
		// Error. No next entry.
	}
}

extern void generic_listiterator_delete_previous(
        generic_listiterator *iter
) {
	generic_listentry *prev = iter->prev;
	if (prev != NULL) {
		// Update previous
		iter->prev = prev->prev;
		// Remove entry
		generic_list_delete(iter->list, prev, 1);
	} else {
		// Error. No previou entry.
	}
}


// LIST MANIPULATION: REMOVAL
//============================

extern void *generic_listiterator_remove_current(
        generic_listiterator *iter
) {
	void *rval = NULL;
	if (iter->cur != NULL) {
		rval = generic_list_delete(iter->list, iter->cur, 0);
		iter->cur = NULL;
	} else {
		// Error. No current entry.
	}
	return rval;
}

extern void *generic_listiterator_remove_next(
        generic_listiterator *iter
) {
	void *rval = NULL;

	generic_listentry *next = iter->next;
	if (next != NULL) {
		// Update next
		iter->next = next->next;
		// Remove entry
		rval = generic_list_delete(iter->list, next, 0);
	} else {
		// Error. No next entry.
	}
	return rval;
}

extern void *generic_listiterator_remove_previous(
        generic_listiterator *iter
) {
	void *rval = NULL;

	generic_listentry *prev = iter->prev;
	if (prev != NULL) {
		// Update previous
		iter->prev = prev->prev;
		// Remove entry
		rval = generic_list_delete(iter->list, prev, 0);
	} else {
		// Error. No next entry.
	}
	return rval;
}


