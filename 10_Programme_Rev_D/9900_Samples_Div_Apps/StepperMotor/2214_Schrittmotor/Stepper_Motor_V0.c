//-----------------------------------------------------
// Stepper_Motor_V0.c   / 10.2.2017  / rma / TBZ 
// Protoyp für Schrittmotoren
// Siehe MCB32_Befehlsliste.
//-----------------------------------------------------
// #include <stm32f10x.h>							// Mikrocontrollertyp
#include "TouchP0P1.h"								// P0-,P1-,Touchscreen      
//#define IN1 P0_0
//#define IN2 P0_1
//#define IN2 P0_2
//#define IN2 P0_3
#define LOW 0
#define UP 1
#define DOWN 0
#define DisplayOn 1
#define DisplayOff 0
#define DisplaySwitch P0_7
#define delayMotor_min 1       // xx ms
// Funktionsprototypen
void StepperOff(void)  ;
void StepperSave()   ; 
void StepperOn(void)  ;
void ldelay_ms(unsigned long delay_ms);


int myPos[] = {36, 108, 180, 252, 324}; // Zeiger Positionen in °

// Schrittmotor Geschwindigkeit je höher die Zahl desto Langsamer fährt der Motor 1=Schnell
int motorSpeed = 2;

int motorSteps = 4096;    // Motorschritte für 360°

int AktuellePos = 0;      // Hilfsvariable zum Speichern der Aktuellen Position
int SollPos = 0;          // Hilfsvariable zum Speichern der Soll Position


// Hilfsvariablen zum Abspeichern der bestromten Wicklungen beim Ausschalten
unsigned char IN1 = 0;
unsigned char IN2 = 0;
unsigned char IN3 = 0;
unsigned char IN4 = 0;
unsigned char Direction = UP;
int main(void) 												// Hauptprogramm
{ 
	unsigned char Button = 0;        		// setze Button(PA0) auf 0 nach PON
   unsigned char oldButton = 0;     		// setze Button-MEM auf 0 nach PON

	InitTouchP0P1("1");										// P0P1-Touchscreen ON =1 oder OFF =0
	// Schalte ON Board LEDs EIN
   GPIOE->CRH   &= 0x00000000;      		// Configure the GPIOE for 
   GPIOE->CRH   |= 0x22222222;      		// General purp.output push-pull,2MHz
	
   P1 = 1;															// PA0-Zähler nullen
	
   while(1)															// Endlosschlaufe
	{

/**
    * @note:  Stepermotor Verdrahtung / Wiring
* Stepper Motor Tabelle Vorwärts CCW, die Farben beschreiben die Motordrahtfarben
* also: Blue ist mit IN1 und dann P1_0 verbunden, 
*       Pink ist mit IN2 und dann P1_1 verbunden usw. 

*              Stepp --> 1     2     3     4     5     6     7     8
*      ---------------|------------------------------------------------
*      P1_3 / OR   4  |  1     1     0     0     0     0     0     1
*      P1_2 / Yel  3  |  0     1     1     1     0     0     0     0      
*      P1_1 / PIN  2  |  0     0     0     1     1     1     0     0
*      P1_0 / BLU  1  |  0     0     0     0     0     1     1     1
*      ------------------------------------------------------------------
*      Hexcode for P1:  08    0C     04    06    02    03    01    09    
*      Der HExCode wird P1 von Links nach rechts zugewiesen damit fahren wir CCW
*/      
// Demo für volle Auflösunge CCW (100Hz)
      // Half-step drive
		if(DisplaySwitch == DisplayOn){
		    // FAST Half-step drive
				P1=0x08; ldelay_ms(delayMotor_min);
				P1=0x0C; ldelay_ms(delayMotor_min);
				P1=0x04; ldelay_ms(delayMotor_min);
				P1=0x06; ldelay_ms(delayMotor_min);
				P1=0x02; ldelay_ms(delayMotor_min);
				P1=0x03; ldelay_ms(delayMotor_min);
				P1=0x01; ldelay_ms(delayMotor_min);
				P1=0x09; ldelay_ms(delayMotor_min);

//      if(Direction==UP){P1++;} else{P1--;}
//			ldelay_ms(10);
//      if(P1 ==0x0F){Direction=DOWN;}          		// Reset P1 mit Button 1
//      if(P1 ==0x00){Direction=UP;}          		// Reset P1 mit Button 1
	    } else {
			if(DisplaySwitch == DisplayOff)
				{
				 // SLOW  // Wave Drive
				 P1=0x08; ldelay_ms(10);
				 P1=0x04; ldelay_ms(10);
				 P1=0x02; ldelay_ms(10);
				 P1=0x01; ldelay_ms(10);
				 StepperSave();						// Speichere den letzten Zustand der Ansteuerung in den Variablen IN1...IN4 ab
				 StepperOff();						// Schalte den Motor stromlos
				 ldelay_ms(300);	
				 StepperOn();							// Schalten den Motor wieder ein.
				}
			}
	}
	
} 

/*************************(Funktion zum auschalten des Motores)****************************/
// Wenn der Schrittmotor nicht ausgeschalten wird verbraucht er zum Bestromen
// der Wicklungen bis zu 250mA, dies generiert unnötige Wärme wenn der Motor weiterläufft

void StepperOff(void)
{
  ldelay_ms(10);     //Warten damit die Mechanik nicht die Position überschiesst
 // StepperSave();     //Motor zustand Speichern

  P1_1= LOW;               //Motorwicklungen ausschalten     IN2    P1_1
  P1_2= LOW;               // IN3   P1_2
  P1_0= LOW;               // IN1   P1_0
  P1_3= LOW;               // IN4   P1_3
}

         /************************(Funktion zum einschalten des Motores)****************************/
void StepperOn(void)
{
  P1_0 =  IN1; //Motorwicklungen letzter Zustand wiederherstellen
  P1_1 =  IN2;
  P1_2 =  IN3;
  P1_3 =  IN4;
  ldelay_ms(10 );
}

/*************(Funktion zum Speichern der bestromten Wicklungen des Motores)***************/
void StepperSave(void)
{
  IN1 = P1_0;    //  
  IN2 = P1_1;    //
  IN3 = P1_2;    //
  IN4 = P1_3;    //
}

/*************(Funktion für lokale delay Routine das delay_ms(x) nicht funktioniert wenn Display OFF ist **************/
void ldelay_ms(unsigned long delay_ms){
   unsigned long l_lms=12000;                     // for(t=12000; t>0; t--); // Verzögerung 1ms
   unsigned long i=0;
   unsigned long  ldv;
   for(ldv=delay_ms;ldv>0;ldv--){
      for(i=l_lms;  i>0; i--);
   }

}
