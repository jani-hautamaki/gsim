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

// strerror, strcmp
#include <string.h>

// Output modes
#define MODE_TEXT 0
#define MODE_BINARY 1


int main(int argc, char *argv[]) {

	gut_datafile *df = NULL;

	int retval = EXIT_FAILURE;
	do {
		if (argc < 2) {
			fprintf(stderr, "Usage: %s <file> [-text]\n", argv[0]);
			break;
		}
		const char *fname = argv[1];
		int mode = MODE_BINARY;

		if (argc >= 3) {
			const char *carg = argv[2];
			if (strcmp(carg, "-text") == 0) {
				mode = MODE_TEXT;
			} else {
				fprintf(stderr, "Unexpected mode: %s\n", carg);
				break;
			} // if-else
		} // if: has mode arg


		df = gut_datafile_create();
		if (df == NULL) {
			fprintf(stderr, "Cannot create gut_datafile: out of memory\n");
			break;
		}

		// Attempt to reset buffer size
		gut_datafile_set_buffer_size(df, 64); // 64 bytes only

		if (gut_datafile_open(df, fname, "rb") == -1) {
			fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
			break;
		}

		// Read file and echo it to stdout as binary
		int byteval;
		int offset = 0;
		while ((byteval = gut_datafile_read_byte(df)) != -1) {
			offset++;
			if (mode == MODE_BINARY) {
				printf("%02x ", byteval);
				if ((offset % 16) == 8) {
					printf("   ");
				}
				if ((offset % 16) == 0) {
					printf("\n");
				}
			} else if (mode == MODE_TEXT) {
				printf("%c", byteval);
			}
		}
		printf("\n\n");

		// either eof or error
		retval = EXIT_SUCCESS;
	} while (0);

	gut_datafile_close(df);
	gut_datafile_free(df);

	exit(retval);
}
