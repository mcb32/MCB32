/** @file  grafikfunkionen_5.c
*  @brief  Zeigt die grundlegenden Grafikfunktionen  Version I von MCB32
*
*  Die Funktion für den Farbraum ist ausgelagert. 
* 
*         
*  @author  mal  
*  @date    28.3.2015
*  @version 1.0    
*  @bug     No known bugs. ...
*  @ideas   Array nutzen um Bild vorzubereiten um dann das Memeory zu füllen: Farbmemory[1][1]= rand( );
*/
//=================================Includes===================================== 
#include "generic_lib.h"          		// Defines, Variablen und Prototypen
#if defined _grafik_lib_h         		// Prüfe ob nicht irgendwo die Grafik Lib schon eingebunden worden ist.  
#echo "_grafik_lib_h ist definiert"		// kann auch wegfallen
#endif
#include "grafik_lib.h"           		// Defines, Variablen und Prototypen


//=================================main()======================================= 

int main(void) 								               // Hauptprogramm
{   

	  long t; 										             // Verzoegerungsvariable
	  float rad;
	  WORD uc_va1,color_toggle=0,uc_va2;		  // Hilfsvariablen;
	 	InitTouchScreen();                       // Init. der Display Hardware
	  setScreenDir (VER);                      // setze Richtung von Display. 0,0 bei Resettaster
	  if(getScreenDir()==VER){                 // Funktioniert nur mit setScreenDir (VER); 
		   clearScreen (0x39C7);                 // 0x841  0x39C7 sollte grau sein
		   ldelay_ms(1000);                     // Warte für Show, 100ms
		}	

     // Textfunktionen
//    textxy(" MCB32 Lib Version:", 2, 32, BLACK, YELLOW);
//	  printAt(2,"--------------------------------- ");       // Schreibe auf der 2ten Zeile den Text
//		setTextcolor (BRIGHT_CYAN);                            // wechsle Textfarbe
//		printLn ( "" ); printLn ( "Kreis" ); setTextcolor ( WHITE );  //springe zur nächsten Zeile und schreibe dann KREIS
//    print ( "               Ellipse" ); 
    // Zeichne
	  circle(50,80,20,2,GREEN,0);                      // Zeichne Kreis
	  ellipse(100, 80, 10,20,1,YELLOW,1);              // Zeichne Ellipse
	  rectan(140,60,180,100,1,BRIGHT_RED,1);          // Zeichne Rechteck
		plotFilledRect (2, 308, 10, 10, RED );      // zeichne ein gefülltes Rechteck  unten links
	  line(5,110,315,110,2,WHITE);                     // Zeichne Linie
	
	  // Zeichne mit plotDot() ein erstes Muster
	  for(uc_va1 =0;uc_va1<160;uc_va1++){
			for (t=0; t<100;t++){
			  plotDot(10+uc_va1+t/2,115+t*2,uc_va1*t*5);      // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
	     }
		}

		ldelay_ms(200);                    // Warte für Show
	  clearScreen (BLACK);
		ldelay_ms(200);                    // Warte für Show

		// Zeichne mit plotDot() ein weiteres Muster
	  for(uc_va2=1;uc_va2>0;uc_va2--){
			for(uc_va1 =0;uc_va1<239;uc_va1++){
			 for (t=20; t<319;t++){
			  plotDot(0+uc_va1,0+t,((uc_va1*(t-20)*uc_va2)%0xFFFF));      // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
			 }
			 // printAt ( 0, ":" );  printHex ( 16, uc_va1*(t-20)*uc_va2*5 );print ("  :" ); printHex ( 8, uc_va2 );
		  }
		 // printAt ( 0, ":" );  printHex ( 32, ((uc_va1*(t-20)*uc_va2)%0xFFFF) );print ("  :" ); printHex ( 8, uc_va2 );
		}
		ldelay_ms(200);                    // Warte für Show
		clearScreen (BLACK);
      // Version 2
		for(t=20; t<320;t++ ){     // y Achse
			 for (uc_va2=0;uc_va2<240;uc_va2++){               // x-Achse
			   plotDot(0+uc_va2,t,(((uc_va2/8)<<11)|(((unsigned int)(t/5))<<5)|((uc_va2/8))));      // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
			 }
		}
		ldelay_ms(200);                    // Warte für Show
			
    zeichne_farbraum_rechteckig(); 		 // Zeichne den Farbraum auf die ganze Fläche. Siehe Funktion für mehr Informationen
			
    plotDot(0,0,0xFFFF);               // Zur Orientierung eine weisser Punkt
    plotDot(239,319,0xFFFF);		
			
		// Konfiguriere nun Port um anzuzeigen dass der Touch berührt wurde	
		GPIOInit("PEH",00000000);
		GPIOE->CRH   &= 0x00000000;                  // Konfiguriere GPIOE für
		GPIOE->CRH   |= 0x22222222;                  // General purpose output push-pull, 2MHz
		GPIOPutByte("PEH",0xAA);
    // SysTick_Config(72* 1030);  		           // SysTimer, SysTick_Handle() alle 1000us	   
		//delay_ms(500);                             // geht nicht wenn InitTouchP0P1("0");
		while(1){
			getTSCxy();           // initialisiert den Touch und liest die Werte für getTSCx() und getTSCy() ein.
//			printAt(8, "TSC:");
//			if(getTSCx() <= 320){printDec(5, getTSCx());}     // grenze Bereich für Rückgabewerte ein und gib sie aus.
//			if(getTSCy() <= 320){printDec(5, getTSCy());}
//			printAt(13, ""); printBin(1,getTSCtouched()); 		// Schreibe Berührungsstatus auf den Screen
			uc_va1 = getTSCtouched();				                  // Hole Touchwert 0,1  Debugging
			GPIOPutByte("PEH",getTSCtouched());               // zeige via LED ob Touch gedrückt wurde
		// Zeichne Sinus		
			 if(uc_va1==1){
					for (t=0; t<238;t++){
			     rad = 4*t * PI / 180;                        // Berechnen des Bogenmaßwinkels
			     if(color_toggle==0) {
						 plotDot(10+t,(20+12*sin((double)(rad))),BRIGHT_BLUE);   // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
					 }
					 else {
						 plotDot(10+t,(20+12*sin((double)(rad))),WHITE);   // oder  plotDot(140+uc_va1+t,115+t,((uc_va1*8)+t*32));
					 }
					}
					color_toggle=color_toggle^0x01;     // Toggle Color für den nächsten SInus. Spielerei
			  }
		}
}



