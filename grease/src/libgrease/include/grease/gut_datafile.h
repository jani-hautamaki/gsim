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

#ifndef GUT_DATAFILE_H
#define GUT_DATAFILE_H

// FILE
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reading/Writing byte order for multi-byte items.
 */

#define GUT_DATAFILE_LITTLE_ENDIAN      1
#define GUT_DATAFILE_BIG_ENDIAN         2

/**
 * Buffering file for reading and/or writing.
 */
struct gut_datafile {

	/**
	 * Handle to the stream.
	 */
	FILE *file;

	/**
	 * Read/Write buffer.
	 */
	unsigned char *buffer;

	/**
	 * Size of the buffer.
	 */
	int size;

	/**
	 * Offset of the next byte to read.
	 */
	int next;

	/**
	 * Length of the data in the buffer.
	 */
	int len;

	/**
	 * Endianness of the multi-byte objects.
	 */
	int endianness;

	/**
	 * Error state
	 */
	int err;
};

typedef struct gut_datafile gut_datafile;

/**
 * Default buffer size for gut_datafile.
 */
#define GUT_DATAFILE_DEFAULT_BUFFER_SIZE        4096

/**
 * Default endianness for gut_datafile.
 */
#define GUT_DATAFILE_DEFAULT_ENDIANNESS         GUT_DATAFILE_LITTLE_ENDIAN



// CONSTRUCTION & DESTRUCTION
//============================

extern gut_datafile *gut_datafile_create();
extern void gut_datafile_free(gut_datafile *df);

extern int gut_datafile_init(gut_datafile *df);
extern void gut_datafile_deinit(gut_datafile *df);

// OTHER METHODS
//===============

extern int gut_datafile_open(
    gut_datafile *df, const char *fname, const char *mode);
extern int gut_datafile_close(gut_datafile *df);

extern int gut_datafile_flush(gut_datafile *df);

extern int gut_datafile_error(gut_datafile *df);
extern int gut_datafile_eof(gut_datafile *df);

extern int gut_datafile_set_buffer_size(gut_datafile *df, int size);
extern int gut_datafile_get_buffer_size(gut_datafile *df);

// LOW-LEVEL READ
//================

extern int gut_datafile_read_byte(gut_datafile *df);
extern int gut_datafile_read(gut_datafile *df, void *ptr, int size);
extern int gut_datafile_read_r(gut_datafile *df, void *ptr, int size);

// LOW-LEVEL WRITE
//=================

extern int gut_datafile_write_byte(gut_datafile *df, unsigned int byteval);
extern int gut_datafile_write(gut_datafile *df, void *ptr, int size);
extern int gut_datafile_write_r(gut_datafile *df, void *ptr, int size);

// READ METHODS
//==============

extern char gut_datafile_read_char8(gut_datafile *df);

extern int gut_datafile_read_int8(gut_datafile *df);
extern unsigned int gut_datafile_read_uint8(gut_datafile *df);

extern int gut_datafile_read_int16(gut_datafile *df);
extern unsigned int gut_datafile_read_uint16(gut_datafile *df);

// both unsafe
extern unsigned int gut_datafile_read_uint32(gut_datafile *df);
extern int gut_datafile_read_int32(gut_datafile *df);

extern long gut_datafile_read_long32(gut_datafile *df);
extern unsigned long gut_datafile_read_ulong32(gut_datafile *df);

// all unsafe
extern float gut_datafile_read_float32(gut_datafile *df);
extern double gut_datafile_read_double64(gut_datafile *df);
extern long double gut_datafile_read_ldouble80(gut_datafile *df);

// WRITE METHODS
//===============

extern void gut_datafile_write_int8(gut_datafile *df, int val);
extern void gut_datafile_write_uint8(gut_datafile *df, unsigned int val);

extern void gut_datafile_write_int16(gut_datafile *df, int val);
extern void gut_datafile_write_uint16(gut_datafile *df, unsigned int val);

// both unsafe
extern void gut_datafile_write_int32(gut_datafile *df, int val);
extern void gut_datafile_write_uint32(gut_datafile *df, unsigned int val);

extern void gut_datafile_write_long32(gut_datafile *df, long val);
extern void gut_datafile_write_ulong32(gut_datafile *df, unsigned long val);

// all unsafe
extern void gut_datafile_write_float32(gut_datafile *df, float val);
extern void gut_datafile_write_double64(gut_datafile *df, double val);
extern void gut_datafile_write_ldouble80(gut_datafile *df, long double val);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

