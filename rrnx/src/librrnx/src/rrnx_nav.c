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

#include "rrnx/rrnx_nav.h"
#include "rrnx/rrnx_list.h"
#include <stdlib.h>


extern rrnx_nav *rrnx_nav_alloc() {
	rrnx_nav *nav = malloc(sizeof(rrnx_nav));
	int incomplete = 1;
	if (nav != NULL) do {
		nav->nodelist = rrnx_list_alloc();
		if (nav->nodelist == NULL) {
			// Memory allocation failed. Abort.
			break;
		}
		incomplete = 0;
	} while(0);

	if (incomplete) {
		rrnx_nav_free(nav);
		nav = NULL;
	}

	return nav;
}

extern void rrnx_nav_free(rrnx_nav *nav) {
	if (nav == NULL) {
		return;
	}

	rrnx_list_free(nav->nodelist);
	nav->nodelist = NULL;
	free(nav);
}

/*
 * TODO: These should probably be separated into
 * "rrnx_nodelist" struct.
*/

extern rrnx_node *rrnx_nav_alloc_node(rrnx_nav *nav, int type) {
	// Return value
	rrnx_node *node = NULL;

	// Attempt to allocate a new list item
	rrnx_list_item *item
	    = rrnx_list_append(nav->nodelist, NULL);

	int incomplete = 1;
	if (item != NULL) do {
		// Attempt to allocate
		item->data = rrnx_node_alloc(type);

		if (item->data == NULL) {
			// Either invalid type or malloc failed.
			break;
		}

		node = item->data;
		incomplete = 0;
	} while(0);

	if (incomplete) {
		rrnx_list_remove(nav->nodelist, item);
		item = NULL;
	}

	return node;
}

extern int rrnx_nav_is_node_type_valid(int type) {
	return rrnx_node_is_valid_type(type);

}

extern void rrnx_nav_append(rrnx_nav *nav, rrnx_node *node) {
}

