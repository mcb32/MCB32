/**
  ******************************************************************************
  * @file		delay.c
  * @author	RMA & YES 
  * @date		10.04.2016 
	* @note		rewritten from Project I2C Intro
  ******************************************************************************
  */
	
#include "delays_cz.h"

// For store tick counts in us
static __IO uint32_t usTicks;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}

void DelayInit()
{
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000000);
}


/* Name: 			delay_us
*  Funktion:	delay_us ohne Interrupt Hilfe
*/
void delay_us (uint32_t /*uint*/ duration_us)  {    // Verzögerung mit Einlese-Parameter für Zeit 
  duration_us *= ((72 / 4 - 15) * 2);       				// While-Schleife braucht 4 Maschinen-Zyklen. | 72 Mhz | 15 us Abweichung | Verdoppelung, wegen Halbzeit 
  while (duration_us != 0)    {         						// Bleibe solange bis der Wert "0" erreicht ist. 
    duration_us--;      														// Dekrementiere       
	}
}

void DelayUs(uint32_t us)
{
	// Reload us value
	usTicks = us;
	// Wait until usTick reach zero
	while (usTicks);
}

void DelayMs(uint32_t ms)
{
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		DelayUs(1000);
	}
}

/********************************* END OF FILE ********************************/
/******************************************************************************/



//// #include <stm32f10x.h>			// uC-Typ bei uVision4

//#ifndef __DELAYS_CZ
//#define __DELAYS_CZ

//#include <stm32f10x.h>			// uC-Typ bei uVision4

///* Name: 			delay_us
//*  Funktion:	delay_us ohne Interrupt Hilfe
//*/
//void delay_us (uint32_t /*uint*/ duration_us)  {    // Verzögerung mit Einlese-Parameter für Zeit 
//  duration_us *= ((72 / 4 - 15) * 2);       				// While-Schleife braucht 4 Maschinen-Zyklen. | 72 Mhz | 15 us Abweichung | Verdoppelung, wegen Halbzeit 
//  while (duration_us != 0)    {         						// Bleibe solange bis der Wert "0" erreicht ist. 
//    duration_us--;      														// Dekrementiere       
//	}
//}
//#endif

// END OF FILE
