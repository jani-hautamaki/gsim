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

#include "rrnx/rrnx_navreader.h"

#include "rrnx/rrnx_node.h"
#include "rrnx/rrnx_nodes_common.h"
#include "rrnx/rrnx_nodes_nav.h"

// internal
#include "rrnx_labels.h"
#include "rrnx_strutil.h"


#include <stdlib.h> // malloc, free, NULL, FILE
#include <string.h>
#include <errno.h> // errno
#include <stdarg.h> // va_list, va_start, va_end

/*
static int noerr(rrnx_navreader *navreader) {
	navreader->err = RRNX_E_OK;
	return navreader->err;
}
*/

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

static int errmsg_none(rrnx_navreader *navreader, int err) {
	navreader->err = err;
	rrnx_str_reset(navreader->errmsg);
	return navreader->err;
}

static int errmsg_format(
    rrnx_navreader *navreader,
    int err,
    const char *fmt, ...
) {
	navreader->err = err;

	va_list args;
	va_start(args, fmt);
	rrnx_str_vformat(navreader->errmsg, fmt, args);
	va_end(args);

	return navreader->err;
}

static int errmsg_prepend(
    rrnx_navreader *navreader,
    const char *fmt, ...
) {
	// Use the workbuffer for this?
	rrnx_string *org = rrnx_str_clone(navreader->errmsg);
	if (org != NULL) {
		va_list args;
		va_start(args, fmt);
		rrnx_str_vformat(navreader->errmsg, fmt, args);
		va_end(args);

		// Append the original
		rrnx_str_concat(navreader->errmsg, org);

	} else {
		// Allocation failed.
		// Leave the message untouched,
		// and ignore the error.
	}

	// Free the original
	rrnx_str_free(org);

	return navreader->err;
}

static int errmsg_prepend_location(rrnx_navreader *navreader) {
	// For convenience
	rrnx_filereader *fr = navreader->fr;
	errmsg_prepend(navreader,
	    "%s:%d: ", fr->filename, fr->row);
	return navreader->err;
}

static rrnx_node *nodelist_alloc_node(rrnx_list *nodelist, int nodetype) {
	// Return value

	rrnx_list_item *listitem = NULL;
	rrnx_node *node = NULL;

	int complete = 0;
	do {
		// Allocate a node with sizeof(nodetype)
		node = rrnx_node_alloc(nodetype);
		if (node == NULL) break;

		listitem = rrnx_list_append(nodelist, node);
		if (listitem == NULL) break;

		complete = 1;
	} while(0);

	if (complete == 0) {
		if (listitem != NULL) {
			rrnx_list_remove(nodelist, listitem);
			listitem = NULL;
		}
		if (node != NULL) {
			rrnx_node_free(node);
			node = NULL;
		}
	}

	return node;
}

static rrnx_node *alloc_node(rrnx_navreader *navreader, int type) {
	// Validate the node type prior to allocation attempt.
	if (rrnx_node_is_type_valid(type) == 0) {
		// Error! Invalid node type.
		errmsg_format(
		    navreader, RRNX_E_NODETYPE,
		    "Cannot allocate node (unknown node type %d)",
		    type
		);
		return NULL;
	}

	// Attempt allocation
	//rrnx_node *node = rrnx_nav_alloc_node(navreader->navdata, type);
	rrnx_node *node = nodelist_alloc_node(navreader->nodelist, type);

	if (node == NULL) {
		// Either a) malloc failed; or b) invalid node type.
		// The option b) has been excluded, so..

		// Allocation failed, abort
		errmsg_none(navreader, RRNX_E_NOMEM);
	} // if

	return node;
}

static rrnx_navmsg *alloc_datarecord_node(
    rrnx_navreader *navreader
) {
	rrnx_navmsg *rval = NULL;

	rrnx_node *node = alloc_node(navreader, RRNX_ID_DATARECORD_NAV);
	if (node != NULL) {
		rrnx_datarecord_nav *datarecord = (void *) node->data;
		// Allocation success.
		rval = &datarecord->navmsg;
	} else {
		// Allocation failed. Abort
	} // if-else

	return rval;
}

static void parse_fortran_double(
    rrnx_navreader *navreader,
    double *result,
    const char *s
) {
	if (navreader->err) {
		return;
	}

	// 1. Copy to local buffer,
	// 2. Fix fortran exponent
	// 3. Use libc to convert
	// 4. Mark errors, if any

	// For convenience
	char *workbuf = navreader->workbuf;

	int len = strlen(s);
	if (len > navreader->workbuf_size) {
		// TODO: This ought cause an error I think.
		len = navreader->workbuf_size;
	}

	// May overlap!
	if (s != workbuf) {
		memmove(workbuf, s, len);
		workbuf[len] = '\0';
	}

	rrnx_replace_fortran_exponent(workbuf);

	// "Since 0 can legitimately be returned on both success and failure,
	//  the calling program should set errno to 0 before the call,
	//  and then determine if an error occurred by checking whether
	//  errno has a nonzero value after the call."
	errno = 0;
	char *endptr;
	*result = strtod(workbuf, &endptr);
	int errnum = errno; // Record errno immediately

	if (*endptr != '\0') {
		// String not fully converted; error.
		errmsg_format(
		    navreader, RRNX_E_CONV,
		    "Cannot convert \"%s\" to a double (Unexpected char \'%c\')",
		    workbuf,
		    *endptr
		);
	}

	if (errnum != 0) {
		// error
		errmsg_format(
		    navreader, RRNX_E_CONV,
		    "Cannot convert \"%s\" to a double (%s)",
		    workbuf,
		    strerror(errnum)
		);
	}
}

static void parse_int(
    rrnx_navreader *navreader,
    int *result,
    const char *s
) {
	if (navreader->err) {
		return;
	}

	// Regarding atoi():
	// "The behavior is the same as strtol(nptr, NULL, 10)
	//  except that atoi() does not detect errors."

	// "Since 0 can legitimately be returned on both success and failure,
	//  the calling program should set errno to 0 before the call,
	//  and then determine if an error occurred by checking whether
	//  errno has a nonzero value after the call."
	errno = 0;
	char *endptr;
	long int value; // For an intermediate result
	value = strtol(s, &endptr, 10); // base 10.
	int errnum = errno; // Record errno immediately

	if (*endptr != '\0') {
		// String not fully converted; error.
		errmsg_format(
		    navreader, RRNX_E_CONV,
		    "Cannot convert \"%s\" to an integer (Unexpected char \'%c\')",
		    s, *endptr
		);
		return;
	}

	// Check numeric limits by converting back and forth

	// Narrowing conversion; may result in loss of information.
	*result = (int) value;
	// Widening; lost information cannot be recovered.
	long int check = (long int) *result;

	// Check match
	if (check != value) {
		// The value does not fit into "int"
		// Simulate ERANGE error
		errnum = ERANGE;
	}

	if (errnum != 0) {
		// error
		errmsg_format(
		    navreader, RRNX_E_CONV,
		    "Cannot convert \"%s\" to a integer (%s)",
		    s, strerror(errnum)
		);
	}
}

static void parse_fortran_double_substr(
    rrnx_navreader *navreader,
    double *result,
    const char *line,
    int offset,
    int len
) {
	rrnx_substr_trimmed(navreader->workbuf, line, offset, len);
	parse_fortran_double(navreader, result, navreader->workbuf);
}

static void parse_int_substr(
    rrnx_navreader *navreader,
    int *result,
    const char *line,
    int offset,
    int len
) {
	rrnx_substr_trimmed(navreader->workbuf, line, offset, len);
	parse_int(navreader, result, navreader->workbuf);
}

static void parse_rinex_decl(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_FORMAT_DECL);

	if (node == NULL) {
		return; // Allocation failed, abort
	}

	rrnx_format_decl *data = (void *) node->data;

	// Format version (F9.2)
	rrnx_substr_trimmed(data->version, line, 0, 9);

	// File type (A1)
	data->type = line[20];

	// TODO:
	// Satellite system
}

static void parse_creation_info(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_CREATION_INFO);

	if (node == NULL) {
		return; // Allocation failed, abort
	}

	rrnx_creation_info *data = (void *) node->data;

	// Program name (A20)
	rrnx_substr_trimmed2(data->program, line, 0, 20);

	// Agency name (A20)
	rrnx_substr_trimmed2(data->agency, line, 20, 20);

	// Creation date (A20)
	rrnx_substr_trimmed2(data->date, line, 40, 20);

}

static void parse_comment(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_COMMENT);

	if (node == NULL) {
		return; // Allocation failed, abort
	}

	rrnx_comment *data = (void *) node->data;

	// Comment (A60)
	rrnx_substr_trimmed2(data->text, line, 0, 60);

}

static void parse_end_of_header(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_END_OF_HEADER);
	if (node == NULL) return; // Allocation failed, abort
}

static void parse_ion_alpha(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_ION_ALPHA);
	if (node == NULL) return; // Allocation failed, abort

	rrnx_ion_alpha *data = (void *) node->data;

	// A0 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->alpha[0], line, 2, 12);

	// A1 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->alpha[1], line, 14, 12);

	// A2 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->alpha[2], line, 26, 12);

	// A3 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->alpha[3], line, 38, 12);
}

static void parse_ion_beta(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_ION_BETA);
	if (node == NULL) return; // Allocation failed, abort

	rrnx_ion_beta *data = (void *) node->data;

	// B0 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->beta[0], line, 2, 12);

	// B1 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->beta[1], line, 14, 12);

	// B2 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->beta[2], line, 26, 12);

	// B3 (D12.4)
	parse_fortran_double_substr(
	    navreader, &data->beta[3], line, 38, 12);

}

static void parse_delta_utc(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_DELTA_UTC);
	if (node == NULL) return; // Allocation failed, abort

	rrnx_delta_utc *data = (void *) node->data;

	// A0 (D19.12)
	parse_fortran_double_substr(
	    navreader, &data->a0, line, 3, 19);

	// A1 (D19.12)
	parse_fortran_double_substr(
	    navreader, &data->a1, line, 22, 19);

	// T (I9)
	// TODO: This is optional!
	parse_int_substr(
	    navreader, &data->T, line, 41, 9);

	// W (I9)
	parse_int_substr(
	    navreader, &data->T_week, line, 50, 9);
}

static void parse_leap_seconds(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_node *node = alloc_node(
	    navreader, RRNX_ID_LEAP_SECONDS);
	if (node == NULL) return; // Allocation failed, abort

	rrnx_leap_seconds *data = (void *) node->data;

	// Leap seconds (I6)
	parse_int_substr(
	    navreader, &data->leap_seconds, line, 0, 6);
}


/**
 * Parses a fortran double field (Fx.y) in a BROADCAST ORBIT line.
 *
 * The BC orbit field is specified by "fieldnum".
 * Its value must be within 0-3.
 *
 * The result is stored into the variable pointed by "result".
 *
 * If unempty_flag is not NULL, then the variable is set to either 0 or 1
 * depending on whether the field is empty (0) or unempty (1).
 * If unempty_flag is NULL, then empty field results in an error.
 *
 */
static void parse_bco_value(
    rrnx_navreader *navreader,
    const char *line,
    int fieldnum,
    double *result,
    int *unempty_flag
) {
	// Length of a field
	int len = 19;
	// Compute the starting offset for the requested field.
	int offset = 3 + (fieldnum * len);

	int workbuf_len = rrnx_substr_trimmed(
	    navreader->workbuf, line, offset, len);

	// Record field blank status
	if (unempty_flag != NULL) {
		*unempty_flag = (workbuf_len > 0);
	}

	if (workbuf_len > 0) {
		// Attempt parsing
		parse_fortran_double(
		    navreader, result, navreader->workbuf);
	} else if (unempty_flag != NULL) {
		// Blanks are allowed.
	} else {
		// Error. Blanks are not allowed.
		errmsg_format(
		    navreader, RRNX_E_BLANK,
		    "Cannot convert broadcast orbit line\'s field #%d to a number: the field is blank",
	    	    fieldnum+1
		); // ermsg_format()
	} // if-else
}

static void parse_broadcast_orbit0(
    rrnx_navreader *navreader,
    const char *line
) {
	// Allocate a new data record node,
	// and store a pointer to the navmsg
	// that is currently being built.
	navreader->cur_navmsg = alloc_datarecord_node(navreader);

	rrnx_navmsg *data = navreader->cur_navmsg;
	if (data == NULL) return; // Alloc failed, abort.


	// Satellite PRN number (I2)
	parse_int_substr(
	    navreader, &data->sv_id, line, 0, 2);

	// For convenience
	rrnx_datetime *Toc = &data->toc;
	// Epoch: year (I2.2)
	parse_int_substr(
	    navreader, &Toc->year, line, 3, 2);
	// Epoch: month (I2)
	parse_int_substr(
	    navreader, &Toc->month, line, 6, 2);
	// Epoch: day (I2)
	parse_int_substr(
	    navreader, &Toc->day, line, 9, 2);
	// Epoch: hour (I2)
	parse_int_substr(
	    navreader, &Toc->hour, line, 12, 2);
	// Epoch: minute (I2)
	parse_int_substr(
	    navreader, &Toc->min, line, 15, 2);
	// Epoch: second (F5.1)
	parse_fortran_double_substr(
	    navreader, &Toc->sec, line, 17, 5);

	// Ordinary broadcast orbit line items.

	parse_bco_value(navreader, line, 1, &data->af0, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->af1, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3,
	    &data->af2, &data->valid_af2);
	if (navreader->err) return;

}

static void parse_broadcast_orbit1(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->IODE, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->Crs, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->delta_n, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3, &data->M0, NULL);
	if (navreader->err) return;

}

static void parse_broadcast_orbit2(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->Cuc, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->e, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->Cus, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3, &data->sqrtA, NULL);
	if (navreader->err) return;
}

static void parse_broadcast_orbit3(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->toe, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->Cic, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->OMEGA0, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3, &data->Cis, NULL);
	if (navreader->err) return;
}

static void parse_broadcast_orbit4(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->i0, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->Crc, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->w, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3, &data->OMEGADOT, NULL);
	if (navreader->err) return;

}

static void parse_broadcast_orbit5(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->idot, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->L2_codes, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->toe_week, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3, &data->L2P_dataflag, NULL);
	if (navreader->err) return;
}

static void parse_broadcast_orbit6(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->accuracy, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->health, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2, &data->Tgd, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3, &data->IODC, NULL);
	if (navreader->err) return;
}

static void parse_broadcast_orbit7(
    rrnx_navreader *navreader,
    const char *line
) {
	rrnx_navmsg *data = navreader->cur_navmsg;

	parse_bco_value(navreader, line, 0, &data->tow, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 1, &data->fit_interval, NULL);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 2,
	    &data->spare1, &data->valid_spare1);
	if (navreader->err) return;

	parse_bco_value(navreader, line, 3,
	    &data->spare2, &data->valid_spare2);
	if (navreader->err) return;

	// The navigation message is now complete.
	// Consequently, unset the current navmsg
	navreader->cur_navmsg = NULL;
}

static void parse_unknown(
    rrnx_navreader *navreader,
    const char *line
) {
	// Dummy. TODO: Remove this function??
}

static int parse_line(rrnx_navreader *navreader, const char *line) {
	int label_id = rrnx_enumerate_linetype(line);

	switch(label_id) {
	case RRNX_LBL_RINEX_DECL:
		parse_rinex_decl(navreader, line);
		break;
	case RRNX_LBL_CREATION_INFO:
		parse_creation_info(navreader, line);
		break;
	case RRNX_LBL_COMMENT:
		parse_comment(navreader, line);
		break;
	case RRNX_LBL_END_OF_HEADER:
		parse_end_of_header(navreader, line);
		break;
	case RRNX_LBL_ION_ALPHA:
		parse_ion_alpha(navreader, line);
		break;
	case RRNX_LBL_ION_BETA:
		parse_ion_beta(navreader, line);
		break;
	case RRNX_LBL_DELTA_UTC:
		parse_delta_utc(navreader, line);
		break;
	case RRNX_LBL_LEAP_SECONDS:
		parse_leap_seconds(navreader, line);
		break;

	case RRNX_LBL_UNKNOWN:
		parse_unknown(navreader, line);
		break;

	default:
		break;
	}

	return 0;
}


// Internal states (could use constants instead)
#define S_HEADER                1
#define S_DATA                  2
#define S_ERROR                 3
#define S_EOF                   4
#define S_FINISHED              5
#define S_EXPECT_ORBIT_OR_EOF   6
#define S_BROADCAST_ORBIT0      7
#define S_BROADCAST_ORBIT1      8
#define S_BROADCAST_ORBIT2      9
#define S_BROADCAST_ORBIT3      10
#define S_BROADCAST_ORBIT4      11
#define S_BROADCAST_ORBIT5      12
#define S_BROADCAST_ORBIT6      13
#define S_BROADCAST_ORBIT7      14

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
		if (linetype != RRNX_LBL_UNKNOWN) {
			parse_line(navreader, line);
			if (navreader->err) {
				navreader->state = S_ERROR;
			}
		}
		if (linetype == RRNX_LBL_END_OF_HEADER) {
			// No more header records.
			// Move on to expect broadcast orbit 0 or eof.
			navreader->state = S_EXPECT_ORBIT_OR_EOF;
		} else if (linetype == RRNX_LBL_UNKNOWN) {
			// Unknown header label.
	                errmsg_format(
         	           navreader, RRNX_E_SYNTAX,
                	    "Unexpected header label"
			);
			navreader->state = S_ERROR;
		} else {
			// Keep on expecting header lines.
		} // if-else
		break;

	case S_DATA:
		navreader->state = S_BROADCAST_ORBIT0;
		break;

	case S_EXPECT_ORBIT_OR_EOF:
		// If cycle_data() is called instead of cycle_eof(),
		// then it is known that line != NULL.
		navreader->state = S_BROADCAST_ORBIT0;
		navreader->eps = 1;
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
		navreader->state = S_FINISHED;
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

	case S_FINISHED:
		// Already finished
		break;

	default:
		break;
	} // switch

	return navreader->err;
}

rrnx_navreader *rrnx_navr_alloc(void) {
	int incomplete = 1;

	// Allocate new object
	rrnx_navreader *navreader
	    = malloc(sizeof(rrnx_navreader));

	if (navreader != NULL) do {
		// Initialize members
		navreader->fr = NULL;
		//navreader->navdata = NULL;
		navreader->nodelist = NULL;
		navreader->cur_navmsg = NULL;
		navreader->errmsg = NULL;
		navreader->err = RRNX_E_OK;
		navreader->workbuf = NULL;
		navreader->workbuf_size = 0;

		// These are not neccessary initializations
		navreader->state = 0;
		navreader->eps = 0;

		// Attempt further allocations
		navreader->fr = rrnx_fr_alloc();
		if (navreader->fr == NULL) {
			// Abort
			break;
		}

		navreader->errmsg = rrnx_str_alloc();
		if (navreader->errmsg == NULL) {
			// Abort
			break;
		}

		int size = RRNX_DEFAULT_WORKBUF_SIZE;
		navreader->workbuf = malloc(size);
		if (navreader->workbuf == NULL) {
			// Abort
			break;
		}
		navreader->workbuf_size = size;

		// Success
		incomplete = 0;
	} while (0);

	if (incomplete) {
		rrnx_navr_free(navreader);
		navreader = NULL;
	}

	return navreader;
}


void rrnx_navr_free(rrnx_navreader *navreader) {
	if (navreader == NULL) {
		// Already freed
		return;
	}


	// Release the currently held nodelist, if any.
	rrnx_list_free(navreader->nodelist);
	navreader->nodelist = NULL;

	// Deallocate filereader, if any
	rrnx_fr_free(navreader->fr);
	navreader->fr = NULL;

	// Deallocate errmsg, if any
	rrnx_str_free(navreader->errmsg);
	navreader->errmsg = NULL;

	free(navreader->workbuf);
	navreader->workbuf = NULL;
	navreader->workbuf_size = 0;

	// Deallocate self
	free(navreader);
}

int rrnx_navr_errno(const rrnx_navreader *navreader) {
	return navreader->err;
}
const char *rrnx_navr_strerror(const rrnx_navreader *navreader) {
	return navreader->errmsg->text;
}

/*
int rrnx_resize_linebuf(
    rrnx_navreader *navreader,
    unsigned int size
) {
	// TODO: Validate argument
	void *oldptr = navreader->linebuf;
	navreader->

}
*/

void rrnx_navr_bind(
    rrnx_navreader *navreader,
    FILE *fp
) {

}


int rrnx_navr_consume(rrnx_navreader *navreader, const char *line) {

	// Determine the tag code on the line, if any
	int linetype = rrnx_enumerate_linetype(line);
	//enumerate_label(line);

	do {
		// Reset null-transition flag
		navreader->eps = 0;

		// Perform one cycle per loop
		if (line != NULL) {
			cycle_data(navreader, line, linetype);
		} else {
			cycle_eof(navreader);
		} // if-else

	} while (navreader->eps);

	return navreader->err;
}

void rrnx_navr_readfile(
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

	// Reset parser state
	navreader->state = S_HEADER;

	// If there's a previous nodelist, free it first.
	rrnx_list_free(navreader->nodelist);
	navreader->nodelist = NULL;

	// Allocate a new nodelist
	navreader->nodelist = rrnx_list_alloc();
	if (navreader->nodelist == NULL) {
		// Mem alloc failed, abort.
		errmsg_none(navreader, RRNX_E_NOMEM);
	}
	// Configure a custom destructor for the nodelist.
	navreader->nodelist->destructor = (void *) rrnx_node_free;

	while ((navreader->state != S_ERROR)
	    && (navreader->state != S_FINISHED))
	{
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

	// If the loop halted with an error code
	if (navreader->err) {
		errmsg_prepend_location(navreader);
	}

	// Close the file.
	// If this causes an error, it hides any parsing errors.. TODO
	rrnx_fr_fclose(fr);
}

rrnx_list *rrnx_navr_release_nodelist(rrnx_navreader *navreader) {
	rrnx_list *nodelist = navreader->nodelist;
	navreader->nodelist = NULL;
	return nodelist;
}

