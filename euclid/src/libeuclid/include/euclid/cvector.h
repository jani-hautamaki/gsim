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

#ifndef CVECTOR_H
#define CVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

void cvector_zero(double *v, int n);

void cvector_scale(double *vdest, double c, const double *vorg, int n);
void cvector_add(double *v, const double *a, const double *b, int n);
void cvector_sub(double *v, const double *a, const double *b, int n);

double cvector_dot(const double *a, const double *b, int n);
long double cvector_dot_l(const double *a, const double *b, int n);

void cvector_cross(double *v, const double *a, const double *b);

double cvector_norm(const double *v, int n);
double cvector_norm2(const double *v, int n);
long double cvector_norm_l(const double *v, int n);
long double cvector_norm2_l(const double *v, int n);

void cvector_normalize(double *vdest, const double *vorg, int n);
void cvector_normalize_l(double *vdest, const double *vorg, int n);

void cvector_memcpy(double *vdest, const double *vorg, int n);

// Specific for 3d vectors

void cvector_xyz(double *v, double x, double y, double z);

void cvector_xrotate(double *vdest, const double *vorg, double rad);
void cvector_yrotate(double *vdest, const double *vorg, double rad);
void cvector_zrotate(double *vdest, const double *vorg, double rad);

//extern void cvector_setxyz(

#ifdef __cplusplus
} // extern "C"
#endif


#endif
