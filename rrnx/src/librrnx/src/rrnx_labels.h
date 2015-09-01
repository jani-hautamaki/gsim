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

#ifndef RRNX_LABELS_H
#define RRNX_LABELS_H

#ifdef __cplusplus
extern "C" {
#endif

// Pseudo type
#define RRNX_LBL_UNKNOWN         -1

// Common to both OBS and NAV
#define RRNX_LBL_RINEX_DECL      1
#define RRNX_LBL_CREATION_INFO   2
#define RRNX_LBL_COMMENT         3
#define RRNX_LBL_END_OF_HEADER   4

// NAV specific
#define RRNX_LBL_ION_ALPHA       5
#define RRNX_LBL_ION_BETA        6
#define RRNX_LBL_DELTA_UTC       7
#define RRNX_LBL_LEAP_SECONDS    8

// OBS specific


int rrnx_enumerate_linetype(const char *line);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
