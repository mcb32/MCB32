//---------------------------------------------------
// DACzuPA4.c  (Einstiegsprogramm)
// Autor:  1.8.15 / rma /  TBZ / 20.9.14  /  rw  / BZU 
// Wandlung des 8 Bit Digitalwertes von P0 mit
// Ausgabe des Analogwertes an Pin PA4
//---------------------------------------------------
//#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0-,P1,Touchscreen
 
int main(void) 								// Hauptprogramm
{
	InitTouchP0P1("1");					// P0P1-Touchscreen ON
			 												// DAC1 initialisieren:   
  GPIOA->CRL &= 0xFFFF0FFFF;	// PA4 analog IN für Out 
	RCC->APB1ENR |= 1<<29;			// DAC Clock ON
	DAC->CR 	  = (0x7<<3)+1;		// DAC1 SW-Trigger & Enable 
	GPIOPutByte("PEH", 0xAA);   														
	while(1)	 									// Endlosschlaufe
	{
	  DAC->DHR8R1 = P0;					// In P0 zu analog Pin PA4 
	}
}

