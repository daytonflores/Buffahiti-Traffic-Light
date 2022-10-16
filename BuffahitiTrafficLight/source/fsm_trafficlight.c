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
volatile bool button_pressed = false;

/**
 * \var		volatile bool transitioning
 * \brief	Flag to indicate whether LEDs are actively transitioning or not
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
 * \var		extern volatile ticktime_t ticks_spent_transitioning
 * \brief	Defined in systick.c
 */
extern volatile ticktime_t ticks_spent_transitioning;

/**
 * \var		extern volatile ticktime_t ticks_spent_stable
 * \brief	Defined in systick.c
 */
extern volatile ticktime_t ticks_spent_stable;

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

    set_onboard_leds();
}

bool enough_time_stable(void)
{
	bool return_value = false;

	switch(current.mode){
	case STOP:
		if((ticks_spent_stable * TICK_SEC) >= SEC_PER_STOP){
			return_value = true;
		}
		break;
	case GO:
		if((ticks_spent_stable * TICK_SEC) >= SEC_PER_GO){
			return_value = true;
		}
		break;
	case WARNING:
		if((ticks_spent_stable * TICK_SEC) >= SEC_PER_WARNING){
			return_value = true;
		}
		break;
	case CROSSWALK:
		if((ticks_spent_stable * TICK_SEC) >= SEC_PER_CROSSWALK){
			return_value = true;
		}
		break;
	default:
		break;
	}

	return (return_value);
}

bool enough_time_transitioning(void)
{
	if((ticks_spent_transitioning * TICK_SEC) >= SEC_PER_TRANSITION){
		return true;
	}
	else{
		return false;
	}
}

#ifdef DEBUG
void transition_state(void)
{
	if(button_pressed){
		button_pressed = false;
		PRINTF("%07u ms: Transitioning from %s to CROSSWALK\r\n",\
				now(),\
				current.mode == STOP ?\
						"STOP" :\
						current.mode == GO ?\
								"GO" :\
								current.mode == WARNING ?\
										"WARNING" :\
										current.mode == CROSSWALK?\
												"CROSSWALK" :\
												"UNKNOWN");
		current.mode = CROSSWALK;
		current.red_level = CROSSWALK_RED_LEVEL;
		current.green_level = CROSSWALK_GREEN_LEVEL;
		current.blue_level = CROSSWALK_BLUE_LEVEL;

		next.mode = GO;
		next.red_level = GO_RED_LEVEL;
		next.green_level = GO_GREEN_LEVEL;
		next.blue_level = GO_BLUE_LEVEL;
	}
	else{
		switch(current.mode){
		case STOP:
			//prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_STOP);
			//ALT_CLOCK_LOAD(SEC_PER_GO);
			PRINTF("%07u ms: Transitioning from STOP to %s\r\n",\
					now(),\
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

			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;
		case GO:
			//prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_GO);
			//ALT_CLOCK_LOAD(SEC_PER_WARNING);
			PRINTF("%07u ms: Transitioning from GO to %s\r\n",\
					now(),\
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

			next.mode = STOP;
			next.red_level = STOP_RED_LEVEL;
			next.green_level = STOP_GREEN_LEVEL;
			next.blue_level = STOP_BLUE_LEVEL;
			break;
		case WARNING:
			//prev_alt_clock_load = ((ALT_CLOCK_HZ - 1) * SEC_PER_WARNING);
			//ALT_CLOCK_LOAD(SEC_PER_STOP);
			PRINTF("%07u ms: Transitioning from WARNING to %s\r\n",\
					now(),\
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

			next.mode = GO;
			next.red_level = GO_RED_LEVEL;
			next.green_level = GO_GREEN_LEVEL;
			next.blue_level = GO_BLUE_LEVEL;
			break;
		case CROSSWALK:
			PRINTF("%07u ms: Transitioning from CROSSWALK to %s\r\n",\
					now(),\
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

			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;
		}
	}
}
#elif NDEBUG
void transition_state()
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
