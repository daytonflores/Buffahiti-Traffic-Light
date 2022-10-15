/**
 * \file    tpm.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for TPM (Timer PWM Module)
 */

#ifndef TPM_H_
#define TPM_H_

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
 * \def		PWM_FREQ_HZ
 * \brief	The desired frequency of the PWM in Hz
 */
#define PWM_FREQ_HZ\
	(16)

/**
 * \def		MAX_TPM_MOD_VALUE
 * \brief	The max division factor for 16-bit TPM->MOD register
 */
#define MAX_TPM_MOD_VALUE\
	(65536)

/**
 * \def		F_TPM_CLOCK_HZ
 * \brief	The TPM input clock frequency in Hz
 */
#define F_TPM_CLOCK_HZ\
	(48000000)

/**
 * \def		STEP_SIZE
 * \brief	The amount to change output data during each TPM interrupt
 * \detail	Keep in mind the DAC_RESOLUTION along with the TPM MOD value when configuring STEP_SIZE
 */
#define STEP_SIZE\
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
 * \fn		void TPM0_IRQHandler
 * \param	N/A
 * \brief   The ISR for the TPM timer (i.e. runs each time the timer reaches 0).
 * \detail	FUNCTION NAME IS CASE SENSITIVE. Since it is weakly defined in
 * 			startup\startup_mkl25z4.c this definition will override
 */
void TPM0_IRQHandler(void);

#endif /* TPM_H_ */
