/** @file P0P1Touch.c
*  @brief  Ein-Ausgabe MCB32
*
*          Schalterstellungen am Eingabeport P0 lesen
*          und an Ausgabeport P1 (LED) ausgeben  
*         
*  @author    rw  / BZU  / Main Design
*  @author    rma / TBZ / REV D implmentation
*  @date      12.7.14 / rma
*  @version   1.0    
*  @revision  D
*  @bug       No known bugs.
*/
//=================================Includes=====================================
#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen   

int main (void) 
{                                                    
  P1 = 0x00;                  // init P1
  InitTouchP0P1("1");					// P0,P1 auf Touchscreen ON. Touchscreen übernimmt Simulation Schalter und LED
	
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


				 


