/** @file  SER_BSP2.C  
*  @brief  Serielles Einlesen eines Buchstabens vom Keyboard des PC
*          an USART2 PD6 und  Ausgabe an PD5 via Terminal am PC
*          USART Init via spezielle Funktionen der TouchP0P1-Library
*  @author  rma / TBZ   
*  @date    12.2.2016 
*  @version 2.0    
*  @bug     No known bugs. ...
*/
//=================================Includes===================================== 
#include <stm32f10x.h>            // Mikrocontrollertyp
#include "TouchP0P1.h"            // P0/P1,8Bit,Touchscreen und Grafik
#include <string.h>
//*****************************Implementation***********************************
void init_usart2(void);          // Funktion um Usart 2 mit 9600Bd zu initialisieren
void Ready_Sequenz(void);        // Sende eine StartupSequenz zum Terminal
void CRLF_Sequenz(void);        // Sende eine CRLF zum Terminal
//=================================main()======================================= 

int main(void)                         // Hauptprogramm
{
   char c = '0';                       // Starte mit Zeichen 0
   USARTInit(2, 0);                    // Default: USART2 ohne Intr. mit 9600Bd, 1,8,1,n
   InitTouchP0P1 ("1");
	 delay_ms(1000);
	// Ascii_Sequenz();
	 delay_ms(1000);
	 Ready_Sequenz();
   delay_ms(1000);
	
	
//		while(1)                            // Endlosschlaufe
//   {
//      
//     
//       c = 'a';               // Zeichen lesen von RX
//       P1=c;                           // Zeichen an P1 ausgeben
//       USARTWrite(2,c);                // Sende Zeichen c oder USARTWrite(2,'c'); // Sende Zeichen 'c'
//		   delay_ms(2);
//      
//   }
		
	while(1)                            // Endlosschlaufe
   {
      if(USARTtoRead(2))               // 1= Zeichen empfangen, 0 = kein zeichen empfangen
      {
       c = USARTRead(2);               // Zeichen lesen von RX
       P1=c;                           // Zeichen an P1 ausgeben
       USARTWrite(2,c);                // Sende Zeichen c oder USARTWrite(2,'c'); // Sende Zeichen 'c'
      }
   }
}

// ----------------------- Funktionen ---------------------------------
void Ready_Sequenz(void)
{
   int pos=0;
   char Text[] = "Ready:";      //
	 CRLF_Sequenz();
   for (pos = 0; pos < strlen(Text); pos++){
       USARTWrite(2,Text[pos]);                  // Sende ReadySequenz  
   }
}



void CRLF_Sequenz(void)
{
	USARTWrite(2,0x07);USARTWrite(2,0x0D);USARTWrite(2,0x0A);   // CR LF
}
