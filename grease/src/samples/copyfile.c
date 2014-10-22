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

#include "grease/gut_file.h"

// exit, EXIT_SUCCESS
#include <stdlib.h>

// stderr, fprintf, fprintf
#include <stdio.h>

// errno
#include <errno.h>

// strerror, strcmp
#include <string.h>

// Read/Write buffer size
#define BUFFER_SIZE 64

static void notify_file_error(const char *path, gut_file *f) {
	fprintf(stderr, "%s: %s\n", path, gut_file_strerror(f));
}

int main(int argc, char *argv[]) {

	gut_file *f1 = NULL;
	gut_file *f2 = NULL;

	int success = 0;

	do {
		if (argc < 3) {
			fprintf(stderr, "Usage: %s <sourcefile> <destfile>\n", argv[0]);
			break;
		}
		const char *source = argv[1];
		const char *dest = argv[2];

		f1 = gut_file_create(BUFFER_SIZE);
		f2 = gut_file_create(BUFFER_SIZE);

		if ((f1 == NULL) || (f2 == NULL)) {
			perror("gut_file_create()");
			break;
		}

		if (!gut_file_open(f1, source, "rb")) {
			notify_file_error(source, f1);
			break;
		}

		if (!gut_file_open(f2, dest, "wb")) {
			notify_file_error(dest, f2);
			break;
		}

		// Copy the file.

		success = 1;

		while (1) {
			unsigned char byteval;

			success = gut_file_read_byte(f1, &byteval);
			if (!success) {
				// Either error or eof.
				if (gut_file_error(f1)) {
					notify_file_error(source, f1);
					success = 0;
				} else {
					// Must be eof; anticipated.

					success = 1;
				} // if-else
				break;
			}

			success = gut_file_write_byte(f2, byteval);
			if (!success) {
				// Cannot be eof, thus it must be an error.
				notify_file_error(dest, f2);
				success = 0;
				break;
			}
		} // while forever

		// Check whether the copy succeeded
		if (!success) break;

		// Flush dest
		success = gut_file_flush(f2);
		if (!success) {
			notify_file_error(dest, f2);
			success = 0;
			break;
		}

	} while (0);

	gut_file_close(f1);
	gut_file_close(f2);

	gut_file_free(f1);
	gut_file_free(f2);

	exit(success ? EXIT_SUCCESS : EXIT_FAILURE);
}
