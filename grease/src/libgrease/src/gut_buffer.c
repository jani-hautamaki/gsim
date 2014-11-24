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

#include <string.h> // memcpy, memmove

#include "grease/gut_buffer.h"

// CONSTRUCTION & DESTRUCTION
//============================

gut_buffer *gut_buffer_create(size_t size) {
	gut_buffer *buf;
	int success = 0;
	do {
		buf = malloc(sizeof(gut_buffer));
		if (buf == NULL) break;

		success = gut_buffer_init(buf, size);
		if (!success) break;

	} while(0);

	if (!success) {
		// Destroy partial, if any
		//free(buf);
		gut_buffer_free(buf);
		buf = NULL;
	}

	return buf;
}

void gut_buffer_free(gut_buffer *buf) {
	gut_buffer_deinit(buf);
	free(buf);
}


int gut_buffer_init(gut_buffer *buf, size_t size) {
	int success = 0;
	do {
		buf->addr = NULL;
		buf->size = 0;
		buf->len = 0;
		buf->index_read = 0;
		buf->index_write = 0;

		buf->addr = malloc(size);
		if (buf->addr == NULL) break;

		// Memblock was allocated, record size.
		buf->size = size;

		// Success
		success = 1;
	} while(0);

	if (!success) {
		gut_buffer_deinit(buf);
	}

	return success;
}

void gut_buffer_deinit(gut_buffer *buf) {
	if (buf != NULL) {
		free(buf->addr);
		buf->addr = NULL;
		buf->size = 0;
		buf->len = 0;
	}

}

// OTHER METHODS
//===============

/**
 * Returns non-zero if the write head is
 * at the end of the buffer.
 */
int gut_buffer_eofw(const gut_buffer *buf) {
	return buf->index_write == buf->size;
}

/**
 * Returns non-zero if the read head is at
 * the end of the valid buffer.
 */
int gut_buffer_eofr(const gut_buffer *buf) {
	return buf->index_write == buf->len;
}

void gut_buffer_seekr(gut_buffer *buf, int index_read) {
	buf->index_read = index_read;
}

void gut_buffer_seekw(gut_buffer *buf, int index_write) {
	buf->index_write = index_write;
}

int gut_buffer_tellr(const gut_buffer *buf) {
	return buf->index_read;
}

int gut_buffer_tellw(const gut_buffer *buf) {
	return buf->index_write;
}

/**
 * @return Number of bytes available for reading.
 */
int gut_buffer_availr(const gut_buffer *buf) {
	return buf->len - buf->index_read;
}

/**
 * @return Number of bytes available for writing.
 */
int gut_buffer_availw(const gut_buffer *buf) {
	return buf->size - buf->index_write;
}

void gut_buffer_setlen(gut_buffer *buf, int len) {
	buf->len = len;
}

void gut_buffer_limitr(gut_buffer *buf) {
	buf->len = buf->index_write;
}

/**
 * Reset both read and write head at the begining,
 * and set valid buffer length to zero.
 */
void gut_buffer_clear(gut_buffer *buf) {
	buf->index_write = 0;
	buf->index_read = 0;
	buf->len = 0;
}

void *gut_buffer_addr(gut_buffer *buf) {
	return buf->addr;
}

void *gut_buffer_addrr(gut_buffer *buf) {
	return &buf->addr[buf->index_read];
}

void *gut_buffer_addrw(gut_buffer *buf) {
	return &buf->addr[buf->index_write];
}

size_t gut_buffer_size(const gut_buffer *buf) {
	return buf->size;
}

// READ/WRITE METHODS
//====================

/**
 * @return Number of bytes written. On success 1, otherwise 0.
 */
size_t gut_buffer_write1(gut_buffer *buf, unsigned char val) {
	if (buf->index_write < buf->size) {
		buf->addr[buf->index_write] = val;
		buf->index_write++;
		return 1;
	}
	return 0;
}

/**
 * @return Number of bytes written. On success @c len, otherwise 0.
 */
size_t gut_buffer_writeall(gut_buffer *buf, const void *ptr, size_t len) {
	if (buf->index_write+len <= buf->size) {
		memcpy(&buf->addr[buf->index_write], ptr, len);
	} else {
		len = 0;
	}

	buf->index_write += len;

	return len;
}

/**
 * @return Number of bytes written, or 0 if no space left.
 */
size_t gut_buffer_write(gut_buffer *buf, const void *ptr, size_t len) {
	// Compute the space left in the buffer
	size_t max_len = buf->size - buf->index_write;

	// If not enough space left, saturate length
	if (len > max_len) {
		len = max_len;
	}

	memcpy(&buf->addr[buf->index_write], ptr, len);
	buf->index_write += len;

	return len;
}

/**
 * @return Number of bytes read. On success 1, otherwise 0.
 */
size_t gut_buffer_read1(gut_buffer *buf, unsigned char *val) {
	int success = 0;
	if (buf->index_read < buf->len) {
		*val = buf->addr[buf->index_read];
		buf->index_read++;
		success = 1;
	}
	return success;
}

/**
 * @return Number of bytes read. On success @c len, otherwise 0.
 */
size_t gut_buffer_readall(gut_buffer *buf, void *ptr, size_t len) {
	int success = 0;
	if (buf->index_read+len <= buf->len) {
		memcpy(ptr, &buf->addr[buf->index_read], len);
		buf->index_read += len;
		success = 1;
	}
	return success;
}

/**
 * @return Number of bytes read, or 0 if nothing to read.
 */
size_t gut_buffer_read(gut_buffer *buf, void *ptr, size_t len) {
	// Compute the bytes left in the buffer
	size_t max_len = buf->len - buf->index_read;

	// If not enough space left, saturate length
	if (len > max_len) {
		len = max_len;
	}

	memcpy(ptr, &buf->addr[buf->index_read], len);
	buf->index_read += len;

	return len;
}
