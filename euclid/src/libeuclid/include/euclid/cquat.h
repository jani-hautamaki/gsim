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

void cquat_eye(double *q);
void cquat_set(double *q, double x, double y, double z, double w);

void cquat_scale(double *qdest, double c, const double *qorg);
void cquat_add(double *q, const double *a, const double *b);
void cquat_sub(double *q, const double *a, const double *b);

void cquat_mul(double *q, const double *a, const double *b);

double cquat_norm(const double *q);
double cquat_norm2(const double *q);

long double cquat_norm_l(const double *q);
long double cquat_norm2_l(const double *q);

void cquat_normalize(double *qdest, const double *qorg);
void cquat_normalize_l(double *qdest, const double *qorg);

void cquat_conjugate(double *qdest, const double *qorg);

void cquat_inverse(double *qdest, const double *qorg);
void cquat_inverse_l(double *qdest, const double *qorg);

// Conversions
/*
void cquat_to_frame(const double *q, double *e1, double *e2, double *e3);
void cquat_from_frame(double *q, const double *e1, const double *e2, const double *e3);
*/

void cquat_to_matrix(const double *q, double *m);
void cquat_to_matrix_t(const double *q, double *m);

void cquat_from_matrix(double *q, const double *m);
void cquat_from_matrix_t(double *q, const double *m);

void cquat_to_axisangle(const double *q, double *v, double *angle);
void cquat_from_axisangle(double *q, const double *v, double angle);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
