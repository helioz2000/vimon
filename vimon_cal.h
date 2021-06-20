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
#define V2_OFFSET 9995.2		// mV
#define V2_MV_PER_MV 2.957879792	// mV

// Temperature measurement CH 2
// Resistance calibration
#define PT_OHM_PER_MV 0.034791252485
#define PT_OFFSET_OHM 86.2		// Ohm

// Ohm to PT100 temp conversion
#define PT_REFERENCE_OHM 100.0	// Ohm (100 for PT100)
#define PT_SLOPE 0.003851		// PT slope factor
#define PT_OFFSET_TEMP -1.8		// Compensation for low quality PT100

// Current measurement Details
// INA180A1 provides voltage gain of 20, between input terminals and ADC
// example: shunt 50mV/500A, 50mV * 20 = 1000mV @ ADC
//          500000mA/1000mV = 500mA/mV @ ADC
// simplified: for a 50mV/xxA shunt the value is xx mV/mV @ ADC
//             for a 100mV/xxA shunt the value is xx/2 mV/mA @ ADC

// Current measurement CH 3
#define I1_MA_PER_MV 50		// mA/mV @ADC

// Current measurement CH 4
#define I2_MA_PER_MV 50		// mA/mV @ADC

#endif	// VIMON_CAL_H
