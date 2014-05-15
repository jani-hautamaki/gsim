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

#include "cgenerics/cg_listiterator.h"
#include "cg_list_internal.h"

// malloc, free, NULL
#include <stdlib.h>

// CONSTRUCTION & DESTRUCTION
//============================

extern cg_listiterator *cg_listiterator_create() {
	cg_listiterator *iter
	    = malloc(sizeof(cg_listiterator));

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

extern void cg_listiterator_free(cg_listiterator *iter) {
	if (iter != NULL) {
		free(iter);
	}
}

extern cg_listiterator *cg_listiterator_from_begin(
        cg_list *list
) {
	cg_listiterator *iter = cg_listiterator_create();
	if (iter != NULL) {
		cg_listiterator_begin(iter, list);
	} else {
		// Not enough memory
	}
	return iter;
}

extern cg_listiterator *cg_listiterator_from_end(
        cg_list *list
) {
	cg_listiterator *iter = cg_listiterator_create();
	if (iter != NULL) {
		cg_listiterator_end(iter, list);
	} else {
		// Not enough memory
	}
	return iter;
}

// INITIALIZATION
//================

extern void cg_listiterator_begin(
        cg_listiterator *iter,
        cg_list *list
) {
	iter->list = list;
	iter->prev = NULL;
	iter->cur = NULL;
	iter->next = list->head;
}


extern void cg_listiterator_end(
        cg_listiterator *iter,
        cg_list *list
) {
	iter->list = list;
	iter->prev = list->tail;
	iter->cur = NULL;
	iter->next = NULL;
}

// ACCESS METHODS
//================

extern int cg_listiterator_has_current(
        const cg_listiterator *iter
) {
	return iter->cur != NULL;
}

extern void *cg_listiterator_get_current(
        cg_listiterator *iter
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

extern int cg_listiterator_has_next(
        const cg_listiterator *iter
) {
	return iter->next != NULL;
}

extern int cg_listiterator_has_previous(
        const cg_listiterator *iter
) {
	return iter->prev != NULL;
}

extern void *cg_listiterator_next(
        cg_listiterator *iter
) {
	void *rval = NULL;

	cg_listentry *next = iter->next;
	if (next != NULL) {
		// Return value
		rval = next->itemptr;
		// Move forward
		iter->prev = next->prev;
		iter->cur = next;
		iter->next = next->next;
	} else if (iter->cur != NULL) {
		iter->prev = iter->cur;
		iter->cur = NULL;
		iter->next = NULL;
	} else {
		// Error, no next available.
	}

	return rval;
}

extern void *cg_listiterator_previous(
        cg_listiterator *iter
) {
	void *rval = NULL;

	cg_listentry *prev = iter->prev;
	if (prev != NULL) {
		// Return value
		rval = prev->itemptr;
		// Move backward
		iter->next = prev->next;
		iter->cur = prev;
		iter->prev = prev->prev;
	} else if (iter->cur != NULL) {
		iter->next = iter->cur;
		iter->cur = NULL;
		iter->prev = NULL;
	} else {
		// Error, no previous available.
	}

	return rval;
}


// LIST MANIPULATION: INSERTION
//==============================

extern void *cg_listiterator_set_current(
        cg_listiterator *iter,
        void *itemptr
) {
	cg_listentry *cur = iter->cur;
	void *rval = NULL;

	if (cur != NULL) {
		rval = cur->itemptr;
		cur->itemptr = itemptr;
	} else {
		// Error, no current
	}

	return rval;
}

extern void cg_listiterator_replace_current(
        cg_listiterator *iter,
        void *itemptr
) {
	void *origptr = cg_listiterator_set_current(iter, itemptr);

	void (*free_itemptr)(void*);
	free_itemptr = iter->list->free;
	if ((free_itemptr != NULL) && (origptr != NULL)) {
		free_itemptr(origptr);
	}
}

extern void cg_listiterator_insert_after(
        cg_listiterator *iter,
        void *itemptr
) {
	if (iter->cur != NULL) {
		// Has current.
		cg_list_insert_after(
		    iter->list, iter->cur, itemptr);

		// Refresh next
		iter->next = iter->cur->next;

	} else if (iter->next != NULL) {
		// cur==NULL && next!=NULL.
		// 1) We are at the beginning of a list (prev==NULL),
		// 2) We are inbetween entries (prev!=NULL)

		// The new entry becomes the new head
		cg_list_insert_before(
		    iter->list, iter->next, itemptr);

		// Refresh next
		iter->next = iter->next->prev;
	} else if (iter->prev != NULL) {
		// prev!=NULL && cur==NULL && next==NULL
		// We are at the end of a list.

		// The new entry becomes the new tail.
		cg_list_insert_after(
		    iter->list, iter->prev, itemptr);

		// Refresh next
		iter->next = iter->prev->next;
	} else {
		// The list is empty.
		cg_list_append(iter->list, itemptr);
		iter->next = iter->list->head;
	}
}

extern void cg_listiterator_insert_before(
        cg_listiterator *iter,
        void *itemptr
) {
	if (iter->cur != NULL) {
		// Has current.
		cg_list_insert_before(
		    iter->list, iter->cur, itemptr);

		// Refresh previous
		iter->prev = iter->cur->prev;

	} else if (iter->prev != NULL) {
		// prev!=NULL && cur==NULL.
		// 1) We are at the end of a list (next==NULL),
		// 2) We are inbetween entries (next!=NULL)

		// The new entry becomes the new head
		cg_list_insert_after(
		    iter->list, iter->prev, itemptr);

		// Refresh next
		iter->prev = iter->prev->next;
	} else if (iter->next != NULL) {
		// prev==NULL && cur==NULL && next!=NULL
		// We are at the beginning of a list.

		// The new entry becomes the new head.
		cg_list_insert_before(
		    iter->list, iter->next, itemptr);

		// Refresh next
		iter->prev = iter->next->prev;
	} else {
		// The list is empty.
		cg_list_prepend(iter->list, itemptr);
		iter->prev = iter->list->tail;
	}
}





// LIST MANIPULATION: DELETION
//=============================

extern void cg_listiterator_delete_current(
        cg_listiterator *iter
) {
	if (iter->cur != NULL) {
		cg_list_delete(iter->list, iter->cur, 1);
		iter->cur = NULL;
	} else {
		// Error. No current entry.
	}
}

extern void cg_listiterator_delete_next(
        cg_listiterator *iter
) {
	cg_listentry *next = iter->next;
	if (next != NULL) {
		// Update next
		iter->next = next->next;
		// Remove entry
		cg_list_delete(iter->list, next, 1);
	} else {
		// Error. No next entry.
	}
}

extern void cg_listiterator_delete_previous(
        cg_listiterator *iter
) {
	cg_listentry *prev = iter->prev;
	if (prev != NULL) {
		// Update previous
		iter->prev = prev->prev;
		// Remove entry
		cg_list_delete(iter->list, prev, 1);
	} else {
		// Error. No previou entry.
	}
}


// LIST MANIPULATION: REMOVAL
//============================

extern void *cg_listiterator_remove_current(
        cg_listiterator *iter
) {
	void *rval = NULL;
	if (iter->cur != NULL) {
		rval = cg_list_delete(iter->list, iter->cur, 0);
		iter->cur = NULL;
	} else {
		// Error. No current entry.
	}
	return rval;
}

extern void *cg_listiterator_remove_next(
        cg_listiterator *iter
) {
	void *rval = NULL;

	cg_listentry *next = iter->next;
	if (next != NULL) {
		// Update next
		iter->next = next->next;
		// Remove entry
		rval = cg_list_delete(iter->list, next, 0);
	} else {
		// Error. No next entry.
	}
	return rval;
}

extern void *cg_listiterator_remove_previous(
        cg_listiterator *iter
) {
	void *rval = NULL;

	cg_listentry *prev = iter->prev;
	if (prev != NULL) {
		// Update previous
		iter->prev = prev->prev;
		// Remove entry
		rval = cg_list_delete(iter->list, prev, 0);
	} else {
		// Error. No next entry.
	}
	return rval;
}


