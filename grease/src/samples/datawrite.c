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

// exit, EXIT_SUCCESS
#include <stdlib.h>

// stderr, fprintf, fprintf
#include <stdio.h>

// errno
#include <errno.h>

// strerror, memset
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
			fprintf(stderr, "gut_datafile_create(): %s\n",
			    strerror(errno));
			break;
		}

		// Attempt to reset buffer size
		if (!gut_datafile_set_buffer_size(df, 16)) {
			fprintf(stderr, "gut_datafile_set_buffer_size(): %s\n",
			    strerror(errno));
		}

		if (!gut_datafile_open(df, fname, "wb")) {
			fprintf(stderr, "gut_datafile_open() failed: %s\n",
			    strerror(errno));
			break;
		}

		unsigned char padding[16];
		memset(padding, 0, 16);
		for (int i = 0; i < 16; i++) {
			int val = 0x80000000+i;
			printf("Writing %d\n", val);
			gut_datafile_write_int16(df, val);
			gut_datafile_write(df, padding, 6);

			gut_datafile_write_int32(df, val);
			gut_datafile_write(df, padding, 4);

			gut_datafile_write_long32(df, val);
			gut_datafile_write(df, padding, 4);

			gut_datafile_write_float32(df, val);
			gut_datafile_write(df, padding, 4);


			gut_datafile_write_double64(df, val);
			gut_datafile_write(df, padding, 8);

			if (gut_datafile_error(df)) break;
		}

		if (gut_datafile_error(df)) {
			fprintf(stderr, "write failed: %s\n", strerror(errno));
			break;
		}

		if (!gut_datafile_flush(df)) {
			fprintf(stderr, "flush failed: %s\n", strerror(errno));
			break;
		}

		retval = EXIT_SUCCESS;

	} while (0);

	gut_datafile_close(df);
	gut_datafile_free(df);

	exit(retval);
}
