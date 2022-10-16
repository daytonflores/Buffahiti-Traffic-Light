/**
 * \file    tpm.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for TPM (Timer PWM Module)
 */

#ifndef TPM_H_
#define TPM_H_

/**
 * \def		PWM_FREQ_HZ
 * \brief	The desired frequency of the PWM in Hz
 */
#define PWM_FREQ_HZ\
	(500)

/**
 * \def		MAX_TPM_MOD_VALUE
 * \brief	The max division factor for 16-bit TPM->MOD register
 */
#define MAX_TPM_MOD_VALUE\
	(65536)

/**
 * \def		TPM_CLOCK_SRC
 * \brief	Configuration for TPM clock source select
 * \detail
 * 		0: Disabled
 * 		1: MCGFLLCLK (or MCGPLLCLK / 2)
 * 		2: OSCERCLK
 * 		3: MCGIRCLK
 */
#define TPM_CLOCK_SRC\
	(1)

/**
 * \def		TPM_DBGMODE
 * \brief	Configuration for TPM debug mode
 * \detail
 * 		0: LPTPM counter does not increment during debug. Trigger inputs and input capture events
 * 		   are also ignored
 * 		3: LPTPM counter continues to increment in debug mode
 */
#define TPM_DBGMODE\
	(3)

/**
 * \def		F_TPM_CLOCK_HZ
 * \brief	The frequency of TPM clock in Hz
 */
#define F_TPM_CLOCK_HZ\
	(48000000)

/**
 * \def		F_TPM_OVFLW
 * \brief
 */
#define F_TPM_OVFLW\
	(100000)

/**
 * \def		F_TPM_PRESCALER
 * \brief	Configuration for hardware circuit that divides down the input signal's frequency by some factor
 * \detail	Can be 1, 2, 4, 8, 16, 32, 64, 128
 */
#define F_TPM_PRESCALER\
	(32)

/**
 * \def		STEP_SIZE
 * \brief	The amount to change output data during each TPM interrupt
 * \detail	Keep in mind the DAC_RESOLUTION along with the TPM MOD value when configuring STEP_SIZE
 */
#define STEP_SIZE\
	(16)

/**
 * \def		RED_LED_TPM2_CHANNEL
 * \brief	Red on-board LED is tied to TPM2 channel 0
 */
#define RED_LED_TPM2_CHANNEL\
	(0)

/**
 * \def		GREEN_LED_TPM2_CHANNEL
 * \brief	Green on-board LED is tied to TPM2 channel 1
 */
#define GREEN_LED_TPM2_CHANNEL\
	(1)

/**
 * \def		BLUE_LED_TPM0_CHANNEL
 * \brief	Blue on-board LED is tied to TPM0 channel 1
 */
#define BLUE_LED_TPM0_CHANNEL\
	(1)

/**
 * \def		DAC_RESOLUTION
 * \brief
 * \detail
 */
#define DAC_RESOLUTION\
	(4096)

/**
 * \fn		void init_onboard_tpm
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the on-board timer PWM
 */
void init_onboard_tpm(void);

/**
 * \fn		void set_pwm_period
 * \param	N/A
 * \return	N/A
 * \brief   Set the PWM period and the TPM->SC prescaler
 */
void set_pwm_period(void);

#endif /* TPM_H_ */
