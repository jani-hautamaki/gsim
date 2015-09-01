//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

#include "grease/gut_file.h"

// exit, EXIT_SUCCESS, EXIT_SUCCESS
#include <stdlib.h>

// stderr, fprintf, fprintf
#include <stdio.h>

// errno
#include <errno.h>

// strerror, strcmp
#include <string.h>

// Default file buffer size
#define BUFFER_SIZE 64

static void notify_file_error(const char *path, gut_file *f) {
	if (gut_file_errno(f)) {
		fprintf(stderr, "%s: %s\n", path, gut_file_strerror(f));
	}
}

static void close_file(gut_file *f) {
	if (f != NULL) {
		int ok;
		ok = gut_file_flush(f);
		ok = gut_file_close(f);
		gut_file_free(f);
	}
}

static void display_usage(const char *program) {
	fprintf(
	    stdout,
	    "Usage:\n"
	    "%s <sourcefile> <destfile>\n"
	    "\n",
	    program
	);
}

int main(int argc, char *argv[]) {
	int exitcode = EXIT_FAILURE;

	gut_file *source = NULL;
	gut_file *target = NULL;
	const char *source_path = NULL;
	const char *target_path = NULL;

	do {
		if (argc < 3) {
			display_usage(argv[0]);
			break;
		}

		source_path = argv[1];
		target_path = argv[2];

		source = gut_file_create(BUFFER_SIZE);
		target = gut_file_create(BUFFER_SIZE);
		if ((source == NULL) || (target == NULL)) {
			perror("gut_file_create");
			break;
		}

		int ok;
		ok = gut_file_open(source, source_path, "rb");
		if (!ok) break;

		ok = gut_file_open(target, target_path, "wb");
		if (!ok) break;

		int got_byte = 1;
		int wrote_ok = 1;
		while (1) {
			unsigned char byteval;

			got_byte = gut_file_read1(source, &byteval);
			if (!got_byte) break; // eof or error

			wrote_ok = gut_file_write1(target, byteval);
			if (!wrote_ok) break;
		}

		if ((!got_byte) && (!gut_file_error(source))) {
			// eof; anticipated
			exitcode = EXIT_SUCCESS;
		}

	} while(0);

	if (source != NULL) {
		notify_file_error(source_path, source);
		close_file(source);
		source = NULL;
	}

	if (target != NULL) {
		notify_file_error(target_path, target);
		close_file(target);
		target = NULL;
	}

	exit(exitcode);
}
