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

#include "grease/gut_file.h"

#include <errno.h>
#include <string.h>

// CONSTRUCTION & DESTRUCTION
//============================

gut_file *gut_file_create(unsigned int buf_size) {
	gut_file *f = NULL;
	int success = 0;
	do {
		f = malloc(sizeof(gut_file));
		if (f == NULL) break;

		success = gut_file_init(f, buf_size);
		if (!success) break;

		success = 1;
	} while(0);

	if (!success) {
		gut_file_free(f);
		f = NULL;
	}

	return f;
}

void gut_file_free(gut_file *f) {
	gut_file_deinit(f);
	free(f);
}


int gut_file_init(gut_file *f, unsigned int buf_size) {
	f->handle = NULL;
	f->buf = NULL;
	f->sys_errno = 0;

	int success = 0;
	do {
		f->buf = gut_buffer_create(buf_size);
		if (f->buf == NULL) break;

		success = 1;
	} while(0);

	if (!success) {
		gut_file_deinit(f);
	}

	return success;
}

void gut_file_deinit(gut_file *f) {
	if (f != NULL) {
		gut_buffer_free(f->buf);
		f->buf = NULL;

		if (f->handle != NULL) {
			fclose(f->handle);
			f->handle = NULL;
			// errno is not copied.
		}
	}
}

// INTERNAL METHODS
//==================

/**
 * Attempts to read as much as possible into the buffer.
 * Does not distinguish between eof and error.
 */
static int fill_buffer(gut_file *f) {
	gut_buffer *buf = f->buf; // For convenience

	size_t bytes;
	bytes = fread(buf->addr, 1, buf->size, f->handle);

	buf->index_write = bytes;
	buf->index_read = 0;
	buf->len = bytes;

	// man fread:
	// fread() does not distinguish between end-of-file and error,
	// and callers must use feof(3) and ferror(3) to determine
	// which occurred.
	int success = 0;
	if ((bytes < buf->size) && ferror(f->handle)) {
		// An error occurred. Some bytes may have been read.
		f->sys_errno = errno;
	}
	else if ((bytes == 0) && feof(f->handle)) {
		// At eof. No further bytes available.
	}
	else {
		// Some bytes were succesfully read.
		success = 1;
	}
	return success;
}

static int flush_buffer(gut_file *f) {
	gut_buffer *buf = f->buf; // for convenience
	size_t bytes;

	// Set buffer length to index_write
	// TODO: Get rid of this?!?
	buf->len = buf->index_write;

	if (buf->len > 0) {
		bytes = fwrite(buf->addr, 1, buf->len, f->handle);
	} else {
		bytes = 0;
	}

	int success = 0;
	if (bytes < buf->len) {
		// Not all bytes were written, should imply ferror.
		f->sys_errno = errno;
	} else {
		// Fully flushed, clear buf
		buf->index_write = 0;
		buf->index_read = 0;
		buf->len = 0;

		success = 1;
	}
	return success;
}


// OTHER METHODS
//===============

int gut_file_open(
    gut_file *f,
    const char *path,
    const char *mode
) {
	f->handle = fopen(path, mode);
	if (f->handle == NULL) {
		f->sys_errno = errno;
		return 0;
	}
	return 1;
}

int gut_file_close(gut_file *f) {
	int success = 0;

	if (f->handle != NULL) {
		success = fclose(f->handle) == 0;
		if (!success) {
			f->sys_errno = errno;
		}
		f->handle = NULL;
		success = 1;
	}
	return success;
}

int gut_file_flush(gut_file *f) {
	int success = 0;
	do {
		success = flush_buffer(f);
		if (!success) break;

		success = !fflush(f->handle);
		if (!success) break;

		success = 1;
	} while(0);
	return success;
}

int gut_file_eof(gut_file *f) {
	return feof(f->handle) && (gut_buffer_availr(f->buf) == 0);
}

int gut_file_error(gut_file *f) {
	return ferror(f->handle);
}

int gut_file_errno(const gut_file *f) {
	return f->sys_errno;
}

const char *gut_file_strerror(const gut_file *f) {
	return strerror(f->sys_errno);
}

// READ/WRITE METHODS
//====================

size_t gut_file_write1(gut_file *f, unsigned char val) {
	gut_buffer *buf = f->buf; // for convenience

	size_t written = 0;
	do {
		if (gut_buffer_availw(buf) == 0) {
			int success = flush_buffer(f);
			if (!success) break;
		}
 
		// Should have space for at least one byte now.
		written = gut_buffer_write1(buf, val);
	} while(0);

	return written;
}

size_t gut_file_write(gut_file *f, const void *ptr, size_t len) {
	gut_buffer *buf = f->buf;
	size_t written = 0;
	const unsigned char *cptr = ptr; // for convenience

	do {
		if (gut_buffer_availw(buf) == 0) {
			int success = flush_buffer(f);
			if (!success) break;
		}
		// Should have space for at least one byte now.

		written += gut_buffer_write(
		    buf,
		    &cptr[written],
		    len - written
		);

	} while (written < len);

	return written;
}

size_t gut_file_read1(gut_file *f, unsigned char *cptr) {
	gut_buffer *buf = f->buf; // for convenience
	size_t read = 0;
	do {
		if (gut_buffer_availr(buf) == 0) {
			int success = fill_buffer(f);
			if (!success) break; // eof or error
		}
		// Should contain at least one byte now

		read = gut_buffer_read1(buf, cptr);
	} while(0);

	return read;
}

size_t gut_file_read(gut_file *f, void *ptr, size_t len) {
	gut_buffer *buf = f->buf; // for convenience
	unsigned char *cptr = ptr; // for convenience
	size_t read = 0;

	do {
		if (gut_buffer_availr(buf) == 0) {
			int success = fill_buffer(f);
			if (!success) break; // eof or error
		}
		// Should contain at least one byte now

		read += gut_buffer_read(
		    buf,
		    &cptr[read],
		    len - read
		);

	} while (read < len);

	return read;
}
