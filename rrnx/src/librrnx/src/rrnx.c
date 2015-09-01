//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

#include <stdio.h>

#include "rrnx/rrnx.h"
#include "rrnx/rrnx_navreader.h"

rrnx_file_nav *rrnx_alloc_navfile(void) {
	return rrnx_fnav_alloc();
}

void rrnx_free_navfile(rrnx_file_nav *nav) {
	rrnx_fnav_free(nav);
}

int rrnx_read_navfile(const char *filename, rrnx_file_nav **nav) {
	int err = RRNX_E_OK;

	rrnx_navreader *navreader = NULL;
	rrnx_list *nodelist = NULL;

	// Reset return variable
	*nav = NULL;

	do {
		// Attempt to allocate a navreader
		navreader = rrnx_navr_alloc();

		if (navreader == NULL) {
			err = RRNX_E_NOMEM;
			break;
		}

		// Attempt to parse the file into a nodelist.
        	rrnx_navr_readfile(navreader, filename);

		// Inspect the result of the operation.
		err = rrnx_navr_errno(navreader);
		if (err) {
			// Operation failed. Abort immediately.
			break;
		}

		// Get the nodelist.
		nodelist = rrnx_navr_release_nodelist(navreader);

		// Attempt to deserialize the list into a navfile object.
		*nav = rrnx_fnav_deserialize(nodelist);

		if (*nav == NULL) {
			// Insufficient memory. Abort immediately.
			err = RRNX_E_NOMEM;
			break;
		}

		// Reaching this line implies success!
	} while(0);

	// Free the nodelist, if any.
	rrnx_list_free(nodelist);

	// Free navreader, if any.
	rrnx_navr_free(navreader);
	navreader = NULL;

	return err;
}



