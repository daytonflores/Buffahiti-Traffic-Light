/**
 * \file    led.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Function definitions for on-board LEDs
 */

#include "board.h"

/**
 * User-defined libraries
 */
#include "bitops.h"
#include "fsm_trafficlight.h"
#include "led.h"
#include "systick.h"
#include "tpm.h"

/**
 * \var		volatile uint8_t red_level_end
 * \brief	The red level current state is transitioning towards
 */
volatile uint8_t red_level_end;

/**
 * \var		volatile uint8_t green_level_end
 * \brief	The green level current state is transitioning towards
 */
volatile uint8_t green_level_end;

/**
 * \var		volatile uint8_t blue_level_end
 * \brief	The blue level current state is transitioning towards
 */
volatile uint8_t blue_level_end;

void init_onboard_leds(void)
{
	/**
     * Enable clock to Port B for red + green on-board LEDs
     * Enable clock to Port D for blue on-board LED
     */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK + SIM_SCGC5_PORTD_MASK;

    /**
     * Set PTB18 as GPIO for red on-board LED
     * Set PTB19 as GPIO for green on-board LED
     * Set PTD1 as GPIO for blue on-board LED
     *
     * The MUX selection in PCR is done with bits 10:8, where 001 is configuration as GPIO
     */
	PORTB->PCR[PORTB_RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PORTB_RED_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL_RED);
	PORTB->PCR[PORTB_GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PORTB_GREEN_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL_GREEN);
	PORTD->PCR[PORTD_BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PORTD_BLUE_LED_PIN] |= PORT_PCR_MUX(PCR_MUX_SEL_BLUE);

	/**
	 * Set PTB18 as output for red on-board LED
	 * Set PTB19 as output for green on-board LED
	 * Set PTD1 as output for blue on-board LED
	 */
    PTB->PDDR |= MASK(PORTB_RED_LED_PIN) | MASK(PORTB_GREEN_LED_PIN);
    PTD->PDDR |= MASK(PORTD_BLUE_LED_PIN);

    /**
     * Turn all on-board LEDs off. Note that on-board LEDs are active-low
     */
    RED_LED_OFF();
    GREEN_LED_OFF();
    BLUE_LED_OFF();
}

void set_onboard_leds(void)
{

    /**
     * Set all on-board LEDs to the current state's RGB levels. Note that on-board LEDs are active-low
     */
	TPM2->CONTROLS[RED_LED_TPM2_CHANNEL].CnV = current.red_level;
	TPM2->CONTROLS[GREEN_LED_TPM2_CHANNEL].CnV = current.green_level;
	TPM0->CONTROLS[BLUE_LED_TPM0_CHANNEL].CnV = current.blue_level;
}

void step_leds(void)
{

    /**
     * Step current state's RGB values closer to the expected RGB values of this state.
     * This function is invoked once per tick, yet the step needs to be recalculated each time
     * since we are dealing with integers and rounding could take us far from target RGB values.
     * If we were dealing with floats, then the steps could be calculated during transition_state
     * and this function would just increment the same steps per tick.
     */
	int8_t red_step = (red_level_end - current.red_level) / (uint8_t)(((SEC_PER_TRANSITION * TICK_HZ)) - ticks_spent_transitioning);
	int8_t green_step = (green_level_end - current.green_level) / (uint8_t)(((SEC_PER_TRANSITION * TICK_HZ)) - ticks_spent_transitioning);
	int8_t blue_step = (blue_level_end - current.blue_level) / (uint8_t)(((SEC_PER_TRANSITION * TICK_HZ)) - ticks_spent_transitioning);

	current.red_level += red_step;
	current.green_level += green_step;
	current.blue_level += blue_step;
}
