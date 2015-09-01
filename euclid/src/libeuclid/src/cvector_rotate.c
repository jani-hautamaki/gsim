//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
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

#include "euclid/cvector.h"

// sin, cos
#include <math.h>

// Around x-axis

void cvector_rotate_x(
    double *vdest,
    const double *vorg,
    double angle
) {
	vdest[0] =  vorg[0];
	vdest[1] =  vorg[1]*cos(angle) + vorg[2]*sin(angle);
	vdest[2] = -vorg[1]*sin(angle) + vorg[2]*cos(angle);
}

void cvector_rotate_x_l(
    double *vdest,
    const double *vorg,
    long double angle
) {
	vdest[0] =  vorg[0];
	vdest[1] =  vorg[1]*cosl(angle) + vorg[2]*sinl(angle);
	vdest[2] = -vorg[1]*sinl(angle) + vorg[2]*cosl(angle);
}

// Around y-axis

void cvector_rotate_y(
    double *vdest,
    const double *vorg,
    double angle
) {
	vdest[0] = vorg[0]*cos(angle) - vorg[2]*sin(angle);
	vdest[1] = vorg[1];
	vdest[2] = vorg[0]*sin(angle) + vorg[2]*cos(angle);
}

void cvector_rotate_y_l(
    double *vdest,
    const double *vorg,
    long double angle
) {
	vdest[0] = vorg[0]*cosl(angle) - vorg[2]*sinl(angle);
	vdest[1] = vorg[1];
	vdest[2] = vorg[0]*sinl(angle) + vorg[2]*cosl(angle);
}

// around z-axis

void cvector_rotate_z(
    double *vdest,
    const double *vorg,
    double angle
) {
	vdest[0] =  vorg[0]*cos(angle) + vorg[1]*sin(angle);
	vdest[1] = -vorg[0]*sin(angle) + vorg[1]*cos(angle);
	vdest[2] = vorg[2];
}

void cvector_rotate_z_l(
    double *vdest,
    const double *vorg,
    long double angle
) {
	vdest[0] =  vorg[0]*cosl(angle) + vorg[1]*sinl(angle);
	vdest[1] = -vorg[0]*sinl(angle) + vorg[1]*cosl(angle);
	vdest[2] = vorg[2];
}



