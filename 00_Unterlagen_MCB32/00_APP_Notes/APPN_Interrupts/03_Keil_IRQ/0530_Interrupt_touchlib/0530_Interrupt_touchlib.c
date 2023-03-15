 /**
  ******************************************************************************
  * @file    0530_Interrupt_touchlib.c
  * @author  rma / tbz für REV D
  * @version V1.0.0
  * @date    20.9.19 / rma
  * @brief   Toggelt LEDs 
	*
	*  				a) Toggelt LEDreihe und P1 (Alternierend) via IRQ  
  *					   SysTick und toggelt LED13 vom Interrupt ControlStick UP EXTI15_10_IRQn
	*         b) Output LED15 löst denselben Intr.EXTI15_10_IRQn aus und toggelt
  *            LED12.
  * <h2><center>&copy; COPYRIGHT 2015 Cityline AG </center></h2>
  ******************************************************************************/
	

#include "TouchP0P1.h"			
unsigned int timerVar=0;
unsigned char timerVarF=0;   // Flag für Überlauf
unsigned char IrqMem=0;   // Flag für Überlauf
// IRQ Handler
void TIM2_IRQHandler(void)    // IRQ Handler
{					               // 
  IRQClearFlag("T2"); 	      // Immer zuerst: IRQClearFlag(..) 
  // IrqMem=P1;
	P1=P1^0xFF;                 // Show an Port E
}


int main(void) 
{																										
	// Simulation von Schalter und LED starten
	InitTouchP0P1("1");	            // Off damit Timer nicht hereinspuckt.
 // Push Pull für die LEDS damit diese angesteuert werden 
	GPIOE->CRH   &= 0x00000000;      // Configure the GPIOE for 
	GPIOE->CRH   |= 0x22222222;      // General purpose output push-pull, 2MHz

	
	TimerInit (2, 10000, 15); // T2 auf 1s, 15 tiefste IR Prio     Maximaler Wert für T2 = 65535 (16Bit reload Register)
   // erster Versuch die Daten zu lesen
   timerVar  = TimerGetTime(2);      // Zeit in n*100us  
   timerVarF = TimerGetFlag(2);       //0 / 1=Überlauf     
   // Fertig mit Versuch 
	// Endlos wiederholen
	P1=0xAA;
	while (1)                        // oder  for (;;) {	
	{ 
		   // Tue etwas anderes ...... 
       
	}
}

