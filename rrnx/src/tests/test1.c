//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

#include <stdio.h>
#include <stdlib.h>
#include "rrnx/rrnx_filereader.h"
#include "rrnx/rrnx_navreader.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("No arguments\n");
		return EXIT_FAILURE;
	}

	rrnx_filereader *reader = rrnx_fr_alloc();

	printf("Opening: %s\n", argv[1]);
	rrnx_fr_fopen(reader, argv[1]);
	if (reader->err) {
		printf("%s\n", reader->errmsg);
		return EXIT_FAILURE;
	}

	// read while possible
	char line[0x100];
	int ecount=0;
	while (rrnx_fr_readline(reader, line, 81) == RRNX_E_OK) {
        	//printf("%d: %s\n", reader->row, line);
	}

	if (reader->err == RRNX_E_EOF) {
        	printf("eof reached normally\n");
		reader->err = RRNX_E_OK;
	}

	// either error or eof
	if (reader->err) {
		printf("error %x:\n%s\n", reader->err, reader->errmsg);
	}

	printf("Closing: %s\n", reader->filename);
	rrnx_fr_fclose(reader);
	rrnx_fr_free(reader);

	printf("Reopening with navreader\n");
	rrnx_navreader *navreader = rrnx_navr_alloc();

	rrnx_navr_readfile(navreader, argv[1]);
	if (navreader->err) {
        	printf("Parsing finished with error (%d)\n",
		    navreader->err);
		printf("%s\n", navreader->errmsg->text);
	}

	printf("Closing..\n");
	rrnx_navr_free(navreader);

	return 0;
}
