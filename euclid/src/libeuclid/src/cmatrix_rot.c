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

#include "euclid/cmatrix.h"

// sin, cos
#include <math.h>

/**
 *
 * These functions build such rotation matrices R that
 *
 *    vrot = R v
 *
 * rotates the vector v about the axis using the right-hand rule.
 * That is, vector will be rotated in the counter-clockwise direction
 * when looking down the axis.
 *
 */

extern void cmatrix_rotx(double *m, double angle) {

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Column 1
	ELEM(m, 1, 1) = 1.0;
	ELEM(m, 2, 1) = 0.0;
	ELEM(m, 3, 1) = 0.0;

	// Column 2
	ELEM(m, 1, 2) = 0.0;
	ELEM(m, 2, 2) = cos(angle);
	ELEM(m, 3, 2) = sin(angle);

	// Column 3
	ELEM(m, 1, 3) = 0.0;
	ELEM(m, 2, 3) = -sin(angle);
	ELEM(m, 3, 3) = cos(angle);

	// Undefine convenience macro
	#undef ELEM
}


extern void cmatrix_roty(double *m, double angle) {

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Column 1
	ELEM(m, 1, 1) = cos(angle);
	ELEM(m, 2, 1) = 0.0;
	ELEM(m, 3, 1) = -sin(angle);

	// Column 2
	ELEM(m, 1, 2) = 0.0;
	ELEM(m, 2, 2) = 1.0;
	ELEM(m, 3, 2) = 0.0;;

	// Column 3
	ELEM(m, 1, 3) = sin(angle);
	ELEM(m, 2, 3) = 0.0;
	ELEM(m, 3, 3) = cos(angle);

	// Undefine convenience macro
	#undef ELEM
}

extern void cmatrix_rotz(double *m, double angle) {

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Column 1
	ELEM(m, 1, 1) = cos(angle);
	ELEM(m, 2, 1) = sin(angle);
	ELEM(m, 3, 1) = 0.0;

	// Column 2
	ELEM(m, 1, 2) = -sin(angle);
	ELEM(m, 2, 2) = cos(angle);
	ELEM(m, 3, 2) = 0.0;

	// Column 3
	ELEM(m, 1, 3) = 0.0;
	ELEM(m, 2, 3) = 0.0;
	ELEM(m, 3, 3) = 1.0;

	// Undefine convenience macro
	#undef ELEM

}
