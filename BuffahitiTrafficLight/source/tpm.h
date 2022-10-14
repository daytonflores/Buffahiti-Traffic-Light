/**
 * \file    tpm.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for TPM (Timer PWM Module)
 */

#ifndef TPM_H_
#define TPM_H_

/**
 * \def		F_TPM_CLOCK
 * \brief
 * \detail
 */
#define F_TPM_CLOCK\
	(48000000)

/**
 * \def		F_TPM_OVFLW
 * \brief
 * \detail
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
