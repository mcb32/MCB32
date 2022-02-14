/** @file  grafikfunkionen_1.c
*  @brief  Zeigt die grundlegenden Grafikfunktionen  Version I von MCB32
* 
*         
*  @author  mal  
*  @date    28.3.2015
*  @version 1.0    
*  @bug     No known bugs. ...
*/
//=================================Includes===================================== 
#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen und Grafik
#include <math.h>             // lib für Sinus
#define PI 3.14159f // Konstante PI
//*****************************Implementation***********************************
//=================================main()======================================= 
int main(void) 								               // Hauptprogramm
{
    long t; 										             // Verzoegerungsvariable
	  float rad;
	  unsigned char uc_va1;
	  char LIBVer[]=dMCB32_LibVersion;         //  Hole Lib Version und zeige sie an
  	//InitTouchScreen();                       // Init. der Display Hardware
	  // InitTouchP0P1("1");		                   // P0,P1 auf Touchscreen ON. Bei 0 wird auch der Systick Handler abgeschaltet

	  setScreenDir (HOR);                      // setze Richtung von Display. 0,0 bei Resettaster
    // InitTouchP0P1("0");	
	  textxy(" MCB32 Lib Version:", 2, 32, BLACK, YELLOW);
	  textxy(LIBVer, 160, 32, WHITE, BLACK);
	  printAt(2,"--------------------------------- ");       // Schreibe auf der 2ten Zeile den Text
    // Zeichne
	  circle(50,80,20,2,GREEN,0);                      // Zeichne Kreis
	  ellipse(100, 80, 10,20,1,YELLOW,1);              // Zeichne Ellipse
	  rectan(100,150,140,180,1,BRIGHT_RED,1);          // Zeichne Rechteck
	  line(5,110,315,110,2,WHITE);                     // Zeichne Linie
	  // Zeichne mit plotDot() ein Muster
	  for(uc_va1 =0;uc_va1<80;uc_va1++){
			for (t=0; t<100;t++){
			  plotDot(140+uc_va1+t,115+t,uc_va1*t*8);      // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
	     }
		}
		// zeichne Sinus  mit plotDot
		for (t=0; t<180;t++){
			rad = 4*t * PI / 180;                             // Berechnen des Bogenmaßwinkels
			plotDot(10+t,(210+12*sin((double)(rad))),WHITE);   // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
	  }
		plotFilledRect ( 300, 20, 10, 10, RED );      // zeichne ein gefülltes Rechteck
		GPIOInit("PEH",00000000);
		GPIOE->CRH   &= 0x00000000;                  // Konfiguriere GPIOE für
		GPIOE->CRH   |= 0x22222222;                  // General purpose output push-pull, 2MHz
		GPIOPutByte("PEH",0xAA);
    // SysTick_Config(72* 1030);  		           // SysTimer, SysTick_Handle() alle 1000us	   
		delay_ms(500);  // geht nicht wenn InitTouchP0P1("0");
		while(1){
		    uc_va1 = getTSCtouched();				      // Hole Touchwert 0,1
			  GPIOPutByte("PEH",getTSCtouched());   // zeige via LED ob Touch gedrückt wurde
				if(uc_va1==1){
					for (t=0; t<180;t++){
			     rad = 4*t * PI / 180;                             // Berechnen des Bogenmaßwinkels
			     plotDot(10+t,(210+12*sin((double)(rad))),BRIGHT_BLUE);   // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
	        }
			  }
		}
		
}



