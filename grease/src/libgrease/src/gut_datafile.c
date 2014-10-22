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



/* DEPRECATED FILE */



#include "grease/gut_datafile.h"
#include "grease/gut_error.h"

// malloc, free
#include <stdlib.h>
// errno
#include <errno.h>

// CONSTRUCTION & DESTRUCTION
//============================



// CONSTRUCTION & DESTRUCTION
//============================

gut_datafile *gut_datafile_create(void) {
	gut_datafile *df = NULL;

	int success = 0;
	do {
		df = malloc(sizeof(gut_datafile));
		if (df == NULL) break;

		// Allocate default size buffer
		success = gut_datafile_init(df);
		//success = 1;
	} while(0);

	if (!success) {
		// Incomplete; destroy partial
		gut_datafile_free(df);
		df = NULL;
	}

	return df;
}

void gut_datafile_free(gut_datafile *df) {
	gut_datafile_deinit(df);
	free(df);
}


int gut_datafile_init(gut_datafile *df) {
	int success = 0;

	df->file = NULL;
	df->buffer = NULL;
	df->size = 0;
	df->next = 0;
	df->len = 0;
	df->sys_errno = 0;

	df->endianness = GUT_DATAFILE_DEFAULT_ENDIANNESS;

	// Attempt to allocate buffer
	do {
		int size = GUT_DATAFILE_DEFAULT_BUFFER_SIZE;

		df->buffer = malloc(size);
		if (df->buffer == NULL) break;

		// Otherwise buffer succesfully allocated
		df->size = size;

		// Initialization completed
		success = 1;
	} while (0);

	if (!success) {
		// Deinit partial
		gut_datafile_deinit(df);
	}

	return success;
}

void gut_datafile_deinit(gut_datafile *df) {
	if (df != NULL) {
		free(df->buffer);
		df->buffer = NULL;
		df->size = 0;
	}
}


// LOW-LEVEL READ/WRITE METHODS
//==============================

int gut_datafile_open(
    gut_datafile *df,
    const char *path,
    const char *mode
) {
	int success = 1;

	df->file = fopen(path, mode);

	// Check for errors
	if (df->file == NULL) {
		success = 0;
		df->sys_errno = errno;
	}

	// Reset buffer state
	df->len = 0;
	df->next = 0;

	return success;
}

int gut_datafile_close(gut_datafile *df) {
	int success = 1;

	if (df->file != NULL) {
		int result;
		result = fclose(df->file);
		if (result != 0) {
			// Error
			success = 0;
			df->sys_errno = errno;
		}
		df->file = NULL;
	}

	return success;
}

/**
 * Tests the error indicator for the underlying FILE stream,
 * returning nonzero if it is set.
 */
int gut_datafile_error(gut_datafile *df) {
	return ferror(df->file);
}

int gut_datafile_errno(const gut_datafile *df) {
	return df->sys_errno;
}

void gut_datafile_clear_errno(gut_datafile *df) {
	df->sys_errno = 0;
}

/**
 * Returns non-zero when the underlying FILE stream
 * has the end-of-file indicator set AND when the buffered
 * data, if any, has been consumed.
 */
int gut_datafile_eof(gut_datafile *df) {
	return feof(df->file) && (df->next == df->len);
}

/**
 * Returns non-zero if the underlying FILE stream is open.
 */
int gut_datafile_is_open(gut_datafile *df) {
	return df->file != NULL;
}

/**
 * Attempts to resize the buffer.
 * On success the buffer state is reset, and non-zero value is returned.
 * On failure the previous buffer is retained, and zero is returned.
 */
int gut_datafile_set_buffer_size(gut_datafile *df, int size) {
	int success = 1;
	void *ptr;

	ptr = realloc(df->buffer, size);

	if (ptr != NULL) {
		// Success; update
		df->buffer = ptr;
		df->size = size;
	} else {
		// Failure; retain
		success = 0;
	}

	return success;
}

/**
 * Return the buffer size.
 */
int gut_datafile_get_buffer_size(gut_datafile *df) {
	return df->size;
}

/**
 * Read data from file to buffer.
 * Returns non-zero if some data was succesfully read to the buffer.
 * Returns zero if a syscall error occurred or if nothing was read
 * because the end-of-file had been reached already.
 */
int gut_datafile_readbuf(gut_datafile *df) {
	int success = 1;

	// "On success, fread() and fwrite() return the number of
	//  items read or written. This number equals the number of
	//  bytes transferred only when size is 1."

	size_t bytes;
	bytes = fread(df->buffer, 1, df->size, df->file);

	// "If an error occurs, or the end of the file is reached,
	//  the return value is a short item count (or zero)."

	if ((bytes < df->size) && ferror(df->file)) {
		// Error after transmitting some or zero bytes.
		// Signal the error eagerly. The bytes read
		// are discarded.
		success = 0;
		df->sys_errno = errno;
	}
	else if ((bytes == 0) && feof(df->file)) {
		// End-of-file has been reached, and zero bytes read.
		success = 0;
		//df->eof = 1;
	}
	else {
		// Read some bytes succesfully (assert bytes > 0).
		// Reset buffer state.
		df->len = bytes;
		df->next = 0;
	} // if-else

	return success;
}

/**
 * Returns the next byte in file, or -1 when eof/error.
 */
int gut_datafile_read_byte(gut_datafile *df) {
	int rval = -1;

	// See if there's data in the buffer
	if (df->next == df->len) {
		// The buffer has been consumed.
		// Attempt to fill the buffer
		gut_datafile_readbuf(df);
	}

	if (df->next < df->len) {
		// The buffer has data
		rval = ((unsigned int) df->buffer[df->next]) & 0xff;
		// Move on
		df->next++;
	}

	return rval;
}

/**
 * Reads a block from the file into memory.
 *
 * Returns:
 *    1   on success
 *    0   on error (premature eof or syscall failure)
 *
 */
int gut_datafile_read(gut_datafile *df, void *ptr, int size) {
	int success = 1;
	char *data = ptr;

	for (int i = 0; i < size; i++) {
		int byteval = gut_datafile_read_byte(df);
		if (byteval == -1) {
			success = 0;
			break;
		}
		data[i] = (unsigned char)(byteval);
	}

	return success;
}

/**
 * Reads a block from the file into memory in reverse order.
 *
 * Returns:
 *    1  on success
 *    0  on error (premature eof or syscall failure)
 *
 */
int gut_datafile_read_r(gut_datafile *df, void *ptr, int size) {
	int success = 1;
	char *data = ptr;

	for (int i = size-1; i >= 0; i--) {
		int byteval = gut_datafile_read_byte(df);
		if (byteval == -1) {
			success = 0;
			break;
		}
		data[i] = (unsigned char)(byteval);
	}

	return success;
}

/**
 * Reads an object into memory according to endianness byte order.
 *
 * Returns:
 *    1   on success
 *    0   on error (premature eof or syscall failure)
 *
 */
int gut_datafile_read_obj(gut_datafile *df, void *ptr, int size) {
	switch(df->endianness) {
	case GUT_DATAFILE_BIG_ENDIAN:
		return gut_datafile_read_r(df, ptr, size);
		break;
	default:
	case GUT_DATAFILE_LITTLE_ENDIAN:
		return gut_datafile_read(df, ptr, size);
	}

	return 0; // Never reached
}

int gut_datafile_writebuf(gut_datafile *df) {
	int success = 1;

	// "On success, fread() and fwrite() return the number of
	//  items read or written. This number equals the number of
	//  bytes transferred only when size is 1."

	size_t bytes;
	if (df->len > 0) {
		bytes = fwrite(df->buffer, 1, df->len, df->file);
	} else {
		bytes = df->len;
	}

	// "If an error occurs, or the end of the file is reached,
	//  the return value is a short item count (or zero)."

	if (bytes < df->len) {
		// Either error or eof after transmitting some or zero bytes.
		success = 0;
		df->sys_errno = errno;
	} else {
		// Wrote all bytes succesfully.
		// Reset buffer state.
		df->len = 0;
		df->next = 0;
	} // if-else

	return success;
}


int gut_datafile_get_writeaddr(const gut_datafile *df) {
	return df->len;
}

void gut_datafile_set_writeaddr(gut_datafile *df, int offset) {
	df->len = offset;
}


/**
 * Writes a byte into the file. Returns -1 when eof/error.
 */
int gut_datafile_write_byte(gut_datafile *df, unsigned int byteval) {
	int success = 1;

	// See if there's space in the buffer.
	if (df->len == df->size) {
		// The buffer has been consumed.
		// Flush the buffer.
		success = gut_datafile_writebuf(df);
	}

	if (df->len < df->size) {
		// The buffer has space
		df->buffer[df->len] = (unsigned char)(byteval & 0xff);
		// Move on
		df->len++;
	}

	return success;
}

int gut_datafile_flush(gut_datafile *df) {
	int success;

	do {
		// Flush buffer
		success = gut_datafile_writebuf(df);
		if (!success) break;

		// Attempt fflush()
		if (fflush(df->file) != 0) {
			// Syscall failed
			success = 0;
			break;
		}

	} while(0);

	return success;
}

int gut_datafile_write(gut_datafile *df, void *ptr, int size) {
	int success = 1;
	char *data = ptr;

	for (int i = 0; i < size; i++) {
		success = gut_datafile_write_byte(df, data[i]);
		if (!success) break; // error
	}

	return success;
}

int gut_datafile_write_r(gut_datafile *df, void *ptr, int size) {
	int success = 1;
	char *data = ptr;

	for (int i = size-1; i >= 0; i++) {
		success = gut_datafile_write_byte(df, data[i]);
		if (!success) break; // error
	}

	return success;
}

int gut_datafile_write_obj(gut_datafile *df, void *ptr, int size) {
	switch(df->endianness) {
	case GUT_DATAFILE_BIG_ENDIAN:
		return gut_datafile_write_r(df, ptr, size);
	default:
	case GUT_DATAFILE_LITTLE_ENDIAN:
		return gut_datafile_write(df, ptr, size);
	}
	return -1; // Never reached
}



// READ METHODS
//==============

int gut_datafile_read_char8(gut_datafile *df, char *val) {
	int success;
	#if CHAR_MIN < 0
		// char is signed
		int temp;
		success = gut_datafile_read_int8(df, &temp);
		*val = temp;
	#else
		// char is unsigned
		unsigned int temp;
		success = gut_datafile_read_uint8(df, &temp);
		*val = temp;
	#endif

	return success;
}


int gut_datafile_read_uint8(gut_datafile *df, unsigned int *val) {
	unsigned char data[1];
	int success = gut_datafile_read_obj(df, data, 1);

	*val = data[0];

	return success;
}

int gut_datafile_read_uint16(gut_datafile *df, unsigned int *val) {
	unsigned char data[2];
	int success = gut_datafile_read_obj(df, data, 2);

	*val = 0;
	for (int i = 0; i < 2; i++) {
		*val |= ((unsigned int) data[i]) << (i*8);
	}

	return success;
}

int gut_datafile_read_uint32(gut_datafile *df, unsigned int *val) {
	unsigned char data[4];
	int success = gut_datafile_read_obj(df, data, 4);

	*val = 0;
	for (int i = 0; i < 4; i++) {
		*val |= ((unsigned int) data[i]) << (i*8);
	}

	return success;
}

int gut_datafile_read_ulong32(gut_datafile *df, unsigned long *val) {
	unsigned char data[4];
	int success = gut_datafile_read_obj(df, data, 4);

	*val = 0;
	for (int i = 0; i < 4; i++) {
		*val |= ((unsigned long) data[i]) << (i*8);
	}

	return success;
}

int gut_datafile_read_int8(gut_datafile *df, int *val) {
	unsigned int uval;
	int success = gut_datafile_read_uint8(df, &uval);

	if (uval > 0x7f) {
		*val = -((int)(0xff - uval)) - 1;
	} else {
		*val = (int) uval;
	}

	return success;
}

int gut_datafile_read_int16(gut_datafile *df, int *val) {
	unsigned int uval;
	int success = gut_datafile_read_uint16(df, &uval);

	if (uval > 0x7fff) {
		*val = -((int)(0xffff - uval)) - 1;
	} else {
		*val = (int) uval;
	}

	return success;
}

int gut_datafile_read_int32(gut_datafile *df, int *val) {
	unsigned int uval;
	int success = gut_datafile_read_uint32(df, &uval);

	if (uval > 0x7fffffff) {
		*val = -((int)(0xffffffff - uval)) - 1;
	} else {
		*val = (int) uval;
	}

	return success;
}

int gut_datafile_read_long32(gut_datafile *df, long *val) {
	unsigned long uval;
	int success = gut_datafile_read_ulong32(df, &uval);

	if (uval > 0x7fffffff) {
		*val = -((long)(0xffffffff - uval)) - 1;
	} else {
		*val = (long) uval;
	}

	return success;
}

int gut_datafile_read_float32(gut_datafile *df, float *val) {
	return gut_datafile_read_obj(df, val, 4);
}

int gut_datafile_read_double64(gut_datafile *df, double *val) {
	return gut_datafile_read_obj(df, val, 8);
}

int gut_datafile_read_ldouble80(gut_datafile *df, long double *val) {
	return gut_datafile_read_obj(df, val, 10);
}


// WRITE METHODS
//===============

int gut_datafile_write_uint16(gut_datafile *df, unsigned int val) {
	unsigned char data[2];

	for (int i = 0; i < 2; i++) {
		data[i] = (unsigned char) ((val >> (i*8)) & 0xff);
	}

	return gut_datafile_write_obj(df, data, 2);
}

int gut_datafile_write_uint32(gut_datafile *df, unsigned int val) {
	unsigned char data[4];

	for (int i = 0; i < 4; i++) {
		data[i] = (unsigned char) ((val >> (i*8)) & 0xff);
	}

	return gut_datafile_write_obj(df, data, 4);
}

int gut_datafile_write_ulong32(gut_datafile *df, unsigned long val) {
	unsigned char data[4];

	for (int i = 0; i < 4; i++) {
		data[i] = (unsigned char) ((val >> (i*8)) & 0xff);
	}

	return gut_datafile_write_obj(df, data, 4);
}

int gut_datafile_write_int16(gut_datafile *df, int val) {
	unsigned int uval;
	if (val > 0) {
		uval = val;
	} else {
		uval = 0xffff - (unsigned int)(-(val+1));
	}

	return gut_datafile_write_uint16(df, uval);
}

int gut_datafile_write_int32(gut_datafile *df, int val) {
	unsigned int uval;
	if (val > 0) {
		uval = val;
	} else {
		uval = 0xffffffff - (unsigned int)(-(val+1));
	}

	return gut_datafile_write_uint32(df, uval);
}

int gut_datafile_write_long32(gut_datafile *df, long val) {
	unsigned long uval;
	if (val > 0) {
		uval = val;
	} else {
		uval = 0xffffffff - (unsigned long)(-(val+1));
	}
	return gut_datafile_write_ulong32(df, uval);
}

int gut_datafile_write_float32(gut_datafile *df, float val) {
	return gut_datafile_write_obj(df, &val, 4);
}

int gut_datafile_write_double64(gut_datafile *df, double val) {
	return gut_datafile_write_obj(df, &val, 8);
}

int gut_datafile_write_ldouble80(gut_datafile *df, long double val) {
	return gut_datafile_write_obj(df, &val, 10);
}
