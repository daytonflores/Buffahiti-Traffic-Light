/**
 * \file    timer.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for on-board timing subsystem
 */

#ifndef TIMER_H_
#define TIMER_H_

/**
 * \def		PRIM_CLOCK_HZ
 * \brief	Frequency of primary processor clock source in Hz. For KL25Z, this is 48 MHz
 */
#define PRIM_CLOCK_HZ\
	(48000000UL)

/**
 * \def		ALT_CLOCK_HZ
 * \brief	Frequency of alternate clock source in Hz. For KL25Z, this is 3 MHz
 */
#define ALT_CLOCK_HZ\
	(3000000UL)

/**
 * \def		SysTick_CTRL_CLKSOURCE_EXT_Msk
 * \brief	The CLKSOURCE field selects the clock source, which can be either the processor
 * 			clock (1) or an external reference clock (0). On the KL25Z MCU, the processor
 * 			clock runs at up to 48 MHz, and the external reference clock is the processor clock
 * 			divided by 16
 */
#define SysTick_CTRL_CLKSOURCE_EXT_Msk\
	(0UL << SysTick_CTRL_CLKSOURCE_Pos)

/**
 * \typedef	typedef uint32_t ticktime_t
 * \brief   Time since boot, in sixteenths of a second
 */
typedef uint32_t ticktime_t;

/**
 * \fn		void init_systick
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the timing system
 */
void init_systick();

/**
 * \fn		void SysTick_Handler
 * \param	N/A
 * \return	N/A
 * \brief   The ISR for the SysTick timer (i.e. runs each time the timer reaches 0).
 * \detail	FUNCTION NAME IS CASE SENSITIVE. Since it is weakly defined in
 * 			startup\startup_mkl25z4.c this definition will override
 */
void SysTick_Handler();

/**
 * \fn		ticktime_t now
 * \param	N/A
 * \return	Time since startup in sixteenths of a second
 * \brief   Returns time since startup, in sixteenths of a second
 */
ticktime_t now();

/**
 * \fn		void reset_timer
 * \param	N/A
 * \return	N/A
 * \brief   Resets timer to 0; doesn't affect now() values
 */
void reset_timer();

/**
 * \fn		ticktime_t get_timer
 * \param	N/A
 * \return	Ticks since the last call to reset_timer()
 * \brief   Returns ticks since the last call to reset_timer()
 */
ticktime_t get_timer();


#endif /* TIMER_H_ */
