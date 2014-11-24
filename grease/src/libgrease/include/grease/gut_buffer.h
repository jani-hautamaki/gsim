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

#ifndef GUT_BUFFER_H
#define GUT_BUFFER_H

#include <stdlib.h> // size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Memory buffer with separate read and write heads.
 */
struct gut_buffer {
	/**
	 * Pointer to the allocated memory block
	 */
	unsigned char *addr;

	/**
	 * Size of the allocated memory block,
	 * casted to an unsigned int for simplicity.
	 */
	unsigned int size;

	/**
	 * Length of valid data in the memory block.
	 * Limits the read head.
	 */
	unsigned int len;

	/**
	 * Next write address
	 */
	unsigned int index_write;

	/**
	 * Next read address
	 */
	unsigned int index_read;
};

typedef struct gut_buffer gut_buffer;

// CONSTRUCTION & DESTRUCTION
//============================

gut_buffer *gut_buffer_create(size_t size);
void gut_buffer_free(gut_buffer *buf);

int gut_buffer_init(gut_buffer *buf, size_t size);
void gut_buffer_deinit(gut_buffer *buf);

// OTHER METHODS
//===============

//int gut_buffer_eofw(const gut_buffer *buf);
//int gut_buffer_eofr(const gut_buffer *buf);

void gut_buffer_seekr(gut_buffer *buf, int index_read);
void gut_buffer_seekw(gut_buffer *buf, int index_write);

int gut_buffer_tellr(const gut_buffer *buf);
int gut_buffer_tellw(const gut_buffer *buf);

int gut_buffer_availr(const gut_buffer *buf);
int gut_buffer_availw(const gut_buffer *buf);

void gut_buffer_setlen(gut_buffer *buf, int len);
void gut_buffer_limitr(gut_buffer *buf);

void gut_buffer_clear(gut_buffer *buf);

void *gut_buffer_addr(gut_buffer *buf);
void *gut_buffer_addrr(gut_buffer *buf);
void *gut_buffer_addrw(gut_buffer *buf);
size_t gut_buffer_size(const gut_buffer *buf);


// READ/WRITE METHODS
//====================

size_t gut_buffer_write1(gut_buffer *buf, unsigned char val);
size_t gut_buffer_writeall(gut_buffer *buf, const void *ptr, size_t len);
size_t gut_buffer_write(gut_buffer *buf, const void *ptr, size_t len);

size_t gut_buffer_read1(gut_buffer *buf, unsigned char *val);
size_t gut_buffer_readall(gut_buffer *buf, void *ptr, size_t len);
size_t gut_buffer_read(gut_buffer *buf, void *ptr, size_t len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

