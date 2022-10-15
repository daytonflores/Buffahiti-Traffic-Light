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

/**
 * \var		extern volatile int i
 * \brief	Declared in main.c
 */
//extern volatile int i;

/**
 * \var		extern uint32_t prev_alt_clock_load;
 * \brief	Declared in fsm_trafficlight.c
 */
extern uint32_t prev_alt_clock_load;

void init_onboard_systick()
{
    /**
     * Configure the SysTick LOAD register:
     * 	- To the expected LOAD for initial STOP state
     */
	ALT_CLOCK_LOAD(SEC_PER_GO);

	/**
     * Set the SysTick interrupt priority (range 0 to 3, with 0 being highest priority)
     */
	NVIC_SetPriority(SysTick_IRQn, 3);

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

    /**
     * Modify SysTick->CTRL register to enable the counter
     */
	ENABLE_SYSTICK_COUNTER();
}

void SysTick_Handler()
{
	//i++;
	ticks_since_startup += prev_alt_clock_load;
	transitioning = true;
	//PRINTF("Touch Value = %d\r\n", get_touch());
}

volatile ticktime_t now()
{
	return((ticks_since_startup + ((SysTick->LOAD + 1) - SysTick->VAL)) / (ALT_CLOCK_HZ / MS_PER_SEC));
}
