/**
  ******************************************************************************
  * @file		delay_cz.c
  * @author	   RMA & YES 
  * @date		10.04.2016 
* @note		   some usefull functions
  ******************************************************************************
  */
	
#include "delays_cz.h"   // Header File for delay_cz.c

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


/**
  * @function  void DelayInit() 
  * @note      Set SysTick (1us) 
  * @brief     Update SystemCoreClock variable according to Clock Register Values.
  *            The SystemCoreClock variable contains the core clock (HCLK), it can
  *            be used by the user application to setup the SysTick timer or configure
  *            other parameters.   see more in SystemCoreClockUpdate()
  *****************************************************/
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
