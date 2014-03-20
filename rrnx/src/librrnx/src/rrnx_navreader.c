
#include "rrnx_navreader.h"
#include "rrnx_labels.h"

#include <stdlib.h> // malloc, free, NULL, FILE
//#include <errno.h> // strerror_r
#include <string.h>

static int noerr(rrnx_navreader *navreader) {
	navreader->err = RRNX_E_OK;
	return navreader->err;
}

static int errmsg_fr(rrnx_navreader *navreader) {
	// TODO: Should the error be copied from the fr,
	// or just tell that it is the fr?

	// For convenience
	rrnx_filereader *fr = navreader->fr;

	// Copy error code and message
	navreader->err = fr->err;
	rrnx_str_strcpy(navreader->errmsg, fr->errmsg);

	return navreader->err;
}

static int errmsg_prepend(
    rrnx_navreader *navreader,
    const char *fmt, ...
) {
	// TODO
	return navreader->err;
}


static rrnx_node *alloc_node(size_t payload_size) {
	// TODO
	return NULL;
}

static void parse_broadcast_orbit0(
    rrnx_navreader *navreader,
    const char *line
) {
	// Create a record, and append it to the list
	//rrnx_node *node = rrnx_n_alloc(rrnx_broadcast_orbit0);
	//rrnx_list_add(navreader->navfile->records, node);

	// Pick the payload
	//rrnx_broadcast_orbit0 *data = (void *) node->data;
	rrnx_broadcast_orbit0 *data
	    = malloc(sizeof(rrnx_broadcast_orbit0));

	// Do parsing
/*
	read_double(line, 1, 2, &data->sqrtA);
	if (navreader->err) goto err;

	read_double(line, 1, 2, &data->sqrtA);
	if (navreader->err) goto err;

	read_double(line, 1, 2, &data->sqrtA);
	if (navreader->err) goto err;

	// Parse success. Append record
*/

	goto finish;
err:
	// Error handling
	errmsg_prepend(navreader, "while parsing broadcast orbit 0: ");
	goto finish;

finish:
	return;
}

static void parse_broadcast_orbit1(
    rrnx_navreader *navreader,
    const char *line
) {
}

static void parse_broadcast_orbit2(
    rrnx_navreader *navreader,
    const char *line
) {
}

static void parse_broadcast_orbit3(
    rrnx_navreader *navreader,
    const char *line
) {
}

static void parse_broadcast_orbit4(
    rrnx_navreader *navreader,
    const char *line
) {
}

static void parse_broadcast_orbit5(
    rrnx_navreader *navreader,
    const char *line
) {
}

static void parse_broadcast_orbit6(
    rrnx_navreader *navreader,
    const char *line
) {
}

static void parse_broadcast_orbit7(
    rrnx_navreader *navreader,
    const char *line
) {
}


/*
static int eat_header(rrnx_navreader *navreader, const char *line) {
	// copy the record label into a work buffer,
	// translate the label into tag code, and switch.


}
*/

// Internal states (could use constants instead)
#define S_HEADER                1
#define S_DATA                  2
#define S_ERROR                 3
#define S_EOF                   4
#define S_ACCEPT                5
#define S_EXPECT_ORBIT_OR_EOF   6
#define S_BROADCAST_ORBIT0      7
#define S_BROADCAST_ORBIT1      8
#define S_BROADCAST_ORBIT2      9
#define S_BROADCAST_ORBIT3      10
#define S_BROADCAST_ORBIT4      11
#define S_BROADCAST_ORBIT5      12
#define S_BROADCAST_ORBIT6      13
#define S_BROADCAST_ORBIT7      14


static int is_allowed_header_line(int linetype) {
	int allowed = 1; // Allowed by default

	// For every line type, always.
	switch(linetype) {
	case 1: // TODO
		break;
	default:
		// Unknown line type.
		// This should result in abort.
		break;
	}

	return allowed;
}

static int cycle_data(
    rrnx_navreader *navreader,
    const char *line,
    int linetype
) {
	// For every state, always.
	switch(navreader->state)
	{
	// Rename into S_EXPECT_HEADER
	case S_HEADER:
		linetype = rrnx_enumerate_linetype(line);
		// Expect any header element
		// get_label_enum()
		// parse_record(label_enum);
		// if label_enum == end_of_header:
		// 	navreader->state = S_DATA;
		switch(linetype) {
		case RRNX_LBL_COMMENT:
			// Comments are accepted.
			// (This approach wont work; comments are allowed
			// in any state, and if the comments are paresed
			// only in a separate state, the following state
			// will always be same; no matter wehther the system
			// came into this state from S_HEADER or S_DATA.
			// That is a problem).
			//navreader->state = S_COMMENT_LINE;
			break;
		}
		break;

	case S_DATA:
		navreader->state = S_BROADCAST_ORBIT0;
		break;

	case S_EXPECT_ORBIT_OR_EOF:
		break;

	case S_BROADCAST_ORBIT0:
		parse_broadcast_orbit0(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT1;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT1:
		parse_broadcast_orbit1(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT2;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT2:
		parse_broadcast_orbit2(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT3;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT3:
		parse_broadcast_orbit3(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT4;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT4:
		parse_broadcast_orbit4(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT5;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT5:
		parse_broadcast_orbit5(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT6;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT6:
		parse_broadcast_orbit6(navreader, line);
		if (!navreader->err) {
			navreader->state = S_BROADCAST_ORBIT7;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_BROADCAST_ORBIT7:
		parse_broadcast_orbit7(navreader, line);
		if (!navreader->err) {
			navreader->state = S_EXPECT_ORBIT_OR_EOF;
		} else {
			navreader->state = S_ERROR;
		}
		break;

	case S_ERROR:
		break;

	default:
		break;
	} // switch

	return navreader->err;
}

static int cycle_eof(rrnx_navreader *navreader) {

	// For every state, always.
	switch(navreader->state)
	{
	case S_HEADER:
	case S_DATA:
		// unexpected
		break;

	case S_EXPECT_ORBIT_OR_EOF:
		// accept
		break;

	case S_BROADCAST_ORBIT0:
	case S_BROADCAST_ORBIT1:
	case S_BROADCAST_ORBIT2:
	case S_BROADCAST_ORBIT3:
	case S_BROADCAST_ORBIT4:
	case S_BROADCAST_ORBIT5:
	case S_BROADCAST_ORBIT6:
	case S_BROADCAST_ORBIT7:
		// unexpected
		break;

	default:
		break;
	} // switch

	return navreader->err;
}

extern rrnx_navreader *rrnx_navr_alloc(void) {
	// Allocate new object
	rrnx_navreader *navreader
	    = malloc(sizeof(rrnx_navreader));

	// Initialize members
	navreader->fr = rrnx_fr_alloc();
	navreader->navdata = NULL;

	navreader->errmsg = rrnx_str_alloc();

	noerr(navreader);

	return navreader;
}


extern void rrnx_navr_free(rrnx_navreader *navreader) {
	// Deallocate members
	rrnx_fr_free(navreader->fr);
	navreader->fr = NULL;
	rrnx_str_free(navreader->errmsg);
	navreader->errmsg = NULL;

	// Deallocate self
	free(navreader);
}

extern void rrnx_navr_bind(
    rrnx_navreader *navreader,
    FILE *fp
) {

}

extern void rrnx_navr_readfile(
    rrnx_navreader *navreader,
    const char *filename
) {
	// For convenience
	rrnx_filereader *fr = navreader->fr;

	// Open file
	rrnx_fr_fopen(fr, filename);
	if (fr->err) {
		// fopen() failed; propagate error
		errmsg_fr(navreader);
		return;
	}

	char line[0x200];

	while (1) {
		// Read a line
		rrnx_fr_readline(fr, line, sizeof(line));

		if (fr->err == RRNX_E_EOF) {
			// Eof reached.
			// Signal the eof to the parser
			// using a special value.
			rrnx_navr_consume(navreader, NULL);
			break;
		}
		else if (fr->err) {
			// Non-eof error. Propagate upwards
			errmsg_fr(navreader);
			break;
		}
		else {
			// Succesfully read a line.
			rrnx_navr_consume(navreader, line);
		} // if-else
	} // while

	// Close the file.
	// If this causes an error, it hides any parsing errors.. TODO
	rrnx_fr_fclose(fr);
}

extern int rrnx_navr_consume(rrnx_navreader *navreader, const char *line) {

	// Determine the tag code on the line, if any
	int linetype = rrnx_enumerate_linetype(line);
	//enumerate_label(line);

	do {
		// Reset null-transition flag
		navreader->eps = 0;

		// Perform one eating cycle
		if (line != NULL) {
			cycle_data(navreader, line, linetype);
		} else {
			cycle_eof(navreader);
		} // if-else

	} while (navreader->eps);

	return navreader->err;
}

