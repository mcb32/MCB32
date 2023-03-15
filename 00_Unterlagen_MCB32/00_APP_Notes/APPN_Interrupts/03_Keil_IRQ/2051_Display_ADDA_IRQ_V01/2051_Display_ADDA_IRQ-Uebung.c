/** @file   2051_Display_ADDA_IRQ-Uebung.c
*  @brief   Lese AD Wandler in einer Interrupt Routine ein und gib 
*           den Wert wieder aus. 
*         
*  @author  mal  
*  @date    18.12.2017 / 12.3.2023
*  @version 1.0    
*  @bug     No known bugs. ...
*/
//=================================Includes===================================== 
#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen und Grafik
#include <math.h>             // lib für Sinus
#include <stdio.h>            // lib für sprintf() usw
#define PI 3.14159f           // Konstante PI

//*****************************Implementation***********************************

unsigned char Int_b;
unsigned int Messarray[1000];
unsigned int x_Axis=0;
unsigned int y_Axis=0;
#define x_Axis_Max 320
#define y_Axis_Max 240
#define y_Axis_Max_Background 160				// KO Hintergrund Max Y-Ablenkung
#define ADC_max 255
float ADC_GAIN = ((float)y_Axis_Max_Background/(float)ADC_max);  // Was passiert hier genau
char wertausgabe[20];

// Funktionsprotoypen
void ZeichneKO_Hintergrund(unsigned int Color);
void TIM2_IRQHandler(void);						// Interrupt Handler für Timer2


//=================================main()======================================= 
int main(void) 								               // Hauptprogramm
{
    long t; 										             // Verzoegerungsvariable
	  float rad;
//	  unsigned char color_toggle=0;						// Hilfsvariablen;
//	  unsigned int ADC_IN =0;
	  InitTouchScreen();                       // Init. der Display Hardware
	  //InitTouchP0P1("1");		                   // P0,P1 auf Touchscreen ON. Bei 0 wird auch der Systick Handler abgeschaltet

	  setScreenDir (VER);                      // setze Richtung von Display. 0,0 bei Resettaster
	  if(getScreenDir()==VER){                 // Funktioniert nur mit setScreenDir (VER); 
		   clearScreen (BLUE);
		   for(t=12000000;t<0;t--){};            // Warte für Show
		}	

		// zeichne Sinus  mit plotDot
		for (t=0; t<300;t++){
			rad = 4*t * PI / 180;                             				// Berechnen des Bogenmaßwinkels
			plotDot(10+t,(210+12*sin((double)(rad))),WHITE);   				// oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
	  }
		GPIOInit("PEH",00000000);
		GPIOE->CRH   &= 0x00000000;                  									// Konfiguriere GPIOE für
		GPIOE->CRH   |= 0x22222222;                  									// General purpose output push-pull, 2MHz
		GPIOPutByte("PEH",0x80);
   
		// So nun ein wenig AD DA Wandler:
		ADCInit(1, "PC4");        									// ADC1 an PC4
		DACInit(1);																	// DAC Ausgang an PA4
		TimerInit (2, 100, 1);     									// T2 auf 20 mS,mit Interrupt
    textxy("Der ADC misst: ", 8, 180, YELLOW, BLUE);
		while(1){
			
		}
}

// Hilfsfunktionen
void ZeichneKO_Hintergrund(unsigned int Color){
plotFilledRect(0,0,x_Axis_Max,y_Axis_Max_Background+1,Color);          // Zeichne Rechteck
}


// Interrupt Handler für Timer2
void TIM2_IRQHandler(void)
{	 
  unsigned int Int_ADC_IN;
  GPIOE->BSRR = 1 << 8; // Set Bit PE8
		                    // Immer zuerst:
  IRQClearFlag("T2"); 	// IRQClearFlag(..) 
	Int_ADC_IN = ADCGetVal(1);     // Analog Wert einlesen von PC4
  // GPIOPutByte("PEH",Int_ADC_IN);  // Gib AD Wandler auf LED aus
	plotDot(10+x_Axis,(y_Axis_Max_Background-ADC_GAIN*Int_ADC_IN),WHITE);
//	Messarray[x_Axis]=Int_ADC_IN;                       // Lösung optimieren, so dass eine Art Ringbuffer entsteht welcher 4-10mal grösser ist als x-Max
	if (x_Axis<x_Axis_Max)
     {x_Axis=x_Axis+1;}
	 else { ZeichneKO_Hintergrund(RED);
		 x_Axis=0;}
   DACPutVal(1, Int_ADC_IN);      // Analog Out PA4
   GPIOE->BSRR = 1 << 24; // Reset Bit PE8
}