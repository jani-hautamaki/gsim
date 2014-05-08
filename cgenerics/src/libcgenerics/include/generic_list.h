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

#ifndef GENERIC_LIST_H
#define GENERIC_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Node of a generic linked-list.
 */
struct generic_listentry {
	/**
	 * Pointer to the preceding list entry.
	 * Set to NULL if no preceding entry.
	 */
	struct generic_listentry *prev;

	/**
	 * Pointer to the succeeding list entry.
	 * Set to NULL if no succeeding entry.
	 */
	struct generic_listentry *next;

	/**
	 * Pointer to the payload data item.
	 * The pointer is allowed to be NULL.
	 */
	void *itemptr;
};

typedef struct generic_listentry generic_listentry;

/**
 * Generic linked-list.
 */
struct generic_list {
	/**
	 * Pointer to the linked-list head.
	 */
	generic_listentry *head;

	/**
	 * Pointer to the linked-list tail.
	 */
	generic_listentry *tail;

	/**
	 * Error code for the latest operation, if any.
	 */
	int err;

	/**
	 * Method used to free itemptr
	 */
	void (*free)(void *);

};

typedef struct generic_list generic_list;


// CONSTRUCTION & DESTRUCTION
//============================

extern generic_list *generic_list_create();
extern void generic_list_free(generic_list *list);

extern void generic_list_init(generic_list *list);
extern void generic_list_deinit(generic_list *list);

// CONFIGURATION
//===============

// MISCELLANEOUS
//===============

extern int generic_list_get_size(
	const generic_list *list
);


// CONTENT INSERTION
//===================

extern void generic_list_append(
	generic_list *list,
	void *itemptr
);

extern void generic_list_prepend(
	generic_list *list,
	void *itemptr
);

// CONTENT REMOVAL
//=================

extern void generic_list_delete_first(
	generic_list *list
);

extern void generic_list_delete_last(
	generic_list *list
);

extern void generic_list_delete_all(
	generic_list *list
);



extern void *generic_list_remove_first(
	generic_list *list
);

extern void *generic_list_remove_last(
	generic_list *list
);

extern void generic_list_remove_all(
	generic_list *list
);



#ifdef __cplusplus
} // extern "C"
#endif

#endif
