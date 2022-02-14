/** @file  Serial2.c  
*  @brief  Text an Terminal senden
*          MCB32 sendet den Text "Ready" and das Terminal am PC
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
void ZeichenSenden(char ch)   // Simple Funktion. Kann auch anderst gelöst werden
{
  USARTWrite(2,ch); 
}
//=================================main()======================================= //---------------------------------------------------

int main (void) 
{
  int pos=0;
  char Text[] = "Ready";      // 
  USARTInit(2, 0);            // Default: USART2 ohne Intr. mit 9600Bd, 1,8,1,n
  InitTouchP0P1 ("0");
  for (pos = 0; pos < strlen(Text); pos++){
     ZeichenSenden(Text[pos]);
  }
  while (1);                  //  Endlosschlaufe
}


