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

void init_fsm_trafficlight(void)
{
	ALT_CLOCK_LOAD(SEC_PER_STOP);

	current.mode = STOP;
	current.red_level = STOP_RED_LEVEL;
	current.green_level = STOP_GREEN_LEVEL;
	current.blue_level = STOP_BLUE_LEVEL;

	next.mode = GO;
	next.red_level = GO_RED_LEVEL;
	next.green_level = GO_GREEN_LEVEL;
	next.blue_level = GO_BLUE_LEVEL;

    set_onboard_leds();
}

#ifdef DEBUG
void transition_state()
{
	PRINTF("%07u ms: Transitioning from %s to %s\r\n",\
			now(),\
			current.mode == STOP ?\
					"STOP" :\
					current.mode == GO ?\
							"GO" :\
							current.mode == WARNING ?\
									"WARNING" :\
									current.mode == CROSSWALK?\
											"CROSSWALK" :\
											"UNKNOWN", \
			next.mode == STOP ?\
					"STOP" :\
					next.mode == GO ?\
							"GO" :\
							next.mode == WARNING ?\
									"WARNING" :\
									next.mode == CROSSWALK?\
											"CROSSWALK" :\
											"UNKNOWN");

	current.mode = next.mode;
	current.red_level = next.red_level;
	current.green_level = next.green_level;
	current.blue_level = next.blue_level;

	switch(current.mode){
	case GO:
		ALT_CLOCK_LOAD(SEC_PER_GO);
		next.mode = WARNING;
		next.red_level = WARNING_RED_LEVEL;
		next.green_level = WARNING_GREEN_LEVEL;
		next.blue_level = WARNING_BLUE_LEVEL;
		break;
	case WARNING:
		ALT_CLOCK_LOAD(SEC_PER_WARNING);
		next.mode = STOP;
		next.red_level = STOP_RED_LEVEL;
		next.green_level = STOP_GREEN_LEVEL;
		next.blue_level = STOP_BLUE_LEVEL;
		break;
	case STOP:
		ALT_CLOCK_LOAD(SEC_PER_STOP);
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
#elif NDEBUG
void transition_state()
{
	current.mode = next.mode;
	current.red_level = next.red_level;
	current.green_level = next.green_level;
	current.blue_level = next.blue_level;

	switch(next.mode){
	case GO:
		next.mode = WARNING;
		next.red_level = WARNING_RED_LEVEL;
		next.green_level = WARNING_GREEN_LEVEL;
		next.blue_level = WARNING_BLUE_LEVEL;
		break;
	case WARNING:
		next.mode = STOP;
		next.red_level = STOP_RED_LEVEL;
		next.green_level = STOP_GREEN_LEVEL;
		next.blue_level = STOP_BLUE_LEVEL;
		break;
	case STOP:
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
