/**
 * \file    fsm_trafficlight.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Function definitions for FSM (finite state machine) implementation of traffic lights
 */

#include <stdbool.h>
#include <stdint.h>
#include "fsl_debug_console.h"

#include "fsm_trafficlight.h"
#include "led.h"
#include "systick.h"

/**
 * \var		volatile bool button_pressed
 * \brief	Flag controlled by physically touching the on-board touch sensor
 */
volatile bool button_pressed;

/**
 * \var		volatile bool transitioning
 * \brief	Flag controlled by SysTick timer
 */
volatile bool transitioning = false;

/**
 * \var		state_t current
 * \brief	The current state of the traffic light
 */
volatile state_t current;

/**
 * \var		state_t next
 * \brief	The next state of the traffic light
 */
volatile state_t next;

/**
 * \var		int8_t red_step
 * \brief	The amount to step current state's red value every 62.5 ms
 */
volatile int8_t red_step;

/**
 * \var		int8_t green_step
 * \brief	The amount to step current state's green value every 62.5 ms
 */
volatile int8_t green_step;

/**
 * \var		int8_t blue_step
 * \brief	The amount to step current state's blue value every 62.5 ms
 */
volatile int8_t blue_step;

/**
 * \var		extern volatile int i
 * \brief	Declared in main.c
 */
//extern volatile int i;

/**
 * \var		uint32_t prev_alt_clock_load;
 * \brief	Holds the value stored in SysTick->LOAD register from previous iteration
 */
//uint32_t prev_alt_clock_load;

void init_fsm_trafficlight(void)
{
	//prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_STOP);
	//ALT_CLOCK_LOAD(SEC_PER_STOP);

	current.mode = STOP;
	current.red_level = STOP_RED_LEVEL;
	current.green_level = STOP_GREEN_LEVEL;
	current.blue_level = STOP_BLUE_LEVEL;

	next.mode = GO;
	next.red_level = GO_RED_LEVEL;
	next.green_level = GO_GREEN_LEVEL;
	next.blue_level = GO_BLUE_LEVEL;

	get_steps();

    //set_onboard_leds();
}

void get_steps(void)
{
	if((ticks_since_startup & (TICK_FREQ_HZ - 1)) > 0){
		red_step = (next.red_level - current.red_level) / (ticks_since_startup & (TICK_FREQ_HZ - 1));
		green_step = (next.green_level - current.green_level) / (ticks_since_startup & (TICK_FREQ_HZ - 1));
		blue_step = (next.blue_level - current.green_level) / (ticks_since_startup & (TICK_FREQ_HZ - 1));
	}
}

#ifdef DEBUG
void transition_state(void)
{
	if(!transitioning){
		switch(current.mode){
		case STOP:
			PRINTF("%07u ms: Transition from STOP to GO\r\n", now());
			next.mode = GO;
			next.red_level = GO_RED_LEVEL;
			next.green_level = GO_GREEN_LEVEL;
			next.blue_level = GO_BLUE_LEVEL;
			break;
		case GO:
			PRINTF("%07u ms: Transition from GO to WARNING\r\n", now());
			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;
		case WARNING:
			PRINTF("%07u ms: Transition from WARNING to STOP\r\n", now());
			next.mode = STOP;
			next.red_level = STOP_RED_LEVEL;
			next.green_level = STOP_GREEN_LEVEL;
			next.blue_level = STOP_BLUE_LEVEL;
			break;
		default:
			break;
		}

		transitioning = true;
	}

	get_steps();

	current.red_level += red_step;
	current.green_level += green_step;
	current.blue_level += blue_step;

	if(current.red_level == next.red_level &&\
		current.green_level == next.green_level &&\
		current.blue_level == next.blue_level){
			transitioning = false;
		}
}
#elif NDEBUG
void transition_state(void)
{
	current.mode = next.mode;
	current.red_level = next.red_level;
	current.green_level = next.green_level;
	current.blue_level = next.blue_level;

	switch(current.mode){
	case GO:
		prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_GO);
		ALT_CLOCK_LOAD(SEC_PER_WARNING);
		next.mode = WARNING;
		next.red_level = WARNING_RED_LEVEL;
		next.green_level = WARNING_GREEN_LEVEL;
		next.blue_level = WARNING_BLUE_LEVEL;
		break;
	case WARNING:
		prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_WARNING);
		ALT_CLOCK_LOAD(SEC_PER_STOP);
		next.mode = STOP;
		next.red_level = STOP_RED_LEVEL;
		next.green_level = STOP_GREEN_LEVEL;
		next.blue_level = STOP_BLUE_LEVEL;
		break;
	case STOP:
		prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_STOP);
		ALT_CLOCK_LOAD(SEC_PER_GO);
		next.mode = GO;
		next.red_level = GO_RED_LEVEL;
		next.green_level = GO_GREEN_LEVEL;
		next.blue_level = GO_BLUE_LEVEL;
		break;
	case CROSSWALK:
		break;
	}

	transitioning = false;
}
#endif
