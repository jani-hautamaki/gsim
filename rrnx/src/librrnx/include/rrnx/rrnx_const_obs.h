//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
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

#ifndef RRNX_CONST_OBS_H
#define RRNX_CONST_OBS_H

#ifdef __cplusplus
extern "C" {
#endif


#define RRNX_BAND_L1                            0x0001
#define RRNX_BAND_L2                            0x0002
// ...
#define RRNX_BAND_L5                            0x0005
// TODO: Glonass G1, G2
// TODO: Galileo E2-L1-E1, E5a, E6, E6b, E5a+b
// TODO: SBAS, L1, L5
// TODO: GPS: L1, L2, L5


/**
 * These correspond to observation codes.
 */

/**
 * Pseudorange in meters (observation code 'C').
 * Measurement unit: meters.
 *
 * GPS: C/A, L2C;
 * Glonass: C/A;
 * Galileo: all.
 */
#define RRNX_OBS_PSEUDORANGE                    0x0001

/**
 * Pseudorange in meters (observation code 'P').
 *
 * GPS and Glonass: P code.
 */
#define RRNX_OBS_PSEUDORANGE_P                  0x0001

/**
 * Carrier phase in full cycles (observation code 'L').
 */
#define RRNX_OBS_PHASE                          0x0004

/**
 * Doppler frequency in Hz (observation code 'D').
 *
 */
#define RRNX_OBS_DOPPLER                        0x0002

/**
 * Raw signal strength or SNR (observation code 'S').
 * As given by the receiver for
 * the respective phase observations.
 * The measurement unit is, therefore, receiver-dependent.
 */
#define RRNX_OBS_RAW                            0x0008




#ifdef __cplusplus
} // extern "C"
#endif

#endif
