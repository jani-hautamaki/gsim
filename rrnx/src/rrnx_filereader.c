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

#include "rrnx_filereader.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h> // strdup
#include <stdarg.h>


//--- internal methods -----------------------------------------------------//

static int errmsg(
    rrnx_filereader *reader,
    int err,
    const char *format, ...
) {
	va_list ap;

	va_start(ap, format);
	vsnprintf(reader->errmsg, reader->errmsg_size, format, ap);
	va_end(ap);

	reader->err = err;
	return reader->err;
} // errmsg


static int errmsg_syscall(
    rrnx_filereader *reader,
    int errnum,
    const char *format, ...
) {
	unsigned int len = 0;

	va_list ap;
	va_start(ap, format);
	// custom message
	len += vsnprintf(
	    &reader->errmsg[len], reader->errmsg_size-len, format, ap);
	va_end(ap);

	// strerror
	strerror_r(errnum, &reader->errmsg[len], reader->errmsg_size-len);
	reader->err = RRNX_E_SYSCALL;
	return reader->err;
} // errmsg_syscall()


static int noerr(rrnx_filereader *reader) {
    reader->err = RRNX_E_OK;
    return reader->err;
} // noerr()


static int errmsg_none(rrnx_filereader *reader, int err) {
	reader->err = err;
	reader->errmsg[0] = '\0';
	return reader->err;
}

//--- external methods: basic primitives -----------------------------------//

extern rrnx_filereader *rrnx_fr_alloc(void) {
	// Allocate
	rrnx_filereader *reader
		= (rrnx_filereader *) malloc(sizeof(rrnx_filereader));

	// Initialize
	reader->filename = NULL;
	reader->fp = NULL;

	// Configure with default read buffer size
	reader->buffer_size = RRNX_FR_DEFAULT_BUFFER_SIZE;

	// Allocate the read buffer
	reader->buffer = (char *) malloc(reader->buffer_size);

	reader->at = 0;
	reader->len = 0;
	reader->row = 0;
	reader->col = 0;

	// Configure with default errmsg buffer size
	reader->errmsg_size = RRNX_FR_DEFAULT_ERRMSG_SIZE;
	// Allocate the errmsg buffer
	reader->errmsg = (char *) malloc(reader->errmsg_size);

	// Clear error status
	reader->err = RRNX_E_OK;

	return reader;
} // rrnx_fr_create()

/**
 * Delete filereader
 */
extern int rrnx_fr_free(rrnx_filereader *reader) {
	// Close the file
	rrnx_fr_fclose(reader);

	// Free buffer
	free(reader->buffer);
	// Remove dangling pointers
	reader->buffer = NULL;
	reader->buffer_size = 0;

	// Free errmsg buffer
	free(reader->errmsg);
	// Remove dangling pointers
	reader->errmsg = NULL;
	reader->errmsg_size = 0;

	// Finally, free the filereader itself
	free(reader);

	return RRNX_E_OK;
}

extern int rrnx_fr_fclose(rrnx_filereader *reader) {
	if (reader->fp != NULL) {
		// Attempt fclose
		if (fclose(reader->fp) != 0) {
			return errmsg_syscall(reader, errno,
			    "%s: fclose() failed: ", reader->filename);
		} // if: error

		// Closed succesfully.

		// Remove handle
		reader->fp = NULL;

		// Free file name
		free(reader->filename);
		reader->filename = NULL;

		// Reset location
		reader->row = 0;
		reader->col = 0;
		reader->len = 0;
		reader->at = 0;
	} // if

	return noerr(reader);
} // rrnx_fr_fclose()

extern int rrnx_fr_fopen(rrnx_filereader *reader, const char *filename) {

	if (reader->fp != NULL) {
		// Already file open
		return errmsg(reader, RRNX_E_ALREADY_OPEN,
		    "%s: cannot open, because previous file (%s) is still open",
		    filename, reader->filename);
	} // if: file already open

	// Attempt opening a stream for the file
	reader->fp = fopen(filename, "r");

	// Inspect result
	if (reader->fp == NULL) {
		// fopen failed
		return errmsg_syscall(reader, errno,
		    "%s: fopen() failed: ", filename);
	} // if: fopen failed

	// File opened succesfully

	// Remember the file name
	reader->filename = strdup(filename);

	// Reset locational info
	reader->row = 0;
	reader->col = 0;
	reader->len = 0;

	return noerr(reader);
}

extern int rrnx_fr_buffer(rrnx_filereader *reader) {

	// Attempt reading from the file
	reader->len = fread(
	    reader->buffer, 1, reader->buffer_size, reader->fp);

	// Reset the current head location
	reader->at = 0;

	// fread() returns zero when eof or error.
	if (reader->len == 0) {
		// Is it error or eof?

		if (ferror(reader->fp)) {
			// Error
			errmsg_syscall(reader, errno,
			    "%s:%d: fread() failed: ",
			    reader->filename, reader->row);
		} else {
			// Eof; nothing to buffer.
			errmsg_none(reader, RRNX_E_EOF);
		} // if-else
	} else {
		noerr(reader);
	} // if-else: error or eof

	return reader->err;
}

//--- external methods: higher level ---------------------------------------//

/**
 * Returns:
 * reader->err
 */
extern int rrnx_fr_readchar(rrnx_filereader *reader, char *dest) {
	// Buffer consumed?
	if (reader->at == reader->len) {
		// Fill buffer with new data. This updated reader->len.
		rrnx_fr_buffer(reader);

		if (reader->err) {
			// Buffering failed; error or eof.
			// Propagate error upwards.
			return reader->err;
		}
	}

	// Otherwise consume a char from the buffer.
	char c = reader->buffer[reader->at];
	reader->at++;

	// Store result
	*dest = c;

	// Update textual location?

	return noerr(reader);
}

/**
 * Possible errors:
 *     RRNX_E_OK        - No error; line was succesfully read
 *     RRNX_E_EOF       - End-of-file
 *     RRNX_E_OVERFLOW  - Current line was too long for the buffer
 *     RRNX_E_SYSCALL   - fread() failed during buffering
 */
extern int rrnx_fr_readline(
    rrnx_filereader *reader,
    char *dest,
    size_t maxlen
) {
	char c; // current char
	unsigned int i; // writing position
	unsigned int overflow = 0;

	maxlen--; // account for the '\0' terminator which is always added
	i = 0;

	// Advance textual location
	reader->row++;
	reader->col = 0;

	do {
		rrnx_fr_readchar(reader, &c);

		if ((reader->err == RRNX_E_EOF) && (i > 0)) {
			// Eof reached with data.
			// Reinterpret as end-of-line, and reset error.
			noerr(reader);
			break;
		} else if (reader->err) {
			// Error. Propagate upwards.
			break;
		}

		if (c == '\n') {
			// Newline. Stop here
			break;
		}
		else if (c == '\r') {
			// Carriage return; ignore.
		}
		else {
			// Advance textual location within the row
			reader->col++;

			// Append char if there's space left
			if (i < maxlen) {
				dest[i] = c;
				i++;
			} else {
				// Mark overflow, but continue reading.
				overflow = 1;
			}
		}
	} while (1);

	// Finish destination string, always.
	dest[i] = '\0';

	// Examine the state
	// error, eof, overflow or ok?
	if (reader->err) {
		// Has error
	}
	else if (overflow) {
		// No error, but overflow
		errmsg(reader, RRNX_E_OVERFLOW,
		    "%s:%d: line too long (%d chars, while %d expected at most)",
		    reader->filename, reader->row, reader->col, maxlen);
	}
	else {
		// No error, no overflow; success!
                noerr(reader);
	}

	return reader->err;
} // rrnx_fr_readline()

