 /**
  ******************************************************************************
  * @file    2311v00_Interrupt_PA0.c
  * @author  rma / tbz für REV D
  * @version V1.0.0
  * @date    12.3.2023 / rma 
  * @brief   Toggelt LEDs 
	*
	*  				a) Wenn PA0 nicht gedrückt wird dann toggeln die LED mit AAhex und 55hex 
  *					   
	*         b) Wenn PA0 gedrückt wird dann toggeln die LEDs mit 00/FFhex
	*         c) wenn wieder PA0 gedrückt wird --> a)  usw
	
  * <h2><center>&copy; COPYRIGHT 2015 Cityline AG </center></h2>
  ******************************************************************************/
	

#include "TouchP0P1.h"			
unsigned int timerVar=0;
unsigned char timerVarF=0;        // Flag für Überlauf
unsigned char IrqMem=0;          // Flag für Überlauf
unsigned char Store_P1 =0;
unsigned char Round_PA0=0;      // Check round 0 or 1 / 0 with new Value, 1 with old value 0xAA or 0x55

// Aux Funcrions 
void my_delay(unsigned my_ms)
{
	long t=0;
	int  t2=0; 
	for (t2=my_ms; t2>0;t2--)
	{
	for(t=12000; t>0; t--);    // 1ms
	}
}

// IRQ Handler
void TIM2_IRQHandler(void)      // IRQ Handler
{					                     // 
  IRQClearFlag("T2"); 	      // Always first: IRQClearFlag(..) 
  // IrqMem=P1;
	P1=P1^0xFF;                 // Show an Port E
	
}

void  EXTI0_IRQHandler(void)   // IRQ Handler
{					               // 
  IRQClearFlag("PA0"); 	      // Always first: IRQClearFlag(..) 
	
	IrqMem=IrqMem+0x01;         // Count numbers of PA0 bouncers
	P1=IrqMem;
//	if(IrqMem<0x40)
//	{
//		if (Round_PA0==0x00)
//			{
//				Store_P1 =P1;							// Store original P1
//				P1=IrqMem;                   // Set new Value
//				Round_PA0=0x01;             // set 01 for action if PA0 occurs again
//	    }else{
//		   	P1=IrqMem;                   // Set new Value
//			 }
//	}else{                            // if IrqMem>=0x44
//		P1=0xAA;                   // restore P1
//		Round_PA0=0x00;                 // Reset Round
//		IrqMem=0;
//		while(1){};                 // TEstpurposes
//	}
//	
//	
////	if (Store_P1==0x00)
////	{
////		Store_P1 =P1;							// Store aktual P1
////		my_delay(33);             // wait because PA0 is bouncing
////	  P1=0xFF;                    // Set new Value
////		Round_PA0=0x01;             // set 01 for action if PA0 occurs again
////	}
////	else{
////		P1=Store_P1;               // Restore original Value 
////		Round_PA0=0x00;             // set 00 for new action if PA0 occurs again
////	}
}



// ---------------------------------------- Main 
int main(void) 
{																										
	// Simulation von Schalter und LED starten
	InitTouchP0P1("1");	            // Off damit Timer nicht hereinspuckt.
	
	// Schalte LEDs EIN: Push Pull für die LEDS damit diese angesteuert werden 
	GPIOE->CRH   &= 0x00000000;      // Configure the GPIOE for 
	GPIOE->CRH   |= 0x22222222;      // General purpose output push-pull, 2MHz

	
   ExtIRQInit("PA0",1,15); // IRQ PA0,neg Flanke Priorität 4
   TimerInit (2, 5500, 10); // T2 auf 1s, 15 tiefste IR Prio     Maximaler Wert für T2 = 65535 (16Bit reload Register)
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

