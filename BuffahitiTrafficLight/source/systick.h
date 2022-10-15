/**
 * \file    systick.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for on-board timing subsystem
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

/**
 * \def		MS_PER_SEC
 * \brief	Used for unit conversions
 */
#define MS_PER_SEC\
	(1000)

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
 * \def		PRIM_CLOCK_LOAD_SEC(x)
 * \param	x The amount of seconds between each SysTick interrupt
 * \brief	The value to place into SysTick->LOAD register in order for interrupts to be generated
 * 			every x sec, based on PRIM_CLOCK_HZ
 */
#define PRIM_CLOCK_LOAD_SEC(x)\
	SysTick->LOAD = ((PRIM_CLOCK_HZ - 1) * x)

/**
 * \def		ALT_CLOCK_LOAD(x)
 * \param	x The amount of seconds between each SysTick interrupt
 * \brief	The value to place into SysTick->LOAD register in order for interrupts to be generated
 * 			every x sec, based on ALT_CLOCK_HZ
 */
#define ALT_CLOCK_LOAD(x)\
	SysTick->LOAD = ((ALT_CLOCK_HZ - 1) * x)

#ifdef DEBUG
/**
 * \def		SEC_PER_STOP
 * \brief	The amount of time in sec to stay in STOP mode
 */
#define SEC_PER_STOP\
	(5)

/**
 * \def		SEC_PER_GO
 * \brief	The amount of time in sec to stay in GO mode
 */
#define SEC_PER_GO\
	(5)

/**
 * \def		SEC_PER_WARNING
 * \brief	The amount of time in sec to stay in WARNING mode
 */
#define SEC_PER_WARNING\
	(3)
#elif NDEBUG
/**
 * \def		SEC_PER_STOP
 * \brief	The amount of time in sec to stay in STOP mode
 */
#define SEC_PER_STOP\
	(20)

/**
 * \def		SEC_PER_GO
 * \brief	The amount of time in sec to stay in GO mode
 */
#define SEC_PER_GO\
	(20)

/**
 * \def		SEC_PER_WARNING
 * \brief	The amount of time in sec to stay in WARNING mode
 */
#define SEC_PER_WARNING\
	(5)
#endif
/**
 * \def		ENABLE_SYSTICK_COUNTER()
 * \brief	Configure SysTick CTRL register to enable the SysTick counter
 */
#define ENABLE_SYSTICK_COUNTER()\
	(SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk)

/**
 * \def		DISABLE_SYSTICK_COUNTER()
 * \brief	Configure SysTick CTRL register to disable the SysTick counter
 */
#define DISABLE_SYSTICK_COUNTER()\
	(SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk))

/**
 * \typedef	typedef unsigned int ticktime_t
 * \brief   Time since boot, where each tick is 1 / ALT_CLOCK_HZ or 1 / PRIM_CLOCK_HZ, depending
 * 			on what's configured
 */
typedef unsigned int ticktime_t;

/**
 * \var		ticktime_t ticks_since_startup
 * \brief	Defined in systick.c
 */
extern volatile ticktime_t ticks_since_startup;

/**
 * \fn		void init_onboard_systick
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the timing system
 */
void init_onboard_systick(void);

/**
 * \fn		void SysTick_Handler
 * \param	N/A
 * \return	N/A
 * \brief   The ISR for the SysTick timer (i.e. runs each time the timer reaches 0).
 * \detail	FUNCTION NAME IS CASE SENSITIVE. Since it is weakly defined in
 * 			startup\startup_mkl25z4.c this definition will override
 */
void SysTick_Handler(void);

/**
 * \fn		ticktime_t now
 * \param	N/A
 * \return	Time since startup in sixteenths of a second
 * \brief   Returns time since startup, in sixteenths of a second
 */
volatile ticktime_t now(void);

/**
 * \fn		void reset_timer
 * \param	N/A
 * \return	N/A
 * \brief   Resets timer to 0; doesn't affect now() values
 */
void reset_timer(void);

/**
 * \fn		ticktime_t get_timer
 * \param	N/A
 * \return	Ticks since the last call to reset_timer()
 * \brief   Returns ticks since the last call to reset_timer()
 */
ticktime_t get_timer(void);


#endif /* SYSTICK_H_ */
