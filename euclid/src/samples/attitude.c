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

#include "euclid/cmatrix.h"
#include "euclid/cvector.h"

// exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// printf, fprintf, stderr
#include <stdio.h>

// atan2
#include <math.h>

// strcmp
#include <string.h>

// inline parse_double()
#include "parse_double.h"

#define PI_L 3.141592653589793238462643L
#define DEG2RAD_L(x) ((x)*PI_L/180.0L)
#define RAD2DEG_L(x) ((x)*180.0L/PI_L)

int main(int argc, char *argv[]) {


	if (argc < 2) {
		printf("Usage: %s <azimuth> [<elevation> [<roll>]]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	double param[3];
	for (int i = 0; i < 3; i++) {
		param[i] = 0.0;
	}

	for (int i = 1; i < argc; i++) {
		const char *carg = argv[i];
		int ok = parse_double(carg, &param[i-1]);
		if (!ok) {
			fprintf(stderr, "Cannot convert argument to double: %s\n", carg);
			exit(EXIT_FAILURE);
		}
	}

	double azimuth;
	double elevation;
	double roll;

	azimuth   = DEG2RAD_L(param[0]);
	elevation = DEG2RAD_L(param[1]);
	roll      = DEG2RAD_L(param[2]);

	double m[3*3];

	cmatrix_eye(m, 3);
	cmatrix_mroty(m, m, roll);
	cmatrix_mrotx(m, m, elevation);
	cmatrix_mrotz(m, m, -azimuth);

	// Output matrix only
	printf("[   right   |  forward  |     up    ]\n");
	printf("[ % 8.6f | % 8.6f | % 8.6f ]\n",
	    CMATRIX_ELEM(m, 0, 0, 3),
	    CMATRIX_ELEM(m, 0, 1, 3),
	    CMATRIX_ELEM(m, 0, 2, 3)
	);
	printf("[ % 8.6f | % 8.6f | % 8.6f ]\n",
	    CMATRIX_ELEM(m, 1, 0, 3),
	    CMATRIX_ELEM(m, 1, 1, 3),
	    CMATRIX_ELEM(m, 1, 2, 3)
	);
	printf("[ % 8.6f | % 8.6f | % 8.6f ]\n",
	    CMATRIX_ELEM(m, 2, 0, 3),
	    CMATRIX_ELEM(m, 2, 1, 3),
	    CMATRIX_ELEM(m, 2, 2, 3)
	);

	// Resolve attitude angles
	double r[3], f[3], u[3];

	// Convenience macros
	#define X 0
	#define Y 1
	#define Z 2

	cmatrix_to_frame(m, r, f, u);

	printf("\n");
	printf("(input)    azimuth = % f    elevation = % f     roll = % f\n",
	    (double) RAD2DEG_L(azimuth),
	    (double) RAD2DEG_L(elevation),
	    (double) RAD2DEG_L(roll)
	);

	double fx = f[X];
	double fy = f[Y];
	double fz = f[Z];

	// Azimuth and elevation are easy

	azimuth = atan2(fx, fy);
	elevation = atan2(fz, sqrt(fx*fx+fy*fy));

	// Roll needs some calculation...

	// Unit vectors for zero-roll right and up.
	double zr[3], zu[3];
	// zero-roll right; zr = cvector_cross(f, z)
	cvector_xyz(zr, fy, -fx, 0.0);
	// zero-roll up; zu
	cvector_cross(zu, zr, f);

	// Determine roll
	roll = atan2(cvector_dot(zr, u, 3), cvector_dot(zu, u, 3));

	printf("(output)   azimuth = % f    elevation = % f     roll = % f\n",
	    (double) RAD2DEG_L(azimuth),
	    (double) RAD2DEG_L(elevation),
	    (double) RAD2DEG_L(roll)
	);

	// Undefine convenience macros
	#undef X
	#undef Y
	#undef Z

	exit(EXIT_SUCCESS);
}
 
