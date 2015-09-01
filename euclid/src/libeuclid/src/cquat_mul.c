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

#include "euclid/cquat.h"

/**
 *
 * Quaternion multiplication axb between two quaternions
 *
 *      a = ax*i + ay*j + az*k + aw
 *      b = bx*i + by*j + bz*k + bw
 *
 * is defined as the usual vector product with the following
 * constraint equations / multiplication table:
 *
 *      ii = -1         ij = k          ji = -k
 *      jj = -1         jk = i          kj = -i
 *      kk = -1         ki = j          ik = -j
 *
 * Consequently, the resulting quaternion axb is
 *
 *      axb = (ax*i + ay*j + az*k + aw) x (bx*i + by*j + bz*k + bw)
 *
 * Using the distributive law first time,
 *
 *      axb =
 *          ax*i x (bx*i + by*j + bz*k + bw)
 *        + ay*j x (bx*i + by*j + bz*k + bw)
 *        + az*k x (bx*i + by*j + bz*k + bw)
 *        +   aw * (bx*i + by*j + bz*k + bw)
 *
 * Using the distributive law second time,
 *
 *      axb =
 *          (ax*bx*ii + ax*by*ij + ax*bz*ik + ax*bw*i)
 *        + (ay*bx*ji + ay*by*jj + ay*bz*jk + ay*bw*j)
 *        + (az*bx*ki + az*by*kj + az*bz*kk + az*bw*k)
 *        + (aw*bx*i  + aw*by*j  + aw*bz*k  + aw*bw  )
 *
 * Applying constraint equations,
 *
 *      axb =
 *          (-ax*bx   +  ax*by*k + -ax*bz*j + ax*bw*i)
 *        + (-ay*bx*k + -ay*by   +  ay*bz*i + ay*bw*j)
 *        + ( az*bx*j + -az*by*i + -az*bz   + az*bw*k)
 *        + ( aw*bx*i +  aw*by*j +  aw*bz*k + aw*bw  )
 *
 * The constraing equations were
 *
 *      ii = -1         ij = k          ji = -k
 *      jj = -1         jk = i          kj = -i
 *      kk = -1         ki = j          ik = -j
 *
 * Grouping and rearranging the terms,
 *
 *      axb =
 *          ( ax*bw +  ay*bz + -az*by +  aw*bx)*i
 *        + (-ax*bz +  ay*bw +  az*bx +  aw*by)*j
 *        + ( ax*by + -ay*bx +  az*bw +  aw*bz)*k
 *        + (-ax*bx + -ay*by + -az*bz +  aw*bw)
 *
 * This can be understood with the usual cross product
 * and dot product,
 *
 *      axb = (aw*bw - a.b) + aw*b + bw*a + axq
 *
 * where the vectorial operations dot(.) and cross(x),
 * are applied only to the vectorial part of the quaternion.
 *
 */

void cquat_mul(double *q, const double *a, const double *b) {

	// For convenience
	#define W CQUAT_W
	#define X CQUAT_X
	#define Y CQUAT_Y
	#define Z CQUAT_Z

	q[X] = ( a[X]*b[W]) + ( a[Y]*b[Z]) + (-a[Z]*b[Y]) + (a[W]*b[X]);
	q[Y] = (-a[X]*b[Z]) + ( a[Y]*b[W]) + ( a[Z]*b[X]) + (a[W]*b[Y]);
	q[Z] = ( a[X]*b[Y]) + (-a[Y]*b[X]) + ( a[Z]*b[W]) + (a[W]*b[Z]);

	q[W] = (-a[X]*b[X]) + (-a[Y]*b[Y]) + (-a[Z]*b[Z]) + (a[W]*b[W]);

	// Undefine convenience macros
	#undef W
	#undef X
	#undef Y
	#undef Z

}
