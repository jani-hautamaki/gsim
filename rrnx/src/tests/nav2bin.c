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
#include <string.h>

// va_list, va_start, va_end
#include <stdarg.h>

#include "rrnx/rrnx.h"

#define MODE_BINARY 1
#define MODE_CSV 2


/*
static void display_syscall_error(int syscall_errno) {
	printf("%s", strerror(syscall_errno));
}
*/

struct bytebuffer {
	unsigned char *data;
	size_t size;
	size_t len;
	int err;
};

typedef struct bytebuffer bytebuffer;

static void deinit_bytebuffer(bytebuffer *bb) {
	do {
		if (bb == NULL) break;

		free(bb->data);
		bb->data = NULL;
		bb->size = 0;
		bb->len = 0;

	} while(0);
}

static int init_bytebuffer(bytebuffer *bb, size_t size) {
	int success = 0;
	do {
		bb->data = NULL;
		bb->size = 0;
		bb->len = 0;
		bb->err = 0;

		bb->data = (unsigned char *) malloc(size);
		if (bb->data == NULL) break;

		bb->size = size;

		success = 1;
	} while(0);

	if (!success) {
		bb->err = 1;
		deinit_bytebuffer(bb);
	}

	return success;
}

static void free_bytebuffer(bytebuffer *bb) {
	do {
		if (bb == NULL) break;
		deinit_bytebuffer(bb);
		free(bb);
	} while(0);
}

static bytebuffer *create_bytebuffer(size_t size) {
	bytebuffer *bb = NULL;
	int success = 0;

	do {
		bb = malloc(sizeof(bytebuffer));
		if (bb == NULL) break;
		success = init_bytebuffer(bb, size);
	} while(0);

	if (!success) {
		free_bytebuffer(bb);
	}

	return bb;
}

static int bytebuffer_error(const bytebuffer *bb) {
	return bb->err;
}

static int bytebuffer_write_mem(bytebuffer *bb, const void *data, size_t len) {
	int success = 0;
	do {
		size_t avail = bb->size - bb->len; // always >= 0
		if (avail < len) {
			// ERROR: Not enough space
			bb->err = 1;
			break;
		}

		// otherwise mempcy
		memcpy(&bb->data[bb->len], data, len);
		bb->len += len;

		success = 1;
	} while(0);

	return success;
}

static int bytebuffer_write_double(bytebuffer *bb, double value) {
	return bytebuffer_write_mem(bb, &value, sizeof(double));
}

static int bytebuffer_write_uint(bytebuffer *bb, unsigned int value) {
	return bytebuffer_write_mem(bb, &value, sizeof(unsigned int));
}

static int bytebuffer_clear(bytebuffer *bb) {
	bb->len = 0;
	return 1;
}

static int bytebuffer_write_copy(bytebuffer *dest, const bytebuffer *src) {
	return bytebuffer_write_mem(dest, src->data, src->len);
}

static int bytebuffer_flush_to_buffer(bytebuffer *bb, bytebuffer *sink) {
	int success = 0;
	do {
		success = bytebuffer_write_copy(sink, bb);
		if (!success) break;

		success = bytebuffer_clear(bb);
	} while(0);
	return success;
}

static int bytebuffer_flush_to_file(bytebuffer *bb, FILE *fp) {
	int success = 0;
	do {
		size_t written = fwrite(bb->data, 1, bb->len, fp);

		success = written == bb->len;
		if (success) {
			// Completely flushed
			bytebuffer_clear(bb);
		} else {
			// Either feof or ferror.
			// Caller distinguishes between these cases.
			size_t left = bb->len - written; // always > 0
			memmove(bb->data, &bb->data[written], left);
			bb->len = left;
			bb->err = 1;
		} // if-else
	} while(0);

	return success;
}

static int bytebuffer_printf(bytebuffer *bb, const char *fmt, ...) {
	// use va_args
        va_list args;

	size_t avail = bb->size - bb->len;

        va_start(args, fmt);
	int len = vsnprintf((char *) &bb->data[bb->len], avail, fmt, args);
        va_end(args);

	// According to the man page of vsnprintf,
	// "a return value of size or more means
	//  that the output was  truncated."
	int success = len < avail;

	if (success) {
		// Update length (terminating null byte excluded)
		bb->len += len;
	} else {
		bb->err = 1;
	} // if-else

	return success;
}


static int write_navmsg_bin(bytebuffer *bb, const rrnx_navmsg *navmsg) {
	int ok = 1;
	if (navmsg != NULL) {
		bytebuffer_write_double(bb, (double)navmsg->sv_id);
		bytebuffer_write_double(bb, navmsg->IODE);
		bytebuffer_write_double(bb, navmsg->toe_week);
		bytebuffer_write_double(bb, navmsg->toe);

		// Keplerian elements
		bytebuffer_write_double(bb, navmsg->sqrtA);
		bytebuffer_write_double(bb, navmsg->e);
		bytebuffer_write_double(bb, navmsg->i0);
		bytebuffer_write_double(bb, navmsg->w);
		bytebuffer_write_double(bb, navmsg->OMEGA0);
		bytebuffer_write_double(bb, navmsg->M0);
		// Linear perturbations
		bytebuffer_write_double(bb, navmsg->delta_n);
		bytebuffer_write_double(bb, navmsg->OMEGADOT);
		bytebuffer_write_double(bb, navmsg->idot);
		// Periodic perturbations
		bytebuffer_write_double(bb, navmsg->Crs);
		bytebuffer_write_double(bb, navmsg->Crc);
		bytebuffer_write_double(bb, navmsg->Cus);
		bytebuffer_write_double(bb, navmsg->Cuc);
		bytebuffer_write_double(bb, navmsg->Cis);
		bytebuffer_write_double(bb, navmsg->Cic);

		// Clock correction parameters
		//bytebuffer_write_double(bb, navmsg->toc);
		bytebuffer_write_double(bb, navmsg->IODC);
		bytebuffer_write_double(bb, navmsg->af0);
		bytebuffer_write_double(bb, navmsg->af1);
		bytebuffer_write_double(bb, navmsg->af2);

		// Miscellaneous parameters
		bytebuffer_write_double(bb, navmsg->Tgd);
		bytebuffer_write_double(bb, navmsg->health);
		bytebuffer_write_double(bb, navmsg->accuracy);
		bytebuffer_write_double(bb, navmsg->fit_interval);
		bytebuffer_write_double(bb, (double)navmsg->valid_af2);

		/*
		int valid_spare1;
		int valid_spare2;

		rrnx_datetime toc;

		double L2_codes;
		double L2P_dataflag;
		double tow;

		double spare1;
		double spare2;
		*/
		ok = !bytebuffer_error(bb);
	}

	return ok;
}

static int write_navmsg_csv(bytebuffer *bb, const rrnx_navmsg *navmsg) {
	int ok = 1;
	if (navmsg != NULL) {
		bytebuffer_printf(
		    bb, "%d, %d, %g, %g, ",
                    navmsg->sv_id,
		    (int) navmsg->IODE,
		    navmsg->toe_week,
		    navmsg->toe
		);
		bytebuffer_printf(
		    bb, "%g, %g, %g, %g, %g, %g, ",
		    navmsg->sqrtA,
		    navmsg->e,
		    navmsg->i0,
		    navmsg->w,
		    navmsg->OMEGA0,
		    navmsg->M0
		);
		bytebuffer_printf(
		    bb, "%g, %g, %g, ",
		    navmsg->delta_n,
		    navmsg->OMEGADOT,
		    navmsg->idot
		);
		bytebuffer_printf(
		    bb, "%g, %g, %g, %g, %g, %g, ",
		    navmsg->Crs,
		    navmsg->Crc,
		    navmsg->Cus,
		    navmsg->Cuc,
		    navmsg->Crs,
		    navmsg->Crc
		);
		bytebuffer_printf(
		    bb, "%d, %g, %g, %g, ",
		    (int) navmsg->IODC,
		    navmsg->af0,
		    navmsg->af1,
		    navmsg->af2
		);
		bytebuffer_printf(
		    bb, "%g, %d, %d, %d, %d",
		    navmsg->Tgd,
		    (int) navmsg->health,
		    (int) navmsg->accuracy,
		    (int) navmsg->fit_interval,
		    (int) navmsg->valid_af2
		);
		bytebuffer_printf(bb, "\n");

		ok = !bytebuffer_error(bb);
	}

	return ok;
}

typedef int (*fun_write_navmsg)(bytebuffer *bb, const rrnx_navmsg *navmsg);

static fun_write_navmsg get_write_navmsg(int mode) {
	fun_write_navmsg write_navmsg = NULL;

	switch(mode) {
	case MODE_BINARY:
		write_navmsg = write_navmsg_bin;
		break;
	case MODE_CSV:
		write_navmsg = write_navmsg_csv;
		break;
	default:
		break;
	} // switch

	return write_navmsg;
}


static int write_binfile(FILE *fp, const rrnx_file_nav *nav, int mode) {
	const rrnx_list_item *iter = nav->navmsg_list->first;

	// Function pointer selected by the mode
	fun_write_navmsg write_navmsg = get_write_navmsg(mode);
	if (write_navmsg == NULL) {
		fprintf(stderr, "Error: unsupported mode (%d)\n", mode);
		return 0;
	}

	int ok = 1;

	// Short-term buffer for serializing single items
	bytebuffer *bb = create_bytebuffer(1024*4);

	// Long-term buffer
	//bytebuffer *bigbuf = create_bytebuffer(1024*64);

	while (iter != NULL) {
		const rrnx_navmsg *navmsg = iter->data;

		// Write single navmsg struct
		int ok;

		ok = write_navmsg(bb, navmsg);
		if (!ok) {
			perror("bytebuffer");
			break;
		}

		ok = bytebuffer_flush_to_file(bb, fp);
		if (!ok) {
			perror("fwrite");
			break;
		}

		iter = iter->next;
	}

	free_bytebuffer(bb);
	bb = NULL;

	return ok;
}

struct run_params {
	//const char *self_filename;
	const char *input_filename;
	const char *output_filename;

	int mode;
};

typedef struct run_params run_params;

static int parse_args(run_params *params, int argc, char *argv[]) {
	int success = 1;
	for (int i = 1; i < argc; i++) {
		const char *carg = argv[i];
		if (strcmp(carg, "-csv") == 0) {
			params->mode = MODE_CSV;
		}
		else if (strcmp(carg, "-bin") == 0) {
			params->mode = MODE_BINARY;
		}
		else if (carg[0] == '-') {
			success = 0;
			fprintf(stderr,
			    "Error: unrecognized switch: %s\n", carg);
			break;
		}
		else if (params->input_filename == NULL) {
			params->input_filename = carg;
		}
		else if (params->output_filename == NULL) {
			params->output_filename = carg;
		}
		else {
			success = 0;
			fprintf(stderr,
			    "Error: too many files on the command line: %s", carg);
			break;
		}
	}
	return success;
}

static void display_usage(void) {
	printf("nav2bin <rinex_nav> [<dest_file>] [<options>]\n");
	printf("\n");
	printf("where options is one of the following\n");
	printf("    -csv       format as csv (default)\n");
	printf("    -bin       format as binary (must have dest file)\n");
	printf("\n");
	printf("When dest file is not specified, stdout is used\n");
	printf("\n");
	printf("Example:\n");
	printf("find -iname \"*.??n\" | xargs -l1 -I \"{}\" nav2bin {} {}.csv\n");
	//printf("Rename afterwards:\n");
	//printf("find -iname \"*.csv\" | sed \"s/[^\\/]*\\/\\(....\\)\\(...\\)..\\(..\\).*/\\0 \\1_20\\3_\\2.csv/\" | xargs -l1 mv\n");
	//printf("Combine files:\n");
	//printf("find -iname \"*.csv\" | sort | xargs cat > combined.csv\n");

}

int main(int argc, char *argv[]) {
	int exitcode = EXIT_FAILURE;

	do {
		run_params params;
		params.input_filename = NULL;
		params.output_filename = NULL;
		params.mode = MODE_CSV;

		if (argc < 2) {
			display_usage();
			break;
		}

		if (!parse_args(&params, argc, argv)) {
			break;
		}

		// Verify that we have a working configuration.

		// For convenience
		const char *input_filename
		    = params.input_filename;
		const char *output_filename
		    = params.output_filename;
		int mode = params.mode;

		if (input_filename == NULL) {
			fprintf(stderr,
			    "Error: input filename not specified\n");
			display_usage();
			break;
		}
		if ((mode == MODE_BINARY) && (output_filename == NULL)) {
			fprintf(stderr,
			    "Error: when binary format is specified, it is not allowed to use stdout for output\n");
			break;
		}

		rrnx_file_nav *nav = NULL;
		int err = rrnx_read_navfile(input_filename, &nav);
		if (err) {
			printf("%s: parse error (rrnx error code %d)\n",
			    input_filename, err);
			break;
		}

		FILE *fp = NULL;

		if (output_filename != NULL) {
			fp = fopen(output_filename, "wb");
			if (fp == NULL) {
				perror("fopen");
				break;
			}
		} else {
			fp = stdout;
		}

		write_binfile(fp, nav, mode);

		if (fp != stdout) {
			fclose(fp);
		}

		rrnx_free_navfile(nav);

		exitcode = EXIT_SUCCESS;

	} while(0);


	return exitcode;
}
