
#include "rrnx_labels.h"

#include <string.h> // strlen()
#include <stdio.h> // TODO: Remove this and printf()

// the label is called as a "record descriptor" in the spec.

extern int rrnx_substr(
    char *dest,
    const char *src,
    unsigned int offset,
    unsigned int len
) {
	// Translate src pointer by the offset
	while ((offset > 0) && (*src != '\0')) {
		offset--;
	}

	// In effect, if the source is too short,
	// the pointer is already at the terminator,
	// and the destination will be made an empty string.

	int i;
	for (i = 0; i < len; i++) {
		char c = src[i];
		if (c == '\0') {
			// Halt here
			break;
		}
		// Otherwise copy
		dest[i] = c;
	}

	// Terminate
	dest[i] = '\0';

	return i;
}

extern int rrnx_trim_trailing(char *s) {
	int j = strlen(s);
	while ((j > 0) && (s[j-1] == ' ')) {
		j--;
	}
	s[j] = '\0';

	return j;
}


extern int rrnx_enumerate_linetype(const char *line) {
	// Identified line type
	int type = RRNX_LBL_UNIDENTIFIED;

	if (line == NULL) {
		return type;
	}

	// Determine the line length
	int len = strlen(line);

	// See if it is long enough
	if (len < 60) {
		// Too short
		printf("Line length: %d is too short\n", len);
		return type;
	}

	// Pluck out the label
	char label[32];

	int j = 0;
	j = rrnx_substr(label, line+60, 0, 20);

/*
	int j = 0;
	for (int i = 60; i < 80; i++) {
		char c = line[i];
		if (c == '\0') {
			break;
		}
		label[j] = c;
		j++;
	} // for
*/
	// Trim trailing whitespaces
	while ((j > 0) && (label[j-1] == ' ')) {
		j--;
	}
	label[j] = '\0';

	// Label is now correctly picked.

	printf("label: <%s>\n", label);

	return type;
}

