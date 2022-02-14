/** @file 0405_bitman.c
*  @brief  Einbit Ein-Ausgabe und Verarbeitung MCB32
*
*          Zeigt das Lesen und Schreiben einzelner 
*          Portbits und die logischen Operationen
*         
*  @author rw  / BZU  / Main Design
*  @author rma / TBZ / REV D implmentation
*  @date     12.11.2014 / rma
*  @version 1.0    
*  @bug No known bugs.
*/
//=================================Includes=====================================
#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen   


/* definiere IN-Output Bits für Ein und Ausgabe   */
#define E0      P0_0	// Eingabeport P0_x
#define E1      P0_1	// Eingabeport P0_x
#define A0      P1_0	// Ausgabeport P1_x
#define A1      P1_1	// 
#define Alarm   P1_2	//
char bTemp = 0;              // Beliebige Bitvariable via Char 
long ltvar =0;	//
int main (void)              // Hauptprogramm 
{
  InitTouchP0P1("1");	       // Touch aktiv, Horizontal gedreht, LSB rechts
  while(1)                     	// Endlosschleife 
  { 
    A0 = 1;                    	// Konstante Ausgabe 0/1
    Alarm = 1;                    	// Konstante Ausgabe 0/1
    A1 = E1;                   	// Bitdurchschaltung
    A1 = !E1;                  	// Invertieren
    A1 = E0 & E1;              	// bitweise AND-Verknüpfung
    A1 = E0 | E1;              	// bitweise OR-Verknüpfung
    A1 = E0 ^ E1;              	// bitweise EXOR-Verknüpfung
    /*      Fragen zum beantworten        */
    A0 = (E0==0);				// Z1
    while (!E1);               			// Z2
    if ((P0 & 8)==0) A1 = 1;   			// Z3
    P1 = P0 | 128;             			// Z4
    for (ltvar=120000; ltvar>0; ltvar--);	// Z5
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
