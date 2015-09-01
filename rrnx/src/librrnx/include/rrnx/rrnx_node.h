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

#ifndef RRNX_NODE_H
#define RRNX_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// CONSTANTS
//============================================================================

// TODO: Should the labels be extern const char * variables?

// Common pseudo-tag
#define RRNX_ID_INVALID            0xffff

// Common to NAV and OBS
#define RRNX_ID_FORMAT_DECL        0x1801
#define RRNX_ID_CREATION_INFO      0x1802
#define RRNX_ID_COMMENT            0x1803
#define RRNX_ID_END_OF_HEADER      0x1804

// NAV only
#define RRNX_ID_ION_ALPHA          0x2001
#define RRNX_ID_ION_BETA           0x2002
#define RRNX_ID_DELTA_UTC          0x2003
#define RRNX_ID_LEAP_SECONDS       0x2004
// The NAV data record
#define RRNX_ID_DATARECORD_NAV     0x2010



//============================================================================
// DATA STRUCTURES
//============================================================================

struct rrnx_node {
        /**
         * Node type
         */
        unsigned int type;

        /**
         * Node payload size (chars).
         */
        unsigned int size;

        /**
         * Node payload as a flexible array.
         */
        char data[];
};

typedef struct rrnx_node rrnx_node;

//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

rrnx_node *rrnx_node_alloc(int type);
void rrnx_node_free(rrnx_node *node);

//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

int rrnx_node_is_type_valid(int type);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
