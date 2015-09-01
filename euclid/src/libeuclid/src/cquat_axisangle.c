//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
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

#include "euclid/cquat.h"

// sqrt
#include <math.h>

void cquat_to_axisangle(const double *q, double *v, double *angle) {
	double n = sqrt(
	    q[CQUAT_X]*q[CQUAT_X]
	    + q[CQUAT_Y]*q[CQUAT_Y]
	    + q[CQUAT_Z]*q[CQUAT_Z]
	);

	// Axis
	v[0] = q[CQUAT_X] / n;
	v[1] = q[CQUAT_Y] / n;
	v[2] = q[CQUAT_Z] / n;

	// Angle
	*angle = 2.0*atan2(n, q[CQUAT_W]);
}


void cquat_from_axisangle(double *q, const double *v, double angle) {
	double n = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

	// Cut the angle half
	angle = angle / 2.0;

	// Vectorial part
	q[CQUAT_X] = sin(angle)*v[0] / n;
	q[CQUAT_Y] = sin(angle)*v[1] / n;
	q[CQUAT_Z] = sin(angle)*v[2] / n;

	// Scalart part
	q[CQUAT_W] = cos(angle);
}

