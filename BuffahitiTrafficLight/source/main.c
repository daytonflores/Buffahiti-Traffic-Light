/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file    main.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Application entry point
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include "bitops.h"
#include "fsm_trafficlight.h"
#include "led.h"
#include "systick.h"
#include "touch.h"

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

/**
 * \var		volatile int i
 * \brief	The amount of times SysTick overflow has occurred
 */
//volatile int i;

#ifdef DEBUG
int main(void)
{

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /**
     * Initialize all 3 on-board LEDs (red, green, blue)
     */
    init_onboard_leds();

    /**
     * Initialize on-board touch sensor
     */
    init_onboard_touch_sensor();

    /**
     * Initialize the global current and next states
     */
    init_fsm_trafficlight();

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Turn on appropriate on-board LEDs based on current state
     */
	set_onboard_leds();

	PRINTF("%07u ms: Entering main loop...\r\n", now());

    while(1) {

        /**
         * Set by SysTick_Handler
         */
        if(transitioning){

            /**
             * Set current state to next state, and set next state appropriately
             */
        	transition_state();

            /**
             * Turn on appropriate on-board LEDs based on current state
             */
        	set_onboard_leds();
        }
    }
    return 0;
}
#elif NDEBUG
int main(void)
{

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /**
     * Initialize all 3 on-board LEDs (red, green, blue)
     */
    init_onboard_leds();

    /**
     * Initialize on-board touch sensor
     */
    init_onboard_touch_sensor();

    /**
     * Initialize the global current and next states
     */
    init_fsm_trafficlight();

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Turn on appropriate on-board LEDs based on current state
     */
	set_onboard_leds();

    while(1) {

        /**
         * Set by SysTick_Handler
         */
        if(transitioning){

            /**
             * Set current state to next state, and set next state appropriately
             */
        	transition_state();

            /**
             * Turn on appropriate on-board LEDs based on current state
             */
        	set_onboard_leds();
        }
    }
    return 0;
}
#endif
