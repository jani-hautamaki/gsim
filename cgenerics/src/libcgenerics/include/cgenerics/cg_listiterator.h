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

#ifndef CG_LISTITERATOR_H
#define CG_LISTITERATOR_H

#include "cg_list.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cg_listiterator {

	/**
	 * The associated list.
	 */
	cg_list *list;

	/**
	 * Pointer to the preceding list entry, or NULL.
	 */
	cg_listentry *prev;

	/**
	 * Pointer to the current list entry, or NULL.
	 */
	cg_listentry *cur;

	/**
	 * Pointer to the succeeding list entry, or NULL.
	 */
	cg_listentry *next;
};

typedef struct cg_listiterator cg_listiterator;

// CONSTRUCTION & DESTRUCTION
//============================

cg_listiterator *cg_listiterator_create(void);
void cg_listiterator_free(cg_listiterator *iter);

cg_listiterator *cg_listiterator_from_begin(cg_list *list);
cg_listiterator *cg_listiterator_from_end(cg_list *list);

// INITIALIZATION
//================

void cg_listiterator_begin(cg_listiterator *iter, cg_list *list);
void cg_listiterator_end(cg_listiterator *iter, cg_list *list);

// ACCESS METHODS
//================

int cg_listiterator_has_current(const cg_listiterator *iter);
void *cg_listiterator_get_current(cg_listiterator *iter);

// TRAVERSAL
//===========

int cg_listiterator_has_next(const cg_listiterator *iter);
int cg_listiterator_has_previous(const cg_listiterator *iter);

void *cg_listiterator_next(cg_listiterator *iter);
void *cg_listiterator_get_next(const cg_listiterator *iter);

void *cg_listiterator_previous(cg_listiterator *iter);
void *cg_listiterator_get_previous(const cg_listiterator *iter);


// LIST MANIPULATION: INSERTION
//==============================

void *cg_listiterator_set_current(cg_listiterator *iter, void *itemptr);
void cg_listiterator_replace_current(cg_listiterator *iter, void *itemptr);
void cg_listiterator_insert_after(cg_listiterator *iter, void *itemptr);
void cg_listiterator_insert_before(cg_listiterator *iter, void *itemptr);

// LIST MANIPULATION: DELETION
//=============================

void cg_listiterator_delete_current(cg_listiterator *iter);
void cg_listiterator_delete_next(cg_listiterator *iter);
void cg_listiterator_delete_previous(cg_listiterator *iter);

// LIST MANIPULATION: REMOVAL
//============================

void *cg_listiterator_remove_current(cg_listiterator *iter);
void *cg_listiterator_remove_next(cg_listiterator *iter);
void *cg_listiterator_remove_previous(cg_listiterator *iter);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
