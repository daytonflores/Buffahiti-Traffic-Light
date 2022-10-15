/**
 * \file    systick.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for on-board SysTick
 */

#include <stdbool.h>
#include <stdint.h>
#include <systick.h>
#include "board.h"
#include "core_cm0plus.h"
#include "fsl_debug_console.h"

#include "bitops.h"
#include "fsm_trafficlight.h"
#include "led.h"
#include "systick.h"
#include "touch.h"

/**
 * \var		ticktime_t ticks_since_startup
 * \brief	Ticks since boot, where each tick is 1 ms
 */
volatile ticktime_t ticks_since_startup = 0;

void init_onboard_systick()
{
	/**
     * Set the SysTick interrupt priority (range 0 to 3, with 0 being highest priority)
     */
	NVIC_SetPriority(SysTick_IRQn, 3);

	ALT_CLOCK_LOAD(SEC_PER_GO);

	/**
     * Configure SysTick VAL register:
     * 	- Reset Value register since this is where the current SysTick timer value will be read from
     */
	SysTick->VAL = 0;

	/**
     * Configure SysTick CTRL register:
     * 	- Operate using processor reference clock
     * 	- Enable SysTick exception register
     */
	SysTick->CTRL =
		SysTick_CTRL_CLKSOURCE_EXT_Msk |
		SysTick_CTRL_TICKINT_Msk;
}

void SysTick_Handler()
{
	ticks_since_startup += (SysTick->LOAD + 1);
	transitioning = true;
	//PRINTF("Touch Value = %d\r\n", get_touch());
}

volatile ticktime_t now()
{
	return((ticks_since_startup + ((SysTick->LOAD + 1) - SysTick->VAL)) / (ALT_CLOCK_HZ / MS_PER_SEC));
}
