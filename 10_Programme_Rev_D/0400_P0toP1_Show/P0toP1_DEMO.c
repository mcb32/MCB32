/** @file P0P1Touch_DEMO.c
*  @brief  Ein-Ausgabe MCB32
*
*          Schalterstellungen am Eingabeport P0 lesen
*          und an Ausgabeport P1 (LED) ausgeben.  
*          Nach dem Reset wird zudem gezeigt wie die Grafik angesteuert werden kann.
*         
*  @author rw  / BZU  / Main Design
*  @author rma / TBZ / REV D implmentation
*  @date 12.7.14 / rma
*  @version 1.0    
*  @bug No known bugs.
*/
//=================================Includes=====================================
#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen   


  int main (void) 
{                                                    
  P1 = 0x00;                  // init P1
  InitTouchP0P1("1");					// P0,P1 auf Touchscreen ON. Touchscreen übernimmt Simulation Schalter und LED
	
	// Setze die Schreibrichtung Horizontal
	setScreenDir (HOR);
  printAt(2,"                   Vertikaler Text");       // Schreibe auf der 2ten Zeile den Text
	textxy("ABcdefghiklmnop", 20, 50, BLACK, YELLOW);
	plotFilledRect(160,160,40,20, WHITE);
	rectan(10,30,145,55,1,BRIGHT_GREEN,0);
	
	setScreenDir (VER);
	rectan(20,1,238,25,2,BRIGHT_GREEN,0);
  plotDot(120,70, WHITE);
  plotDot(50,80, YELLOW);
	plot_mark_Ver(30,150,200,30,WHITE);		// vert+hor Linie schneller
	line(90,90,120,200,10,CYAN);
	line(90,90,120,200,10,BRIGHT_RED);
	circle(150,200,60,3,YELLOW,0);
	circle(180,250,60,5,BRIGHT_RED,1);
	ellipse(200,50,40,20,2,WHITE,0);
	 P1 = P0;  
  while (1)      				                 // Endlos
  {  
    P1 = P0;                   	         // P1 <-- P0
		//P1_7=1;                            // Setzt Bit 7 vpn Port P1
		delay_ms(20);                        // zu Demozwecken
   }
}


/*   Information zu InitP0P1Touch()

1)  InitP0P1Touch (“0”) ;   										Der Touchscreen bleibt ausgeschaltet
																								P0 ist als Input, P1 als Output konfiguriert
2)    InitP0P1Touch ("1") .. ("1 r m  p" ) ;   	Der Touchscreen wird aktiviert und 	konfiguriert, 
											         | | |  |													einfachste Konfiguration mit   InitP0P1Touch ("1"). 
															 | | |	 --------	p:   	P0 aussen,   sonst mittig.
															 | |  -----------	m:  	MSB oben/rechts,   sonst unten/links. 
															 | --------------	r:    	Rotiert horizontal,  sonst vertikal.
															 |--------------- 1…4: Gemeinsamer Schalter-/Tastertyp 
*/


				 


