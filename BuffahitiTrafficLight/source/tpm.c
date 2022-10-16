/**
 * \file    tpm.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for TPM (Timer PWM Module)
 */

#include "board.h"
#include "fsl_debug_console.h"

#include "bitops.h"
#include "fsm_trafficlight.h"
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

void init_onboard_tpm(void)
{
	/**
	 * Enable clock to TPM module
	 */
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;

	/**
     * Configure SOPT2:
     * 	- To use MCGFLLCLK and MCGPLLCLK/2 as clock source
     * 	- To select FLL clock from above
     */
	SIM->SOPT2 |=
		SIM_SOPT2_TPMSRC(TPM_CLOCK_SRC) |
		SIM_SOPT2_PLLFLLSEL_MASK;

	/**
     * Set the smallest needed prescaler along with PWM period for desired PWM frequency
     */
	set_pwm_period();

	/**
     * Load the TPM MOD register with prescaler
     */
	TPM0->MOD = 255;
	TPM2->MOD = 255;

	/**
     * Configure the TPM SC register:
     * 	- Count up with divide by tpm_prescaler
     */
	TPM0->SC = TPM_SC_PS(tpm_sc_ps);
	TPM2->SC = TPM_SC_PS(tpm_sc_ps);

	/**
     * Configure the TPM CONF register:
     * 	- Continue counting operation in debug mode
     */
	TPM0->CONF |= TPM_CONF_DBGMODE(TPM_DBGMODE);
	TPM2->CONF |= TPM_CONF_DBGMODE(TPM_DBGMODE);

	/**
     * Configure TPM CnSC:
     * 	- Edge-aligned PWM
     * 	- Low-true pulses (set output on match, clear output on reload)
     */
	TPM0->CONTROLS[1].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[0].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[1].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;

	/**
     * Set initial duty cycle
     */
	TPM0->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[0].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;

	/**
     * Configure the TPM SC register:
     * 	- Start TPM
     */
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
}

void set_pwm_period(void)
{
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
}
