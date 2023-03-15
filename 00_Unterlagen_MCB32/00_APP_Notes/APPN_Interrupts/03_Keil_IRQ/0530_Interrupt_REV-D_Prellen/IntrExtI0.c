//-------------------------------------------------
// IntrExtI0.c   / 12.8.15 / rma / rw  / TBZ REV D 
// Die LEDs zählen die pos. Flanken von Button0 PA0
// mit P1-Anzeige am Touchscreen;  Prellen beachten
//-------------------------------------------------
// #include <stm32f10x_cl.h>			// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0-, P1-Definition

void EXTI0_IRQHandler(void)		// Vorgeg. Handlername
{															// Zuerst: Loesche
	EXTI->PR |= 1;							// Intr Flag 1 = Pin0
	P1++;												// Zaehler erhoehen
}

int main(void) 								// Hauptprogramm
{
	InitTouchP0P1("1");					// P0P1-Touchscreen ON
															// Ports initialisieren:   
  RCC->APB2ENR |= 1<<2;    		// Enable GPIOA clock, Input       
  RCC->APB2ENR |= 1<<6;    		// Enable GPIOE clock, LEDs         
  GPIOA->CRL   &= 0xFFFFFF00; // Configure the GPIOA 0..7 
  GPIOA->CRL   |= 0x00000088; // Pullup Input PA_0, PA_1
															// Interrupt konfigurieren:   
  RCC->APB2ENR |= 1;	  			// Alternate Funct. AFIO ON
	AFIO ->EXTICR[0] = 0x00;		// Pin0 von PA0 auf EXTI0
	EXTI->IMR  |= 1;						// EXTI0 ON-Maske ON
	EXTI->RTSR |= 1;						// Steigende Triggerflanke
	EXTI->FTSR |= 1;						// Fallende Triggerflanke
	NVIC->ISER[0]  |= 1<<6;			// Enable IntrNr 6 für EXTI0
	while(1)	 									// Endlosschlaufe
	{														// Nichts tun 										
	}														
} 
