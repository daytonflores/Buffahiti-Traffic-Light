/**
 * \file    touch.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for on-board touch sensor
 */

#include <stdint.h>
#include "board.h"
#include "fsl_debug_console.h"

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
	 * 	- Enable the TSI interrupt
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
			TSI_GENCS_TSIIEN_MASK |
			TSI_GENCS_EOSF_MASK;

	TSI0->TSHD = \
		(708 << 16);
	/**
     * Set the TSI interrupt priority (range 0 to 3, with 0 being highest priority)
     */
	NVIC_SetPriority(TSI0_IRQn, 1);
	NVIC_ClearPendingIRQ(TSI0_IRQn);
	NVIC_EnableIRQ(TSI0_IRQn);

	PRINTF("TOUCH = %u\r\n", get_touch());
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

void TSI0_IRQHandler(void)
{
	get_touch();
	PRINTF("TOUCH\r\n");
}
