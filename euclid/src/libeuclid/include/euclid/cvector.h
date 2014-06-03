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

extern void cvector_zero(double *v, int n);

extern void cvector_scale(double *vdest, double c, const double *vorg, int n);
extern void cvector_add(double *v, const double *a, const double *b, int n);
extern void cvector_sub(double *v, const double *a, const double *b, int n);

extern double cvector_dot(const double *a, const double *b, int n);
extern long double cvector_dot_l(const double *a, const double *b, int n);

extern void cvector_cross(double *v, const double *a, const double *b);

extern double cvector_norm(const double *v, int n);
extern double cvector_norm2(const double *v, int n);
extern long double cvector_norm_l(const double *v, int n);
extern long double cvector_norm2_l(const double *v, int n);

extern void cvector_normalize(double *vdest, const double *vorg, int n);
extern void cvector_normalize_l(double *vdest, const double *vorg, int n);

extern void cvector_memcpy(double *vdest, const double *vorg, int n);

// Specific for 3d vectors

extern void cvector_xrotate(double *vdest, const double *vorg, double rad);
extern void cvector_yrotate(double *vdest, const double *vorg, double rad);
extern void cvector_zrotate(double *vdest, const double *vorg, double rad);

//extern void cvector_setxyz(

#ifdef __cplusplus
} // extern "C"
#endif


#endif
