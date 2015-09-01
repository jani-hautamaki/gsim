//******************************{begin:header}******************************//
//           cgenerics - The C Library for Generic Data Structures          //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
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

#ifndef CG_LIST_H
#define CG_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Node of a generic linked-list.
 */
struct cg_listentry {
	/**
	 * Pointer to the preceding list entry.
	 * Set to NULL if no preceding entry.
	 */
	struct cg_listentry *prev;

	/**
	 * Pointer to the succeeding list entry.
	 * Set to NULL if no succeeding entry.
	 */
	struct cg_listentry *next;

	/**
	 * Pointer to the payload data item.
	 * The pointer is allowed to be NULL.
	 */
	void *itemptr;
};

typedef struct cg_listentry cg_listentry;

/**
 * Generic linked-list.
 */
struct cg_list {
	/**
	 * Pointer to the linked-list head.
	 */
	cg_listentry *head;

	/**
	 * Pointer to the linked-list tail.
	 */
	cg_listentry *tail;

	/**
	 * Latest error code, if any.
	 */
	int err;

	/**
	 * The number of list items.
	 */
	int size;

	/**
	 * Method used to free itemptr
	 */
	void (*free)(void *);

};

typedef struct cg_list cg_list;


// CONSTRUCTION & DESTRUCTION
//============================

cg_list *cg_list_create(void);
void cg_list_free(cg_list *list);

void cg_list_init(cg_list *list);
void cg_list_deinit(cg_list *list);

// CONFIGURATION
//===============

// MISCELLANEOUS
//===============

int cg_list_get_size(const cg_list *list);

// CONTENT INSERTION
//===================

int cg_list_append(cg_list *list, void *itemptr);
int cg_list_prepend(cg_list *list, void *itemptr);

// CONTENT REMOVAL
//=================

void cg_list_delete_first(cg_list *list);
void cg_list_delete_last(cg_list *list);
void cg_list_delete_all(cg_list *list);

void *cg_list_remove_first(cg_list *list);
void *cg_list_remove_last(cg_list *list);
void cg_list_remove_all(cg_list *list);

// SORTING
//=========

void cg_list_bubblesort(
    cg_list *list,
    int (*compare)(const void *itemptr1, const void *itemptr2)
);

/*
void cg_list_find_first_geq(
    cg_list *list,
    int (*compare)(const void *itemptr, const void *value)
    const void *value
);

void cg_list_find_last_lt(
    cg_list *list,
    int (*compare)(const void *itemptr, const void *value)
    const void *value
);
*/


#ifdef __cplusplus
} // extern "C"
#endif

#endif
