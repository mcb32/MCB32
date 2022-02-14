//-----------------------------------------------------
// ADC1xPC4.c  
// Autor: rma 2.8.15 und rw 20.9.14  /TBZ, BZU
// Wandlung des Analogwertes des lokalen Potenziometers
// 0..3.3V; P1 zeigt die oberen 8 der 12 Bits digital
//-----------------------------------------------------

#include "TouchP0P1.h"				// P0-,P1-,Touchscreen

int main(void) 								// Hauptprogramm
{
	InitTouchP0P1("1");					// P0P1-Touchscreen ON
	//	setScreenDir(HOR);      // Bei Ausgabe der Werte auf Dspl.
															// ADC1 initialisieren:   
  GPIOC->CRL 	 &= 0xFFF0FFFF;	// PC4 analog IN, floating
	RCC->APB2ENR |= 1<<9;				// ADC1 Clock (ADC2 1<<10)
	ADC1->CR1	   &= 0xFFFFFFDF;	// IRQ OFF, keine Interrupts
	ADC1->CR2 	  = 0x00000003;	// ADC Cont & ADC ON 1.Mal 
  ADC1->SQR3    = 14; 				// ADCxIN_0 .. 15, zB. PC4
	ADC1->CR2 	 |= 1<<0;				// ADC ON 2.Mal 
	
	while(1)	 									// Endlosschlaufe
	{														// Aus Analog 12 Bit 11..0
	  P1 = ADC1->DR>>4;					// obere 8 Bit 11..4 
//		printAt(1,"ADC: "); printHex(16,P1);  // Ausgabe der Werte auf Dspl. 
	}														// zu P1-LEDs 7..0
}


