//******************************{begin:header}******************************//
//                      sundial - The Datetime Library                      //
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

#ifndef SUNDIAL_H
#define SUNDIAL_H

#ifdef __cplusplus
extern "C" {
#endif

// EPOCHS
//========

/**
 * GPS Time: epoch is 1980 Jan 6 (Sunday)
 */
#define SUN_EPOCH_GPSTIME           44244

/**
 * Modified Julian Day: epoch is 1858 Nov 17
 */
#define SUN_EPOCH_MJD                   0

/**
 * Julian Day epoch is [FIXME].
 */
#define SUN_EPOCH_JD             -2400001

// NUMBER OF SECONDS
//===================

/**
 * Number of seconds in a day (24 hours).
 */
#define SUN_SECONDS_IN_DAY              (60*60*24)

/**
 * Number of seconds in a week (7 days).
 */
#define SUN_SECONDS_IN_WEEK             (7*SUN_SECONDS_IN_DAY)

/**
 * Constant difference GPS-TAI = 19 s.
 */
#define SUN_DELTA_GPS_TAI                19


/**
 * Full Gregorian date and time of day with sub-second accuracy.
 */
struct sun_datetime {

	// GREGORIAN DATE
	//================

	/** Year (0 is 1 BC). */
	int year;

	/** Month (1-12). */
	int month;

	/** Day (1-31). */
	int day;

	// TIME OF DAY
	//=============

	/** Hour (0-23). */
	int hour;

	/** Minute (0-59). */
	int min;

	/** Second (0-59, or 60 on leap second). */
	int sec;

	/** Sub-second (0-1 excluding 1). */
	double subsec;
};

typedef struct sun_datetime sun_datetime;

/*
struct sun_mjulianday {
	long day;
	double timeofday;
};


struct sun_gpstime {
	int week;
	double tow;
};
*/

// LEAP SECONDS
//==============

int sun_delta_tai_utc(long mjd);

// CONVERSIONS
//=============

long sun_date2mjulianday(
    int year,
    int month,
    int day
);

void sun_mjulianday2date(
    long daynum,
    int *year,
    int *month,
    int *day
);

void sun_date2gpstime(
    int year,
    int month,
    int day,
    int *week,
    double *tow
);

void sun_gpstime2date(
    int week,
    double tow,
    int *year,
    int *month,
    int *day
);

void sun_datetime2gpstime(
    const sun_datetime *dt,
    int *week,
    double *tow
);

void sun_gpstime2datetime(
    int week,
    double tow,
    sun_datetime *dt
);

void sun_gpstime2utcdatetime(
    int week,
    double tow,
    sun_datetime *dt
);

void sun_utcdatetime2gpstime(
    const sun_datetime *dt,
    int *week,
    double *tow
);

// TIME OF DAY
//=============

void sun_timeofday2time(
    long timeofday,
    int *hour,
    int *min,
    int *sec
);

long sun_time2timeofday(
    int hour,
    int min,
    int sec
);

// TRANSLATION AND DELTA
//=======================

void sun_normalize_delta(
    long *delta_day,
    long *delta_timeofday,
    double *delta_subsec
);

void sun_translate_gpstime(
    int *week,
    double *tow,
    long delta_day,
    long delta_sec,
    double delta_subsec
);

double sun_delta_gpstime(
    int week2,
    double tow2,
    int week1,
    double tow1
);

void sun_translate_datetime(
    sun_datetime *dt,
    long delta_day,
    long delta_sec,
    double delta_subsec
);

double sun_delta_datetime(
   const sun_datetime *dt2,
   const sun_datetime *dt1
);

void sun_translate_utcdatetime(
    sun_datetime *dt,
    long delta_day,
    long delta_sec,
    double delta_subsec
);

double sun_delta_utcdatetime(
    const sun_datetime *dt2,
    const sun_datetime *dt1
);

void sun_subsec2delta(
    double subsec,
    long *delta_day,
    long *delta_sec,
    double *delta_subsec
);

double sun_delta2subsec(
    long delta_day,
    long delta_sec,
    double delta_subsec
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
