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

/**
 *
 * This utility program computes symbolically the matrix multiplication
 *
 *      R(q)*M(q) =
 *
 *        [ w -z  y  x]    [  w -z  y 0 ]
 *        [ z  w -x  y]    [  z  w -x 0 ]
 *        [-y  x  w  z]    [ -y  x  w 0 ]
 *        [-x -y -z  w]    [  x  y  z 0 ]
 *
 * Resulting matrix is the rotation matrix corresponding to the quaternion.
 * The implementation is in the file "quat_matrix.c".
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

	const char *qa[] = {
		 "w", "-z",  "y", "x",
		 "z",  "w", "-x", "y",
		"-y",  "x",  "w", "z",
		"-x", "-y", "-z", "w",
	};

	const char *qb[] = {
		 "w", "-z",  "y", "0",
		 "z",  "w", "-x", "0",
		"-y",  "x",  "w", "0",
		 "x",  "y",  "z", "0",
	};

	char *m[16];

	char buffer[256];
	for (int ay = 0; ay < 4; ay++) {
	for (int bx = 0; bx < 4; bx++) {
		// Used buffer length
		int len = 0;

		for (int k = 0; k < 4; k++) {
			const char *a = qa[(ay*4)+k];
			const char *b = qb[(k*4)+bx];

			// Determine sign
			int sign = 1;
			if ((a[0] == '-') && (b[0] != '-')) {
				sign = -1;
				a++;
			}
			else if ((a[0] != '-') && (b[0] == '-')) {
				sign = -1;
				b++;
			}
			else if ((a[0] == '-') && (b[0] == '-')) {
				sign = 1;
				a++;
				b++;
			}
			else {
				sign = 1;
			} // if-else: sign

			if ((k > 0) && (len > 0)) {
				len += sprintf(buffer+len, " + ");
			}

			if ((a[0] != '0') && (b[0] != '0')) {
				char c = sign > 0 ? ' ' : '-';
				// Append sign or white space
				len += sprintf(buffer+len, "%c", c);
				// Append product
				len += sprintf(buffer+len, "%s%s", a, b);
			} else {
				// Result is zero
				//len += sprintf(buffer+len, "  0");
			}

		} // for

		if (len == 0) {
			// Result is zero
			len += sprintf(buffer+len, "0");
		}

		// Store result
		char *s = malloc(len+1);
		memcpy(s, buffer, len+1);
		m[(ay*4)+bx] = s;
	} // for: x
	} // for: y

	// Output result
	for (int y = 0; y < 4; y++) {
		printf("[");
		for (int x = 0; x < 4; x++) {
			const char *s = m[(y*4)+x];
			if (x > 0) {
				printf(" | ");
			}
			printf("%s", s);
		}
		printf("]\n");
	}

	// Free all
	for (int i = 0; i < 16; i++) {
		free(m[i]);
		m[i] = NULL;
	}

	return 0;
}
