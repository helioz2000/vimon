/**
 * @file vimon_cal.h
 *
 * Calibration values for VI-monitor board
 *
 * Note:	CH2 can be configured (in hardware) for either Voltage
 *			or Temperature (PT100) measurement.
 *
 *			CH3 & CH4 can be combined (in hardware) to measure
 *			bidirectional current (e.g. battery charge/discharge
 */

#ifndef VIMON_CAL_H
#define VIMON_CAL_H

#include "ADS1115.h"

#define BOARD_ADDRESS ADS1115_ADDRESS_ADDR_SDA

// Voltage measurement CH 1
#define V1_OFFSET 9977.0		// mV
#define V1_MV_PER_MV 2.92368682	// mV

// Voltage measurement CH 2 (not used)
#define V2_OFFSET 10000.0		// mV
#define V2_MV_PER_MV 2.9296857	// mV

// Temperature measurement CH 2
#define PT_OHM_PER_MV 0.034791252485
#define PT_OFFSET_OHM 86.2		// Ohm
#define PT_REFERENCE_OHM 100.0	// Ohm (100 for PT100)
#define PT_SLOPE 0.003851		// PT slope factor

// Current measurement CH 3
#define I1_MA_PER_MV 50		// mA

// Current measurement CH 4
#define I2_MA_PER_MV 50		// mA

#endif	// VIMON_CAL_H
