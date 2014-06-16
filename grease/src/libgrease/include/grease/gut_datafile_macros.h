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

#ifndef GUT_DATAFILE_MACROS_H
#define GUT_DATAFILE_MACROS_H

/** Defines various convenience macros for reading and writing */

#ifdef __cplusplus
extern "C" {
#endif

// WRITING
//=========

#define write_bytes(df, buf, size) gut_datafile_write(df, buf, size)

#define write_float32(df, val)     gut_datafile_write_float32(df, val)
#define write_double64(df, val)    gut_datafile_write_double64(df, val)
#define write_ldouble80(df, val)   gut_datafile_write_ldouble80(df, val)

#define write_ulong32(df, val)     gut_datafile_write_ulong32(df, val)
#define write_long32(df, val)      gut_datafile_write_long32(df, val)

#define write_uint32(df, val)      gut_datafile_write_uint32(df, val)
#define write_int32(df, val)       gut_datafile_write_int32(df, val)

#define write_uint16(df, val)      gut_datafile_write_uint16(df, val)
#define write_int16(df, val)       gut_datafile_write_int16(df, val)

#define write_uint8(df, val)       gut_datafile_write_uint8(df, val)
#define write_int8(df, val)        gut_datafile_write_int8(df, val)

// READING
//=========

#define read_bytes(df, buf, size) gut_datafile_read(df, buf, size)

#define read_float32(df, val)     gut_datafile_read_float32(df, val)
#define read_double64(df, val)    gut_datafile_read_double64(df, val)
#define read_ldouble80(df, val)   gut_datafile_read_ldouble80(df, val)

#define read_ulong32(df, val)     gut_datafile_read_ulong32(df, val)
#define read_long32(df, val)      gut_datafile_read_long32(df, val)

#define read_uint32(df, val)      gut_datafile_read_uint32(df, val)
#define read_int32(df, val)       gut_datafile_read_int32(df, val)

#define read_uint16(df, val)      gut_datafile_read_uint16(df, val)
#define read_int16(df, val)       gut_datafile_read_int16(df, val)

#define read_uint8(df, val)       gut_datafile_read_uint8(df, val)
#define read_int8(df, val)        gut_datafile_read_int8(df, val)

#ifdef __cplusplus
} // extern "C"
#endif

#endif

