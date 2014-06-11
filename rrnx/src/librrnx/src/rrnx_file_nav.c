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

#include "rrnx/rrnx_file_nav.h"

// Nodes
#include "rrnx/rrnx_node.h"
#include "rrnx/rrnx_nodes_common.h"
#include "rrnx/rrnx_nodes_nav.h"
// Errors
#include "rrnx/rrnx_error.h"

#include <stdlib.h> // malloc, free, NULL
#include <string.h> // strcpy, memcpy
#include <assert.h> // assert() TODO get rid of this.

// CONSTRUCTION & DESTRUCTION
//============================

rrnx_file_nav *rrnx_fnav_alloc(void) {
	rrnx_file_nav *nav = NULL;

	int complete = 0;
	do {
		// Attempt allocation
		nav = malloc(sizeof(rrnx_file_nav));
		if (nav == NULL) break;

		// Initialize
		//nav->err = RRNX_E_OK;
		nav->has_iono_alpha = 0;
		nav->has_iono_beta = 0;
		nav->has_delta_utc = 0;
		nav->has_leap_seconds = 0;

		nav->format.version[0] = '\0';
		nav->format.type = '\0';
		nav->format.system = 0;

		nav->info.program[0] = '\0';
		nav->info.agency[0] = '\0';
		nav->info.date[0] = '\0';

		// No need to initialize iono or utc,
		// since they are marked as invalid.
		nav->navmsg_list = NULL;

		// Initialization finished.
		// Allocate sub-objects.

		nav->navmsg_list = rrnx_list_alloc();
		if (nav->navmsg_list == NULL) break;

		// Instantiation complete.
		complete = 1;
	} while(0);

	if (complete == 0) {
		rrnx_fnav_free(nav);
		nav = NULL;
	}

	return nav;

}

void rrnx_fnav_free(rrnx_file_nav *nav) {
	if (nav == NULL) return;

	rrnx_list_free(nav->navmsg_list);
	nav->navmsg_list = NULL;

	free(nav);
}

// OTHER METHODS
//===============

static void parse_format_decl(
    rrnx_file_nav *nav,
    const rrnx_format_decl *data
) {
	strcpy(nav->format.version, data->version);
	nav->format.type = data->type;
	nav->format.system = '?'; // Undefined!
}

static void parse_creation_info(
    rrnx_file_nav *nav,
    const rrnx_creation_info *data
) {
	strcpy(nav->info.program, data->program);
	strcpy(nav->info.agency, data->agency);
	strcpy(nav->info.date, data->date);
}

static void parse_ion_alpha(
    rrnx_file_nav *nav,
    const rrnx_ion_alpha *data
) {
	nav->has_iono_alpha = 1;
	for (int i = 0; i < 4; i++) {
		nav->iono.alpha[i] = data->alpha[i];
	}
}

static void parse_ion_beta(
    rrnx_file_nav *nav,
    const rrnx_ion_beta *data
) {
	nav->has_iono_beta = 1;
	for (int i = 0; i < 4; i++) {
		nav->iono.beta[i] = data->beta[i];
	}
}

static void parse_delta_utc(
    rrnx_file_nav *nav,
    const rrnx_delta_utc *data
) {
	nav->has_delta_utc = 1;

	nav->utc.a0 = data->a0;
	nav->utc.a1 = data->a1;
	nav->utc.tot = data->T;
	nav->utc.tot_week = data->T_week;
}

static void parse_leap_seconds(
    rrnx_file_nav *nav,
    const rrnx_leap_seconds *data
) {
	nav->has_leap_seconds = 1;
	nav->utc.delta_ls = data->leap_seconds;
}

static int parse_datarecord_nav(
    rrnx_file_nav *nav,
    const rrnx_datarecord_nav *data
) {
	const rrnx_navmsg *source_navmsg = &data->navmsg;
	rrnx_navmsg *navmsg = NULL;
	rrnx_list_item *list_item = NULL;
	int err = RRNX_E_OK;

	// Create a copy
	int complete = 0;
	do {
		// Attempt allocation
		navmsg = malloc(sizeof(rrnx_navmsg));
		if (navmsg == NULL) {
			err = RRNX_E_NOMEM;
			break;
		}
		// Clone the navmsg
		memcpy(navmsg, source_navmsg, sizeof(rrnx_navmsg));

		// Attempt appending to the list
		list_item = rrnx_list_append(nav->navmsg_list, navmsg);
		if (list_item == NULL) {
			err = RRNX_E_NOMEM;
			break;
		}

		complete = 1;
	} while(0);

	if (complete == 0) {
		// Destroy
		if (list_item != NULL) {
			// Remove list item
			rrnx_list_remove(nav->navmsg_list, list_item);
			list_item = NULL;
		}

		if (navmsg != NULL) {
			free(navmsg);
			navmsg = NULL;
		}
	} // if: incomplete

	return err;
}

static int parse_node(rrnx_file_nav *nav, const rrnx_node *node) {
	const void *payload = node->data;

	int err = RRNX_E_OK;

	switch(node->type) {
	case RRNX_ID_FORMAT_DECL:
		parse_format_decl(nav, payload);
		break;
	case RRNX_ID_CREATION_INFO:
		parse_creation_info(nav, payload);
		break;
	case RRNX_ID_COMMENT:
		// Ignored
		break;
	case RRNX_ID_END_OF_HEADER:
		// Ignored
		break;

	case RRNX_ID_ION_ALPHA:
		parse_ion_alpha(nav, payload);
		break;
	case RRNX_ID_ION_BETA:
		parse_ion_beta(nav, payload);
		break;
	case RRNX_ID_DELTA_UTC:
		parse_delta_utc(nav, payload);
		break;
	case RRNX_ID_LEAP_SECONDS:
		parse_leap_seconds(nav, payload);
		break;
	case RRNX_ID_DATARECORD_NAV:
		// May run out of memory here.
		err = parse_datarecord_nav(nav, payload);
		break;

	default:
		// Ignored
		break;
	} // switch

	return err;
}

rrnx_file_nav *rrnx_fnav_deserialize(const rrnx_list *nodelist) {
	rrnx_file_nav *nav = NULL;

	nav = rrnx_fnav_alloc();
	if (nav == NULL) return nav;

	if (nodelist == NULL) return nav;

	rrnx_list_item *iter = nodelist->first;

	int err = RRNX_E_OK;
	while (iter != NULL) {

		// Parse the node into file
		err = parse_node(nav, iter->data);

		if (err != RRNX_E_OK) {
			// Abort parsing immediately
			break;
		}

		// Move forward
		iter = iter->next;
	} // while


	if (err != RRNX_E_OK) {
		// Cancel
		rrnx_fnav_free(nav);
		nav = NULL;
	}

	return nav;
}

//rrnx_list *rrnx_fnav_serialize(rrnx_file_nav *nav);


