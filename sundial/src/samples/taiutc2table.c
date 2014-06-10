/*
 * http://www.iers.org/IERS/EN/Publications/Bulletins/directLinks/bulletin__C__MD.html
 * http://toshi.nofs.navy.mil/ser7/tai-utc.dat
 */

#include "sundial/sundial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "parse_long.h"
#include "parse_double.h"

/**
 * Month names in uppercase
 */
const char *MONTH_NAME[12] = {
	"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
	"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

/**
 * Converts the month name abbreviation into a 1-based numeric value.
 * Returns non-zero value when conversion succeeds,
 * and zero if the conversion fails.
 */
static int parse_month(const char *name, long *month) {
	for (int i = 0; i < 12; i++) {
		if (strcmp(MONTH_NAME[i], name) == 0) {
			*month = i+1;
			return 1;
		}
	}
	return 0;
}

static void substr(char *dest, const char *src, int offset, int size) {
	for (int i = 0; i < offset; i++) {
		if (src[i] == '\0') {
			dest[0] = '\0';
			return;
		}
	}

	for (int i = 0; i < size; i++) {
		char c = src[offset+i];
		dest[i] = c;
		if (c == '\0') {
			return;
		}
	}

	dest[size] = '\0';
}

static int parse_line(
    const char *linebuf,
    int *year,
    int *month,
    int *day,
    double *delta
) {
	char s[0x100]; // for substrings

	long tmp;

	substr(s, linebuf, 1, 4); // year
	if (!parse_long(s, &tmp)) {
		fprintf(stderr, "cannot parse year: \"%s\"\n", s);
		return 0;
	}
	*year = (int) tmp;

	substr(s, linebuf, 6, 3); // month name abbr
	if (!parse_month(s, &tmp)) {
		fprintf(stderr, "cannot parse month: \"%s\"\n", s);
		return 0;
	}
	*month = (int) tmp;

	substr(s, linebuf, 10, 2); // day
	if (!parse_long(s, &tmp)) {
		fprintf(stderr, "cannot parse day: \"%s\"\n", s);
		return 0;
	}
	*day = (int) tmp;

	substr(s, linebuf, 36, 13); // tai-utc delta
	if (!parse_double(s, delta)) {
		fprintf(stderr, "cannot parse delta: %s\n", s);
		return 0;
	}
	return 1;

//         1         2         3         4         5         6         7         8
//12345678901234567890123456789012345678901234567890123456789012345678901234567890
//                                    1234567890123
// 2012 JUL  1 =JD 2456109.5  TAI-UTC=  35.0       S + (MJD - 41317.) X 0.0      S
}

static int parse_linebuf(const char *linebuf, long *mjd, double *delta) {
	int year, month, day;
	int rval;

	rval = parse_line(
	    linebuf, &year, &month, &day, delta);

	if (rval == 0) {
		return 0;
	}

	*mjd = sun_date2mjulianday(year, month, day);

	return 1;
}

static void output_begin_table(FILE *f) {
	fprintf(f, "static const long TAI_UTC_DELTA[] = {\n");
}

static void output_table_line(FILE *f, long mjday, double delta) {
	fprintf(f, "\t%ld, %d,\n", mjday, (int) delta);
}

static void output_end_table(FILE *f) {
	fprintf(f, "};\n");
}

static void parse_file(FILE *f, const char *filename) {
	char linebuf[0x100];
	int linenum = 0;

	output_begin_table(stdout);

	while (fgets(linebuf, sizeof(linebuf), f) != NULL) {
		long mjd;
		double delta;
		int ok;

		linenum++;

		ok = parse_linebuf(linebuf, &mjd, &delta);

		if (!ok) {
			fprintf(stderr, "Error on line %d in file %s",
			    linenum, filename);
			break;
		}

		// If the delta is fractional, skip
		if (((double) ((int) delta)) != delta) {
			// Skip
			continue;
		}
		/*
		if (mjd < SUN_EPOCH_GPSTIME) {
			// Skip the line
			continue;
		}
		*/

		output_table_line(stdout, mjd, delta);
		//output_table_line(stdout, mjd, delta - delta_at_epoch);

	} // while

	output_end_table(stdout);

}


int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <datafile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *f = NULL;

	do {
		const char *filename = argv[1];
		f = fopen(filename, "rt");
		if (f == NULL) {
			fprintf(stderr, "%s: %s\n",
			    filename, strerror(errno));
			break;
		}

		parse_file(f, filename);

		if (ferror(f)) {
			fprintf(stderr, "%s: %s\n",
			    filename, strerror(errno));
			break;
		}

	} while(0);

	if (f != NULL) {
		fclose(f);
		f = NULL;
	}

	exit(EXIT_SUCCESS);
}


