//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

// These are internal
#include "rrnx_labels.h"
#include "rrnx_strutil.h"

#include <string.h> // strlen()
#include <ctype.h> // isdigit()

struct enumerated_label {
	int id;
	const char *text;
};
typedef struct enumerated_label enumerated_label;

static const enumerated_label NAV_LABEL[] = {
	{RRNX_LBL_RINEX_DECL, "RINEX VERSION / TYPE"},
	{RRNX_LBL_CREATION_INFO, "PGM / RUN BY / DATE"},
	{RRNX_LBL_COMMENT, "COMMENT"},
	{RRNX_LBL_ION_ALPHA, "ION ALPHA"},
	{RRNX_LBL_ION_BETA, "ION BETA"},
	{RRNX_LBL_DELTA_UTC,"DELTA-UTC: A0,A1,T,W"},
	{RRNX_LBL_LEAP_SECONDS, "LEAP SECONDS"},
	{RRNX_LBL_END_OF_HEADER, "END OF HEADER"}
};

// TODO: enum_navr_label?
int rrnx_enumerate_label(const char *label) {
	int labels = sizeof(NAV_LABEL) / sizeof(NAV_LABEL[0]);

	const enumerated_label *cur = NAV_LABEL;
	for (int i = 0; i < labels; i++, cur++) {
		if (strcmp(cur->text, label) == 0) {
			return cur->id;
		}
	}
	return RRNX_LBL_UNKNOWN;
}

int rrnx_enumerate_linetype(const char *line) {
	// Identified line type
	int id = RRNX_LBL_UNKNOWN;

	if (line == NULL) {
		return id;
	}

	char label[32];
	rrnx_substr_trimmed(label, line, 60, 20);
	id = rrnx_enumerate_label(label);
	return id;
}

