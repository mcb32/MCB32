/** @file  grafik_lib.h
*  @brief  Header File für Defines, Variablen und prototypen  (MCB32)
* 
*         
*  @author  mal  
*  @date    28.3.2015
*  @version 1.0    
*  @bug     No known bugs. ...
*/
//=================================Includes=====================================
#ifndef _grafik_lib_h // nachdem das *.h fFile hier zum ersten mal geladen wird soll sich der Präprozessor das EInbinden merken
    #define _grafik_lib_h
     // in anderen Files #include "grafik_lib.h"         // Defines, Variablen und Prototypen
    
    #include "generic_lib.h"          // Defines, Variablen und Prototypen
    #include <stm32f10x.h>				// Mikrocontrollertyp
    #include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen und Grafik
    #include <math.h>             // lib für Sinus
    #include <stdlib.h>						// lib für rand()
    
    /* DEBUGLEVEL SETUP   */
    #define DEBUGLEVEL 0
    #if (DEBUGLEVEL >= 1)
        #define  printdebug1  printAt(16, "FarbRasterPoints");  printDec(4, FarbRasterPoints); \
        		                  printAt(17, "ucFarbRaster" ); printDec(2,ucFarbRaster );  print (":Sy " ); printDec (2 ,wStartGuiyV ); print (":Ey " ); printDec (2 ,wEndGuiyV );
        #define  printdebug2  printAt(18,""); print ("R" ); printDec (3, wRED);print (":G" ); printDec ( 3,wGREEN );print (":B" );printDec ( 3,wBLUE );print (":" );printDec ( 3,ucFarbRasterPoint ); print (":" );printDec ( 3,FarbRasterPoints );
    #else 
        #define  printdebug1   // mache nichts
        #define  printdebug2
    #endif 
    #if (DEBUGLEVEL >= 2)
        #  define printdebug3
    #else 
        #  define printdebug3
    #endif 
    // ENDE Debug LEvel Setup
    
    
    
    
    
    
    // Defines für Farbkreis um alle Farben richig darzustellen
    #define Redmax       31U                  // 5 Bits für ROT (Bit15....Bit11)
    #define Bluemax      31U                  // 5 Bits für ROT (Bit 4....Bit 0)
    #define Greenmax     63U                  // 6 Bits für ROT (Bit10....Bit 5)
    #define StartGuixV    0U
    #define StartGuiyV    0U
    #define EndGuixV    239U                  // U= unsigned int für das Ende der X Koordinaten
    #define EndGuiyV    319U
    #define FarbRaster     7									// 7 ist FIX bei dieser Version. ANzahl der Farbkombinationen die dargestellt werden. R00, RG0, 0G0........
    
    // Variablen für Farbkreis um alle Farben darzustellen
    WORD wStartValueRED   =0;            // ab diesem Wert starte die Farbrampe
    WORD wStartValueGREEN =0;
    WORD wStartValueBLUE  =0;
    WORD wEndValueRED     =0;            // bis zu  diesem Wert läuft die Farbrampe
    WORD wEndValueGREEN   =0;
    WORD wEndValueBLUE    =0;
    WORD wRED							=0;             // RED Wert welcher berechnet wird
    WORD wGREEN						=0;             // GREEN Wert welcher berechnet wird
    WORD wBLUE						=0;             // BLUE Wert welcher berechnet wird
    WORD wStartGuiyV      =0;             // Variable für dynamische Berechnung der aktuellen Startposition für die y Achse
    WORD wEndGuiyV        =0;             // Variable für dynamische Berechnung der aktuellen Endposition   für die y Achse
    uchar ucFarbRaster		=0;						  // Farbrastercounter
    WORD FarbRasterPoints	=0;							// Anzahl Punkte pro Farbsektor
    uchar ucFarbRasterPoint	=0;						// Aktueller Punkt im zu zeichnenden Farbsektor
    uchar ucZeichneLiniezwSequenz =OFF;
    // Zeichne eine Linie zwischen den Sequenzen

// unsigned int Farbmemory[320][240];  // Array für Farb speicher in RAM 

void zeichne_farbraum_rechteckig(void)   // Zeichne den Farbraum über den ganzen Bildschirmuchar Linie_ON
   {
        WORD uc_va1,uc_va2;		// Hilfsvariablen;
        // Version 3 Zeichne Farbraum RGB auf. 
        /* Setze Werte für die erste Sequenz von R00 bis RG0  (RED GREEN BLUE = RGB)   */	
        wStartValueRED	=	Redmax;	wEndValueRED =Redmax;				// setze Rot auf Maximum da ROT in diesem Fall gesetzt ist aber nicht ändern soll
        wStartValueGREEN=	0;		wEndValueGREEN =Greenmax;			// setze GREEN auf 0..Maximum da GREEN in diesem Fall von minimaler Sättigung auf maximale ändern soll
        wStartValueBLUE=	0;		wEndValueBLUE =0;			        		// setze BLUE auf OFF da BLUE in diesem Fall nicht erscheinen soll
        ucFarbRaster=1;											// erste Sequenz von R00 bis RG0
        wStartGuiyV=0;												// Start Koordinate für Y. Erster Punkt der Sequenz in Y-Richtung
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-wStartGuiyV);  // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden
        ucFarbRasterPoint=0;                                                    					// Setze Zähler der aktuellen Y-Position (Farbsättigung) innerhalb der Sequenz  auf 0. 
        wEndGuiyV=wStartGuiyV+FarbRasterPoints;					          // BErechne die neue Position wo die Sequenz aufhören soll
        printdebug1;												// um interne Werte darzustellen kann mit dem Debuglevel die Ausgabe gesteuert werden
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wRED	=	((wEndValueRED*uc_va1)/EndGuixV);   									 // setze Farben zusammen
                wGREEN	=	(wEndValueGREEN*ucFarbRasterPoint*uc_va1)/(FarbRasterPoints*EndGuixV); 
                wBLUE   	=	0;      
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));					// zeichne nun die aktuelle Farbe
            }
            printdebug2;                              // Macro aus Debugumgebung. Kein C-Code
            ucFarbRasterPoint++;              // Erhöhe FarbrasterPoint -> damit wird die Intensität der neu dazukommenden Farbe erhöht. (0 .. 100% innerhalb der Sequenz)
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            // Zeichne Linie 1 (Position xxx). Nur wenn der Paramter ZeichneLinie = ON ist
            printAt(8, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // --------------------------------- Erste Sequenz Fertig  Wir haben nun RG0 erreicht. 
        /* Setze nun Werte für 2te Sequenz   von RG0 bis 0G0  (RED GREEN BLUE = RGB)   */	
        wStartValueRED		=Redmax;		wEndValueRED =0;					    // starte mit Rot auf Maximum und fahre es auf 0 runter
        wStartValueGREEN	=Greenmax;	wEndValueGREEN =Greenmax;			// setze GREEN auf Maximum da GREEN in diesem Fall nicht geändert wird
        wStartValueBLUE	=0;			wEndValueBLUE =0;			        // setze BLUE auf OFF da BLUE in diesem Fall nicht erscheinen soll
        wStartGuiyV=((ucFarbRaster*EndGuiyV)/FarbRaster)+2*ucZeichneLiniezwSequenz;					// Endwert des letzten Loops	+1 für neue Linie  (Linie 41)
        ucFarbRaster=2;
        
        // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden -2 wegen der Linie (AUFPASSEN) (-1 am Schluss weil der erste Loop schon zählt)
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-(wStartGuiyV-2*ucZeichneLiniezwSequenz))-1;  
        
        wEndGuiyV=wStartGuiyV+FarbRasterPoints;					                                     // BErechne die neue Position wo das Zeichnen aufhören soll
        ucFarbRasterPoint=0;                                                   // Setze Zähler der aktuellen Position innerhalb der Sequenz  auf 0
        printdebug1;
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wRED=((uc_va1*wStartValueRED-((uc_va1*wStartValueRED*ucFarbRasterPoint)/FarbRasterPoints))/EndGuixV);    // setze Farben zusammen
                wGREEN=((wEndValueGREEN*uc_va1)/EndGuixV);                                     // ((uc_va2*uc_va1)/EndGuixV); 
                wBLUE=0;      
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));
            }
            printdebug2;
            ucFarbRasterPoint++;
            // if((uc_va2+1)%5==0){ucFarbRasterPoint = ucFarbRasterPoint+5;}  // ucFarbRasterPoint wird unregelmässig erhöht um Rot Anteil länger wirken zu lassen
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            printAt(9, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // Zeichne Linie 1 (Position xxx)
        /* Zweite Sequenz Fertig  Wir haben nun RG0 erreicht.   */
        
        
        
        
        /* Setze nun Werte für 3te Sequenz   von 0G0 bis 0GB  (RED GREEN BLUE = RGB)   */	
        wStartValueRED=0;			wEndValueRED =0;					                 // ROT OFF
        wStartValueGREEN=Greenmax;	wEndValueGREEN =Greenmax;			      // setze GREEN auf Maximum da GREEN in diesem Fall nicht geändert wird
        wStartValueBLUE=0;					wEndValueBLUE =Bluemax;			        // ändere BLUE von 0.. max
        
        wStartGuiyV=((ucFarbRaster*EndGuiyV)/FarbRaster)+3*ucZeichneLiniezwSequenz;					// ACHTUNG: Diese Position ist wichtig. Endwert des letzten Loops	+1+1 für neue Linie und Strich dazwischen
        
        ucFarbRaster=3;	
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-(wStartGuiyV-3*ucZeichneLiniezwSequenz))-1;  // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden -2 wegen der Linie (AUFPASSEN) (-1 am Schluss weil der erste Loop schon zählt)
        wEndGuiyV=wStartGuiyV+FarbRasterPoints;					                                     // BErechne die neue Position wo das Zeichnen aufhören soll
        ucFarbRasterPoint=0;                                      // Setze Zähler der aktuellen Position innerhalb der Sequenz  auf 0
        printdebug1;
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wRED=0;
                wGREEN=((wEndValueGREEN*uc_va1)/EndGuixV);                                     // ((uc_va2*uc_va1)/EndGuixV); 
                wBLUE=(wEndValueBLUE*ucFarbRasterPoint*uc_va1)/(FarbRasterPoints*EndGuixV);      
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));
            }
            printdebug2;
            ucFarbRasterPoint++;
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            printAt(10, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // Zeichne Linie 1 (Position xxx)
        /* Dritte Sequenz Fertig  Wir haben nun 0GB erreicht.   */
        
        /* Setze nun Werte für 4te Sequenz   von 0GB bis 00B  (RED GREEN BLUE = RGB)   */	
        wStartValueRED=0;			wEndValueRED =0;					                 // ROT OFF
        wStartValueGREEN=Greenmax;	wEndValueGREEN =0;			      // setze GREEN auf Maximum da GREEN in diesem Fall nicht geändert wird
        wStartValueBLUE=0;					wEndValueBLUE =Bluemax;			        // ändere BLUE von 0.. max
        
        wStartGuiyV=((ucFarbRaster*EndGuiyV)/FarbRaster)+4*ucZeichneLiniezwSequenz;					// ACHTUNG: Diese Position ist wichtig. Endwert des letzten Loops	+1+1 für neue Linie und Strich dazwischen
        
        ucFarbRaster=4;	
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-(wStartGuiyV-4*ucZeichneLiniezwSequenz))-1;  // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden -2 wegen der Linie (AUFPASSEN) (-1 am Schluss weil der erste Loop schon zählt)
        wEndGuiyV=wStartGuiyV+FarbRasterPoints;					                                     // BErechne die neue Position wo das Zeichnen aufhören soll
        ucFarbRasterPoint=0;                                                   // Setze Zähler der aktuellen Position innerhalb der Sequenz  auf 0
        printdebug1;
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wRED=0;
                wGREEN=((uc_va1*wStartValueGREEN-((uc_va1*wStartValueGREEN*ucFarbRasterPoint)/FarbRasterPoints))/EndGuixV);    // setze Farben zusammen
                wBLUE=(wEndValueBLUE*uc_va1)/(EndGuixV);      
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));
            }
            printdebug2;
            ucFarbRasterPoint++;
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            printAt(11, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // Zeichne Linie 1 (Position xxx)
        
        /* Vierte Sequenz Fertig  Wir haben nun 00B erreicht.   */		
        
        /* Setze nun Werte für 5te Sequenz   von 00B bis R0B  (RED GREEN BLUE = RGB)   */	
        wStartValueRED=0;						wEndValueRED 		=Redmax;	    // ROT von 0 bis max
        wStartValueGREEN=0;					wEndValueGREEN 	=0;			      // Green OFF
        wStartValueBLUE=0;					wEndValueBLUE 	=Bluemax;			// ändere BLUE von 0.. max
        
        wStartGuiyV=((ucFarbRaster*EndGuiyV)/FarbRaster)+5*ucZeichneLiniezwSequenz;					// ACHTUNG: Diese Position ist wichtig. Endwert des letzten Loops	+1+1 für neue Linie und Strich dazwischen
        
        ucFarbRaster=5;	
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-(wStartGuiyV-5*ucZeichneLiniezwSequenz))-1;  // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden -2 wegen der Linie (AUFPASSEN) (-1 am Schluss weil der erste Loop schon zählt)
        wEndGuiyV=wStartGuiyV+FarbRasterPoints;					                                     // BErechne die neue Position wo das Zeichnen aufhören soll
        ucFarbRasterPoint=0;                                                   // Setze Zähler der aktuellen Position innerhalb der Sequenz  auf 0
        printdebug1;
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wGREEN =0;
                wRED = (wEndValueRED*ucFarbRasterPoint*uc_va1)/(FarbRasterPoints*EndGuixV);      // setze Farben zusammen
                wBLUE=(wEndValueBLUE*uc_va1)/(EndGuixV);      
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));
            }
            printdebug2;
            ucFarbRasterPoint++;
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            printAt(12, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // Zeichne Linie 1 (Position xxx)
        
        /* Fünfte Sequenz Fertig  Wir haben nun R0B erreicht.   */				
        
        /* Setze nun Werte für 6te Sequenz   von R0B bis RGB  (RED GREEN BLUE = RGB)   */	
        wStartValueRED=0;						wEndValueRED 		=Redmax;	    // ROT von 0 bis max
        wStartValueGREEN=0;					wEndValueGREEN 	=Greenmax;			      // Green OFF
        wStartValueBLUE=0;					wEndValueBLUE 	=Bluemax;			// ändere BLUE von 0.. max
        
        wStartGuiyV=((ucFarbRaster*EndGuiyV)/FarbRaster)+6*ucZeichneLiniezwSequenz;					// ACHTUNG: Diese Position ist wichtig. Endwert des letzten Loops	+1+1 für neue Linie und Strich dazwischen
        
        ucFarbRaster=6;	
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-(wStartGuiyV-6*ucZeichneLiniezwSequenz))-1;  // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden -2 wegen der Linie (AUFPASSEN) (-1 am Schluss weil der erste Loop schon zählt)
        wEndGuiyV=wStartGuiyV+FarbRasterPoints;					                                     // BErechne die neue Position wo das Zeichnen aufhören soll
        ucFarbRasterPoint=0;                                                   // Setze Zähler der aktuellen Position innerhalb der Sequenz  auf 0
        printdebug1;
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wGREEN =(wEndValueGREEN*ucFarbRasterPoint*uc_va1)/(FarbRasterPoints*EndGuixV);   // Farbe kommt langsam dazu
                wRED = (wEndValueRED  *uc_va1)/(EndGuixV);      // setze Farben zusammen
                wBLUE= (wEndValueBLUE *uc_va1)/(EndGuixV);      
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));
            }
            printdebug2;
            ucFarbRasterPoint++;
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            printAt(13, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // Zeichne Linie 1 (Position xxx)
        
        /* Sechste Sequenz Fertig  Wir haben nun RGB erreicht.   */		
        
        
        /* Setze nun Werte für 7te Sequenz   von RGB bis R00  (RED GREEN BLUE = RGB)   */	
        wStartValueRED=Redmax;			wEndValueRED 		=Redmax;	    		// ROT von 0 bis max
        wStartValueGREEN=Greenmax;	wEndValueGREEN 	=0;			      // Green OFF
        wStartValueBLUE=Bluemax;		wEndValueBLUE 	=0;			// ändere BLUE von 0.. max
        
        wStartGuiyV=((ucFarbRaster*EndGuiyV)/FarbRaster)+7*ucZeichneLiniezwSequenz;					// ACHTUNG: Diese Position ist wichtig. Endwert des letzten Loops	+1+1 für neue Linie und Strich dazwischen
        
        ucFarbRaster=7;	
        FarbRasterPoints=((((ucFarbRaster)*EndGuiyV)/FarbRaster)-(wStartGuiyV-7*ucZeichneLiniezwSequenz))-1;  // BErechne wieviel Punkte im aktuellen Sektor dargestellt werden -2 wegen der Linie (AUFPASSEN) (-1 am Schluss weil der erste Loop schon zählt)
        wEndGuiyV=wStartGuiyV+FarbRasterPoints+3;					                     // BErechne die neue Position wo das Zeichnen aufhören soll d kommut noch korrektur daza letzte Sequenz
        ucFarbRasterPoint=0;                                                   // Setze Zähler der aktuellen Position innerhalb der Sequenz  auf 0
        printdebug1;
        for ( uc_va2=wStartGuiyV;uc_va2<=wEndGuiyV;uc_va2++ )
        {
            // 1ter äusserer Loop um Farbe darzustellen  Y-Achse
            for ( uc_va1=StartGuixV;uc_va1<=EndGuixV;uc_va1++ )
            {
                // innerer Loop um Farbsättigung darzustellen  (X-Achse)
                wGREEN = ((uc_va1*wStartValueGREEN-((uc_va1*wStartValueGREEN*ucFarbRasterPoint)/FarbRasterPoints))/EndGuixV); // Farbe geht langsam weg
                wRED   = (wEndValueRED  *uc_va1)/(EndGuixV);      // setze Farben zusammen
                wBLUE  = ((uc_va1*wStartValueBLUE-((uc_va1*wStartValueBLUE*ucFarbRasterPoint)/FarbRasterPoints))/EndGuixV); // Farbe geht langsam weg
                plotDot(uc_va1,uc_va2,(((wRED)<<11)|((wGREEN)<<5)|(wBLUE)));
            }
            printdebug2;
            ucFarbRasterPoint++;
        }
        if ( ucZeichneLiniezwSequenz==ON )
        {
            printAt(14, "uc_va2");printDec (4, uc_va2); line(1,uc_va2,319,uc_va2,1,WHITE);
        }
        // Zeichne Linie 1 (Position xxx)
        
        /* Siebte Sequenz Fertig  Wir haben nun R00 erreicht.   */
    }

#endif 
