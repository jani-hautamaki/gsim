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

#ifndef CMATRIX_H
#define CMATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines row-major ordering for the matrix elements
 */
#define CMATRIX_INDEX(y, x, cols) (((y)*cols)+(x))
#define CMATRIX_ELEM(m, y, x, cols) m[CMATRIX_INDEX(y, x, cols)]

extern void cmatrix_zero(double *m, int rows, int cols);
extern void cmatrix_eye(double *m, int n);

extern void cmatrix_transpose(
    double *mdest,
    const double *morg,
    int rows,
    int cols
);

extern void cmatrix_add(
    double *m,
    const double *a,
    const double *b,
    int rows,
    int cols
);

extern void cmatrix_sub(
    double *m,
    const double *a,
    const double *b,
    int rows,
    int cols
);

extern void cmatrix_mul(
    double *m,
    const double *a,
    const double *b,
    int arows,
    int nboth,
    int bcols
);


// Specific for 3x3 matrices

//extern void cmatrix_rotx(
//extern void cmatrix_setcol(m, rows, cols, int x, v)


#ifdef __cplusplus
} // extern "C"
#endif


#endif
