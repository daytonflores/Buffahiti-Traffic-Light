/**
 * \file    fsm_trafficlight.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Function definitions for FSM (finite state machine) implementation of traffic lights
 */

#include <stdbool.h>
#include <stdint.h>
#include "fsl_debug_console.h"

/**
 * User-defined libraries
 */
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
 * \var		volatile bool crosswalk_on
 * \brief	Flag to indicate whether LED is actively on during CROSSWALK mode (since it should blink)
 */
volatile bool crosswalk_on = false;

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
 * \var		extern volatile uint8_t red_level_end
 * \brief	Defined in led.c
 */
extern volatile uint8_t red_level_end;

/**
 * \var		volatile uint8_t green_level_end
 * \brief	Defined in led.c
 */
extern volatile uint8_t green_level_end;

/**
 * \var		volatile uint8_t blue_level_end
 * \brief	Defined in led.c
 */
extern volatile uint8_t blue_level_end;

void init_fsm_trafficlight(void)
{
	current.mode = STOP;
	current.red_level = STOP_RED_LEVEL;
	current.green_level = STOP_GREEN_LEVEL;
	current.blue_level = STOP_BLUE_LEVEL;

	next.mode = GO;
	next.red_level = GO_RED_LEVEL;
	next.green_level = GO_GREEN_LEVEL;
	next.blue_level = GO_BLUE_LEVEL;
}

char *mode_to_string(mode_t mode)
{
	char *return_value;

	switch(mode){
	case STOP:
		return_value = "STOP";
		break;
	case GO:
		return_value = "GO";
		break;
	case WARNING:
		return_value = "WARNING";
		break;
	case CROSSWALK:
		return_value = "CROSSWALK";
		break;
	default:
		return_value = "UNKNOWN";
		break;
	}

	return (return_value);
}

uint32_t mode_state_sec(mode_t mode)
{
	uint32_t return_value;

	switch(mode){
	case STOP:
		return_value = SEC_PER_STOP;
		break;
	case GO:
		return_value = SEC_PER_GO;
		break;
	case WARNING:
		return_value = SEC_PER_WARNING;
		break;
	case CROSSWALK:
		return_value = SEC_PER_CROSSWALK;
		break;
	default:
		return_value = 0;
		break;
	}

	return (return_value);
}

bool enough_time_stable(void)
{
    /**
     * Check if enough time has been spent stable in the current state (i.e. not transitioning)
     */
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
    /**
     * Check if enough time has been spent transitioning to the current state (i.e. not stable)
     */
	if((ticks_spent_transitioning * TICK_SEC) >= SEC_PER_TRANSITION){
		return true;
	}
	else{
		return false;
	}
}

bool enough_time_crosswalk_on(void)
{
    /**
     * Check if enough time has been spent keeping LED on in CROSSWALK mode for blink
     */
	if((ticks_spent_crosswalk_on * TICK_SEC * MSEC_PER_SEC) >= MSEC_PER_CROSSWALK_ON){
		return true;
	}
	else{
		return false;
	}
}

bool enough_time_crosswalk_off(void)
{
    /**
     * Check if enough time has been spent keeping LED off in CROSSWALK mode for blink
     */
	if((ticks_spent_crosswalk_off * TICK_SEC * MSEC_PER_SEC) >= MSEC_PER_CROSSWALK_OFF){
		return true;
	}
	else{
		return false;
	}
}

#ifdef DEBUG
void transition_state(void)
{
    /**
     * Button has been pressed so transition to CROSSWALK and set next state to GO
     */
	if(button_pressed){

	    /**
	     * Reset flag raised by touch sensor
	     */
		button_pressed = false;

		PRINTF("%07u ms: Transitioning from %s to CROSSWALK\r\n", now(), mode_to_string(current.mode));
		current.mode = CROSSWALK;

		red_level_end = CROSSWALK_RED_LEVEL;
		green_level_end = CROSSWALK_GREEN_LEVEL;
		blue_level_end = CROSSWALK_BLUE_LEVEL;

		next.mode = GO;
		next.red_level = GO_RED_LEVEL;
		next.green_level = GO_GREEN_LEVEL;
		next.blue_level = GO_BLUE_LEVEL;
	}

    /**
     * Button has not been pressed so continue through FSM as normal
     */
	else{
		switch(current.mode){
		case STOP:
			PRINTF("%07u ms: Transitioning from STOP to %s\r\n", now(), mode_to_string(next.mode));

			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;

		case GO:
			PRINTF("%07u ms: Transitioning from GO to %s\r\n", now(), mode_to_string(next.mode));

			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = STOP;
			next.red_level = STOP_RED_LEVEL;
			next.green_level = STOP_GREEN_LEVEL;
			next.blue_level = STOP_BLUE_LEVEL;
			break;

		case WARNING:
			PRINTF("%07u ms: Transitioning from WARNING to %s\r\n", now(), mode_to_string(next.mode));

			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = GO;
			next.red_level = GO_RED_LEVEL;
			next.green_level = GO_GREEN_LEVEL;
			next.blue_level = GO_BLUE_LEVEL;
			break;

		case CROSSWALK:
			PRINTF("%07u ms: Transitioning from CROSSWALK to %s\r\n", now(), mode_to_string(next.mode));

			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;
		}
	}
}
#elif NDEBUG
void transition_state(void)
{
    /**
     * Button has been pressed so transition to CROSSWALK and set next state to GO
     */
	if(button_pressed){

	    /**
	     * Reset flag raised by touch sensor
	     */
		button_pressed = false;

		current.mode = CROSSWALK;

		red_level_end = CROSSWALK_RED_LEVEL;
		green_level_end = CROSSWALK_GREEN_LEVEL;
		blue_level_end = CROSSWALK_BLUE_LEVEL;

		next.mode = GO;
		next.red_level = GO_RED_LEVEL;
		next.green_level = GO_GREEN_LEVEL;
		next.blue_level = GO_BLUE_LEVEL;
	}

    /**
     * Button has not been pressed so continue through FSM as normal
     */
	else{
		switch(current.mode){
		case STOP:
			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;

		case GO:
			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = STOP;
			next.red_level = STOP_RED_LEVEL;
			next.green_level = STOP_GREEN_LEVEL;
			next.blue_level = STOP_BLUE_LEVEL;
			break;

		case WARNING:
			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = GO;
			next.red_level = GO_RED_LEVEL;
			next.green_level = GO_GREEN_LEVEL;
			next.blue_level = GO_BLUE_LEVEL;
			break;

		case CROSSWALK:
			current.mode = next.mode;

			red_level_end = next.red_level;
			green_level_end = next.green_level;
			blue_level_end = next.blue_level;

			next.mode = WARNING;
			next.red_level = WARNING_RED_LEVEL;
			next.green_level = WARNING_GREEN_LEVEL;
			next.blue_level = WARNING_BLUE_LEVEL;
			break;
		}
	}
}
#endif
