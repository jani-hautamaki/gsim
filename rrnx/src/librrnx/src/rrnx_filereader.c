//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
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

#include "rrnx/rrnx_filereader.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h> // strerror, strncpy
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

	// strerror_r(errnum, &reader->errmsg[len], reader->errmsg_size-len);
	// strerror_r is not available in C99.
	// Replacmeent for strerror_r:

	const char *msg = strerror(errnum);
	// Copy at most size-len-1 chars (may or may not include '\0')
	strncpy(&reader->errmsg[len], msg, reader->errmsg_size-len-1);
	// Append terminator to the end in any case.
	reader->errmsg[reader->errmsg_size-1] = '\0';

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

rrnx_filereader *rrnx_fr_alloc(void) {
	int failure = 1;

	// Allocate
	rrnx_filereader *reader
		= malloc(sizeof(rrnx_filereader));

	if (reader != NULL) do {
		int size; // auxiliary

		// Initialize all members first
		reader->filename = NULL;
		reader->fp = NULL;

		reader->buffer_size = 0;
		reader->buffer = NULL;

		reader->errmsg_size = 0;
		reader->errmsg = NULL;

		reader->at = 0;
		reader->len = 0;
		reader->row = 0;
		reader->col = 0;

		reader->err = RRNX_E_OK;

		// Make allocations

		size = RRNX_FR_DEFAULT_BUFFER_SIZE;
		reader->buffer = malloc(size);
		if (reader->buffer == NULL) {
			break; // malloc failed, abort
		}
		reader->buffer_size = size;

		size = RRNX_DEFAULT_ERRMSG_SIZE;
		reader->errmsg = malloc(size);
		if (reader->errmsg == NULL) {
			break; // malloc failed, abort
		}
		reader->errmsg_size = size;

		// Success.
		failure = 0;

	} while (0);

	if (failure) {
		// Alloc failed.
		rrnx_fr_free(reader);
		reader = NULL;
	}

	return reader;
}

void rrnx_fr_free(rrnx_filereader *reader) {
	if (reader == NULL) {
		// Already freed
		return;
	}

	// Close the file, if any (errors are ignored)
	rrnx_fr_fclose(reader);

	// Free buffer, and remove dangling ptr
	free(reader->buffer);
	reader->buffer = NULL;
	reader->buffer_size = 0;

	// Free errmsg buffer, and remove dangling ptr
	free(reader->errmsg);
	reader->errmsg = NULL;
	reader->errmsg_size = 0;

	// Finally, free the filereader itself
	free(reader);
}

int rrnx_fr_set_buffer_size(rrnx_filereader *reader, int size) {
	void *newptr = realloc(reader->buffer, size);

	if (newptr != NULL) {
		// Allocation succeeded.

		// Update buffer info
		reader->buffer = newptr;
		reader->buffer_size = size;
		// Reset head and valid length
		reader->len = 0;
		reader->at = 0;

		noerr(reader);
	} else {
		// Allocation failed
		errmsg_none(reader, RRNX_E_NOMEM);
	}

	return reader->err;
}

int rrnx_fr_set_filename(
    rrnx_filereader *reader,
    const char *filename
) {
	noerr(reader);

	if (reader->filename != NULL) {
		// Free previous
		free(reader->filename);
		reader->filename = NULL;
	}

	if (filename != NULL) {
		// Attempt allocation
		int len = strlen(filename);
		reader->filename = malloc(len);
		if (reader->filename != NULL) {
			// Memory allocation succeeded. Copy
			strncpy(reader->filename, filename, len);
		} else {
			// Memory allocation failed.
			errmsg_none(reader, RRNX_E_NOMEM);
		} // if-else
	}

	return reader->err;
}

static int fclose_silently(rrnx_filereader *reader) {
	// errno is never set to zero by
	// any system call or library function.
	int errnum = 0;

	if (reader->fp != NULL) {
		// Attempt fclose
		if (fclose(reader->fp) != 0) {
			// Record errno
			errnum = errno;
		}

		// Remove handle
		reader->fp = NULL;

		// Reset location
		reader->row = 0;
		reader->col = 0;
		reader->len = 0;
		reader->at = 0;
	} // if

	return errnum;
}

int rrnx_fr_fclose(rrnx_filereader *reader) {
	noerr(reader);

	if (reader->fp != NULL) {
		int errnum = fclose_silently(reader);

		if (errnum != 0) {
			errmsg_syscall(reader, errnum,
			    "%s: fclose() failed: ", reader->filename);
		} // if: error

		// Free file name, if any.
		rrnx_fr_set_filename(reader, NULL);
	} // if

	return reader->err;
} // rrnx_fr_fclose()

int rrnx_fr_fopen(rrnx_filereader *reader, const char *filename) {
	// Reset error
	noerr(reader);

	if (reader->fp != NULL) {
		// Already file open
		return errmsg(reader, RRNX_E_HASFILE,
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

	// File opened succesfully.

	// Reset locational info.
	reader->row = 0;
	reader->col = 0;
	// Reset valid buffer length and head position
	reader->len = 0;
	reader->at = 0;

	// Remember the file name
	rrnx_fr_set_filename(reader, filename);
	if (reader->err) {
		// Memory allocation failed. Abort.
		// Silently close the file
		fclose_silently(reader);
		// Error is propagated upwards.
	}

	return reader->err;
}

int rrnx_fr_bind(rrnx_filereader *reader, FILE *fp) {
	if (reader->fp != NULL) {
		// Already file open
		return errmsg(reader, RRNX_E_HASFILE,
		    "bind error: previous file (%s) is still open",
		    reader->filename);
	} // if: file already open

	// Binding
	reader->fp = fp;

	// Leave filename unset

	// Reset locational info
	reader->row = 0;
	reader->col = 0;

	// Reset head location and valid buffer length
	reader->at = 0;
	reader->len = 0;

	return noerr(reader);
}

int rrnx_fr_unbind(rrnx_filereader *reader) {
	if (reader->fp != NULL) {
                // Remove handle
                reader->fp = NULL;

                // Free file name, if any
		rrnx_fr_set_filename(reader, NULL);

                // Reset location
                reader->row = 0;
                reader->col = 0;

		// Reset head location and valid buffer length
                reader->at = 0;
                reader->len = 0;
	} else {
		// Already unbound, do nothing
	} // if-else

	return noerr(reader);
}

int rrnx_fr_buffer(rrnx_filereader *reader) {
	if (reader->fp == NULL) {
		// No open file
		return errmsg_none(reader, RRNX_E_NOFILE);
	}

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
			    "fread() failed: ");
			    //"%s:%d: fread() failed: ",
			    //reader->filename, reader->row);
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
int rrnx_fr_readchar(rrnx_filereader *reader, char *dest) {
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
int rrnx_fr_readline(
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
		    "Line too long (%d chars, while %d expected at most)",
		    reader->col, maxlen);
		    //"%s:%d: line too long (%d chars, while %d expected at most)",
		    //reader->filename, reader->row, reader->col, maxlen);
	}
	else {
		// No error, no overflow; success!
                noerr(reader);
	}

	return reader->err;
} // rrnx_fr_readline()

