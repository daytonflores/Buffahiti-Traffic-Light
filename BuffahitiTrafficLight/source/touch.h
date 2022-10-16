/**
 * \file    touch.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Macros and function headers for on-board touch sensor
 */

#ifndef TOUCH_H_
#define TOUCH_H_

/**
 * \def		GENCS_MODE
 * \brief	Configuration for TSI operation mode. 4 modes available
 * \detail
 * 		0: Non-noise mode
 * 		1: Noise threshold detection mode
 * 		2: Noise threshold detection mode
 * 		3: Automatic noise detection mode
 */
#define GENCS_MODE\
	(0UL)

/**
 * \def		GENCS_REFCHRG
 * \brief	Configuration for reference oscillator charge/discharge. 8 values available
 * \detail
 *  	0: 500 nA
 *  	1: 1 uA
 *  	2: 2 uA
 *  	3: 4 uA
 *  	4: 8 uA
 *  	5: 16 uA
 *  	6: 32 uA
 *  	7: 64 uA
 */
#define GENCS_REFCHRG\
	(0UL)

/**
 * \def		GENCS_DVOLT
 * \brief 	Configuration for oscillator voltage rails. 4 levels available
 * \detail
 * 	0:
 * 	1:
 * 	2:
 * 	3:
 */
#define GENCS_DVOLT\
	(0UL)

/**
 * \def		GENCS_EXTCHRG
 * \brief	Configuration for electrode oscillator charge/discharge. 8 values available from 500 nA to 64 uA
 * \detail
 *  	0: 500 nA
 *  	1: 1 uA
 *  	2: 2 uA
 *  	3: 4 uA
 *  	4: 8 uA
 *  	5: 16 uA
 *  	6: 32 uA
 *  	7: 64 uA
 */
#define GENCS_EXTCHRG\
	(0UL)

/**
 * \def		GENCS_PS
 * \brief	Configuration for clock divisor value. 8 values available
 * \detail
 *  	0: 1
 *  	1: 2
 *  	2: 4
 *  	3: 8
 *  	4: 16
 *  	5: 32
 *  	6: 64
 *  	7: 128
 */
#define GENCS_PS\
	(0UL)

/**
 * \def		GENCS_NSCN
 * \brief	Configuration for electrode oscillator count used in making scan. Value can be any
 * 			value between 1 and 32, inclusive
 */
#define GENCS_NSCN\
	(31UL)

/**
 * \def		TSI0_CHANNEL_10
 * \brief	TSI0 channel 10
 */
#define TSI0_CHANNEL_10\
	(10UL)

/**
 * \def		TOUCH_OFFSET
 * \brief	The amount to offset each scanned value by. Calculated by having 0 offset and then
 * 			printing the scanned value when touch sensor is not being touched
 */
#define TOUCH_OFFSET\
	(650)

/**
 * \def		TOUCH_DATA
 * \brief	Grab the 16-bit scanned value data from the TSI0 Data register
 */
#define TOUCH_DATA\
	(TSI0->DATA & 0xFFFF)

/**
 * \def		MIN_TOUCH
 * \brief	Any (scanned_value - TOUCH_OFFSET) greater than 100 will be considered a touch
 */
#define MIN_TOUCH\
	(100)

/**
 * \fn		void init_onboard_touch_sensor
 * \brief	Initialize capacitive touch sensor
 * \param	N/A
 * \return 	N/A
 * \detail 	Many operations were referenced from Alexander G Dean's TSI project on GitHub
 * 			(https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense)
 *			TSI: 	Touch Sensing Interface refers to interfacing with the on-board capacitive touch pad
 *			SIM:	System Integration Module is a peripheral containing many control registers, including SCGC5
 *			SCGC5:	System Clock Gating Control Register 5 is a register containing different controls,
 * 					including clock gating for TSI
 *			TSI0:	TSI peripheral 0 (there are 16 channels for each TSI peripheral, though this board
 *					only has 1 on-board TSI peripheral)
 *			GENCS:	General Control and Status Register for TSI module (where each channel 0-15 has
 *					its own GENCS register)
 *			TSIEN:	GENCS configuration for enabling/disabling TSI module. 0 to disable, 1 to enable
 *			EOSF:	GENCS configuration for end-of-scan flag. 0 means scan incomplete, 1 means scan complete.
 *					To clear this flag, write 1 to it
 */
void init_onboard_touch_sensor(void);

/**
 * \fn		uint32_t get_touch
 * \brief	Start scanning the touch sensor and return the relative touch value
 * \param	N/A
 * \return	The raw scanned value minus touch offset
 * \detail	Many operations were referenced from Alexander G Dean's TSI project on GitHub
 * 			(https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense)
 */
uint32_t get_touch(void);

/**
 * \fn		bool touchpad_is_touched
 * \brief	Will calculate the current touch value and determine whether touchpad is being touched
 * \param	N/A
 * \return	Returns true if the touch sensor detects a touch
 */
bool touchpad_is_touched(void);

#endif /* TOUCH_H_ */
