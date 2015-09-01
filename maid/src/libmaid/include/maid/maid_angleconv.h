//******************************{begin:header}******************************//
//            maid - The Mathematical Aid for Miscellaneous Tasks           //
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

#ifndef MAID_ANGLECONV_H
#define MAID_ANGLECONV_H

// MAID_PI_L, MAID_PI
#include "maid_const.h"

#define MAID_DEG2RAD_L(x) ((x)*MAID_PI_L/180.0L)
#define MAID_RAD2DEG_L(x) ((x)*180.0L/MAID_PI_L)

#define MAID_DEG2RAD(x) ((x)*MAID_PI/180.0)
#define MAID_RAD2DEG(x) ((x)*180.0/MAID_PI)

#ifdef __cplusplus
extern "C" {
#endif

/*
double maid_deg2rad(double deg);
double maid_rad2deg(double rad);

long double maid_deg2rad_l(long double deg);
long double maid_rad2deg_l(long double rad);
*/

#ifdef __cplusplus
} // extern "C"
#endif

#endif
