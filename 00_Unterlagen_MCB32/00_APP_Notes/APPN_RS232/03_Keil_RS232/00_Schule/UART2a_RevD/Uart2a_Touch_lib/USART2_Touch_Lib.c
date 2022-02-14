/** @file  USART2_Touch_Lib.c  
*  @brief  Serielles Einlesen eines Buchstabens an USART2 PD6 
*          und um 1 erhöht Ausgabe an PD5 via Terminsl an PC
*          USART Init via spezielle Funktionen der TouchP0P1-Library
*  @author  rma / TBZ 
*  @author  rw  / BZU   
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
void Ready_Sequenz(void);        // Sende Ready an Terminal
//=================================main()======================================= //---------------------------------------------------

int main(void)                         // Hauptprogramm
{
   char c = '0';                       // Starte mit Zeichen 0
   USARTInit(2, 0);                    // Default: USART2 ohne Intr. mit 9600Bd, 1,8,1,n
   InitTouchP0P1 ("1");
   Ready_Sequenz();

   while(1)                            // Endlosschlaufe
   {
      if(USARTtoRead(2))               // 1= Zeichen empfangen, 0 = kein zeichen empfangen
      {
       c = USARTRead(2);               // Zeichen lesen von RX
       c = c+1;                        // Zeichen +1 an TX
       USARTWrite(2,c);                // Sende Zeichen c oder USARTWrite(2,'c'); // Sende Zeichen 'c'
      }
   }
}

// ------- Funktionen
/** @brief  void init_usart2(void)
*           Initialisiert USART2 mit default Wert:9600Bd, 1,8,1,nsw. weitere Optionen sind:  
*           USARTInit(1,0);          // Schalte USART1 mit 19200Bd ein (Default), ohne IR
*           USARTInit(2,0);          // Schalte USART1 mit  9600Bd ein (Default), ohne IR
*           USART1->BRR   = 0x1D4C;  // USART1: 9600Bd @ 72MHz benötigt einen Teiler von 468.75 
*                                    // siehe RefManual Page 792
*  @author  rma / TBZ  
*  @date    12.2.2016  
*  @version 1.0    
*/
void init_usart2(void)
   {
   // USART2 initialisieren:   
      USARTInit(2, 0); // Default: USART2 ohne Intr. mit 9600Bd, 1,8,1,n
       
   }  
/** @brief  void Ready_Sequenz(void)
*           Sende eine Sequenz an das Terminal resp. COM  
*  @author  rma / TBZ  
*  @date    12.2.2016  
*  @version 1.0    
*/   
void Ready_Sequenz(void)
{
   int pos=0;
   char Text[] = "Ready:";      //
   for (pos = 0; pos < strlen(Text); pos++){
       USARTWrite(2,Text[pos]);                  // Sende ReadySequenz  
   }
}