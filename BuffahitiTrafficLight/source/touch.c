/**
 * \file    touch.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Function definitions for on-board touch sensor
 */

#include <stdint.h>

#include "board.h"

/**
 * User-defined libraries
 */
#include "touch.h"

void init_onboard_touch_sensor(void)
{
	/**
	 * Enable clock to TSI module
	 */
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

	/**
	 * Configure TSI0 as:
	 * 	- Operate in non-noise mode
	 * 	- Reference oscillator charge and discharge value of 500 nA
	 * 	- Oscillator voltage rails set to default
	 * 	- Electrode oscillator charge and discharge value of 500 nA
	 * 	- Frequency clock divided by 1
	 * 	- Scan electrode 32 times
	 * 	- Enable the TSI module
	 * 	- Write 1 to clear the end of scan flag
	 */
	TSI0->GENCS = \
			TSI_GENCS_MODE(GENCS_MODE) |\
			TSI_GENCS_REFCHRG(GENCS_REFCHRG) |\
			TSI_GENCS_DVOLT(GENCS_DVOLT) |\
			TSI_GENCS_EXTCHRG(GENCS_EXTCHRG) |\
			TSI_GENCS_PS(GENCS_PS) |\
			TSI_GENCS_NSCN(GENCS_NSCN) |\
			TSI_GENCS_TSIEN_MASK |
			TSI_GENCS_EOSF_MASK;
}

uint32_t get_touch(void)
{
	/**
	 * Select TSI0 channel 10
	 */
	TSI0->DATA = TSI_DATA_TSICH(TSI0_CHANNEL_10);

	/**
	 * Software trigger to start scan
	 */
	TSI0->DATA |= TSI_DATA_SWTS_MASK;

	/**
	 * Wait for scan to complete 32 times
	 */
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));

	/**
	 * Clear the end-of-scan flag
	 */
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK;

	/**
	 * Now that scan has completed 32 times, return the raw data after subtracting TOUCH_OFFSET
	 */
	return (TOUCH_DATA - TOUCH_OFFSET);
}

bool touchpad_is_touched(void)
{
    /**
     * Check if touchpad has been touched
     */
	bool return_value = false;

	if(get_touch() > MIN_TOUCH){
		return_value = true;
	}

	return (return_value);
}
