/** @file TicTacToe.c
*  @brief  Spiele auf 1 oder 2 Display
*
*   Verbinde !richtig gekreuzt! P0(PAL) und P1(PEH), \n
*   da P1 mit PushPull kurzschlussgefärdet ist!
* x/y   (Koordinatenangaben für Display)
* 0/320   --------------------------------------- 0/0 \n
*         ¦ Farbe    FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦          FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦          FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦                ¦         ¦          ¦ \n
*         ¦ --->     FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦ ZUG      FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦ <---     FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦                ¦         ¦          ¦ \n
*         ¦          FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦          FELD  ¦   FELD  ¦   FELD   ¦ \n
*         ¦ NEU      FELD  ¦   FELD  ¦   FELD   ¦ \n
* 240/320 --------------------------------------- 0/320\n
* 
*  @author   rw  /  bzu   Main Design 20.9.14 
*  @author   rma / tbz  REV D implementation (Ver 1.1)
*  @date     20.9.14  
*  @date     20.02.2014 (Ver 1.1)
*  @version  1.1    
*  @todo    -
*  @see 
*  @note    Code ist nur fuer Version D  mit Display Chip ILI 9341 gueltig
*  @warning -
*  @bug No known bugs.
*/
//=================================Includes===================================== 
#include <stm32f10x.h>			// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen 
//-----------------------------------------------------

typedef struct
{
	uint16_t x, y, w, h;
} TFeld;

TFeld ttt[9] = {
	15,15, 55, 55, 		87,15, 55, 55,   162,15, 55, 55, 
  15,90, 55, 55,    87,90, 55, 55,   162,90, 55, 55, 
  15,165, 55, 55,   87,165, 55, 55,  162,165, 55, 55};
TFeld tttWin = {15,250,55,55};
TFeld tttNew = {162,250,55,55};
	
char Spieler = 0, Win = 0, SingleScreen = 1;
char Tips1[9] = {0,0,0,0,0,0,0,0,0};
char Tips2[9] = {0,0,0,0,0,0,0,0,0};

//------------------------------------------------------
void Paint(TFeld f, long col)
{
	plotFilledRect(f.x,f.y,f.w,f.h,col);
}

//------------------------------------------------------
void TextNEU(int x, int y)			// Schreibt NEU an x,y
{
	line(	x,			y,			x+16,	y,		2,WHITE);		//  U
	line(	x,			y+8,		x+16,	y+8,	2,WHITE);		//  U
	line(	x+16,		y,			x+16,	y+8,	2,WHITE);		//  U
	line(	x,			y+20,		x+16,	y+20,	2,WHITE);   //  E
	line(	x,			y+12,		x,		y+20,	2,WHITE);   //  E
	line(	x+16,		y+12,		x+16,	y+20,	2,WHITE);   //  E
	line(	x+9,		y+15,		x+9,	y+20,	2,WHITE);   //  E
	line(	x,			y+24,		x+17,	y+24,	2,WHITE);		//  N
	line(	x,			y+32,		x+17,	y+32,	2,WHITE);		//  N
	line(	x,			y+32,		x+17,	y+24,	2,WHITE);		//  N
}

//-------------------------------------------------------
void TextZUG(int x, int y)			// Schreibt ZUG an x,y
{
	line(	x,			y,			x,		y+8,	2,WHITE);   //  G
	line(	x,			y+8,		x+16,	y+8,	2,WHITE);   //  G
	line(	x+16,		y,			x+16,	y+8,	2,WHITE);   //  G
	line(	x+8,		y,			x+16,	y,		2,WHITE);   //  G
	line(	x+8,		y,			x+8,	y+4,	2,WHITE);   //  G
	line(	x,			y+20,		x+16,	y+20,	2,WHITE);		//  U
	line(	x,			y+12,		x+16,	y+12,	2,WHITE);		//  U
	line(	x+16,		y+12,		x+16,	y+20,	2,WHITE);		//  U
	line(	x,			y+24,		x,		y+32,	2,WHITE);		//  Z
	line(	x+16,		y+24,		x+16,	y+32,	2,WHITE);		//  Z	
	line(	x,			y+24,		x+16,	y+32,	2,WHITE);		//  Z	
}

//--------------------------------------------------------
void TextSIEG(int x, int y)			// Schreibt SIEG an x,y
{
	rectan(x-1,y-1,x+18,y+40,1,BLACK,1);
	line(	x,			y,			x,		y+8,	2,WHITE);   //  G
	line(	x,			y+8,		x+16,	y+8,	2,WHITE);   //  G
	line(	x+16,		y,			x+16,	y+8,	2,WHITE);   //  G
	line(	x+8,		y,			x+16,	y,		2,WHITE);   //  G
	line(	x+8,		y,			x+8,	y+4,	2,WHITE);   //  G
	line(	x,			y+20,		x+16,	y+20,	2,WHITE);   //  E
	line(	x,			y+12,		x,		y+20,	2,WHITE);   //  E
	line(	x+16,		y+12,		x+16,	y+20,	2,WHITE);   //  E
	line(	x+9,		y+15,		x+9,	y+20,	2,WHITE);   //  E
	line(	x,			y+25,		x+17,	y+25,	2,WHITE);		//  I 
	line(	x,			y+30,		x,		y+38,	2,WHITE);		//  S
	line(	x+7,		y+30,		x+7,	y+38,	2,WHITE);		//  S
	line(	x+16,		y+30,		x+16,	y+40,	2,WHITE);		//  S	
	line(	x,			y+38,		x+7,	y+38,	2,WHITE);		//  S	
	line(	x+7,		y+30,		x+16,	y+30,	2,WHITE);		//  S	
}

//---------------------------------------------------------
void TextUPS(int x, int y)			// Schreibt UPS! an x,y
{
	rectan(x-1,y-1,x+18,y+40,1,BLACK,1);
	line(	x,			y+2,		x+10,	y+2,	2,WHITE);		//  !
	line(	x+13,		y+2,		x+16,	y+2,	2,WHITE);		//  !	
	line(	x,			y+7,		x,		y+15,	2,WHITE);		//  S
	line(	x+7,		y+7,		x+7,	y+15,	2,WHITE);		//  S
	line(	x+16,		y+7,		x+16,	y+17,	2,WHITE);		//  S	
	line(	x,			y+15,		x+7,	y+15,	2,WHITE);		//  S	
	line(	x+7,		y+7,		x+16,	y+7,	2,WHITE);		//  S		
	line(	x,			y+19,		x+8,	y+19,	2,WHITE);   //  P
	line(	x,			y+19,		x,		y+26,	2,WHITE);   //  P
	line(	x+8,		y+19,		x+8,	y+26,	2,WHITE);   //  P
	line(	x,			y+26,		x+17,	y+26,	2,WHITE);   //  P				
	line(	x,			y+30,		x+17,	y+30,	2,WHITE);		//  U
	line(	x+16,		y+30,		x+16,	y+37,	2,WHITE);		//  U
	line(	x,			y+37,		x+17,	y+37,	2,WHITE);		//  U				
}

//----------------------------------------------------------
void PfeilLeft(int x, int y, int col)		// Zeichnet <--
{
	line(	x-4,		y,			x-4,	y+32 ,	8, col);   	// 
	line(	x,			y+32,		x-8,	y+20,		8, col);  	//  
	line(	x,			y+32,		x+8,	y+20,		8, col);  	// 
}

//----------------------------------------------------------
void PfeilRight(int x, int y, int col)		// Zeichnet -->
{
	line(	x-4,		y+8,		x-4,	y+38 ,	8, col);   // 
	line(	x,			y,			x-8,	y+12,		8, col);   //  
	line(	x,			y,			x+8,	y+12,		8, col);   // 
}

//----------------------------------------------------------
void ZeigeSpielfeld(void)										// 
{  
	InitTouchP0P1("0");									// Output P1 pullup
	GPIOE->CRH = 0x22222222;						// Output PushPull für
	line(15,80,225,80,5, WHITE);				// Linie senkrecht rechts
	line(15,155,225,155,5, WHITE);      // Linie senkrecht links
  line(80,15,80,225,5, WHITE);        // Linie waagrecht oben
	line(155,15,155,225,5, WHITE);      // Linie waagrecht unten
	TextSIEG(34,260);
	TextZUG(110,260);
	Paint(tttNew, GRAY3);               // Feld unten links mit NEUes Spiel
	TextNEU(180,260);                   // Koordinaten für Text NEUes Spiel
	Paint(tttWin, BRIGHT_GREEN);
	PfeilLeft(142,255, LIGHT_CYAN);
	PfeilRight(95,255, BRIGHT_GREEN);
  P1 = 0x10;													// Zeigt den Gegenspieler an	
}																			// sonst offen 0xFF

//----------------------------------------------------------------
uint8_t FeldCatch(TFeld f)						// X,Y im Feld f getouched
{  
	unsigned int X = getTSCx(), Y = getTSCy();
	if ((X>=f.x && X<=f.x+f.w) && (Y>=f.y && Y<=f.y+f.h))  return 1;
	else	return 0;
}

//----------------------------------------------------------------
void SendUpdate(char update)
{
	unsigned long t;
		P1 = update|0x80;  				// fuer pos. Flanke
		for(t=120000; t>0; t--);	// entprellen 10ms
		P1 = 0;  									// Ende pos. Puls
}

//----------------------------------------------------------------
// FremdTipp: An P0 ankommender Tipp verarbeiten; Parm in immer 0
// Nach Neustart Spieler=0: Erster fremder Touch heisst, fremder
// Spieler ist Spieler1, eigener wird Spieler2.
//----------------------------------------------------------------
unsigned char FremdTipp(unsigned char in)
{
	unsigned long t;
	static unsigned char oldTipp=0xFF;
  unsigned char i, Tipp = in, newTipp;
	newTipp = P0;
	if(newTipp==0xFF) SingleScreen = 1;
	else  SingleScreen = 0;
	if(newTipp != oldTipp)										// Wenn Aenderung
	{
		oldTipp = newTipp;
		if(newTipp&0x80)												// pos. Zustand Bit7
		{
			if(Spieler == 0) Spieler = 2;					// Packt den Erst-Tippenden
			i = (newTipp&0x0F)-1;
			if(newTipp==0xF0)	Tipp = 0xF0;				//  alle, auch 0xF0 Neustart
			else  
				if(Spieler==2)
					if((Tips1[i] == 0)&&(Tips2[i] == 0)&&(Win == 0))
				    Tipp = newTipp&0x0F;		 
		}
		for(t=120000; t>0; t--);								// entprellen 10ms
	}
	return Tipp;															// in(=0) 1..9 oder 0xFF
}

//----------------------------------------------------------------
// EigenTipp: Eigener Touch verarbeiten; Parm in immer 0
// Nach Neustart Spieler=0: Erster eigener Touch wird Spieler 1
//------------------------------------------------------------------
unsigned char EigenTipp(unsigned char in)
{
	unsigned long t;
	static unsigned char oldTouchDown=0;
  unsigned char isTouchDown, i, Tipp = in;
  isTouchDown = getTSCtouched();				// Hole Touchwert 0,1
	//  rma 20.02.2015 REV C ganz alt  isTouchDown = TSC_getTouchedStatus();				// Hole Touchwert 0,1
	if (isTouchDown != oldTouchDown)						// Wenn verändert
	{
	  oldTouchDown = isTouchDown;								// dann Touchwert übernehmen
		if (isTouchDown==1)												// Wenn Touch down (= 1)
		{
//  rma 20.02.2015 REV C ganz alt			TSC_getXY();														// Touchpos  X,Y 
			getTSCxy();														// Touchpos  X,Y 
			if(FeldCatch(tttNew)) Tipp = 0xF0;			// Neues Spiel
			else
			{
				if(Spieler == 0) Spieler = 1;					// Packt den Erst-Tippenden
				if((SingleScreen)||(Spieler==1))			// Im 1-Screenmode beide Spieler
			    for(i=0; i<9; i++)									// Im 2-Screenmode nur Spieler1 lokal
			      if((FeldCatch(ttt[i])==1)&&(Tips1[i] == 0)&&(Tips2[i] == 0)&&(Win == 0))
					    Tipp = i+1;											// wird in freiem Feld  1..9
			}																				// oder Parm 'in', wenn daneben getippt 				
		}
		for(t=120000; t>0; t--);									// entprellen 10ms
	}
	return Tipp;																// in(=0) 1..9 oder 0xFF
}

//------------------------------------------------------------------
void UpdateSpiel(unsigned char update)
{ 
	char i;
	if(update==0xF0)											// Neues Spiel angefordert
	{
		for(i=0; i<9; i++)									// Alle 9 Felder:
		{
			Tips1[i] = 0;											// Spieler1 Tipps löschen
			Tips2[i] = 0;											// Spieler2 Tipps löschen
			Paint(ttt[i], BLACK);							// Tippfelder löschen
		}
		Paint(tttWin, BRIGHT_GREEN);				// Siegtext löschen		
		PfeilLeft(142,255, LIGHT_CYAN);			// Beide Pfeile anzeigen
		PfeilRight(95,255, BRIGHT_GREEN);
		Win = 0;														// Gewinner löschen
		Spieler = 0;												// Kein Spieler gestartet
	}
	else 																				// im laufenden Spiel
		if(update>0)		// 1..9
		{
			if(Spieler==1)  												// Lokaler Tipp
			{
				Paint(ttt[update-1], BRIGHT_GREEN);		// gibt grünes Feld
				Tips1[update-1] = 1;									// in lokales Datenfeld
				Spieler = 2;													// Spieler wechseln	
				PfeilLeft(142,255, LIGHT_CYAN);				// Pfeile wechseln	
				PfeilRight(95,255, BLACK);
			}
			else  
				if(Spieler==2)  											// Fremder Tipp
				{
					Paint(ttt[update-1], LIGHT_CYAN);		// gibt cyan Feld
					Tips2[update-1] = 1;								// in fremdes Datenfeld
					Spieler = 1;												// Spieler wechseln
					PfeilLeft(142,255, BLACK);					// Pfeile wechseln	
					PfeilRight(95,255, BRIGHT_GREEN);
				}
		}
}

//----------------------------------------------------------------
void SpielStatus(void)		// Gewinner bestimmen und anzeigen
{
	if(Tips1[0]+Tips1[1]+Tips1[2]==3) Win=1;	//  3x horiz Spieler1
	if(Tips1[3]+Tips1[4]+Tips1[5]==3) Win=1;
	if(Tips1[6]+Tips1[7]+Tips1[8]==3) Win=1;
	if(Tips1[0]+Tips1[3]+Tips1[6]==3) Win=1;	//  3x vert
	if(Tips1[1]+Tips1[4]+Tips1[7]==3) Win=1;
	if(Tips1[2]+Tips1[5]+Tips1[8]==3) Win=1;
	if(Tips1[0]+Tips1[4]+Tips1[8]==3) Win=1;	//  2x diag
	if(Tips1[2]+Tips1[4]+Tips1[6]==3) Win=1;
		
	if(Tips2[0]+Tips2[1]+Tips2[2]==3) Win=2;	//  3x horiz Spieler2
	if(Tips2[3]+Tips2[4]+Tips2[5]==3) Win=2;
	if(Tips2[6]+Tips2[7]+Tips2[8]==3) Win=2;
	if(Tips2[0]+Tips2[3]+Tips2[6]==3) Win=2;	//  3x vert
	if(Tips2[1]+Tips2[4]+Tips2[7]==3) Win=2;
	if(Tips2[2]+Tips2[5]+Tips2[8]==3) Win=2;
	if(Tips2[0]+Tips2[4]+Tips2[8]==3) Win=2;	//  2x diag
	if(Tips2[2]+Tips2[4]+Tips2[6]==3) Win=2;
	if(Win)
	{
		PfeilRight(95,255, BLACK);
		PfeilLeft(142,255, BLACK);
		Paint(tttWin, BRIGHT_GREEN);
		if(Spieler==2) TextSIEG(34,260);	
		if(Spieler==1) TextUPS(34,260);		
	}
}

//----------------------------------------------------------------
int main(void) 											// Hauptprogramm
{ 
	unsigned char Update = 0;
	ZeigeSpielfeld();									// Zaehlung nullen
// Debug RMA	plotFilledRect(0,0,10,10,BRIGHT_GREEN);
	while(1)													// Endlosschlaufe
	{
		Update = EigenTipp(0);					// Aktion nur bei Tipp
		if(Update)
		{																// Senden bei Eigentipp
			SendUpdate(Update);						// enthält entprellen 10ms
			UpdateSpiel(Update);
			SpielStatus();
			Update = 0;
		}
		Update = FremdTipp(0);					// Aktion nur bei Tip
		if(Update)
		{																// Kein Senden bei Fremdtipp
			UpdateSpiel(Update);
			SpielStatus();
			Update = 0;
		}	
	}
} 
//*****************************************************************
