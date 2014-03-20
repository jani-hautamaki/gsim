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

#ifndef RRNX_FILEREADER_H
#define RRNX_FILEREADER_H

#include <stdio.h> // FILE
#include "rrnx_error.h" // TODO: rrnx_errmsg, rrnx_errno?

#ifdef __cplusplus
#define extern "C" {
#endif

/**
 * File reader
 */
struct rrnx_filereader {
	/**
	 * The file name that is being read.
	 */
	char *filename;

	/**
	 * Handle to the file stream
	 */
	FILE *fp;

	/**
	 * Read buffer
	 */
	char *buffer;

	/**
	 * Next read location within the valid buffer.
	 */
	 unsigned int at;

	/**
	 * Length of the valid buffer.
	 */
	unsigned int len;

	/**
	 * Size of the buffer.
	 */
	unsigned int buffer_size;

	/**
	 * Current row.
	 */
	unsigned int row;

	/**
	 * Current column.
	 */
	unsigned int col;

	/**
	 * Last operation result
	 */
	int err;

	/**
	 * Human-readable error message
	 */
	char *errmsg;

	/**
	 * Size of the error mesage buffer.
	 */
	unsigned int errmsg_size;
};

typedef struct rrnx_filereader rrnx_filereader;

//--- constants ------------------------------------------------------------//
#define RRNX_FR_DEFAULT_BUFFER_SIZE 0x10000

//--- methods --------------------------------------------------------------//

extern rrnx_filereader *rrnx_fr_alloc(void);
extern int rrnx_fr_free(rrnx_filereader *reader);

extern int rrnx_fr_fclose(rrnx_filereader *reader);
extern int rrnx_fr_fopen(rrnx_filereader *reader, const char *filename);
extern int rrnx_fr_buffer(rrnx_filereader *reader);


//--- methods --------------------------------------------------------------//
extern int rrnx_fr_readchar(rrnx_filereader *reader, char *dest);
extern int rrnx_fr_readline(rrnx_filereader *reader, char *dest, size_t maxlen);



#ifdef __cplusplus
} // extern "C"
#endif

#endif
