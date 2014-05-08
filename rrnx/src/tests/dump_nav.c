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
#include "rrnx/rrnx_navreader.h"

static void dump_node_unknown(const rrnx_node *node) {
	printf("Node: unknown\n");
	printf("  Type:       %d (unknown)\n", node->type);
	printf("  Size:       %d\n", node->size);
}

static void dump_format_decl(void *ptr) {
	const rrnx_format_decl *data = ptr;
	printf("Node: format declaration\n");
	printf("  Version:    %s\n", data->version);
	printf("  File type:  %c\n", data->type);
}

static void dump_comment(void *ptr) {
	const rrnx_comment *data = ptr;
	printf("Node: comment\n");
	printf("  Comment:    %s\n", data->text);
}

static void dump_creation_info(void *ptr) {
	const rrnx_creation_info *data = ptr;
	printf("Node: creation info\n");
	printf("  Program:    %s\n", data->program);
	printf("  Agency:     %s\n", data->agency);
	printf("  Date:       %s\n", data->date);
}

static void dump_ion_alpha(void *ptr) {
	const rrnx_ion_alpha *data = ptr;
	printf("Node: ion alpha\n");
	printf("  A0:         %19.12g\n", data->alpha[0]);
	printf("  A1:         %19.12g\n", data->alpha[1]);
	printf("  A2:         %19.12g\n", data->alpha[2]);
	printf("  A3:         %19.12g\n", data->alpha[3]);
}

static void dump_ion_beta(void *ptr) {
	const rrnx_ion_beta *data = ptr;
	printf("Node: ion beta\n");
	printf("  B0:         %19.12g\n", data->beta[0]);
	printf("  B1:         %19.12g\n", data->beta[1]);
	printf("  B2:         %19.12g\n", data->beta[2]);
	printf("  B3:         %19.12g\n", data->beta[3]);
}

static void dump_delta_utc(void *ptr) {
	const rrnx_delta_utc *data = ptr;
	printf("Node: delta utc\n");
	printf("  a0:         %19.12g\n", data->a0);
	printf("  a1:         %19.12g\n", data->a1);
	printf("  T:          %19d\n", data->T);
	printf("  T_week:     %19d\n", data->T_week);
}

static void dump_leap_seconds(void *ptr) {
	const rrnx_leap_seconds *data = ptr;
	printf("Node: leap seconds\n");
	printf("  leap secs:  %19d\n", data->leap_seconds);
}

static void dump_end_of_header(void *ptr) {
	printf("Node: end of header\n");
}

static void dump_broadcast_orbit0(void *ptr) {
	const rrnx_broadcast_orbit0 *data = ptr;
	printf("Node: broadcast orbit 0\n");
	/*
	const rrnx_time *Toc = &data->Toc; // For convenience
	printf("  Toc:       %02d-%02d-%02d %02d:%02d:%05.2f\n",
	    data->Toc.year, data->Toc.month, data->Toc.day,
	    data->Toc.hour, data->Toc.min, data->Toc.sec
	);
	*/
	printf("  toc:       %02d-%02d-%02d %02d:%02d:%05.2f\n",
	    data->toc_year, data->toc_month, data->toc_day,
	    data->toc_hour, data->toc_min, data->toc_sec
	);
	printf("  SV ID:      %19d\n", data->sv_id);
	printf("  af0:        %19.12g\n", data->af0);
	printf("  af1:        %19.12g\n", data->af1);
	printf("  af2:        %19.12g\n", data->af2);
}

static void dump_broadcast_orbit1(void *ptr) {
	const rrnx_broadcast_orbit1 *data = ptr;
	printf("Node: broadcast orbit 1\n");
	printf("  IODE:       %19d\n", data->IODE);
	printf("  Crs:        %19.12g\n", data->Crs);
	printf("  delta_n:    %19.12g\n", data->delta_n);
	printf("  M0:         %19.12g\n", data->M0);
}

static void dump_broadcast_orbit2(void *ptr) {
	const rrnx_broadcast_orbit2 *data = ptr;
	printf("Node: broadcast orbit 2\n");
	printf("  Cuc:        %19.12g\n", data->Cuc);
	printf("  e:          %19.12g\n", data->e);
	printf("  Cus:        %19.12g\n", data->Cus);
	printf("  sqrtA:      %19.12g\n", data->sqrtA);
}

static void dump_broadcast_orbit3(void *ptr) {
	const rrnx_broadcast_orbit3 *data = ptr;
	printf("Node: broadcast orbit 3\n");
	printf("  toe:        %19.12g\n", data->toe);
	printf("  Cic:        %19.12g\n", data->Cic);
	printf("  OMEGA:      %19.12g\n", data->OMEGA0);
	printf("  Cis:        %19.12g\n", data->Cis);
}

static void dump_broadcast_orbit4(void *ptr) {
	const rrnx_broadcast_orbit4 *data = ptr;
	printf("Node: broadcast orbit 4\n");
	printf("  i0:         %19.12g\n", data->i0);
	printf("  Crc:        %19.12g\n", data->Crc);
	printf("  w:          %19.12g\n", data->w);
	printf("  OMEGADOT:   %19.12g\n", data->OMEGADOT);
}

static void dump_broadcast_orbit5(void *ptr) {
	const rrnx_broadcast_orbit5 *data = ptr;
	printf("Node: broadcast orbit 5\n");
	printf("  IDOT:       %19.12g\n", data->idot);
	printf("  L2 codes:   %19.12g\n", data->L2_codes);
	printf("  toe_week:   %19d\n", data->toe_week);
	printf("  L2P data:   %19d\n", data->L2P_dataflag);
}

static void dump_broadcast_orbit6(void *ptr) {
	const rrnx_broadcast_orbit6 *data = ptr;
	printf("Node: broadcast orbit 6\n");
	printf("  Accuracy:   %19.12g\n", data->accuracy);
	printf("  Health:     %19.12g\n", data->health);
	printf("  Tgd:        %19.12g\n", data->Tgd);
	printf("  IODC:       %19d\n", data->IODC);
}

static void dump_broadcast_orbit7(void *ptr) {
	const rrnx_broadcast_orbit7 *data = ptr;
	printf("Node: broadcast orbit 7\n");
	printf("  toz:        %19.12g\n", data->toz);
	printf("  Fit intvl:  %19.12g\n", data->fit_interval);
	printf("  Spare1:     %19.12g\n", data->unused1);
	printf("  Spare2:     %19.12g\n", data->unused2);
}



static void dump_node(const rrnx_node *node) {
	void (*f)(void *) = NULL;

	switch(node->type) {
	case RRNX_ID_FORMAT_DECL:
		f = dump_format_decl;
		break;
	case RRNX_ID_CREATION_INFO:
		f = dump_creation_info;
		break;
	case RRNX_ID_COMMENT:
		f = dump_comment;
		break;
	case RRNX_ID_END_OF_HEADER:
		f = dump_end_of_header;
		break;
	case RRNX_ID_ION_ALPHA:
		f = dump_ion_alpha;
		break;
	case RRNX_ID_ION_BETA:
		f = dump_ion_beta;
		break;
	case RRNX_ID_DELTA_UTC:
		f = dump_delta_utc;
		break;
	case RRNX_ID_LEAP_SECONDS:
		f = dump_leap_seconds;
		break;
	case RRNX_ID_BROADCAST_ORBIT0:
		f = dump_broadcast_orbit0;
		break;
	case RRNX_ID_BROADCAST_ORBIT1:
		f = dump_broadcast_orbit1;
		break;
	case RRNX_ID_BROADCAST_ORBIT2:
		f = dump_broadcast_orbit2;
		break;
	case RRNX_ID_BROADCAST_ORBIT3:
		f = dump_broadcast_orbit3;
		break;
	case RRNX_ID_BROADCAST_ORBIT4:
		f = dump_broadcast_orbit4;
		break;
	case RRNX_ID_BROADCAST_ORBIT5:
		f = dump_broadcast_orbit5;
		break;
	case RRNX_ID_BROADCAST_ORBIT6:
		f = dump_broadcast_orbit6;
		break;
	case RRNX_ID_BROADCAST_ORBIT7:
		f = dump_broadcast_orbit7;
		break;

	default:
		dump_node_unknown(node);
		break;
	}

	if (f != NULL) {
		f((void *) node->data);
	}
}

static void dump_nav(const rrnx_nav *nav) {
	// Traverse all nodes
	int nodenum = 0;
	const rrnx_list *list = nav->nodelist;
	const rrnx_list_item *iter = list->first;
	while (iter != NULL) {
		rrnx_node *node = iter->data;
		//printf("Node %d\n", nodenum+1);
		dump_node(node);
		iter = iter->next;
		nodenum++;
	}

}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage:\n");
		printf("\n");
		printf("    dump_nav <rinex_nav>\n");
		printf("\n");
		return EXIT_FAILURE;
	}

	// Allocate a new navreader
	rrnx_navreader *navreader = rrnx_navr_alloc();

	// Try to read and parse the file specified
	// on the command line.
	printf("%s\n", argv[1]);
	rrnx_navr_readfile(navreader, argv[1]);
	int exitcode = EXIT_SUCCESS;

	// If an error, halt now
	int err = rrnx_navr_errno(navreader);
	if (err) {
        	printf("Parse failed (err=%d):\n", err);
		printf("%s\n", rrnx_navr_strerror(navreader));
		exitcode = EXIT_FAILURE;
	} else {
		printf("\n");
		// Pop navdata
		rrnx_nav *navdata = rrnx_navr_pop(navreader);
		dump_nav(navdata);
		rrnx_nav_free(navdata);
	}


	printf("Closing..\n");
	rrnx_navr_free(navreader);

	return exitcode;
}
