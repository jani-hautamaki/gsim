//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

#ifndef GUT_FILE_H
#define GUT_FILE_H

#include <stdio.h> // FILE

#include "gut_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Buffered libc FILE
 */
struct gut_file {
	/**
	 * Underlying file stream
	 */
	FILE *handle;

	/**
	 * Data buffer
	 */
	gut_buffer *buf;

        /**
         * Copy of errno after a failed syscall.
         */
        int sys_errno;

};

typedef struct gut_file gut_file;


// CONSTRUCTION & DESTRUCTION
//============================

gut_file *gut_file_create(unsigned int buf_size);
void gut_file_free(gut_file *f);

int gut_file_init(gut_file *f, unsigned int buf_size);
void gut_file_deinit(gut_file *f);

// OPEN/CLOSE METHODS
//====================

int gut_file_open(
    gut_file *f,
    const char *path,
    const char *mode
);
int gut_file_close(gut_file *f);

// OTHER METHODS
//===============

int gut_file_eof(gut_file *f);
int gut_file_error(gut_file *f);
int gut_file_errno(const gut_file *f);
const char *gut_file_strerror(const gut_file *f);

int gut_file_flush(gut_file *f);

// READ/WRITE METHODS
//====================

size_t gut_file_write1(gut_file *f, unsigned char val);
size_t gut_file_write(gut_file *f, const void *ptr, size_t len);

size_t gut_file_read1(gut_file *f, unsigned char *cptr);
size_t gut_file_read(gut_file *f, void *ptr, size_t len);







#ifdef __cplusplus
} // extern "C"
#endif

#endif

