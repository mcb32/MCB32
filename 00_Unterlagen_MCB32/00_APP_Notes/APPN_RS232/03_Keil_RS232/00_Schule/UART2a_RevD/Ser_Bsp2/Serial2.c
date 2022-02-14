/** @file  P4-Terminal.c  
*  @brief  Schalterstellungen an P0 an MCB32 senden
*          und an P1 anzeigen 
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
void ZeichenSenden(char ch);   // Simple Funktion. Kann auch anderst gelöst werden
void Ready_Sequenz(void);        // --------------- Sende Ready_Sequenz
//=================================main()======================================= //---------------------------------------------------

int main (void) 
{
  char ch=0;
  USARTInit(2, 0);            // Default: USART2 ohne Intr. mit 9600Bd, 1,8,1,n
  InitTouchP0P1("1");        // Schalte Touchscreen ein
  Ready_Sequenz();
  while (1)
  {
    if (P0 != ch)          // Falls Eingabe geaendert
    {
      ch = P0;                // lese Port P0
      P1 = P0;                // zeige Port P0 auch an Port P1 an
      ZeichenSenden(ch);
    }
  }

}

// ---------------- Funktionen

void ZeichenSenden(char ch)   // Simple Funktion. Kann auch anderst gelöst werden
{
  USARTWrite(2,ch); 
}
// --------------- Sende Ready_Sequenz
void Ready_Sequenz(void)
{
   int pos=0;
   char Text[] = "Ready:";      //
   for (pos = 0; pos < strlen(Text); pos++){
       USARTWrite(2,Text[pos]);                  // Sende ReadySequenz  
   }
}


