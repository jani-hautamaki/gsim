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

/*
 *
 * Reference:
 *
 * Gurtner W, Estey L.
 * RINEX: The Received Independent Exchange Format Version 2.11.
 * 10 Dec 2007 [updated 26 Jun 2012].
 * Available from: ftp://igs.org/pub/data/format/rinex211.txt
 *
 */

/*
 * TBC:
 * Some of the broadcast orbit fields are defined as integers,
 * even though the file allows them to be floating-points.
 * These fields should probably be doubles, and leave it
 * to the caller to decide whether to convert them into ints,
 * and what to do when the value is not an integer.
 *
 * TBC:
 * Different GNSS have different kind of broadcast orbit data.
 * The data struct names should reflect this.
 * That is, the naming convention for system specific nodes
 * should be rrnx_<sysname>_<func>, where <sysname> is
 * a three-letter abbreviation for the system.
 *
 * TBC:
 * It is not possible for broadcast_orbitN structs
 * to appear in a random order. In other words, they are not
 * indpenedent of each other. This suggests that these
 * structs should be put into a single big struct that
 * represents the set of repeating 8 lines (epoch+broadcast orbits).
 *
 */

#ifndef RRNX_NAV_H
#define RRNX_NAV_H

#include "rrnx_nodes_common.h"
#include "rrnx_nodes_nav.h"

#include "rrnx_list.h"
#include "rrnx_node.h"

#ifdef __cplusplus
extern "C" {
#endif


struct rrnx_nav {
	/**
	 * Node list
	 */
	rrnx_list *nodelist;

};

typedef struct rrnx_nav rrnx_nav;

//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

extern rrnx_nav *rrnx_nav_alloc();
extern void rrnx_nav_free(rrnx_nav *nav);

//============================================================================
// METHODS: OTHER
//============================================================================

extern int rrnx_nav_is_node_type_valid(int type);
extern rrnx_node *rrnx_nav_alloc_node(rrnx_nav *nav, int type);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

