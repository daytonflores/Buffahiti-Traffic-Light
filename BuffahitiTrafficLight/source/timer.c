/**
 * \file    timer.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for on-board SysTick
 */

#include "board.h"
#include "core_cm0plus.h"
#include "fsl_debug_console.h"

#include "bitops.h"
#include "led.h"
#include "timer.h"
#include "touch.h"

void init_systick()
{
	/**
     * Configure SysTick LOAD register:
     * 	- To divide input frequency by factor of N, store (N - 1) in Reload register
     */
	SysTick->LOAD = (ALT_CLOCK_HZ - 1);

	/**
     * Set the SysTick interrupt priority (range 0 to 3, with 0 being highest priority
     */
	NVIC_SetPriority(SysTick_IRQn, 3);

	/**
     * Configure SysTick VAL register:
     * 	- Reset Value register since this is where the current SysTick timer value will be read from
     */
	SysTick->VAL = 0;

	/**
     * Configure SysTick CTRL register:
     * 	- Operate using external reference clock
     * 	- Enable SysTick exception request
     * 	- Enable the SysTick counter
     */
	SysTick->CTRL =
		SysTick_CTRL_CLKSOURCE_EXT_Msk |
		SysTick_CTRL_TICKINT_Msk |
		SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler()
{

	RED_LED_TOGGLE();
	PRINTF("Touch Value = %d\r\n", get_touch());
}
