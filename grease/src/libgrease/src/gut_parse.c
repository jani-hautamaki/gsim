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

#include "grease/gut_parse.h"
#include "grease/gut_error.h"

// strtol
#include <stdlib.h>
// errno
#include <errno.h>
// isspace
#include <ctype.h>

// Ranges of integer types.
// http://www.gnu.org/software/libc/manual/html_mono/libc.html#Range-of-Type
#include <limits.h>

/**
 * Parse base-10 string to long by using @c strtol().
 *
 * @param[in] s
 *     The input number as a string in base-10.
 * @param[out] result
 *     The result of the conversion
 *
 * @return
 *     Error code.
 *
 * @par
 * The string is allowed to leading and trailing white spaces
 * as determined by @c isspace() function. However, completely
 * empty strings are disallowed.
 *
 * @par
 * If the input number is too big (either negative or positive),
 * the error code @c GUT_E_RANGE is returned.
 * In addition, the output parameter @c *result has received either
 * @c LONG_MIN if underflow occurred or @c LONG_MAX if overflow occurred.
 *
 * @par
 * If the input string is empty (zero length or contains
 * only white spaces or contains an invalid character)
 * the error code @c GUT_E_SYNTAX is returned.
 *
 * @par
 * The underlying @c strtol() function may return
 * some unanticipated @c errno value, in which case
 * the error code @c GUT_E_OTHER is returned.
 *
 */
int gut_parse_long(const char *s, long *result) {
	char *endptr;
	long val;

	// Using strtol as proposed by the man page.
	errno = 0;
	val = strtol(s, &endptr, 10);

	// "If an underflow occurs, strtol() returns LONG_MIN.
	//  If an overflow  occurs, strtol() returns LONG_MAX.
	//  In both cases, errno is set to ERANGE."
	if (errno == ERANGE) {
		// Error, value out of range.

		// Assign the result in order to let the caller
		// differentiate between underflow and overflow.
		*result = val;

		return GUT_E_RANGE;
	}
	else if (errno != 0) {
		// Error, other reason.
		// TBD: What are the possible errno values?

		return GUT_E_OTHER;
	}

	if (endptr == s) {
		// Error, no digits were found

		return GUT_E_SYNTAX;
	}

	// According to "man strtol",
	// "The string may begin with an arbitrary amount
	//  of white space (as determined by isspace(3))".

	// Consequently, this routine allows that
	//  the string may end with an arbitrary amount
	//  of white space (as determined by isspace).

	// Trim trailing white spaces, if any
	while ((*endptr != '\0') && isspace(*endptr)) endptr++;

	// Verify that first invalid character is the string terminator.
	if (*endptr != '\0') {
		// Error, string contains an invalid character.
		return GUT_E_SYNTAX;
	}

	// The string is valid and its conversion was succesful.
	// Store the value to the output parameter,
	// and return successfully.
	*result = val;
	return GUT_E_OK;
}

/**
 * Parse base-10 string to unsigned long by using @c strtoul().
 *
 * @param[in] s
 *     The input number as a string in base-10.
 * @param[out] result
 *     The result of the conversion
 *
 * @return
 *     Error code.
 *
 * @par
 * The string is allowed to leading and trailing white spaces
 * as determined by @c isspace() function. However, completely
 * empty strings are disallowed.
 *
 * @par
 * The input number is NOT allowed to have a leading minus sign.
 * Even though the underlying @c strtoul() accepts negative numbers,
 * it is impossible to determine by inspecting the output variables
 * whether the input number had a leading minus sign.
 *
 * @par
 * If the input number is too big, the error code @c GUT_E_RANGE
 * is returned, and @c *result is set to @c ULONG_MAX.
 * If there is a leading minus sign, the error code @c GUT_E_RANGE
 * is returned, and @c *result is set to @c 0.
 *
 * @par
 * If the input string is empty (zero length or contains
 * only white spaces or contains an invalid character)
 * the error code @c GUT_E_SYNTAX is returned.
 *
 * @par
 * The underlying @c strtoul() function may return
 * some unanticipated @c errno value, in which case
 * the error code @c GUT_E_OTHER is returned.
 *
 */
int gut_parse_ulong(const char *s, unsigned long *result) {
	char *endptr;
	unsigned long val;

	// Note, in the case of a negative input number strtoul()
	// behaves unexpectedly. According to gnu libc documentation:
	//
	// "If string depicts a negative number, strtoul acts the same as strtol
	//  but casts the result to an unsigned integer. That means for example that
	//  strtoul on "-1" returns ULONG_MAX and an input more negative
	//  than LONG_MIN returns (ULONG_MAX + 1) / 2."
	//
	// Consequently, this method verifies first that the input number
	// is not a negative number. The "correct" way to do it, would be
	// to use strtol(), but due to efficiency reasons, a *hack* is used
	// instead.

	// According to "man strtoul"
	// "The string may begin with an arbitrary amount
	//  of white space(as determined by isspace(3))
	//  followed by a single optional '+' or '-' sign."

	// Skip the leading white space.
	while ((*s != '\0') && isspace(*s)) s++;

	if (*s == '-') {
		// Negative number.
		// The input number may still be
		// in the range if it is zero.
		// However, leading minus are not accepted.
		*result = 0;
		return GUT_E_RANGE;
	}

	// Using strtoul as proposed by the man page.
	errno = 0;
	val = strtoul(s, &endptr, 10);

	// In the event of overflow, strtoul() returns ULONG_MAX,
	// and sets errno to ERANGE.
	if (errno == ERANGE) {
		// Error, value out of range.

		*result = val;
		return GUT_E_RANGE;
	}
	else if (errno != 0) {
		// Error, other reason.
		// TBD: What are the possible errno values?

		return GUT_E_OTHER;
	}

	if (endptr == s) {
		// Error, no digits were found

		return GUT_E_SYNTAX;
	}

	// Trim trailing white spaces, if any
	while ((*endptr != '\0') && isspace(*endptr)) endptr++;

	// Verify that first invalid character is the string terminator.
	if (*endptr != '\0') {
		// Error, string contains an invalid character.
		return GUT_E_SYNTAX;
	}

	// The string is valid and its conversion was succesful.
	// Store the value to the output parameter,
	// and return successfully.
	*result = val;
	return GUT_E_OK;
}


/**
 * Parse string to int.
 *
 * Works similarly to gut_parse_long().
 */
int gut_parse_int(const char *s, int *result) {
	long val;
	int err;

	err = gut_parse_long(s, &val);

	if ((err != GUT_E_OK) && (err != GUT_E_RANGE)) {
		// Error, non-range related.
		// Pass the error to the caller.
		return err;
	}

	// Verify that the narrowing conversion
	// does not cause an overflow or underflow
	// by checking the data type limits.

	if (((err == GUT_E_RANGE) && (val == LONG_MIN))
	    || (val < ((long) INT_MIN)))
	{
		// Error, conversion underflows.
		*result = INT_MIN;
		return GUT_E_RANGE;
	}
	if (((err == GUT_E_RANGE) && (val == LONG_MAX))
	    || (val > ((long) INT_MAX)))
	{
		// Error, conversion overflows.
		*result = INT_MAX;
		return GUT_E_RANGE;
	}

	// Narrowing conversion will succeed. Do it.
	*result = (int) val;

	return GUT_E_OK;
}

/**
 * Parse string to unsigned int.
 *
 * Works similarly to gut_parse_ulong()
 */
int gut_parse_uint(const char *s, unsigned int *result) {
	unsigned long val;
	int err;

	// In many systems sizeof(long) equals to sizeof(int),
	// Consequently, "long" may not be able to hold
	// values of "unsigned int".
	// However, "unsigned long" is guaranteed to be
	// at least as wide as "unsigned int".
	// Therefore, gut_parse_ulong() is used here.

	err = gut_parse_ulong(s, &val);

	if ((err != GUT_E_OK) && (err != GUT_E_RANGE)) {
		// Error, non-range related.
		// Pass the error to the caller.
		return err;
	}

	// Verify that the narrowing conversion
	// does not cause an overflow or underflow
	// by checking the data type limits.


	if ((err == GUT_E_RANGE) && (val == 0)) {
		// Error, conversion underflows.
		*result = 0;
		return GUT_E_RANGE;
	}
	if (((err == GUT_E_RANGE) && (val == ULONG_MAX))
	    || (val > ((unsigned long) UINT_MAX)))
	{
		// Error, conversion overflows.
		*result = UINT_MAX;
		return GUT_E_RANGE;
	}

	// Narrowing conversion will succeed. Do it.
	*result = (unsigned int) val;

	return GUT_E_OK;
}


/**
 * Parse string into double using @c strtod().
 *
 * Accepts both decimal and hexadecimal numbers
 * Accepts also infinity and NAN.
 *
 * TBD: Should the ERANGE cases be converted into
 * DBL_MAX and DBL_MIN?
 */
int gut_parse_double(const char *s, double *result) {
	char *endptr;
	double val;

	// Using strtod as proposed by the man page.
	errno = 0;
	val = strtod(s, &endptr);

	// "If the correct value would cause overflow,
	//  plus or minus HUGE_VAL is returned
	//  (according to the sign of the value),
	//  and ERANGE is stored in errno."
	//
	// "If the correct value would cause underflow,
	// zero is returned and ERANGE is stored in errno.

	if (errno == ERANGE) {
		// Error, value out of range.

		// Assign the result in order to let the caller
		// differentiate between underflow and overflow.
		*result = val;

		return GUT_E_RANGE;
	}
	else if (errno != 0) {
		// Error, other reason.
		// TBD: Cannot happen?

		return GUT_E_OTHER;
	}

	if (endptr == s) {
		// Error, no digits were found

		return GUT_E_SYNTAX;
	}

	// According to "man strtol",
	// "The string may begin with an arbitrary amount
	//  of white space (as determined by isspace(3))".

	// Consequently, this routine allows that
	//  the string may end with an arbitrary amount
	//  of white space (as determined by isspace).

	// Trim trailing white spaces, if any
	while ((*endptr != '\0') && isspace(*endptr)) endptr++;

	// Verify that the first invalid character
	// is the string terminator.
	if (*endptr != '\0') {
		// Error, string contains an invalid character.
		return GUT_E_SYNTAX;
	}


	// The string is valid and its conversion was succesful.
	// Store the value to the output parameter,
	// and return successfully.
	*result = val;
	return GUT_E_OK;

}
