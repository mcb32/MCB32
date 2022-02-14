/** @file  USART2a.c  
*  @brief  Serielles Einlesen eines Buchstabens an USART2 PD6 
*          und um 1 erhöht Ausgabe an PD5 via Terminsl an PC
*          
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
void Ready_Sequenz(void)
{
   int pos=0;
   char Text[] = "Ready:";      //
   for (pos = 0; pos < strlen(Text); pos++){
       USARTWrite(2,Text[pos]);                  // Sende ReadySequenz  
   }
}
//=================================main()======================================= 
int main(void)                         // Hauptprogramm
{ 
   char c = '0';
                                              // USART2 initialisieren:   
   RCC->APB2ENR |= (1<<5)|1;        // PortD + AFIO Enable
   RCC->APB1ENR |= 1<<17;           // USART2 Clock ON 36MHz
   GPIOD->CRL   &= 0xF00FFFFF;      // PD5 AF Out 50MHz und ..
   GPIOD->CRL   |= 0x04B00000;      // PD6 In Floating 
   AFIO->MAPR   |= 1<<3;            // USART2 Remap TX-PD5 RX-PD6
   USART2->BRR   = 0xEA6;           // 9600Bd @ 36MHz 
   USART2->CR1  |= 3<<2;            // TX und RX Enable 
   USART2->CR1  |= 1<<13;           // USART Enable 
   Ready_Sequenz();                                          
   while(1)                         // Endlosschlaufe
   {
      if(USART2->SR&(1<<5))         // Zeichen empfangen
      {
         c = USART2->DR;            // Zeichen lesen von RX
       USART2->DR = c+1;            // Zeichen +1 an TX
      while(!(USART2->SR&(1<<7)));  // Warte bis TX frei
      }
   }
}
