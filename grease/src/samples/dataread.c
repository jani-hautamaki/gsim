//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
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

#include "grease/gut_datafile.h"
#include "grease/gut_datafile_macros.h"
#include "grease/gut_error.h"

// exit, EXIT_SUCCESS
#include <stdlib.h>

// stderr, fprintf, fprintf
#include <stdio.h>

// errno
#include <errno.h>

// strerror
#include <string.h>

#define MAY_BREAK(fun_call) if (!(fun_call)) break

static int read_datafile(gut_datafile *df) {
	int success = 1; // returns success if zero loops
	for (int i = 0; i < 16; i++) {
		//int val = i-8;

		int val_int16;
		int val_int32;
		long val_long32;
		float val_float32;
		double val_double64;
		unsigned char padding[16];

		//printf("Reading %d\n", val);

		success = 0;
		MAY_BREAK(read_int16(df, &val_int16));
		MAY_BREAK(read_bytes(df, padding, 6));

		MAY_BREAK(read_int32(df, &val_int32));
		MAY_BREAK(read_bytes(df, padding, 4));

		MAY_BREAK(read_long32(df, &val_long32));
		MAY_BREAK(read_bytes(df, padding, 4));

		MAY_BREAK(read_float32(df, &val_float32));
		MAY_BREAK(read_bytes(df, padding, 4));

		MAY_BREAK(read_double64(df, &val_double64));
		MAY_BREAK(read_bytes(df, padding, 8));
		success = 1;

		printf("Read:   % d   % d   % ld    % f    % f\n",
		    val_int16, val_int32, val_long32,
		    val_float32, val_double64
		);

		if (gut_datafile_eof(df)) break; // anticipated eof
	} // for

	return success;
}

int main(int argc, char *argv[]) {

	gut_datafile *df = NULL;

	int retval = EXIT_FAILURE;
	do {
		if (argc < 2) {
			fprintf(stderr, "Usage: %s <file>\n", argv[0]);
			break;
		}
		const char *fname = argv[1];

		df = gut_datafile_create();
		if (df == NULL) {
			fprintf(stderr, "gut_datafile_create: %s\n",
			    strerror(errno));
			break;
		}

		// Attempt to reset buffer size
		if (!gut_datafile_set_buffer_size(df, 16)) {
			fprintf(stderr, "gut_datafile_set_buffer_size(): %s\n",
			    strerror(errno));
			break;
		}

		if (!gut_datafile_open(df, fname, "rb")) {
			fprintf(stderr, "fopen() failed: %s\n",
			    strerror(errno));
			break;
		}

		int success;
		success = read_datafile(df);

		if (success) {
			// Succesfully read.
		}
		else if (gut_datafile_eof(df)) {
			// Premature eof
			fprintf(stderr, "read failed: unexpected end-of-file\n");
			break;
		}
		else {
			// assert gut_datafile_error(df)
			// Syscall failed
			fprintf(stderr, "read failed: %s\n", strerror(errno));
			break;
		}

		retval = EXIT_SUCCESS;

	} while (0);

	gut_datafile_close(df);
	gut_datafile_free(df);

	exit(retval);
}
