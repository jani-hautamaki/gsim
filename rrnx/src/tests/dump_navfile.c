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

#include "rrnx/rrnx.h"

static void dump_navfile_format(const rrnx_file_format *format) {
	printf("File format:\n");
	printf("  version:           \"%s\"\n", format->version);
	printf("  type:              \"%c\"\n", format->type);
	printf("  system:            \"%c\"\n", format->system);
}

static void dump_navfile_info(const rrnx_file_info *info) {
	printf("File origin:\n");
	printf("  program:           \"%s\"\n", info->program);
	printf("  agency:            \"%s\"\n", info->agency);
	printf("  date:              \"%s\"\n", info->date);
}

static void dump_navfile_navmsg(const rrnx_navmsg *navmsg) {
	//printf("  Sat   IODE   IODC            toe                  toc\n");
	printf("  %02d  %6d %6d      %9.2f    %02d:%02d:%02d %02d.%02d.%02d\n",
		navmsg->sv_id, (int) navmsg->IODE, (int) navmsg->IODC, navmsg->toe,
		navmsg->toc.hour, navmsg->toc.min, (int) navmsg->toc.sec,
		navmsg->toc.day, navmsg->toc.month, navmsg->toc.year
	);

}

static void dump_navfile_navmsg_list(const rrnx_list *list) {
	const rrnx_list_item *iter = list->first;

	int array[100];
	for (int i = 0; i < 100; i++) {
		array[i] = -1;
	}

	printf("\n");
	printf("Brief list of all NAV records\n");

	printf("  Sat   IODE   IODC            toe                  toc\n");
	int total = 0;
	while (iter != NULL) {
		const rrnx_navmsg *navmsg = iter->data;

		int sat_id = navmsg->sv_id;
		if ((sat_id >= 0) && (sat_id <= 99)) {
			if (array[sat_id] == -1) {
				// Initialize it
				array[sat_id] = 0;
			}
			// Increase count
			array[sat_id]++;
		} else {
			printf("Inappropriate satellite id: %d\n", sat_id);
		}

		dump_navfile_navmsg(navmsg);
		iter = iter->next;
		total++;
	}

	printf("\n");

	printf("NAV record summary:\n");
	printf("  Satellite ID    Total\n");
	int satellites = 0;
	for (int i = 0; i < 100; i++) {
		if (array[i] == -1) continue;
		satellites++;
		printf("            %02d      %3d\n", i, array[i]);
	}
	printf("\n");

	printf("Grand total summary:\n");
	printf("  Satellites:      %4d\n", satellites);
	printf("  Records:         %4d\n", total);
	printf("\n");

}

static void dump_navfile(const rrnx_file_nav *nav) {
	// File format
	dump_navfile_format(&nav->format);
	// File origin
	dump_navfile_info(&nav->info);

	printf("Ionospheric parameters:\n");
	printf("  has alpha:         %d\n", nav->has_iono_alpha);
	printf("  has beta:          %d\n", nav->has_iono_beta);

	printf("UTC parameters:\n");
	printf("  has delta-UTC:     %d\n", nav->has_delta_utc);
	printf("  has leap seconds:  %d\n", nav->has_leap_seconds);

	dump_navfile_navmsg_list(nav->navmsg_list);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage:\n");
		printf("\n");
		printf("    dump_navfile <rinex_nav>\n");
		printf("\n");
		return EXIT_FAILURE;
	}

	const char *filename = argv[1];
	printf("%s\n", filename);

	rrnx_file_nav *nav = NULL;
	int err = rrnx_read_navfile(filename, &nav);

	if (err) {
		printf("Unable to parse (rrnx error code %d)\n", err);
		return EXIT_FAILURE;
	}

	dump_navfile(nav);

	rrnx_free_navfile(nav);

	return EXIT_SUCCESS;
}
