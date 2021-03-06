//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

#include "rrnx/rrnx_list.h"
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

rrnx_list *rrnx_list_alloc(void) {
	rrnx_list *list = malloc(sizeof(rrnx_list));
	if (list != NULL) {
		list->first = NULL;
		list->last = NULL;
		list->destructor = free;
	}
	return list;
}

void rrnx_list_free(rrnx_list *list) {
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

rrnx_list_item *rrnx_list_append(rrnx_list *list, void* data) {
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
	if (list->first == NULL) {
		list->first = item;
	}

	if (list->last != NULL) {
		list->last->next = item;
	}
	list->last = item;

	return item;
}

void rrnx_list_remove(rrnx_list *list, rrnx_list_item *item) {

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

/*
// Internally rrnx_list_iterator is just rrnx_list_item
typedef struct rrnx_list_item rrnx_list_iter;

rrnx_list_item *rrnx_list_iterator(rrnx_list *list) {
	rrnx_list_item *item = NULL;
	if (list != NULL) {
		item = list->first;
	}
	return item;
}
rrnx_list_item *rrnx_list_next(rrnx_list_item *item) {
	if (item != NULL) {
		item = item->next;
	}
	return item;
}

int rrnx_list_has_next(const rrnx_list_item *item) {
	if (item != NULL) {
		return item->next != NULL;
	}
	return 0;
}


*/

