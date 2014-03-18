//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Parser
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://code.google.com/p/gsim
//
//      Copyright (C) 2013 Jani Hautamaki <jani.hautamaki@hotmail.com>
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

#ifndef RRNX_H
#define RRNX_H

//#include "rrnx_common.h" // Included through rrnx_nav and rrnx_obs
#include "rrnx_nav.h"
#include "rrnx_obs.h"

#ifdef __cplusplus
extern "C" {
#endif


struct rrnx_sat_health {
};
typedef struct rrnx_sat_health rrnx_sat_health;

//============================================================================
// METHODS:
//============================================================================

extern char rrnx_system2char();
extern unsigned int rrnx_char2system();

/*--- types: parser --------------------------------------------------------*/

/* Opaque type for the parser */
struct rrnx_parser;
typedef struct rrnx_parser rrnx_parser;

/*--- read and write -------------------------------------------------------*/

// Implementations in: rrnx_nav_parser.c and rrnx_obs_parser.c

extern int rrnx_read_nav(rrnx_parser *parser, const char *filename);
extern int rrnx_read_obs(rrnx_parser *parser, const char *filename);

/*--- parser methods -------------------------------------------------------*/

// rrnx_parser.c
extern rrnx_parser rrnx_parser_new();
extern void rrnx_parser_free(rrnx_parser *parser);
extern void rrnx_parser_resize_buffer(rrnx_parser *parser, size_t size);

extern int rrnx_parser_row();
extern int rrnx_parser_col();
extern const char* rrnx_parser_line();
extern int rrnx_parser_errno();
extern const char* rrnx_parser_strerror();

/* no writing implemented at the moment */

/*--- struct methods -------------------------------------------------------*/
extern rrnx_nav *rrnx_nav_new();
extern rrnx_obs *rrnx_obs_new();

extern void rrnx_nav_free(rrnx_nav *nav);
extern void rrnx_obs_free(rrnx_obs *obs);

/*--- retrieval ------------------------------------------------------------*/
/*--- other methods --------------------------------------------------------*/

         // 123456789012345678901234567890
extern void rrnx_nav_get_ephemeris();

extern void rrnx_obs_get_observation();


#ifdef __cplusplus
} // extern "C"
#endif

#endif

