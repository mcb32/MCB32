/** @file 0503_Bitman3_Einzelblinker_Funk.c
*  @brief  MCB32: Aufgabe Bitman 3 Portanst. via Funktion
*
*
*         Funktion für Portansteuerung: Verschiede Übungen für die Einführung in die 
*   			uP-Programmierung mit dem MCB32  /ARM M3
*         
*  @author    rw  / BZU  / Main Design
*  @author    rma / TBZ / REV D implmentation
*  @date      20.12.2014 / rma
*  @version   1.1    
*  @revision  D
*  @bug       No known bugs.
*/
//=================================Includes=====================================
#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen   


#define  uchar unsigned char   	// Macro-Trick um nicht immer unsigne char zu schreiben


// Sammlung von Funktionen (Prototypen
// *****************************************************
//	 einfache delay-subroutine mit Aufruf in ms
	   void delay(unsigned int ltvar);  
//   Funktion für das Schalten eines Bits
		 void Port1 (unsigned char BitNr, unsigned char EinAus);
// *****************************************************


int main( void ) 
{ 
 	uchar on  = 1;				// Konstante für on
  uchar off = 0;				// Konstante für off
 InitTouchP0P1("1");	// Touch aktiv, Horizontal gedreht, LSB rechts
  P1 = 0xB5;		  					// Ausgabe vorgegebener Wert
	while(1)									// Endlosschleife
  { 
	 //  Start Loesung mit Invertierung via Shiftoperation
		Port1(3,on);						// Bit 3 setzen
		delay(1000);           	// warte 1 Sekunde
		Port1(3,off);        		// Bit 3 loeschen		  					// Schiebe die Bit 4 mal nach Rechts
		delay(1000);            // warte 1 Sekunde
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

void Port1 (uchar BitNr, uchar EinAus)
{
  uchar Maske = 0x01 << BitNr;	// Maske schieben
  if (EinAus) P1 |=  Maske;
  else        P1 &= ~Maske;
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

