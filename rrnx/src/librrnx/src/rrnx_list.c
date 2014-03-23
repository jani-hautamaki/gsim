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

#include "rrnx_list.h"
#include <stdlib.h> // free

static void free_payload(rrnx_list *list, rrnx_list_item *item) {
	if (item == NULL) {
		// Already destroyed
		return;
	}
	// Otherwise free the payload
	list->destructor(item->data);
	item->data = NULL;
}

extern rrnx_list *rrnx_list_alloc() {
	rrnx_list *list = malloc(sizeof(rrnx_list));
	if (list != NULL) {
		list->first = NULL;
		list->last = NULL;
		list->destructor = free;
	}
	return list;
}

extern void rrnx_list_free(rrnx_list *list) {
	if (list == NULL) {
		// Already deallocated
		return;
	}
	rrnx_list_item *item = list->first;
	while (item != NULL) {
		rrnx_list_item *next = item->next;
		// Free user-defined payload
		free_payload(list, item);
		// Free the list item itself.
		// Don't bother to maintain links.
		free(item);
		// Advance
		item = next;
	}
	list->first = NULL;
	list->last = NULL;

	// Remove the list itself
	free(list);
}

// Manipulation

extern rrnx_list_item *rrnx_list_append(rrnx_list *list, void* data) {
	rrnx_list_item *item = malloc(sizeof(rrnx_list_item));
	if (item == NULL) {
		// Allocation failed
		return item;
	}

	// Record the user data
	item->data = data;

	// Set up the links
	item->next = NULL;
	item->prev = list->last;

	// Maintain the existing links
	if (list->last != NULL) {
		list->last->next = item;
	}
	list->last = item;

	return item;
}

extern void rrnx_list_remove(rrnx_list *list, rrnx_list_item *item) {

	// Maintain links

	rrnx_list_item *next = item->next;
	rrnx_list_item *prev = item->prev;

	if (next != NULL) {
		next->prev = prev;
	}
	if (prev != NULL) {
		prev->next = next;
	}

	if (list->last == item) {
		list->last = prev;
	}

	if (list->first == item) {
		list->first = next;
	}

	// Free user data
	free_payload(list, item);
	// free the node itself
	free(item);
}

