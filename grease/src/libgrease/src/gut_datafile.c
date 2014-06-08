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

#include "grease/gut_datafile.h"
#include "grease/gut_error.h"

// malloc, free
#include <stdlib.h>

// CONSTRUCTION & DESTRUCTION
//============================



// CONSTRUCTION & DESTRUCTION
//============================

extern gut_datafile *gut_datafile_create() {
	gut_datafile *df = NULL;

	int complete = 0;
	do {
		df = malloc(sizeof(gut_datafile));
		if (df == NULL) break;

		// Allocate default size buffer
		complete = gut_datafile_init(df);
		//complete = 1;
	} while(0);

	if (complete == 0) {
		// Incomplete; destroy partial
		gut_datafile_free(df);
		df = NULL;
	}

	return df;
}

extern void gut_datafile_free(gut_datafile *df) {
	gut_datafile_deinit(df);
	free(df);
}


extern int gut_datafile_init(gut_datafile *df) {
	int complete = 0;

	df->file = NULL;
	df->buffer = NULL;
	df->size = 0;
	df->next = 0;
	df->len = 0;
	df->err = GUT_E_OK;

	df->endianness = GUT_DATAFILE_DEFAULT_ENDIANNESS;

	// Attempt to allocate buffer
	do {
		int size = GUT_DATAFILE_DEFAULT_BUFFER_SIZE;

		df->buffer = malloc(size);
		if (df->buffer == NULL) {
			df->err = GUT_E_NOMEM;
			break;
		}

		// Otherwise buffer succesfully allocated
		df->size = size;

		// Initialization completed
		complete = 1;
	} while (0);

	if (!complete) {
		gut_datafile_deinit(df);
	}

	return complete;
}

extern void gut_datafile_deinit(gut_datafile *df) {
	if (df != NULL) {
		if (df->buffer != NULL) {
			free(df->buffer);
			df->buffer = NULL;
			df->size = 0;
		}
	}
}


// LOW-LEVEL READ/WRITE METHODS
//==============================

extern int gut_datafile_open(
    gut_datafile *df,
    const char *path,
    const char *mode
) {
	int rval;

	df->file = fopen(path, mode);

	// Determine rval
	if (df->file == NULL) {
		// Error
		df->err = GUT_E_SYSCALL;
		rval = -1;
	} else {
		rval = 0;
	}

	// Reset buffer state
	df->len = 0;
	df->next = 0;

	return rval;
}

extern int gut_datafile_close(gut_datafile *df) {
	int rval = 0;

	if (df->file != NULL) {
		int result;
		result = fclose(df->file);
		if (result != 0) {
			// Error
			df->err = GUT_E_SYSCALL;
			rval = -1;
		}
		df->file = NULL;
	}

	return rval;
}

extern int gut_datafile_error(gut_datafile *df) {
	return ferror(df->file);
}

extern int gut_datafile_eof(gut_datafile *df) {
	return feof(df->file);
}

extern int gut_datafile_set_buffer_size(gut_datafile *df, int size) {
	int rval = 0;
	void *ptr;

	ptr = realloc(df->buffer, size);
	if (ptr != NULL) {
		// Success
		df->buffer = ptr;
		df->size = size;
	} else {
		// Failure; original untouched.
		rval = -1;
	}

	return rval;
}

extern int gut_datafile_get_buffer_size(gut_datafile *df) {
	return df->size;
}

/**
 * Read data from file to buffer.
 */
extern int gut_datafile_readbuf(gut_datafile *df) {
	int rval = 0;

	// "On success, fread() and fwrite() return the number of
	//  items read or written. This number equals the number of
	//  bytes transferred only when size is 1."

	size_t bytes;
	bytes = fread(df->buffer, 1, df->size, df->file);

	// "If an error occurs, or the end of the file is reached,
	//  the return value is a short item count (or zero)."

	if ((bytes < df->size) && ferror(df->file)) {
		// error after transmitting some or zero bytes.
		df->err = GUT_E_SYSCALL;
		rval = -1;
	}
	else if ((bytes == 0) && feof(df->file)) {
		// end-of-file
		rval = -1;
	}
	else {
		// Read some bytes succesfully (assert bytes > 0).
		// Reset buffer state.
		df->len = bytes;
		df->next = 0;
	} // if-else

	return rval;
}

/**
 * Returns the next byte in file, or -1 when eof/error.
 */
extern int gut_datafile_read_byte(gut_datafile *df) {
	int rval = 0;

	// See if there's data in the buffer
	if (df->next == df->len) {
		// The buffer has been consumed.
		// Attempt to fill the buffer
		rval = gut_datafile_readbuf(df);
	}

	if (df->next < df->len) {
		// The buffer has data
		rval = ((unsigned int) df->buffer[df->next]) & 0xff;
		// Move on
		df->next++;
	}

	return rval;
}


extern int gut_datafile_read(gut_datafile *df, void *ptr, int size) {
	int rval = 0;
	char *data = ptr;

	for (int i = 0; i < size; i++) {
		int byteval = gut_datafile_read_byte(df);
		if (byteval == -1) {
			// error or unexpected eof
			if (feof(df->file)) {
				df->err = GUT_E_EOF;
			} else {
				df->err = GUT_E_SYSCALL;
			}
			rval = -1;
			break;
		}
		data[i] = (unsigned char)(byteval);
	}

	return rval;
}

extern int gut_datafile_read_r(gut_datafile *df, void *ptr, int size) {
	int rval = 0;
	char *data = ptr;

	for (int i = size-1; i >= 0; i--) {
		int byteval = gut_datafile_read_byte(df);
		if (byteval == -1) {
			// error or unexpected eof
			if (feof(df->file)) {
				df->err = GUT_E_EOF;
			} else {
				df->err = GUT_E_SYSCALL;
			}
			rval = -1;
			break;
		}
		data[i] = (unsigned char)(byteval);
	}

	return rval;
}

extern int gut_datafile_read_obj(gut_datafile *df, void *ptr, int size) {

	switch(df->endianness) {
	case GUT_DATAFILE_BIG_ENDIAN:
		return gut_datafile_read_r(df, ptr, size);
		break;
	default:
	case GUT_DATAFILE_LITTLE_ENDIAN:
		return gut_datafile_read(df, ptr, size);
	}

	return -1; // Never reached
}



extern int gut_datafile_writebuf(gut_datafile *df) {
	int rval = 0;

	// "On success, fread() and fwrite() return the number of
	//  items read or written. This number equals the number of
	//  bytes transferred only when size is 1."

	size_t bytes;
	bytes = fwrite(df->buffer, 1, df->len, df->file);

	// "If an error occurs, or the end of the file is reached,
	//  the return value is a short item count (or zero)."

	if (bytes < df->len) {
		// Either error or eof after transmitting some or zero bytes.
		df->err = GUT_E_SYSCALL;
		rval = -1;
	} else {
		// Wrote all bytes succesfully.
		// Reset buffer state.
		df->len = 0;
		df->next = 0;
	} // if-else

	return rval;
}


/**
 * Writes a byte into the file. Returns -1 when eof/error.
 */
extern int gut_datafile_write_byte(gut_datafile *df, unsigned int byteval) {
	int rval = 0;

	// See if there's space in the buffer.
	if (df->len == df->size) {
		// The buffer has been consumed.
		// Flush the buffer.
		rval = gut_datafile_writebuf(df);
	}

	if (df->len < df->size) {
		// The buffer has space
		df->buffer[df->len] = (unsigned char)(byteval & 0xff);
		// Move on
		df->len++;
	}

	return rval;
}

extern int gut_datafile_flush(gut_datafile *df) {
	int rval = 0;

	do {
		// Flush buffer
		rval = gut_datafile_writebuf(df);
		if (rval == -1) break;

		// Attempt fflush()
		if (fflush(df->file) != 0) {
			// Syscall failed
			df->err = GUT_E_SYSCALL;
			rval = -1;
			break;
		}

		rval = 0;
	} while(0);

	return rval;
}



extern int gut_datafile_write(gut_datafile *df, void *ptr, int size) {
	int rval = -1;
	char *data = ptr;

	do {
		for (int i = 0; i < size; i++) {
			int ok = gut_datafile_write_byte(df, data[i]);
			if (ok == -1) break; // error
		}

		rval = 0;
	} while(0);

	return rval;
}

extern int gut_datafile_write_r(gut_datafile *df, void *ptr, int size) {
	int rval = -1;
	char *data = ptr;

	do {
		for (int i = size-1; i >= 0; i++) {
			int ok = gut_datafile_write_byte(df, data[i]);
			if (ok == -1) break; // error
		}

		rval = 0;
	} while(0);

	return rval;
}

extern int gut_datafile_write_obj(gut_datafile *df, void *ptr, int size) {
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

extern char gut_datafile_read_char8(gut_datafile *df) {
	#if CHAR_MIN < 0
		// char is signed
		return (char) gut_datafile_read_int8(df);
	#else
		// char is unsigned
		return (char) gut_datafile_read_uint8(df);
	#endif
}


extern unsigned int gut_datafile_read_uint8(gut_datafile *df) {
	unsigned char data[1];

	gut_datafile_read_obj(df, data, 1);

	return (unsigned int) data[0];
}

extern unsigned int gut_datafile_read_uint16(gut_datafile *df) {
	unsigned char data[2];

	gut_datafile_read_obj(df, data, 2);

	unsigned int rval = 0;
	for (int i = 0; i < 2; i++) {
		rval |= ((unsigned int) data[i]) << (i*8);
	}

	return rval;
}

extern unsigned int gut_datafile_read_uint32(gut_datafile *df) {
	unsigned char data[4];

	gut_datafile_read_obj(df, data, 4);

	unsigned int rval = 0;
	for (int i = 0; i < 4; i++) {
		rval |= ((unsigned int) data[i]) << (i*8);
	}

	return rval;
}

extern unsigned long gut_datafile_read_ulong32(gut_datafile *df) {
	unsigned char data[4];

	gut_datafile_read_obj(df, data, 4);

	unsigned long rval = 0;
	for (int i = 0; i < 4; i++) {
		rval |= ((unsigned long) data[i]) << (i*8);
	}

	return rval;
}


extern int gut_datafile_read_int8(gut_datafile *df) {
	int rval = 0;
	unsigned int uval = gut_datafile_read_uint8(df);

	if (uval > 0x7f) {
		rval = -((int)(0xff - uval)) - 1;
	} else {
		rval = (int) uval;
	}

	return rval;
}

extern int gut_datafile_read_int16(gut_datafile *df) {
	int rval = 0;
	unsigned int uval = gut_datafile_read_uint16(df);

	if (uval > 0x7fff) {
		rval = -((int)(0xffff - uval)) - 1;
	} else {
		rval = (int) uval;
	}

	return rval;
}

extern int gut_datafile_read_int32(gut_datafile *df) {
	int rval = 0;
	unsigned int uval = gut_datafile_read_uint32(df);

	if (uval > 0x7fffffff) {
		rval = -((int)(0xffffffff - uval)) - 1;
	} else {
		rval = (int) uval;
	}

	return rval;
}

extern long gut_datafile_read_long32(gut_datafile *df) {
	long rval = 0;
	unsigned long uval = gut_datafile_read_uint32(df);

	if (uval > 0x7fffffff) {
		rval = -((long)(0xffffffff - uval)) - 1;
	} else {
		rval = (long) uval;
	}

	return rval;
}



extern float gut_datafile_read_float32(gut_datafile *df) {
	float rval;
	gut_datafile_read_obj(df, &rval, 4);
	return rval;
}

extern double gut_datafile_read_double64(gut_datafile *df) {
	double rval;
	gut_datafile_read_obj(df, &rval, 8);
	return rval;
}

extern long double gut_datafile_read_ldouble80(gut_datafile *df) {
	long double rval;
	gut_datafile_read_obj(df, &rval, 10);
	return rval;
}


// WRITE METHODS
//===============

extern void gut_datafile_write_uint16(gut_datafile *df, unsigned int val) {
	unsigned char data[2];

	for (int i = 0; i < 2; i++) {
		data[i] = (unsigned char) ((val >> (i*8)) & 0xff);
	}

	gut_datafile_write_obj(df, data, 2);
}

extern void gut_datafile_write_uint32(gut_datafile *df, unsigned int val) {
	unsigned char data[4];

	for (int i = 0; i < 4; i++) {
		data[i] = (unsigned char) ((val >> (i*8)) & 0xff);
	}

	gut_datafile_write_obj(df, data, 4);
}

extern void gut_datafile_write_ulong32(gut_datafile *df, unsigned long val) {
	unsigned char data[4];

	for (int i = 0; i < 4; i++) {
		data[i] = (unsigned char) ((val >> (i*8)) & 0xff);
	}

	gut_datafile_write_obj(df, data, 4);
}

extern void gut_datafile_write_int16(gut_datafile *df, int val) {
	if (val < 0) {
		gut_datafile_write_uint16(df, val);
	} else {
		gut_datafile_write_uint16(
		    df, 0xffff - (unsigned int) (-(val+1)) );
	}
}

extern void gut_datafile_write_int32(gut_datafile *df, int val) {
	if (val < 0) {
		gut_datafile_write_uint32(df, val);
	} else {
		gut_datafile_write_uint32(
		    df, 0xffffffff - (unsigned int) (-(val+1)) );
	}
}

extern void gut_datafile_write_long32(gut_datafile *df, long val) {
	if (val < 0) {
		gut_datafile_write_ulong32(df, val);
	} else {
		gut_datafile_write_uint32(
		    df, 0xffffffff - (unsigned long) (-(val+1)) );
	}
}




extern void gut_datafile_write_float32(gut_datafile *df, float val) {
	gut_datafile_write_obj(df, &val, 4);
}

extern void gut_datafile_write_double64(gut_datafile *df, double val) {
	gut_datafile_write_obj(df, &val, 8);
}

extern void gut_datafile_write_ldouble80(gut_datafile *df, long double val) {
	gut_datafile_write_obj(df, &val, 10);
}
