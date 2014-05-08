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

#ifndef GENERIC_LISTITERATOR_H
#define GENERIC_LISTITERATOR_H

#include "generic_list.h"

#ifdef __cplusplus
extern "C" {
#endif

struct generic_listiterator {

	/**
	 * The associated list.
	 */
	generic_list *list;

	/**
	 * Pointer to the preceding list entry, or NULL.
	 */
	generic_listentry *prev;

	/**
	 * Pointer to the current list entry, or NULL.
	 */
	generic_listentry *cur;

	/**
	 * Pointer to the succeeding list entry, or NULL.
	 */
	generic_listentry *next;
};

typedef struct generic_listiterator generic_listiterator;

// CONSTRUCTION & DESTRUCTION
//============================

extern generic_listiterator *generic_listiterator_create();
extern void generic_listiterator_free(generic_listiterator *iter);

extern generic_listiterator *generic_listiterator_from_begin(
	generic_list *list
);

extern generic_listiterator *generic_listiterator_from_end(
	generic_list *list
);

// INITIALIZATION
//================

extern void generic_listiterator_begin(
	generic_listiterator *iter,
	generic_list *list
);

extern void generic_listiterator_end(
	generic_listiterator *iter,
	generic_list *list
);

// ACCESS METHODS
//================

extern int generic_listiterator_has_current(
	const generic_listiterator *iter
);

extern void *generic_listiterator_get_current(
	generic_listiterator *iter
);

// TRAVERSAL
//===========

extern int generic_listiterator_has_next(
	const generic_listiterator *iter
);

extern int generic_listiterator_has_previous(
	const generic_listiterator *iter
);

extern void *generic_listiterator_next(
	generic_listiterator *iter
);

extern void *generic_listiterator_previous(
	generic_listiterator *iter
);


// LIST MANIPULATION: INSERTION
//==============================

extern void *generic_listiterator_set_current(
	generic_listiterator *iter,
	void *itemptr
);

extern void generic_listiterator_replace_current(
	generic_listiterator *iter,
	void *itemptr
);

extern void generic_listiterator_insert_after(
	generic_listiterator *iter,
	void *itemptr
);

extern void generic_listiterator_insert_before(
	generic_listiterator *iter,
	void *itemptr
);

// LIST MANIPULATION: DELETION
//=============================

extern void generic_listiterator_delete_current(
	generic_listiterator *iter
);

extern void generic_listiterator_delete_next(
	generic_listiterator *iter
);

extern void generic_listiterator_delete_previous(
	generic_listiterator *iter
);

// LIST MANIPULATION: REMOVAL
//============================

extern void *generic_listiterator_remove_current(
	generic_listiterator *iter
);

extern void *generic_listiterator_remove_next(
	generic_listiterator *iter
);

extern void *generic_listiterator_remove_previous(
	generic_listiterator *iter
);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
