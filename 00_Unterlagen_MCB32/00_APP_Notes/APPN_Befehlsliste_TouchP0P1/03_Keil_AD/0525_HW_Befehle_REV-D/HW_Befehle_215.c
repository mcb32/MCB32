//-----------------------------------------------------
// HW_Befehle_215.c  
// Autor: rma 2.12.15
// Wandlung des Analogwertes des lokalen Potenziometers
// 0..3.3V; P1 zeigt die oberen 8 der 12 Bits digital
// Muster für Benutzung Counter und DA Wandler
//-----------------------------------------------------

#include "TouchP0P1.h"				// P0-,P1-,Touchscreen


void TIM2_IRQHandler(void)
{				// Immer zuerst:
  IRQClearFlag("T2"); 	// IRQClearFlag(..) 
				// dann IntrService	
}


unsigned char b=0;

int main(void) 								// Hauptprogramm
{
	InitTouchP0P1("1");					// P0P1-Touchscreen ON
	//	setScreenDir(HOR);      // Bei Ausgabe der Werte auf Dspl.
	
	// TEstbench für  in Lib integrierte Funktionen
	GPIOInit("PAL", "11110000");														  
  GPIOInit("PAL", "11110000"); // 4Out,4In 
	GPIOGetByte("PAL");  	    // PAL->var
  GPIOPutByte("PEH", 0xAA);   // 0xAA->PEH
  b = GPIOGetByte("PAL");	  // 8Bit In von Port A Low-Byte

	ADCInit(1,"PC4");
	// Lies ADC mit Hilfe des Befehls aus der Lib.
//	for (b=0;b<250;b++){
//	  P1 = ADCGetVal(1);
//		delay_ms(8);
//	}
	
	DACInit(1);
	DACPutVal(1,100);
	delay_ms(2000);
	
  CounterInit(2,"PA0",0,0);  // Prio 0 PA0 = Button auf Board
	CounterPutCount(2, 0);
	b=CounterGetCount(2); 
//	ExtIRQInit("PA0",0,4); 
	
	
	// ---------------------------------------------
	
  // ADC1 initialisieren, direkt via Register: 
	GPIOC->CRL 	 &= 0xFFF0FFFF;	// PC4 analog IN, floating
	RCC->APB2ENR |= 1<<9;				// ADC1 Clock (ADC2 1<<10)
	ADC1->CR1	   &= 0xFFFFFFDF;	// IRQ OFF, keine Interrupts
	ADC1->CR2 	  = 0x00000003;	// ADC Cont & ADC ON 1.Mal 
  ADC1->SQR3    = 14; 				// ADCxIN_0 .. 15, zB. PC4
	ADC1->CR2 	 |= 1<<0;				// ADC ON 2.Mal 
	
	while(1)	 									// Endlosschlaufe
	{														// Aus Analog 12 Bit 11..0
	  P1 = ADC1->DR>>4;					// obere 8 Bit 11..4 

	}														// zu P1-LEDs 7..0
}


