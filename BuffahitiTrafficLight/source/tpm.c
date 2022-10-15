/**
 * \file    tpm.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for TPM (Timer PWM Module)
 */

#include <stdbool.h>
#include <stdint.h>
#include "board.h"
#include "fsl_debug_console.h"

#include "bitops.h"
#include "led.h"
#include "tpm.h"

/**
 * \var		uint8_t tpm_sc_ps;
 * \brief	Holds x for 2^x, where 2^x is the tpm_prescaler
 */
uint8_t tpm_sc_ps;

/**
 * \var		uint16_t pwm_period;
 * \brief	The calculated PWM period in Hz to allow for largest possible TPM->MOD value
 */
uint16_t pwm_period;

/**
 * \var		volatile uint16_t tpm_cnv;
 * \brief	The proportional width of the current pulse
 */
volatile uint16_t tpm_cnv = 0;

/**
 * \var		volatile bool brightening;
 * \brief	True if the TPM is currently brightening the LED
 */
volatile bool brightening = true;

void init_onboard_tpm(void)
{
	/**
	 * Enable clock to TPM module
	 */
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	/**
     * Configure SOPT2:
     * 	- To use MCGFLLCLK and MCGPLLCLK/2 as clock source
     * 	- To select FLL clock from above
     */
	SIM->SOPT2 |=
		SIM_SOPT2_TPMSRC(TPM_CLOCK_SRC) |
		SIM_SOPT2_PLLFLLSEL_MASK;

	/**
     * Calculate the smallest needed prescaler to allow for largest possible TPM->MOD value
     * and thus more granular control
     */
	if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 1){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 1);
		tpm_sc_ps = 0;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 2){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 2);
		tpm_sc_ps = 1;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 4){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 4);
		tpm_sc_ps = 2;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 8){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 8);
		tpm_sc_ps = 3;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 16){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 16);
		tpm_sc_ps = 4;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 32){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 32);
		tpm_sc_ps = 5;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 64){
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 64);
		tpm_sc_ps = 6;
	}
	else{
		pwm_period = (uint16_t)((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / 128);
		tpm_sc_ps = 7;
	}

	/**
     * Load the TPM->MOD register. We will be able to set the PWM to one of TPM->MOD levels
     */
	TPM0->MOD = pwm_period - 1;

	/**
     * Configure the TPM SC register:
     * 	- Count up with divide by tpm_prescaler
     * 	- Enable ISR
     */
	TPM0->SC =
		TPM_SC_PS(tpm_sc_ps);// |
		//TPM_SC_TOIE_MASK;

	/**
     * Configure the TPM CONF register:
     * 	- Continue counting operation in debug mode
     */
	TPM0->CONF |= TPM_CONF_DBGMODE(TPM_DBGMODE);

	/**
     * Configure TPM CnSC:
     * 	- Edge-aligned PWM
     * 	- Low-true pulses (set output on match, clear output on reload)
     */
	TPM0->CONTROLS[1].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;

	/**
     * Set initial duty cycle
     */
	TPM0->CONTROLS[1].CnV = 0;

	/**
     * Set the TPM interrupt priority (range 0 to 3, with 0 being highest priority)
     */
	NVIC_SetPriority(TPM0_IRQn, 2);
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);

	/**
     * Configure the TPM SC register:
     * 	- Start TPM
     */
	TPM0->SC |= TPM_SC_CMOD(1);
}

void TPM0_IRQHandler(void)
{
	//PRINTF("tpm_cnv = %u\r\n", tpm_cnv);

	volatile int32_t delay;

	if(brightening){
		if((int)tpm_cnv < pwm_period){
			tpm_cnv += STEP_SIZE;
		}
		else{
			brightening = false;
		}
	}
	else{
		if((int)tpm_cnv > 0){
			tpm_cnv -= STEP_SIZE;
		}
		else{
			brightening = true;
		}
	}

	for(delay = 0; delay < 100; delay++);

	TPM0->CONTROLS[1].CnV = tpm_cnv;
}
