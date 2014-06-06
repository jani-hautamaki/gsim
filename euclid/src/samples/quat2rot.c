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
#define RAD2DEG_L(x) ((x)*180.0L/PI_L)

int main(int argc, char *argv[]) {

	if ((argc != 5) && (argc != 9)) {
		printf("Usage: %s <angle> <ax> <ay> <az> [<x> <y> <z>]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	double param[4+3];
	for (int i = 0; i < 4+3; i++) {
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

	double a[3];
	double angle;
	double v[3];

	double q[4];
	double m[3*3];
	double vrot[3];

	angle = DEG2RAD_L(param[0]);
	a[0] = param[1];
	a[1] = param[2];
	a[2] = param[3];

	v[0] = param[4+0];
	v[1] = param[4+1];
	v[2] = param[4+2];

	// Convert axisangle to quaternion

	//cquat_axisangle2q()
	//cquat_q2axisangle()
	cquat_from_axisangle(q, a, angle);

	// Convert quaternion to rotm
	cquat_to_matrix(q, m);

	// Output the rotation matrix
        if (argc == 5) {
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

		printf("\n");

		// For evaluating the quaternion

		// Output the quaternion
		printf("(input)   q = [% 8.6f | % 8.6f | % 8.6f | % 8.6f ]\n",
		    q[CQUAT_X], q[CQUAT_Y], q[CQUAT_Z], q[CQUAT_W]);

		// Convert rotm to quaternion
		cquat_from_matrix(q, m);

		printf("(output)  q = [% 8.6f | % 8.6f | % 8.6f | % 8.6f ]\n",
		    q[CQUAT_X], q[CQUAT_Y], q[CQUAT_Z], q[CQUAT_W]);

		printf("\n");

		printf("(input)   angle = % 10.6f     axis = [ % 8.6f | % 8.6f | % 8.6f ]\n",
		    (double) RAD2DEG_L(angle), a[0], a[1], a[2]);

		// Convert quaternion to axis-angle
		cquat_to_axisangle(q, a, &angle);

		printf("(input)   angle = % 10.6f     axis = [ % 8.6f | % 8.6f | % 8.6f ]\n",
		    (double) RAD2DEG_L(angle), a[0], a[1], a[2]);
        } else {
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
	}


	exit(EXIT_SUCCESS);
}
