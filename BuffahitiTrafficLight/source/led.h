/**
 * \file    led.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Macros and function headers for on-board LEDs
 */

#ifndef LED_H_
#define LED_H_

/**
 * \def		PCR_MUX_SEL_RED
 * \brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * \detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		011: TPM2_CH0
 * 		100: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		101: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		110: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		111: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 */
#define PCR_MUX_SEL_RED\
	(3)

/**
 * \def		PCR_MUX_SEL_GREEN
 * \brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * \detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		011: TPM2_CH1
 * 		100: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		101: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		110: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		111: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 */
#define PCR_MUX_SEL_GREEN\
	(3)

/**
 * \def		PCR_MUX_SEL_BLUE
 * \brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * \detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		011: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		100: TPM0_CH1
 * 		101: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		110: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		111: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 */
#define PCR_MUX_SEL_BLUE\
	(4)

/**
 * \def		PORTB_RED_LED_PIN
 * \brief	On-board red LED is located at PB18
 */
#define PORTB_RED_LED_PIN\
	(18)

/**
 * \def		PORTB_GREEN_LED_PIN
 * \brief	On-board green LED is located at PB19
 */
#define PORTB_GREEN_LED_PIN\
	(19)

/**
 * \def		PORTD_BLUE_LED_PIN
 * \brief	On-board blue LED is located at PD1
 */
#define PORTD_BLUE_LED_PIN\
	(1)

/**
 * \def		RED_LED_ON()
 * \brief	Turn on on-board red LED
 */
#define RED_LED_ON()\
	(PTB->PCOR |= MASK(PORTB_RED_LED_PIN))

/**
 * \def		GREEN_LED_ON()
 * \brief	Turn on on-board green LED
 */
#define GREEN_LED_ON()\
	(PTB->PCOR |= MASK(PORTB_GREEN_LED_PIN))

/**
 * \def		BLUE_LED_ON()
 * \brief	Turn on on-board blue LED
 */
#define BLUE_LED_ON()\
	(PTD->PCOR |= MASK(PORTD_BLUE_LED_PIN))

/**
 * \def		RED_LED_OFF()
 * \brief	Turn off on-board red LED
 */
#define RED_LED_OFF()\
	(PTB->PSOR |= MASK(PORTB_RED_LED_PIN))

/**
 * \def		GREEN_LED_OFF()
 * \brief	Turn off on-board green LED
 */
#define GREEN_LED_OFF()\
	(PTB->PSOR |= MASK(PORTB_GREEN_LED_PIN))

/**
 * \def		BLUE_LED_OFF()
 * \brief	Turn off on-board blue LED
 */
#define BLUE_LED_OFF()\
	(PTD->PSOR |= MASK(PORTD_BLUE_LED_PIN))

/**
 * \def		RED_LED_TOGGLE()
 * \brief	Toggle on-board red LED
 */
#define RED_LED_TOGGLE()\
	(PTB->PTOR |= MASK(PORTB_RED_LED_PIN))

/**
 * \def		GREEN_LED_TOGGLE()
 * \brief	Toggle on-board green LED
 */
#define GREEN_LED_TOGGLE()\
	(PTB->PTOR |= MASK(PORTB_GREEN_LED_PIN))

/**
 * \def		BLUE_LED_TOGGLE()
 * \brief	Toggle on-board blue LED
 */
#define BLUE_LED_TOGGLE()\
	(PTD->PTOR |= MASK(PORTD_BLUE_LED_PIN))

/**
 * \var		extern volatile uint8_t red_level_end
 * \brief	Defined in led.c
 */
extern volatile uint8_t red_level_end;

/**
 * \var		extern volatile uint8_t green_level_end
 * \brief	Defined in led.c
 */
 extern volatile uint8_t green_level_end;

/**
 * \var		extern volatile uint8_t blue_level_end
 * \brief	Defined in led.c
 */
 extern volatile uint8_t blue_level_end;

/**
 * \fn		void init_onboard_leds
 * \brief	Initialize all 3 on-board LEDs as GPIO outputs and turn them all off. Referenced
 * 			operations from https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_2/Source/main.c
 * \param	N/A
 * \return	N/A
 * \detail	Many operations were referenced from Alexander G Dean (Chapter 2 of Embedded Systems
 * 			Fundamentals with ARM Cortex-M Based Microcontrollers)
 * 		SIM:	System Integration Module is a peripheral containing many control registers, including SCGC5
 * 		SCGC5:	System Clock Gating Control Register 5 is a register containing different controls,
 * 				including clock gating for Ports A-E (controlled with bits 9:13 respectively)
 * 		PORTB:	Port containing 32 pins, including red + green on-board LEDs
 * 		PORTD:	Port containing 32 pins, including blue on-board LED
 * 		PCR:	Pin Control Register, though in this function PCR is an array of 32 PCR registers
 * 				(where each Port A-E has its own array of PCR registers). All 32 pins within each
 * 				Port A-E have their own PCR register (e.g. pin 1 on Port A has its own 32-bit PCR)
 * 		PTB:	Points to all GPIO Control Registers for Port B, including PDDR
 * 		PTD:	Points to all GPIO Control Registers for Port D, including PDDR
 * 		PDDR:	Pin Data Direction Register is a register allowing configuration of a pin on the
 * 				respective port A-E (where 0 is input, 1 is output)
 * 		PCOR:	Port Clear Output Register is a register used to clear some bits on the respective
 * 				port A-E (e.g. writing 0x00000070 to PTA->PCOR will clear bits 6:4 on Port A)
 * 		PSOR:	Port Set Output Register is a register used to set some bits on the respective port
 * 				A-E (e.g. writing 0x00000070 to PTA->PSOR will set bits 6:4 on Port A)
 *		PTOR:	Port Toggle Output Register is a register used to toggle some bits on the respective
 *				port A-E (e.g. writing 0x00000070 to PTA-PTOR will toggle bits 6:4 on Port A)
 */
void init_onboard_leds(void);

/**
 * \fn		void clear_onboard_leds
 * \param	N/A
 * \return	N/A
 * \brief   Clear on-board LEDs
 */
void clear_onboard_leds(void);

/**
 * \fn		void set_onboard_leds
 * \param	N/A
 * \return	N/A
 * \brief   Set on-board LEDs based on current state's RGB values using TPM modules
 */
void set_onboard_leds(void);

/**
 * \fn		void step_leds
 * \param	N/A
 * \return	N/A
 * \brief   Calculate and step current state's RGB values
 */
void step_leds(void);

#endif /* LED_H_ */
