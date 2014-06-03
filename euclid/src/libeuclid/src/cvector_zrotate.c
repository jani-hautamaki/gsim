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

#include "euclid/cvector.h"

// sin, cos
#include <math.h>

extern void cvector_zrotate(double *vdest, const double *vorg, double rad) {

	vdest[0] =  vorg[0]*cos(rad) + vorg[1]*sin(rad);
	vdest[1] = -vorg[0]*sin(rad) + vorg[1]*cos(rad);
	vdest[2] = vorg[2];

}

