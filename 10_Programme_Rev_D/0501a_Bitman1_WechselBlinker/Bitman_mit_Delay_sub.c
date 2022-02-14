//-------------------------------------------------------
// Titel: 		Ein-Ausgabe 
//					Verschiede Übungen für die Einführung in die 
//   				uP-Programmierung mit einen Atmel T89C51ED2
//					InOut.c
// Specs:		Taktfrequenz f = 24.000 MHz
// Datum:		2014
// Autor:		TBZ E. Malacarne , 12.08.2014
// Bemerkung:   Siehe auch Invertierung_dez_Binaer.cpp   
//              für mehr Infos zur Invertierung
// Test:		läuft
// ------------------------------------------------------
#include <stm32f10x_cl.h>	// Mikrocontrollertyp
#include "P0P1Touch.h"	// Library mit P0-, P1-Definition

/* definiere IN-Output Bits für Ein und Ausgabe   */
#define A0      P1_0	// Ausgabeport P1_x

int Buffer;								// Buffervariable

// Sammlung von Funktionen 
// *****************************************************
   void delay(void);  // einfache delay-subroutine
// *****************************************************

int main( void ) 
{ 
 InitP0P1Touch(1111);	// Touch aktiv, Horizontal gedreht, LSB rechts
 while(1)									// Endlosschleife
  { 
	//  Start Loesung mit logischer Invertierung  ..........................
  //	Buffer =P0;		  						// LEse Schalter in Buffer
	//  Buffer = Buffer ^ 0xFF;					// logisches invertieren Von Buffer mit der Hilfe der Exor Funktion
	//  P1 = Buffer;							// Ausgabe des Buffers auf die Schnittstelle mit den LEDs
  //  Ende  Loesung mit logischer Invertierung ............................

  //  Start Loesung mit arithmetischer Invertierung  NOT ~ ...................
	Buffer =P0;		  						// LEse Schalter in Buffer	
	P1 = Buffer;                // gib Schalter nun aus
	delay();
	Buffer = ~Buffer;					   // arithmetischer invertieren mit der NOT (~) 
	P1 = Buffer;                 // Ausgabe des Buffers auf die Schnittstelle mit den LEDs							
	delay();
  }
}

// Subroutine ausprogrammiert
void delay(void)         // einfache delay-subroutine
{
	long ltvar =0;	   //;
	for (ltvar=0;ltvar< 500000; ltvar++);
}
