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

#ifndef CQUAT_H
#define CQUAT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines the indexing of the quaternion
 */
#define CQUAT_X 0
#define CQUAT_Y 1
#define CQUAT_Z 2
#define CQUAT_W 3

extern void cquat_eye(double *q);
extern void cquat_set(double *q, double x, double y, double z, double w);

extern void cquat_scale(double *qdest, double c, const double *qorg);
extern void cquat_add(double *q, const double *a, const double *b);
extern void cquat_sub(double *q, const double *a, const double *b);

extern void cquat_mul(double *q, const double *a, const double *b);

extern double cquat_norm(const double *q);
extern double cquat_norm2(const double *q);

extern long double cquat_norm_l(const double *q);
extern long double cquat_norm2_l(const double *q);

extern void cquat_normalize(double *qdest, const double *qorg);
extern void cquat_normalize_l(double *qdest, const double *qorg);

extern void cquat_conjugate(double *qdest, const double *qorg);

extern void cquat_inverse(double *qdest, const double *qorg);
extern void cquat_inverse_l(double *qdest, const double *qorg);

// Conversions
/*
extern void cquat_to_frame(const double *q, double *e1, double *e2, double *e3);
extern void cquat_from_frame(double *q, const double *e1, const double *e2, const double *e3);
*/

extern void cquat_to_matrix(const double *q, double *m);
extern void cquat_to_matrix_t(const double *q, double *m);

extern void cquat_from_matrix(double *q, const double *m);
extern void cquat_from_matrix_t(double *q, const double *m);


extern void cquat_to_axisangle(const double *q, double *v, double *angle);
extern void cquat_from_axisangle(double *q, const double *v, double angle);



#ifdef __cplusplus
} // extern "C"
#endif


#endif
