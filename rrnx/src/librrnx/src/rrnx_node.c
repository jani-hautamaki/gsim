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

#include "rrnx_node.h"
#include "rrnx_nav.h"

#include <stdlib.h>
#include <unistd.h> // ssize_t

static ssize_t get_data_size(int type) {
	switch(type) {

	case RRNX_ID_FORMAT_DECL:
		return sizeof(rrnx_format_decl);
	case RRNX_ID_CREATION_INFO:
		return sizeof(rrnx_creation_info);
	case RRNX_ID_COMMENT:
		return sizeof(rrnx_comment);
	case RRNX_ID_END_OF_HEADER:
		//return sizeof(rrnx_end_of_header); // no such struct
		return 0;

	case RRNX_ID_ION_ALPHA:
		return sizeof(rrnx_ion_alpha);
	case RRNX_ID_ION_BETA:
		return sizeof(rrnx_ion_beta);
	case RRNX_ID_DELTA_UTC:
		return sizeof(rrnx_delta_utc);
	case RRNX_ID_LEAP_SECONDS:
		return sizeof(rrnx_leap_seconds);

	case RRNX_ID_BROADCAST_ORBIT0:
		return sizeof(rrnx_broadcast_orbit0);
	case RRNX_ID_BROADCAST_ORBIT1:
		return sizeof(rrnx_broadcast_orbit1);
	case RRNX_ID_BROADCAST_ORBIT2:
		return sizeof(rrnx_broadcast_orbit2);
	case RRNX_ID_BROADCAST_ORBIT3:
		return sizeof(rrnx_broadcast_orbit3);
	case RRNX_ID_BROADCAST_ORBIT4:
		return sizeof(rrnx_broadcast_orbit4);
	case RRNX_ID_BROADCAST_ORBIT5:
		return sizeof(rrnx_broadcast_orbit5);
	case RRNX_ID_BROADCAST_ORBIT6:
		return sizeof(rrnx_broadcast_orbit6);
	case RRNX_ID_BROADCAST_ORBIT7:
		return sizeof(rrnx_broadcast_orbit7);

	default:
		break;
	}
	return -1;
}

extern rrnx_node *rrnx_node_alloc(int type) {
	// Determine payload size.
	// Returns -1 if type is unknown.
	ssize_t data_size = get_data_size(type);

	// Since returning NULL is reserved for
	// memory allocation failures, invalid type
	// needs to signaled differently to the caller.

	if (data_size < 0) {
		data_size = 0;
		type = RRNX_ID_INVALID;
	}

	// Determine the total size to allocate
	size_t total_size = sizeof(rrnx_node) + (size_t)(data_size);

	// Attempt allocation
	rrnx_node *node = malloc(total_size);
	if (node != NULL) {
		node->type = type;
		node->size = data_size;
	}

	return node;
}

extern void rrnx_node_free(rrnx_node *node) {
	if (node == NULL) {
		// Already freed
		return;
	}

	switch(node->type) {

	case RRNX_ID_FORMAT_DECL:
	case RRNX_ID_CREATION_INFO:
	case RRNX_ID_COMMENT:
	case RRNX_ID_END_OF_HEADER:
	case RRNX_ID_ION_ALPHA:
	case RRNX_ID_ION_BETA:
	case RRNX_ID_DELTA_UTC:
	case RRNX_ID_LEAP_SECONDS:
	case RRNX_ID_BROADCAST_ORBIT0:
	case RRNX_ID_BROADCAST_ORBIT1:
	case RRNX_ID_BROADCAST_ORBIT2:
	case RRNX_ID_BROADCAST_ORBIT3:
	case RRNX_ID_BROADCAST_ORBIT4:
	case RRNX_ID_BROADCAST_ORBIT5:
	case RRNX_ID_BROADCAST_ORBIT6:
	case RRNX_ID_BROADCAST_ORBIT7:
		// No special considerations
		break;
	case RRNX_ID_INVALID:
		break;

	default:
		break;
	}

	free(node);
}

