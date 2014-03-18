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

    unsigned int at;
    unsigned int len;
    unsigned int buffer_size;

    unsigned int row;
    unsigned int col;

    int err;
    char *errmsg;
    unsigned int errmsg_size;
};

typedef struct rrnx_filereader rrnx_filereader;

//--- constants ------------------------------------------------------------//
#define RRNX_FR_DEFAULT_BUFFER_SIZE 0x10000
#define RRNX_FR_DEFAULT_ERRMSG_SIZE 0x200

#define RRNX_E_OK              0
#define RRNX_E_SYSCALL        -1
#define RRNX_E_ALREADY_OPEN   -2
#define RRNX_E_EOF            -3
#define RRNX_E_OVERFLOW       -4

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
