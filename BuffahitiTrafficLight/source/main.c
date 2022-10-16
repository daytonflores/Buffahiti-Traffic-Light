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
 * \date	10/16/2022
 * \brief   Application entry point
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/**
 * User-defined libraries
 */
#include "bitops.h"
#include "fsm_trafficlight.h"
#include "led.h"
#include "systick.h"
#include "touch.h"
#include "tpm.h"


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
     * Initialize TPM on-board module
     */
    init_onboard_tpm();

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Turn on appropriate on-board LEDs based on current state
     */
	set_onboard_leds();

	PRINTF("%07u ms: Entering main loop...\r\n", now());
	PRINTF("%07u ms: Initialized to %s. Staying for %u sec...\r\n", now(), mode_to_string(current.mode), mode_state_sec(current.mode));

    /**
     * Main infinite loop
     */
    while(1) {

        /**
         * Set by SysTick_Handler every TICK_SEC
         */
        if(tick){

            /**
             * Reset flag that was set by SysTick ISR
             */
        	tick = false;

            /**
             * Increment for timestamp purposes
             */
        	ticks_since_startup++;

            /**
             * Increment to track stable or transitioning periods
             */
        	if(transitioning){
        		ticks_spent_transitioning++;
        	}
        	else{
        		ticks_spent_stable++;

                /**
                 * Increment to track CROSSWALK blink periods
                 */
            	if(current.mode == CROSSWALK){
            		if(crosswalk_on){
                		ticks_spent_crosswalk_on++;
            		}
            		else{
                		ticks_spent_crosswalk_off++;
            		}
            	}
        	}



            /**
             * First check if touchpad is touched and current state is not CROSSWALK.
             * Then flag the button press, reset any ticks counted during previous state
             * from when button was pushed, and flag the need to transition to CROSSWALK
             */
        	if(current.mode != CROSSWALK && touchpad_is_touched()){

        		button_pressed = true;

        		ticks_spent_stable = 0;
        		ticks_spent_transitioning = 0;

        		transitioning = true;

        		transition_state();
        	}
        	else{
        		if(!transitioning){

                    /**
                     * If we have been stable in the current state for enough time, reset stable tick
                     * counter and begin transitioning
                     */
        			if(enough_time_stable()){
						ticks_spent_stable = 0;
						transitioning = true;
						transition_state();
					}

        			/**
					 * Else if we have kept the LED on for enough time this blink in the CROSSWALK state,
					 * reset tick counter and turn off LEDs
					 */
        			else if(current.mode == CROSSWALK && enough_time_crosswalk_on()){
                		ticks_spent_crosswalk_on = 0;
            			crosswalk_on = false;
            			clear_onboard_leds();
            		}

                    /**
                     * Else if we have kept the LED off for enough time this blink in the CROSSWALK state,
                     * reset tick counter and turn on LEDs
                     */
        			else if(current.mode == CROSSWALK && enough_time_crosswalk_off()){
                		ticks_spent_crosswalk_off = 0;
            			crosswalk_on = true;
            			set_onboard_leds();
            		}
        		}
        		else{

        			/**
					 * If we have been transitioning to the current state for enough time, reset
					 * transitioning tick counter and begin tracking ticks as stable
					 */
        			if(enough_time_transitioning()){
						ticks_spent_transitioning = 0;
						transitioning = false;
						PRINTF("%07u ms: Done transitioning to %s. Staying for %u sec...\r\n", now(), mode_to_string(current.mode), mode_state_sec(current.mode));
					}

                    /**
                     * Else if we are transitioning but not for enough time, step the LEDs
                     */
        			else{
						step_leds();
						set_onboard_leds();
        			}
        		}
        	}


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
     * Initialize TPM on-board module
     */
    init_onboard_tpm();

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Turn on appropriate on-board LEDs based on current state
     */
	set_onboard_leds();

    /**
     * Main infinite loop
     */
    while(1) {

        /**
         * Set by SysTick_Handler every TICK_SEC
         */
        if(tick){

            /**
             * Reset flag that was set by SysTick ISR
             */
        	tick = false;

            /**
             * Increment for timestamp purposes
             */
        	ticks_since_startup++;

            /**
             * Increment to track stable or transitioning periods
             */
        	if(transitioning){
        		ticks_spent_transitioning++;
        	}
        	else{
        		ticks_spent_stable++;

                /**
                 * Increment to track CROSSWALK blink periods
                 */
            	if(current.mode == CROSSWALK){
            		if(crosswalk_on){
                		ticks_spent_crosswalk_on++;
            		}
            		else{
                		ticks_spent_crosswalk_off++;
            		}
            	}
        	}



            /**
             * First check if touchpad is touched and current state is not CROSSWALK.
             * Then flag the button press, reset any ticks counted during previous state
             * from when button was pushed, and flag the need to transition to CROSSWALK
             */
        	if(current.mode != CROSSWALK && touchpad_is_touched()){

        		button_pressed = true;

        		ticks_spent_stable = 0;
        		ticks_spent_transitioning = 0;

        		transitioning = true;

        		transition_state();
        	}
        	else{
        		if(!transitioning){

                    /**
                     * If we have been stable in the current state for enough time, reset stable tick
                     * counter and begin transitioning
                     */
        			if(enough_time_stable()){
						ticks_spent_stable = 0;
						transitioning = true;
						transition_state();
					}

        			/**
					 * Else if we have kept the LED on for enough time this blink in the CROSSWALK state,
					 * reset tick counter and turn off LEDs
					 */
        			else if(current.mode == CROSSWALK && enough_time_crosswalk_on()){
                		ticks_spent_crosswalk_on = 0;
            			crosswalk_on = false;
            			clear_onboard_leds();
            		}

                    /**
                     * Else if we have kept the LED off for enough time this blink in the CROSSWALK state,
                     * reset tick counter and turn on LEDs
                     */
        			else if(current.mode == CROSSWALK && enough_time_crosswalk_off()){
                		ticks_spent_crosswalk_off = 0;
            			crosswalk_on = true;
            			set_onboard_leds();
            		}
        		}
        		else{

        			/**
					 * If we have been transitioning to the current state for enough time, reset
					 * transitioning tick counter and begin tracking ticks as stable
					 */
        			if(enough_time_transitioning()){
						ticks_spent_transitioning = 0;
						transitioning = false;
					}

                    /**
                     * Else if we are transitioning but not for enough time, step the LEDs
                     */
        			else{
						step_leds();
						set_onboard_leds();
        			}
        		}
        	}


        }
    }
    return 0;
}
#endif
