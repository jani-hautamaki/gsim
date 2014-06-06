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

// cquat_from_axisangle, cquat_to_matrix()
#include "euclid/cquat.h"

// cmatrix_mul()
#include "euclid/cmatrix.h"

// exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// printf, fprintf, stderr
#include <stdio.h>

// strcmp
#include <string.h>

// inline parse_double()
#include "parse_double.h"

#define PI_L 3.141592653589793238462643L
#define DEG2RAD_L(x) ((x)*PI_L/180.0L)

int main(int argc, char *argv[]) {

	if ((argc != 3) && (argc != 6)) {
		printf("Usage: %s <x|y|z> <angle_deg> [<x> <y> <z>]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char axis;
	double param[4];

	double angle;
	double v[3];

	for (int i = 0; i < 4; i++) {
		param[i] = 0.0;
	}

	// Process command-line arguments.
	axis = tolower(argv[1][0]);

	for (int i = 2; i < argc; i++) {
		const char *carg = argv[i];
		int ok = parse_double(carg, &param[i-2]);
		if (!ok) {
			fprintf(stderr, "Cannot convert argument to double: %s\n", carg);
			exit(EXIT_FAILURE);
		}
	}

	angle = DEG2RAD_L(param[0]);
	v[0] = param[1];
	v[1] = param[2];
	v[2] = param[3];


	// Rotation axis; initially zero vector.
	double a[3];

	if (strlen(argv[1]) > 1) {
		fprintf(stderr, "Rotation axis name too long: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	else if (axis == 'x') {
		a[0] = 1.0;
		a[1] = 0.0;
		a[2] = 0.0;
	}
	else if (axis == 'y') {
		a[0] = 0.0;
		a[1] = 1.0;
		a[2] = 0.0;
	}
	else if (axis == 'z') {
		a[0] = 0.0;
		a[1] = 0.0;
		a[2] = 1.0;
	}
	else {
		fprintf(stderr, "Rotation axis name unrecognized: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	} // if-else

	// The quaternion, rotation matrix and the rotated vector.
	double q[4];
	double m[3*3];
	double vrot[3];

	// Convert rotation axis and angle to a quaternion
        cquat_from_axisangle(q, a, angle);

	// Convert quaternion to a rotation matrix
	cquat_to_matrix(q, m);

	if (argc == 3) {
		// Output matrix only
		printf("[% 8.6f | % 8.6f | % 8.6f ]\n",
		    CMATRIX_ELEM(m, 0, 0, 3),
		    CMATRIX_ELEM(m, 0, 1, 3),
		    CMATRIX_ELEM(m, 0, 2, 3)
		);
		printf("[% 8.6f | % 8.6f | % 8.6f ]\n",
		    CMATRIX_ELEM(m, 1, 0, 3),
		    CMATRIX_ELEM(m, 1, 1, 3),
		    CMATRIX_ELEM(m, 1, 2, 3)
		);
		printf("[% 8.6f | % 8.6f | % 8.6f ]\n",
		    CMATRIX_ELEM(m, 2, 0, 3),
		    CMATRIX_ELEM(m, 2, 1, 3),
		    CMATRIX_ELEM(m, 2, 2, 3)
		);
	}
	else {
		// Perform the matrix multiplication
		cmatrix_mul(vrot, m, v, 3, 3, 1);

		// Output all items
		printf("[% 8.6f | % 8.6f | % 8.6f ]   [ % f ]  =  [ % f ]\n",
		    CMATRIX_ELEM(m, 0, 0, 3),
		    CMATRIX_ELEM(m, 0, 1, 3),
		    CMATRIX_ELEM(m, 0, 2, 3),
		    v[0],  vrot[0]
		);
		printf("[% 8.6f | % 8.6f | % 8.6f ]   [ % f ]  =  [ % f ]\n",
		    CMATRIX_ELEM(m, 1, 0, 3),
		    CMATRIX_ELEM(m, 1, 1, 3),
		    CMATRIX_ELEM(m, 1, 2, 3),
		    v[1],  vrot[1]
		);
		printf("[% 8.6f | % 8.6f | % 8.6f ]   [ % f ]  =  [ % f ]\n",
		    CMATRIX_ELEM(m, 2, 0, 3),
		    CMATRIX_ELEM(m, 2, 1, 3),
		    CMATRIX_ELEM(m, 2, 2, 3),
		    v[2],  vrot[2]
		);
	} // if-else

	exit(EXIT_SUCCESS);
}
