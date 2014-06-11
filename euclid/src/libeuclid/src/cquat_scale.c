//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
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

#include "euclid/cquat.h"

void cquat_scale(double *qdest, double c, const double *qorg) {
	qdest[0] = c * qorg[0];
	qdest[1] = c * qorg[1];
	qdest[2] = c * qorg[2];
	qdest[3] = c * qorg[3];
}

