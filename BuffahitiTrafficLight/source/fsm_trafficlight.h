/**
 * \file    fsm_trafficlight.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for FSM (finite state machine) implementation of traffic lights
 */

#ifndef FSM_TRAFFICLIGHT_H_
#define FSM_TRAFFICLIGHT_H_

/**
 * \def		STOP_RED_LEVEL
 * \brief	The STOP state's red value (RGB)
 */
#define STOP_RED_LEVEL\
	(0x61)

/**
 * \def		STOP_GREEN_LEVEL
 * \brief	The STOP state's green value (RGB)
 */
#define STOP_GREEN_LEVEL\
	(0x1E)

/**
 * \def		STOP_BLUE_LEVEL
 * \brief	The STOP state's blue value (RGB)
 */
#define STOP_BLUE_LEVEL\
	(0x3C)

/**
 * \def		GO_RED_LEVEL
 * \brief	The GO state's red value (RGB)
 */
#define GO_RED_LEVEL\
	(0x22)

/**
 * \def		GO_GREEN_LEVEL
 * \brief	The GO state's green value (RGB)
 */
#define GO_GREEN_LEVEL\
	(0x96)

/**
 * \def		GO_BLUE_LEVEL
 * \brief	The GO state's blue value (RGB)
 */
#define GO_BLUE_LEVEL\
	(0x22)

/**
 * \def		WARNING_RED_LEVEL
 * \brief	The WARNING state's red value (RGB)
 */
#define WARNING_RED_LEVEL\
	(0xFF)

/**
 * \def		WARNING_GREEN_LEVEL
 * \brief	The WARNING state's green value (RGB)
 */
#define WARNING_GREEN_LEVEL\
	(0xB2)

/**
 * \def		WARNING_BLUE_LEVEL
 * \brief	The WARNING state's blue value (RGB)
 */
#define WARNING_BLUE_LEVEL\
	(0x00)

/**
 * \def		CROSSWALK_RED_LEVEL
 * \brief	The CROSSWALK state's red value (RGB)
 */
#define CROSSWALK_RED_LEVEL\
	(0x00)

/**
 * \def		CROSSWALK_GREEN_LEVEL
 * \brief	The CROSSWALK state's green value (RGB)
 */
#define CROSSWALK_GREEN_LEVEL\
	(0x10)

/**
 * \def		CROSSWALK_BLUE_LEVEL
 * \brief	The CROSSWALK state's blue value (RGB)
 */
#define CROSSWALK_BLUE_LEVEL\
	(0x30)

/**
 * \typedef	mode_t
 * \brief	To allow objects of enum mode_e to be declared with ease
 */
typedef enum mode_e mode_t;

/**
 * \typedef	state_t
 * \brief	To allow objects of struct state_s to be declared with ease
 */
typedef struct state_s state_t;

/**
 * \enum	mode_e
 * \brief	To indicate the mode of a current state
 */
enum mode_e {
	STOP,
	GO,
	WARNING,
	CROSSWALK
};

/**
 * \struct	state_s
 * \brief	Holds all attributes of a given state
 */
struct state_s {
	mode_t mode;
	uint8_t red_level;
	uint8_t green_level;
	uint8_t blue_level;
};

/**
 * \var		extern volatile bool button_pressed
 * \brief	Declared in fsm_trafficlight.c
 */
extern volatile bool button_pressed;

/**
 * \var		extern volatile bool transitioning
 * \brief	Declared in fsm_trafficlight.c
 */
extern volatile bool transitioning;

/**
 * \var		extern state_t current
 * \brief	Declared in fsm_trafficlight.c
 */
extern volatile state_t current;

/**
 * \var		extern state_t next
 * \brief	Declared in fsm_trafficlight.c
 */
extern volatile state_t next;

/**
 * \fn		void init_fsm_trafficlight
 * \param	N/A
 * \return	N/A
 * \brief   Initialize members of current state and next state
 */
void init_fsm_trafficlight(void);

/**
 * \fn		void get_steps
 * \param	N/A
 * \return	N/A
 * \brief   Get step sizes for RGB values when analyzing current state vs next state
 */
void get_steps(void);

/**
 * \fn		void transition_state
 * \param	N/A
 * \return	N/A
 * \brief   Set the members of current state to reflect members of next state
 */
void transition_state(void);

#endif /* FSM_TRAFFICLIGHT_H_ */
