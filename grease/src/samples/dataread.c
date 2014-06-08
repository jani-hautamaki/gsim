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
#include "grease/gut_error.h"

// exit, EXIT_SUCCESS
#include <stdlib.h>

// stderr, fprintf, fprintf
#include <stdio.h>

// errno
#include <errno.h>

// strerror
#include <string.h>

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
			fprintf(stderr, "gut_datafile_create: %s\n", strerror(errno));
			break;
		}

		// Attempt to reset buffer size
		gut_datafile_set_buffer_size(df, 16); // 16 bytes

		if (gut_datafile_open(df, fname, "rb") == -1) {
			fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
			break;
		}

		for (int i = 0; i < 16; i++) {
			//int val = i-8;
			int val_int16;
			int val_int32;
			long val_long32;
			float val_float32;
			double val_double64;
			unsigned char padding[16];

			//printf("Reading %d\n", val);
			val_int16 = gut_datafile_read_int16(df);
			gut_datafile_read(df, padding, 6);

			val_int32 = gut_datafile_read_int32(df);
			gut_datafile_read(df, padding, 4);

			val_long32 = gut_datafile_read_long32(df);
			gut_datafile_read(df, padding, 4);

			val_float32 = gut_datafile_read_float32(df);
			gut_datafile_read(df, padding, 4);

			val_double64 = gut_datafile_read_double64(df);
			gut_datafile_read(df, padding, 8);

			gut_datafile_read(df, padding, 40);

			if (df->err == GUT_E_EOF) break; // premature eof
			if (gut_datafile_error(df)) break; // file error

			printf("Read:   % d   % d   % ld    % f    % f\n",
			    val_int16, val_int32, val_long32,
			    val_float32, val_double64
			);

			if (gut_datafile_eof(df)) break; // anticipated eof

		}

		if (df->err == GUT_E_EOF) {
			fprintf(stderr, "read failed: unexpected end-of-file\n");
			break;
		}
		if (gut_datafile_error(df)) {
			fprintf(stderr, "read failed: %s\n", strerror(errno));
			break;
		}

		retval = EXIT_SUCCESS;

	} while (0);

	gut_datafile_close(df);
	gut_datafile_free(df);

	exit(retval);
}
