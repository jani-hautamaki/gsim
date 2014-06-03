//******************************{begin:header}******************************//
//                  latitude - The Ellipsoid Math Library                   //
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

#ifndef LAT_ELLIPSOID_H
#define LAT_ELLIPSOID_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Oblate ellipsoid of revolution, ie spheroid.
 */
struct lat_ellipsoid {
	/**
	 * Semi-major axis.
	 */
	double a;

	/**
	 * Eccentricity.
	 */
	double e;
};

typedef struct lat_ellipsoid lat_ellipsoid;

/**
 * Local frame (east, north, up) at a point.
 */
struct lat_localframe {
	/**
	 * East unit vector
	 */
	double e[3];

	/**
	 * North unit vector
	 */
	double n[3];

	/**
	 * Up unit vector
	 */
	double u[3];
};

typedef struct lat_localframe lat_localframe;


// CONSTRUCTION & DESTRUCTION
//============================

/**
 * Initialize ellipsoid using semi-major axis and eccentricity.
 */
extern void lat_ellipsoid_set_ae(
    lat_ellipsoid *ellipsoid,
    double a,
    double e
);

/**
 * Initialize ellipsoid using semi-major axis and inverse flattening.
 */
extern void lat_ellipsoid_set_af(
    lat_ellipsoid *ellipsoid,
    double a,
    double f
);

// OTHER METHODS
//===============

/**
 * Change of coordinates from LLA to XYZ.
 * This is the easy direction.
 */
extern void lat_ellipsoid_geo2xyz(
    const lat_ellipsoid *ellipsoid,
    const double *lla,
    double *xyz
);

/**
 * Change of coordinates from XYZ to LLA.
 * This is the difficult direction,
 * and requires iteration.
 */
extern void lat_ellipsoid_xyz2geo(
    const lat_ellipsoid *ellipsoid,
    const double *xyz,
    double *lla
);

/**
 * Get local coordinate basis at LLA.
 */
extern void lat_ellipsoid_geo2enu(
    const lat_ellipsoid *ellipsoid,
    const double *lla,
    lat_localframe *frame
);


/**
 * Curvature in prime meridian (north-south direction).
 */
extern double lat_ellipsoid_curvature_prime_meridian(
    const lat_ellipsoid *ellipsoid,
    double latitude
);

/**
 * Curvature in prime vertical (east-west direction).
 */
extern double lat_ellipsoid_curvature_prime_vertical(
    const lat_ellipsoid *ellipsoid,
    double latitude
);

/**
 * Curvature in normal section to azimuth direction at a height.
 * Azimuth is measured clockwise from north?
 */
extern double lat_ellipsoid_curvature_normal_section(
    const lat_ellipsoid *ellipsoid,
    double latitude,
    double azimuth,
    double height
);

/**
 *  (Geodesic) Torsion in normal section to azimuth direction at a height.
 */
extern double lat_ellipsoid_torsion_normal_section(
    const lat_ellipsoid *ellipsoid,
    double latitude,
    double azimuth,
    double height
);



#ifdef __cplusplus
} // extern "C"
#endif

#endif
