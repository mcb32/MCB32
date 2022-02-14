/** @file  Serial4.c  
*  @brief  ADC-Wert an Terminal senden 
*          ADC wandelt Analogwert des Potentiometers via ADC 1
*          und sendet ihn digitalisiert im Hex-Code an Terminal an MCB32 
*          Mittelt die Messungen mit einem Mittelwertfilter
*          USART Init via spezielle Funktionen der TouchP0P1-Library
*  @author  rma / TBZ    
*  @date    12.2.2016 
*  @version 2.0    
*  @bug     No known bugs. ...
*/
//=================================Includes===================================== 
#include <stm32f10x.h>            // Mikrocontrollertyp
#include "TouchP0P1.h"            // P0/P1,8Bit,Touchscreen und Grafik
// **************************** Globale Variablen ******************************
char Buf[3] = ""; 
unsigned char accu_ret =0;
//*****************************Implementation***********************************
void ZeichenSenden(char ch)   // Simple Funktion. Kann auch anderst gelöst werden
{
  USARTWrite(2,ch); 
}
// Init ADC
void InitADC () {              		// ADC initialisieren
  ADCInit(1,"PC4");                  // Init auf ADC1 und Pin PC4 /kontinierliche Wandlung
}
// Lies ADC n mal und bilde Mittelwert (Tiefpassfilter-Mittelwertfilter)
char ADC_TP(unsigned char nmal){
 unsigned int accumulator=0, i=0;
   for (i=1;i<=nmal;i++){
   accumulator=accumulator+ADCGetVal(1);
   }
   return (char)(accumulator/nmal);
}
// Wandle DEZ in Hex für transfer
char* Dez_Hex (unsigned char d) {      // Dez-Hex-Wandlung
  if (d/16 <= 9) Buf[0] = d/16+48;     // MSD: '0'..'9'
  else           Buf[0] = d/16+65-10;  // MSD: 'A'..'F' 
  if (d%16 <= 9) Buf[1] = d%16+48;     // LSD: '0'..'9'
  else           Buf[1] = d%16+65-10;  // LSD: 'A'..'F'
  return Buf;
}


//=================================main()======================================= //---------------------------------------------------

int main (void) 
{
  unsigned char Wert = 0;
  unsigned char AnzahlMeas=100;   // Anzahl Messungen für TP Effekt
  InitADC();                     // ADC initialisieren
  USARTInit(2, 0);               // Default: USART2 ohne Intr. mit 9600Bd, 1,8,1,n
  InitTouchP0P1("1");            // Schalte Touchscreen ein
  while (1)
  {
     P1 = ADC_TP(AnzahlMeas);          // Analog von PC4 an P1 anzeigen
     if (Wert != P1)  {                // Falls Werte aendert
      ZeichenSenden(Dez_Hex(Wert)[0]); // MSD senden
      ZeichenSenden(Dez_Hex(Wert)[1]); // LSD senden
      ZeichenSenden(' ');              // Lehrzeichen senden
      Wert = ADC_TP(AnzahlMeas);
    }
  }
}


