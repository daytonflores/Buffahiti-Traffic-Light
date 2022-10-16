/**
 * \file    systick.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Function definitions for on-board SysTick
 */

#include <stdbool.h>
#include <stdint.h>
#include <systick.h>
#include "board.h"
#include "core_cm0plus.h"
#include "fsl_debug_console.h"

/**
 * User-defined libraries
 */
#include "bitops.h"
#include "fsm_trafficlight.h"
#include "led.h"
#include "systick.h"
#include "touch.h"

/**
 * \var		ticktime_t ticks_since_startup
 * \brief	Ticks since boot, where each tick is 62.5 ms
 */
volatile ticktime_t ticks_since_startup = 0;

/**
 * \var		volatile ticktime_t ticks_spent_transitioning
 * \brief	Ticks spent transitioning to current state, where each tick is 62.5 ms
 */
volatile ticktime_t ticks_spent_transitioning = 0;

/**
 * \var		volatile ticktime_t ticks_spent_stable
 * \brief	Ticks spent in current state (not including transition period), where each tick is 62.5 ms
 */
volatile ticktime_t ticks_spent_stable = 0;

/**
 * \var		volatile bool tick
 * \brief	Flag controlled by SysTick timer
 */
volatile bool tick = false;

/**
 * \var		extern uint32_t prev_alt_clock_load;
 * \brief	Declared in fsm_trafficlight.c
 */
extern uint32_t prev_alt_clock_load;

void init_onboard_systick(void)
{
    /**
     * Configure the SysTick LOAD register:
     * 	- To generate interrupt every TICK_SEC
     */
	SysTick->LOAD = ((ALT_CLOCK_HZ - 1) * TICK_SEC);

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
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    /**
     * Raise flag that TICK_SEC time has passed
     */
	tick = true;
}

volatile uint32_t now(void)
{
    /**
     * Convert ticks to sec, and then sec to ms
     */
	return((ticks_since_startup * TICK_SEC * MS_PER_SEC));
}
