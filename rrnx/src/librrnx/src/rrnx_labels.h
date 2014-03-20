#ifndef RRNX_LABELS_H
#define RRNX_LABELS_H

#ifdef __cplusplus
extern "C" {
#endif

// Pseudo type
#define RRNX_LBL_UNIDENTIFIED 0

// Common to both OBS and NAV
#define RRNX_LBL_HEADER 1
#define RRNX_LBL_CREATION_INFO 2
#define RRNX_LBL_COMMENT 3

// NAV specific
#define RRNX_LBL_ION_ALPHA 4
#define RRNX_LBL_ION_BETA 5
#define RRNX_LBL_DELTA_UTC 6
#define RRNX_LBL_LEAP_SECONDS 7

// OBS specific

extern int rrnx_enumerate_linetype(const char *line);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
