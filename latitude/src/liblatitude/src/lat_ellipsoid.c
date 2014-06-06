
#include "latitude/lat_ellipsoid.h"

// sqrt, sin, cos
#include <math.h>

// List of cited references:
//
// [Jekeli]
//      Jekeli C.
//      Geometric Reference Systems in Geodesy.
//      Unpublished course notes. Ohio State University. 2006 Jul;
//
// [Grewal et al]
//      Grewal MS, Weill LR, Andrews AP.
//      Global Positioning Systems, Inertial Navigation and Integration.
//      New York (NY): John Wiley & Sons, Inc.; 2001.
//
// [Borre and Strang]
//      Strang G, Borre K.
//      Linear Algebra, Geodesy, and GPS.
//      Wellesley (MA): Wellesley-Cambridge Press; 1997.
//

// INTERNAL METHODS
//==================


// OTHER METHODS
//===============

extern void lat_ellipsoid_set_ae(
    lat_ellipsoid *ellipsoid,
    double a,
    double e
) {
	ellipsoid->a = a;
	ellipsoid->e = e;
}

extern void lat_ellipsoid_set_af(
    lat_ellipsoid *ellipsoid,
    double a,
    double f
) {
	ellipsoid->a = a;
	// (Jekeli, 2-2, Eq 2.7)
	ellipsoid->e = sqrt(f*(2.0-f));
}

/**
 * Semi-latus rectum (the parameter) of a meridian ellipse.
 */
extern double lat_ellipsoid_p(
    const lat_ellipsoid *ellipsoid
) {
	return 0.0;
}


/**
 * Flattening of a meridian ellipse.
 */
extern double lat_ellipsoid_f(
    const lat_ellipsoid *ellipsoid
) {
	// Derived from (Jekeli, p 2-4, Eq 2.7):
	// e^2 = 2f - f^2
	// by completing the square and using the fact 0<f<1
	// to discard the other solution.
	double e2 = ellipsoid->e * ellipsoid->e;
	return 1.0-sqrt(1.0-e2);
}

/**
 * Semi-minor axis of a meridian ellipse.
 */
extern double lat_ellipsoid_b(
    const lat_ellipsoid *ellipsoid
) {
	// Derived from (Jekeli, p 2-4, Eq 2.5):
	// e = sqrt(a^2 - b^2) / a
	double e2 = ellipsoid->e * ellipsoid->e;
	return ellipsoid->a*sqrt(1.0-e2);
}

/**
 * Radius of curvature in the prime vertical (east-west).
 *
 * A vertical circle is a great circle on the celestial sphere
 * that is perpendicular to the horizon. Therefore it passes
 * through the zenith and the nadir. There is a vertical circle
 * for any given azimuth, where azimuth is the angle measured
 * east from the north on the celestial horizon.
 *
 * The vertical circle which is on the east-west direction is
 * called the prime vertical.
 *
 */
extern double lat_ellipsoid_N(
    const lat_ellipsoid *ellipsoid,
    double latitude
) {
	// (Jekeli, p 2-20, Eq 2.50)
	double e2 = ellipsoid->e * ellipsoid->e;
	double sin2lat = sin(latitude)*sin(latitude);
        return ellipsoid->a / sqrt(1.0-(e2*sin2lat));
}

/**
 * Radius of curvature in the prime meridian (north-south).
 *
 * Prime meridian is the half great circle at longitude=0.
 * The curvature in north-south direction is unaffected
 * by the longitude.
 *
 */
extern double lat_ellipsoid_M(
    const lat_ellipsoid *ellipsoid,
    double latitude
) {
	// (Jekeli, p 2-16, Eq 2.39)
	double e2 = ellipsoid->e * ellipsoid->e;
	double sin2lat = sin(latitude)*sin(latitude);
	double s = 1.0-(e2*sin2lat);
	return ellipsoid->a*(1.0-e2) / sqrt(s*s*s);
}


/**
 * Radius of curvature in the normal section
 */
extern double lat_ellipsoid_R_alfa(
    const lat_ellipsoid *ellipsoid,
    double latitude,
    double height,
    double azimuth
) {
	double R_N = lat_ellipsoid_N(ellipsoid, latitude) + height;
	double R_M = lat_ellipsoid_M(ellipsoid, latitude) + height;
	double sin2az = sin(azimuth)*sin(azimuth);
	double cos2az = cos(azimuth)*cos(azimuth);
	double R_inv = (sin2az/R_N) + (cos2az/R_M);
        return 1.0/R_inv;

}

/**
 * Convert geodetic to rectangular coordinates.
 *
 * The geodetic coordinates (longitude, latitude, altitude)
 * are converted into rectangular coordinates (x, y, z).
 */
extern void lat_ellipsoid_geo2xyz(
    const lat_ellipsoid *ellipsoid,
    const double *geo,
    double *xyz
) {
	// For convenience
	double lon = geo[0];
	double lat = geo[1];
	double h = geo[2];
	double e2 = ellipsoid->e * ellipsoid->e;

	double N = lat_ellipsoid_N(ellipsoid, lat);

	// (Jekeli, p 2-42, Eq 2.136)
	// (Borre and Strang, p 362, Eq 10.28)
	xyz[0] = (N+h)*cos(lat)*cos(lon);
	xyz[1] = (N+h)*cos(lat)*sin(lon);
	xyz[2] = ((N*(1.0-e2))+h)*sin(lat);
}

extern void lat_ellipsoid_xyz2geo(
    const lat_ellipsoid *ellipsoid,
    const double *xyz,
    double *geo
) {
	// For convenience
	double a = ellipsoid->a;
	double e2 = ellipsoid->e * ellipsoid->e;

	// For convenience and clarity
	double x = xyz[0];
	double y = xyz[1];
	double z = xyz[2];

	// Auxiliary variables
	double lat = 0;
	double h = 0;
	double hprev;
	double N;

	// Distance in the XY plane.
	double p = sqrt((x*x) + (y*y));

	do {
		hprev = h;
		// Re-compute N
		N = a / sqrt(1.0 - (e2*sin(lat)*sin(lat)));

		// Re-estimate height
		// Works only for lat != 90 deg.
		// (Jekeli, p 2-43, Eq 2.142)
		h = (p / cos(lat)) - N;

		// Re-estimate latitude
		// (Jekeli, p 2-43)
		lat = atan( z / ( p * (1.0 - ((e2*N)/(N+h)))) );

	} while (fabs(h-hprev) > 0.001); // TODO: Use a constant

	// Compute longitude
	// (Jekeli, 2-42, Eq 2.137)
	geo[0] = atan2(y, x);
	// Assign latitude and height
	geo[1] = lat;
	geo[2] = h;
}

/**
 * Get local coordinate basis at LLA.
 */
extern void lat_ellipsoid_localframe_at_geo(
    const lat_ellipsoid *ellipsoid,
    const double *geo,
    lat_localframe *frame
) {
	// For convenience and clarity
	double lon = geo[0];
	double lat = geo[1];
	// double h = lla[2]; // Unused

	// Denoting the equation (Jekeli, 2-41, Eq 2.136) by f(lon, lat, h),
	// then e, n and u are unit vectors in the directions
	//
	// e: df/d(lon)
	// n: df/d(lat)
	// u: df/d(h)

	// Derivative of f wrt longitude, normalized.
	// (Grewal et al, p 339, Eq C.74)
	// (Borre and Strang, p. 363, Eq 10.32)
	frame->e[0] = -sin(lon);
	frame->e[1] = cos(lon);
	frame->e[2] = 0;

	// Derivative of f wrt latitude, normalized.
	// (Grewal et al, p 339, Eq C.75)
	// (Borre and Strang, p. 363, Eq 10.32)
	frame->n[0] = -sin(lat)*cos(lon);
	frame->n[1] = -sin(lat)*sin(lon);
	frame->n[2] = cos(lat);

	// Derivative of f wrt height, normalized.
	// (Grewal et al, p 339, Eq C.76)
	// (Borre and Strang, p. 363, Eq 10.32)
	frame->u[0] = cos(lat)*cos(lon);
	frame->u[1] = cos(lat)*sin(lon);
	frame->u[2] = sin(lat);
}

extern void lat_ellipsoid_localframe_at(
    const lat_ellipsoid *ellipsoid,
    const double *geo,
    double *frame
) {
	// For convenience and clarity
	double lon = geo[0];
	double lat = geo[1];

	frame[0*3+0] = -sin(lon);
	frame[1*3+0] = cos(lon);
	frame[2*3+0] = 0.0;

	frame[0*3+1] = -sin(lat)*cos(lon);
	frame[1*3+1] = -sin(lat)*sin(lon);
	frame[2*3+1] = cos(lat);

	frame[0*3+2] = cos(lat)*cos(lon);
	frame[1*3+2] = cos(lat)*sin(lon);
	frame[2*3+2] = sin(lat);

}


extern void lat_ellipsoid_geo2enu(
    const lat_ellipsoid *ellipsoid,
    const double *geo,
    lat_localframe *frame
) {
	lat_ellipsoid_localframe_at_geo(ellipsoid, geo, frame);
}


