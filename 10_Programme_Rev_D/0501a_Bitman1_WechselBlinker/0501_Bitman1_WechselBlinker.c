/** @file  0501_Bitman1_WechselBlinker
*  @brief  Ein-Ausgabe Bitman1_WechselBlinker
*
*          Ein-Ausgabe Bitman1_WechselBlinker 
*					 Verschiede Übungen für die Einführung in die 
*   			 uP-Programmierung mit dem MCB32  /ARM M3.
*          Bemerkung:   Aufgabe 1 Wechselblinker mit direkter Ausgabe
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


// Sammlung von Funktionen 
// *****************************************************
//	 einfache delay-subroutine mit Aufruf in ms
	   void delay(unsigned int ltvar);  
// *****************************************************

int main( void ) 
{ 
 InitTouchP0P1("1");	// Touch aktiv, Horizontal gedreht, LSB rechts
 while(1)									// Endlosschleife
  { 
	//  Starte Loesung mit direkter Ausgabe der Port Werte 
	P1 = 0x0F;		  						// gib 0Fx nun aus
	delay(1000);
	P1 = 0xF0;		  						// gib F0x nun aus
	delay(1000);
  }
}

// Subroutine delay ausprogrammiert
// Paramter: ltvar = Anzahl ms
void delay(unsigned int ltvar )         // einfache delay-subroutine
{
	unsigned int i =0;	                 // lokale Variable für die Anzahl-ms
	unsigned int itvar=0;	               // lokale Variable für 1ms
	for (i=ltvar;i >0 ; i--)
	{
		for (itvar=12000;itvar> 0; itvar--);   // 12000 ca. 1ms
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

