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


int main(int argc, char *argv[]) {

	gut_datafile *df1 = NULL;
	gut_datafile *df2 = NULL;

	int retval = EXIT_FAILURE;
	do {
		if (argc < 3) {
			fprintf(stderr, "Usage: %s <sourcefile> <destfile>\n", argv[0]);
			break;
		}
		const char *source = argv[1];
		const char *dest = argv[2];

		df1 = gut_datafile_create();
		df2 = gut_datafile_create();

		if ((df1 == NULL) || (df2 == NULL)) {
			fprintf(stderr, "Cannot create gut_datafile: out of memory\n");
			break;
		}

		// Attempt to reset buffer size
		gut_datafile_set_buffer_size(df1, 64); // 64 bytes only
		gut_datafile_set_buffer_size(df2, 64); // 64 bytes only

		// Open the files
		if ((gut_datafile_get_buffer_size(df1) != 64)
		   || (gut_datafile_get_buffer_size(df2) != 64))
		{
			fprintf(stderr, "Buffer realloc failed: %s\n",
			    strerror(errno));
			break;
		}

		if (gut_datafile_open(df1, source, "rb") == -1) {
			fprintf(stderr, "%s: %s\n", source, strerror(errno));
			break;
		}

		if (gut_datafile_open(df2, dest, "wb") == -1) {
			fprintf(stderr, "%s: %s\n", dest, strerror(errno));
			break;
		}

		// Copy the file.

		retval = EXIT_SUCCESS;

		while (1) {
			int byteval = gut_datafile_read_byte(df1);

			if (byteval == -1) {
				// Either error or eof.
				if (gut_datafile_error(df1)) {
					fprintf(stderr, "%s: %s\n",
					    source, strerror(errno));
					retval = EXIT_FAILURE;
				}
				break;
			}

			int ok = gut_datafile_write_byte(df2, byteval);

			if (ok == -1) {
				fprintf(stderr, "%s: %s\n",
				    dest, strerror(errno));
				retval = EXIT_FAILURE;
				break;
			}
		} // while forever

		// Check whether the copy succeeded
		if (retval == EXIT_FAILURE) break;

		// Flush dest
		if (gut_datafile_flush(df2) == -1) {
			fprintf(stderr, "%s: %s\n",
			    dest, strerror(errno));
			retval = EXIT_FAILURE;
			break;
		}

	} while (0);

	gut_datafile_close(df1);
	gut_datafile_close(df2);

	gut_datafile_free(df1);
	gut_datafile_free(df2);

	exit(retval);
}
