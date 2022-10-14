/**
 * \file    tpm.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for TPM (Timer PWM Module)
 */

#include "board.h"
#include "fsl_debug_console.h"

#include "bitops.h"
#include "led.h"
#include "tpm.h"

void init_onboard_tpm(void)
{
	/**
	 * Enable clock to TPM module
	 */
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	/**
     * Set clock source for TPM module
     */
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	/**
     * Load the TPM MOD register with prescaler
     */
	TPM0->MOD = (F_TPM_CLOCK / (F_TPM_OVFLW * F_TPM_PRESCALER)) - 1;

	/**
     * Load the TPM SC register:
     * 	- Divide by prescaler
     * 	- Enable counting
     * 	- Enable interrupts
     */
	TPM0->SC =
		TPM_SC_CMOD(1) |
		TPM_SC_PS(5) |
		TPM_SC_TOIE_MASK;

	/**
     * Set the TPM interrupt priority (range 0 to 3, with 0 being highest priority)
     */
	NVIC_SetPriority(TPM0_IRQn, 2);
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);
}

void TPM0_IRQHandler(void)
{
	static int change = STEP_SIZE;
	static uint16_t out_data = 0;

	/**
     * Entering ISR so turn on blue LED
     */
	BLUE_LED_ON();

	/**
     * Reset the overflow flag
     */
	TPM0->SC |= TPM_SC_TOIE_MASK;

	out_data += change;

	if(out_data < STEP_SIZE){
		change = STEP_SIZE;
	}
	else if(out_data >= DAC_RESOLUTION - STEP_SIZE){
		change = -STEP_SIZE;
	}

	DAC0->DAT[0].DATH = DAC_DATH_DATA1(out_data >> 8);
	DAC0->DAT[0].DATL = DAC_DATL_DATA0(out_data);

	PRINTF("TPM\r\n");

	/**
     * Leaving ISR so turn off blue LED
     */
	BLUE_LED_OFF();
}
