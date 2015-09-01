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

/**
 * Row-major addressing for matrices
 */
#define ELEM_ROWMAJ(m, y, x, rows) m[(x)*(rows)+(y)]

/**
 * Column-major addressing for matrices
 */
#define ELEM_COLMAJ(m, y, x, cols) m[(y)*(cols)+(x)]


/**
 * Minimum trace value for the default matrix->cquat method.
 */
#define CQUAT_MIN_TRACE 0.1


/**
 * The correspondence between a rotation matrix
 * and a quaternion q is computed from the equation
 *
 *      vrot = qvq* = Rv
 *
 * where
 *
 *	v       original vector
 *      vrot    rotated vector
 *      q*      conjugate quaternion
 *      R       rotation matrix
 *
 * Furthermore, the multiplication between
 * a quaternion and a vector is understood
 * through the quaternion multiplication of
 *
 *      [qx, qy, qz, qw] * [vx, vy, vz, 0]
 *
 * ----------------------------------------------------
 *
 * The multiplication axb of two quaternions a and b
 * was computed to be (see cquat_mul.c):
 *
 *      axb =
 *          ( ax*bw +  ay*bz + -az*by +  aw*bx)*i
 *        + (-ax*bz +  ay*bw +  az*bx +  aw*by)*j
 *        + ( ax*by + -ay*bx +  az*bw +  aw*bz)*k
 *        + (-ax*bx + -ay*by + -az*bz +  aw*bw)
 *
 * Reformulating this as a matrix product gives
 *
 *      axb =
 *        [ aw -az  ay  ax] [bx]
 *        [ az  aw -ax  ay] [by]
 *        [-ay  ax  aw  az] [bz]
 *        [-ax -ay -az  aw] [bw]
 *
 * Denote this matrix as P(a).
 *
 * ----------------------------------------------------
 *
 * Compute
 *
 *      [v,0] x q =
 *        [  vx*qw +  vy*qz + -vz*qy +  0 ]
 *        [ -vx*qz +  vy*qw +  vz*qx +  0 ]
 *        [  vx*qy + -vy*qx +  vz*qw +  0 ]
 *        [ -vx*qx + -vy*qy + -vz*qz +  0 ]
 *
 * Reformulating this as a matrix pruduct gives
 *
 *      [v,0] x q =
 *        [  qw  qz -qy 0 ]   [ vx ]
 *        [ -qz  qw  qx 0 ]   [ vy ]
 *        [  qy -qx  qw 0 ]   [ vz ]
 *        [ -qx -qy -qz 0 ]   [ 0  ]
 *
 * Conjugate q,
 *
 *      [v,0] x q* =
 *        [  qw -qz  qy 0 ]   [ vx ]
 *        [  qz  qw -qx 0 ]   [ vy ]
 *        [ -qy  qx  qw 0 ]   [ vz ]
 *        [  qx  qy  qz 0 ]   [ 0  ]
 *
 * Denote this matrix as M(q).
 *
 * ----------------------------------------------------
 *
 * Compute the matrix product
 *
 *      P(q)*M(q)
 *
 * That is,
 *
 *        [ w -z  y  x]    [  w -z  y 0 ]
 *        [ z  w -x  y]    [  z  w -x 0 ]
 *        [-y  x  w  z]    [ -y  x  w 0 ]
 *        [-x -y -z  w]    [  x  y  z 0 ]
 *
 * The result is (see "samples/equations.c")
 *
 * [ ww + -zz + -yy +  xx | -wz + -zw +  yx +  xy |  wy +  zx +  yw +  xz | 0]
 * [ zw +  wz +  xy +  yx | -zz +  ww + -xx +  yy |  zy + -wx + -xw +  yz | 0]
 * [-yw +  xz + -wy +  zx |  yz +  xw +  wx +  zy | -yy + -xx +  ww +  zz | 0]
 * [-xw + -yz +  zy +  wx |  xz + -yw + -zx +  wy | -xy +  yx + -zw +  wz | 0]
 *
 * After eliminating the terms that cancel each other,
 * and combining similar terms,
 *
 * [ ww + -zz + -yy +  xx |            -2wz + 2yx |            2wy +  2zx | 0]
 * [            2zw + 2xy | -zz +  ww + -xx +  yy |            2zy + -2wx | 0]
 * [           -2yw + 2xz |             2yz + 2xw | -yy + -xx +  ww +  zz | 0]
 * [                    0 |                     0 |                     0 | 0]
 *
 * Remembering that q is a unit quaternion,
 *
 *       xx + yy + zz + ww = 1
 *
 * The diagonal elements can be expressed as
 *
 *       ww + -zz + -yy +  xx = 1-2(zz+yy)
 *      -zz +  ww + -xx +  yy = 1-2(zz+xx)
 *      -yy + -xx +  ww +  zz = 1-2(yy+xx)
 *
 * Conseequently, the final matrix is
 *
 *      [ 1-2( zz+yy) |   2(-wz+yx) |   2(wy+zx) | 0]
 *      [   2( zw+xy) | 1-2( zz+xx) |   2(zy-wx) | 0]
 *      [   2(-yw+xz) |   2( yz+xw) | 1-2(yy+xx) | 0]
 *      [           0 |           0 |          0 | 0]
 *
 * With a bit of rearranging,
 *
 *      [ 1-2(zz+yy) |   2(xy-zw) |   2(yw+xz) | 0]
 *      [   2(zw+yx) | 1-2(xx+zz) |   2(yz-xw) | 0]
 *      [   2(zx-yw) |   2(xw+zy) | 1-2(yy+xx) | 0]
 *      [          0 |          0 |          0 | 0]
 *
 */

void cquat_to_matrix(const double *q, double *m) {

	// Define convenience macros
	#define X CQUAT_X
	#define Y CQUAT_Y
	#define Z CQUAT_Z
	#define W CQUAT_W

	// Use 1-based column-major addressing for 3x3 matrices.
	#define ELEM(m, y, x) ELEM_COLMAJ(m, (y)-1, (x)-1, 3)

	ELEM(m, 1, 1) = 1.0-2.0*( q[Z]*q[Z] + q[Y]*q[Y] );
	ELEM(m, 2, 1) =     2.0*( q[Z]*q[W] + q[Y]*q[X] );
	ELEM(m, 3, 1) =     2.0*( q[Z]*q[X] - q[Y]*q[W] );

	ELEM(m, 1, 2) =     2.0*( q[X]*q[Y] - q[Z]*q[W] );
	ELEM(m, 2, 2) = 1.0-2.0*( q[X]*q[X] + q[Z]*q[Z] );
	ELEM(m, 3, 2) =     2.0*( q[X]*q[W] + q[Z]*q[Y] );

	ELEM(m, 1, 3) =     2.0*( q[Y]*q[W] + q[X]*q[Z] );
	ELEM(m, 2, 3) =     2.0*( q[Y]*q[Z] - q[X]*q[W] );
	ELEM(m, 3, 3) = 1.0-2.0*( q[Y]*q[Y] + q[X]*q[X] );

	// Undefine convenience macro
	#undef ELEM

	// Undefine the convenience macros
	#undef X
	#undef Y
	#undef Z
	#undef W
}

void cquat_to_matrix_t(const double *q, double *m) {

	// Define convenience macros
	#define X CQUAT_X
	#define Y CQUAT_Y
	#define Z CQUAT_Z
	#define W CQUAT_W

	// Use 1-based row-major addressing for 3x3 matrices.
	#define ELEM(m, y, x) ELEM_ROWMAJ(m, (y)-1, (x)-1, 3)

	ELEM(m, 1, 1) = 1.0-2.0*( q[Z]*q[Z] + q[Y]*q[Y] );
	ELEM(m, 2, 1) =     2.0*( q[Z]*q[W] + q[Y]*q[X] );
	ELEM(m, 3, 1) =     2.0*( q[Z]*q[X] - q[Y]*q[W] );

	ELEM(m, 1, 2) =     2.0*( q[X]*q[Y] - q[Z]*q[W] );
	ELEM(m, 2, 2) = 1.0-2.0*( q[X]*q[X] + q[Z]*q[Z] );
	ELEM(m, 3, 2) =     2.0*( q[X]*q[W] + q[Z]*q[Y] );

	ELEM(m, 1, 3) =     2.0*( q[Y]*q[W] + q[X]*q[Z] );
	ELEM(m, 2, 3) =     2.0*( q[Y]*q[Z] - q[X]*q[W] );
	ELEM(m, 3, 3) = 1.0-2.0*( q[Y]*q[Y] + q[X]*q[X] );

	// Undefine convenience macro
	#undef ELEM

	// Undefine the convenience macros
	#undef X
	#undef Y
	#undef Z
	#undef W
}



/**
 *
 * Take the matrix
 *
 * [ ww + -zz + -yy +  xx |            -2wz + 2yx |            2wy +  2zx | 0]
 * [            2zw + 2xy | -zz +  ww + -xx +  yy |            2zy + -2wx | 0]
 * [           -2yw + 2xz |             2yz + 2xw | -yy + -xx +  ww +  zz | 0]
 * [                    0 |                     0 |                     0 | 0]
 *
 * By looking at the diagonal the following relations are gathered
 *
 *       xx + -yy + -zz + ww = m11
 *      -xx +  yy + -zz + ww = m22
 *      -xx + -yy +  zz + ww = m33
 *
 * Furthermore, it is known that the quaternion ought to have unit lenght,
 *
 *       xx +  yy +  zz + ww = 1
 *
 * Combining these give 4 equations with 4 unknowns.
 *
 *      [  1 -1 -1  1 ]   [ xx ]     [ m11 ]
 *      [ -1  1 -1  1 ]   [ yy ]  =  [ m22 ]
 *      [ -1 -1  1  1 ]   [ zz ]     [ m33 ]
 *      [  1  1  1  1 ]   [ ww ]     [ m44 ]
 *
 * Inversing the matrix with wolfram alpha, http://www.wolframalpha.com/
 * by entering
 *
 *      inv {{1, -1, -1, 1}, {-1, 1, -1, 1}, {-1, -1, 1, 1}, {1, 1, 1, 1}}
 *
 * gives,
 *
 *      [ xx ]         [  1 -1 -1  1 ]   [ m11 ]
 *      [ yy ]  =  1/4 [ -1  1 -1  1 ]   [ m22 ]
 *      [ zz ]         [ -1 -1  1  1 ]   [ m33 ]
 *      [ ww ]         [  1  1  1  1 ]   [  1  ]
 *
 * Turning these into equations and taking square roots of both sides gives
 *
 *       x = -+ (1/2)*sqrt(1 +  m11 + -m22 + -m33)
 *       y = -+ (1/2)*sqrt(1 + -m11 +  m22 + -m33)
 *       z = -+ (1/2)*sqrt(1 + -m11 + -m22 +  m33)
 *       w = -+ (1/2)*sqrt(1 +  m11 +  m22 +  m33)
 *
 * The signs need to be determined. In practice only one element is
 * solved by using an equation from this system, and the rest are
 * determined from the other relations...
 *
 * Take again the matrix,
 *
 * [ ww + -zz + -yy +  xx |            -2wz + 2yx |            2wy +  2zx | 0]
 * [            2zw + 2xy | -zz +  ww + -xx +  yy |            2zy + -2wx | 0]
 * [           -2yw + 2xz |             2yz + 2xw | -yy + -xx +  ww +  zz | 0]
 * [                    0 |                     0 |                     0 | 0]
 *
 * This time look at the non-diagonal elements.
 * Taking
 *
 *   M + M' =
 *      [         0 | m12 + m21 | m13 + m31 ]
 *      [ m21 + m12 |         0 | m23 + m32 ]
 *      [ m31 + m13 | m32 + m23 |         0 ]
 *
 * gives the relations
 *
 *      m12 + m21 = 4xy
 *      m13 + m31 = 4xz
 *      m23 + m32 = 4yz
 *
 * By doing the same with
 *
 *   M - M' =
 *      [         0 | m12 - m21 | m13 - m31 ]
 *      [ m21 - m12 |         0 | m23 - m32 ]
 *      [ m31 - m13 | m32 - m23 |         0 ]
 *
 * gives the relations
 *
 *      m12 - m21 = -4zw
 *      m13 - m31 =  4yw
 *      m23 - m32 = -4xw
 *
 * Combining these six relations gives
 *
 *      m12 + m21 = 4xy
 *      m13 + m31 = 4xz
 *      m23 + m32 = 4yz
 *
 *      m21 - m12 = 4zw    (*)
 *      m13 - m31 = 4yw
 *      m32 - m23 = 4xw    (*)
 *
 * (The equations marked with (*) have been rearranged).
 * Given only one element of q, one can use these relations
 * to solve the others.
 *
 * If x is given:
 *
 *     y = (m12 + m21) / 4x
 *     z = (m13 + m31) / 4x
 *     w = (m32 - m23) / 4x
 *
 * If y is given
 *
 *     x = (m12 + m21) / 4y
 *     z = (m23 + m32) / 4y
 *     w = (m13 - m31) / 4y
 *
 * If z is given
 *
 *     x = (m13 + m31) / 4z
 *     y = (m23 + m32) / 4z
 *     w = (m21 - m12) / 4z
 *
 * If w is given
 *
 *     x = (m32 - m23) / 4w
 *     y = (m13 - m31) / 4w
 *     z = (m21 - m12) / 4w
 *
 */

void cquat_from_matrix(double *q, const double *m) {

	// Use 1-based column-major addressing for 3x3 matrices.
	#define ELEM(m, y, x) ELEM_COLMAJ(m, (y)-1, (x)-1, 3)

	double m11 = ELEM(m, 1, 1);
	double m22 = ELEM(m, 2, 2);
	double m33 = ELEM(m, 3, 3);

	double s;

	s = 1.0 + m11 + m22 + m33; // trace

	if (s > CQUAT_MIN_TRACE) {
		// well above zero
		s = 2.0*sqrt(s); // equals 4w
		q[CQUAT_W] = s / 4.0;
		q[CQUAT_X] = (ELEM(m, 3, 2) - ELEM(m, 2, 3)) / s;
		q[CQUAT_Y] = (ELEM(m, 1, 3) - ELEM(m, 3, 1)) / s;
		q[CQUAT_Z] = (ELEM(m, 2, 1) - ELEM(m, 1, 2)) / s;
	}
	else if ((m11 > m22) && (m11 > m33)) {
		// m11 is greatest
		s = 2.0 * sqrt(1.0 + m11 - m22 - m33); // equals 4x
		q[CQUAT_X] = s / 4.0;
		q[CQUAT_Y] = (ELEM(m, 1, 2) + ELEM(m, 2, 1)) / s;
		q[CQUAT_Z] = (ELEM(m, 1, 3) + ELEM(m, 3, 1)) / s;
		q[CQUAT_W] = (ELEM(m, 3, 2) - ELEM(m, 2, 3)) / s;
	}
	else if ((m22 > m11) && (m22 > m33)) {
		// m22 is greatest
		s = 2.0 * sqrt(1.0 - m11 + m22 - m33); // equals 4y
		q[CQUAT_Y] = s / 4.0;
		q[CQUAT_X] = (ELEM(m, 1, 2) + ELEM(m, 2, 1)) / s;
		q[CQUAT_Z] = (ELEM(m, 2, 3) + ELEM(m, 3, 2)) / s;
		q[CQUAT_W] = (ELEM(m, 1, 3) - ELEM(m, 3, 1)) / s;
	}
	else if ((m33 > m11) && (m33 > m22)) {
		s = 2.0 * sqrt(1.0 - m11 - m22 + m33); // equals 4z
		q[CQUAT_Z] = s / 4.0;
		q[CQUAT_W] = (ELEM(m, 1, 3) + ELEM(m, 3, 1)) / s;
		q[CQUAT_X] = (ELEM(m, 2, 3) + ELEM(m, 3, 2)) / s;
		q[CQUAT_Y] = (ELEM(m, 2, 1) - ELEM(m, 1, 2)) / s;
	}

	// Undefine convenience macro
	#undef ELEM
}

void cquat_from_matrix_t(double *q, const double *m) {

	// Use 1-based row-major addressing for 3x3 matrices.
	#define ELEM(m, y, x) ELEM_ROWMAJ(m, (y)-1, (x)-1, 3)

	double m11 = ELEM(m, 1, 1);
	double m22 = ELEM(m, 2, 2);
	double m33 = ELEM(m, 3, 3);

	double s;

	s = 1.0 + m11 + m22 + m33; // trace

	if (s > CQUAT_MIN_TRACE) {
		// well above zero
		s = 2.0*sqrt(s); // equals 4w
		q[CQUAT_W] = s / 4.0;
		q[CQUAT_X] = (ELEM(m, 3, 2) - ELEM(m, 2, 3)) / s;
		q[CQUAT_Y] = (ELEM(m, 1, 3) - ELEM(m, 3, 1)) / s;
		q[CQUAT_Z] = (ELEM(m, 2, 1) - ELEM(m, 1, 2)) / s;
	}
	else if ((m11 > m22) && (m11 > m33)) {
		// m11 is greatest
		s = 2.0 * sqrt(1.0 + m11 - m22 - m33); // equals 4x
		q[CQUAT_X] = s / 4.0;
		q[CQUAT_Y] = (ELEM(m, 1, 2) + ELEM(m, 2, 1)) / s;
		q[CQUAT_Z] = (ELEM(m, 1, 3) + ELEM(m, 3, 1)) / s;
		q[CQUAT_W] = (ELEM(m, 3, 2) - ELEM(m, 2, 3)) / s;
	}
	else if ((m22 > m11) && (m22 > m33)) {
		// m22 is greatest
		s = 2.0 * sqrt(1.0 - m11 + m22 - m33); // equals 4y
		q[CQUAT_Y] = s / 4.0;
		q[CQUAT_X] = (ELEM(m, 1, 2) + ELEM(m, 2, 1)) / s;
		q[CQUAT_Z] = (ELEM(m, 2, 3) + ELEM(m, 3, 2)) / s;
		q[CQUAT_W] = (ELEM(m, 1, 3) - ELEM(m, 3, 1)) / s;
	}
	else if ((m33 > m11) && (m33 > m22)) {
		s = 2.0 * sqrt(1.0 - m11 - m22 + m33); // equals 4z
		q[CQUAT_Z] = s / 4.0;
		q[CQUAT_W] = (ELEM(m, 1, 3) + ELEM(m, 3, 1)) / s;
		q[CQUAT_X] = (ELEM(m, 2, 3) + ELEM(m, 3, 2)) / s;
		q[CQUAT_Y] = (ELEM(m, 2, 1) - ELEM(m, 1, 2)) / s;
	}

	// Undefine convenience macro
	#undef ELEM
}
