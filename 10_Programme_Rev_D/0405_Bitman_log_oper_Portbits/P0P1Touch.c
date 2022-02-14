//****************************************************************
//  P0P1Touch.c  /  2.2.14  /  rw  /  bzu
//  Enthält die Funktionen zur Darstellung von P0 und P1 des
//  Hauptprogramms auf dem Grafikdisplay.
//****************************************************************

#include "stm32f10x.h"
#include "P0P1Touch.h"				// Touchscreenfunktionen
#include "P0P1Grafik.h"

//uint32_t SwitchTypes __attribute__((at(0x40000000)));  // Place to SRAM
//static uint32_t SwitchTypes __attribute__((section ("RW")));

extern uint32_t TimingDelay;
long divider,An,Bn,Cn,Dn,En,Fn,X,Y;		
char num1,num2,num3;

uint8_t InitDone = 0, ROTATE = 0, P0P1xch = 0, MSBLSBxch = 0;
unsigned char TouchAllP0=0xFF, TouchP0[8] = {1,1,1,1,1,1,1,1};
uint32_t TimingDelay;
uint32_t TimeDisplay = 0;
uint8_t PitCatched(TPit Pin);

void SetTouchP0(char i);
void ClearTouchP0(char i);
void ShowP0P1Pits(void);
void ShowMSBLSB(void);

char Headline1[] = " T:1234  In->P0       Out->P1";
char Headline2[] = " T:1234  Out->P1      In->P0";
char *Headline = Headline1;
int VertPos[8] = {286,250,214,178,142,106,70,34};

TPit TypePit = {5, 0,45,25,"",	BRIGHT_GREEN, GREEN , 1};
TPit RotatePit = {130, 0,45,25,"",	BRIGHT_GREEN, GREEN , 1};
TPit ExchP0P1Pit = {185, 0,45,25,"",	BRIGHT_GREEN, GREEN , 1};
TPit MSBLSBPit = {130, 40,40,270,"",	BRIGHT_GREEN, GREEN , 1};
TPit P0Icon[8] = 
{ //x,y,w,h,"Text", Farbe, Status = SwitchType 0..3
	25,286,45,26,"SW0",	BRIGHT_GREEN, GREEN , 0,
	25,250,45,26,"SW1",	BRIGHT_GREEN, GREEN , 0,
	25,214,45,26,"SW2",	BRIGHT_GREEN, GREEN , 0,
	25,178,45,26,"SW3",	BRIGHT_GREEN, GREEN , 0,
	25,142,45,26,"SW4",	BRIGHT_GREEN, GREEN , 0,
	25,106,45,26,"SW5",	BRIGHT_GREEN, GREEN , 0,
	25, 70,45,26,"SW6",	BRIGHT_GREEN, GREEN , 0,
	25, 34,45,26,"SW7",	BRIGHT_GREEN, GREEN , 0};

TPit P0SW[8] = 
{ //x,y,w,h,"Text", Farbe, Status
	80,286,45,26,"T0",	BRIGHT_GREEN, DARK_GREEN , 0,
	80,250,45,26,"T1",	BRIGHT_GREEN, DARK_GREEN , 0,
	80,214,45,26,"T2",	BRIGHT_GREEN, DARK_GREEN , 0,
	80,178,45,26,"T3",	BRIGHT_GREEN, DARK_GREEN , 0,
	80,142,45,26,"T4",	BRIGHT_GREEN, DARK_GREEN , 0,
	80,106,45,26,"T5",	BRIGHT_GREEN, DARK_GREEN , 0,
	80, 70,45,26,"T6",	BRIGHT_GREEN, DARK_GREEN , 0,
	80, 34,45,26,"T7",	BRIGHT_GREEN, DARK_GREEN , 0};

TPit P1LED[8] = 
{ //x,y,w,h,Caption, OnFarbe, OffFarbe, Status
	185,286,45,26," 0 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185,250,45,26," 1 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185,214,45,26," 2 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185,178,45,26," 3 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185,142,45,26," 4 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185,106,45,26," 5 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185, 70,45,26," 6 ",	BRIGHT_YELLOW, DARK_YELLOW , 0,
	185, 34,45,26," 7 ",	BRIGHT_YELLOW, DARK_YELLOW , 0};

//-----------------------------------------------------------------------------------
void InitP0P1(void)
{ 
	int i;
	// GPIO Clock -------------------------------------------------------------
  RCC->APB2ENR |=  1 <<  2;                    // Enable GPIOA clock, Button0         
  RCC->APB2ENR |=  1 <<  4;                    // Enable GPIOC clock, P0 und Button1          
  RCC->APB2ENR |=  1 <<  5;                    // Enable GPIOD clock, ControlStick          
  RCC->APB2ENR |=  1 <<  6;                    // Enable GPIOE clock, P1         
	// GPIO LEDs --------------------------------------------------------------
  GPIOA->ODR   &= 0xFFFF;                  		 // Configure the GPIOA for 
  GPIOA->CRL   &= 0xFFFFFFF0;                  // Configure the GPIOA for 
  GPIOA->CRL   |= 0x00000008;                  // Pullup Input OnBoardButton PA_0 
  GPIOC->ODR    = 0xFFFF;                  		 // GPIOC0..7 ext. SW In PullUp
  GPIOC->CRL   &= 0x00000000;                  // Configure the GPIOC for 
  GPIOC->CRL   |= 0x88888888;                  // Pullup Input 8 Switch 
  GPIOC->CRH   &= 0xFF0FFFFF;                  // Configure the GPIOC13 for 
  GPIOC->CRH   |= 0x00800000;                  // Pullup Input 1 Switch 
  GPIOD->ODR    = 0xFFFF;                  		 // GPIOD11..15 ext. ControlStick
  GPIOD->CRH   &= 0x00000FFF;                  // Configure the GPIOD for 
  GPIOD->CRH   |= 0x88888000;                  // Pullup Input 8 Switch 
  GPIOE->CRH   &= 0x00000000;                  // Configure the GPIOE for 
  GPIOE->CRH   |= 0x22222222;                  // Pushpull Output 8 LEDs 
	for(i=0;i<8;i++) TouchP0[i]=1;							 // Touch ON -> Switch aktiv
}

//-----------------------------------------------------------------------------------
void InitP0P1Touch(uint16_t StartType)
{ 
	int i; 
  InitP0P1();
	if(StartType>0)
	{
  // System Clocks Configuration 
  //ET_STM32_RCC_Configuration();  
	// Setup SysTick Timer for 10 msec interrupts, 1000->1ms
  SysTick_Config(72000000 / 1000);  // 10->100ms; 100->10ms
		
  // Start Initial GLCD and Calibrate Touch Screen 
    An = 0x00062391;
    Bn = 0x0000098F;
    Cn = 0xF8F60DAB;
    Dn = 0x00001AE2;
    En = 0x00082697;
    Fn = 0xF87E41A9;
    divider = 0x005CC7D2;
	Initial_GLCD_Hardware();
  Initial_GLCD_Ver();
  //Touch_Calibrate_Ver();
		
	if(StartType>999)	// StartType enthält 1000
	{
		P0P1xch = 1;
	  StartType -= 1000;
	}
	if(StartType>99)	// StartType enthält 100
	{
		MSBLSBxch = 1;
	  StartType -= 100;
	}
	if(StartType>9)	// StartType enthält 10
	{
		ROTATE = 4;		
	  StartType -= 10;
	}
	if((StartType<5)&&(StartType>0))	StartType -=1;	// wird 0..3
	else StartType = 0;  // alle ungültigen  				
		
	i = 27+(StartType<<3);							// Startpos + StartType*8 Pixel
	TypePit.status = StartType;
  Line_Ver(i, 20, i+10, 20, 2, BRIGHT_CYAN);		// Underline
	TouchAllP0 = -(StartType&1);									// =0x00 oder 0xFF
	for(i=0;i<8;i++)
	{ 
		TouchP0[i]=StartType&1;
		P0Icon[i].status = StartType;
    //drawPit(P0SW[i],StartType&1);
    //drawPit(P1LED[i],1);
	}
	ShowMSBLSB();			// Switchtyp und 0..7, 7..0
	ShowP0P1Pits();		// Pits und Headerzeile
	InitDone = 1;
	}  // end StartType>0
}

//-----------------------------------------------------------------------------------
void TSCTouchHandle(void)
{
  static unsigned char oldTouchDown=0;
  unsigned char isTouchDown, pos;
  uint8_t i; 

	isTouchDown = Get_Status_Touch();				// Hole Touchwert 0,1
	if (isTouchDown != oldTouchDown)				// Wenn verändert
	{
	  oldTouchDown = isTouchDown;						// dann Touchwert übernehmen
		if (isTouchDown)											// Wenn Touch down (=0)
		{
			TCS_Get_Point_Ver(20);								// Touchpos in X,Y 
			Y=Y+(Y>>5);														// Prop.Korrektur +1/32

			if(PitCatched(RotatePit))								// Tausche Horiz/Vertikal
			{
				if(ROTATE) ROTATE = 0; else ROTATE = 4;
				for(i=0;i<8;i++) 
				{
				lcd_icon_Ver(P0Icon[i].status+ROTATE, 30 ,P0Icon[i].PosY+20 , BRIGHT_CYAN, BLACK);
				if(ROTATE==0) lcd_printStr_Ver(P1LED[i].caption, 145 ,P1LED[i].PosY+20 , BRIGHT_CYAN, BLACK);
				else lcd_icon_Ver(8+i, 145 , P0Icon[i].PosY+20, BRIGHT_CYAN, BLACK);		// 8..15: Horiz 0..7
				}
			}

			if(PitCatched(MSBLSBPit))								// Tausche Horiz/Vertikal
			{
				MSBLSBxch = !MSBLSBxch;
				ShowMSBLSB();
				for(i=0;i<8;i++) 
				{
					drawPit(P0SW[i], TouchP0[i]);
					drawPit(P1LED[i], P1&(1<<i));
				}
			}
			
			if(PitCatched(ExchP0P1Pit))								// Vertausche P0 P1
			{
				P0P1xch = !P0P1xch;
				ShowP0P1Pits();
				for(i=0;i<8;i++) 
				{
					drawPit(P0SW[i], TouchP0[i]);
					drawPit(P1LED[i], P1&(1<<i));
				}
			}

			if(PitCatched(TypePit))								// Next Schaltertyp alle
			{
				if(TypePit.status<3) TypePit.status++;
				else TypePit.status=0;
				pos = 27+(TypePit.status<<3);											// X-Pos des Zeichens 0..3
				Line_Ver(0, 20, 65, 20, 2, BLACK);								// Underline löschen
				Line_Ver(pos, 20, pos+10, 20, 2, BRIGHT_CYAN);		// Underline unter  0..3
				
				for(i=0;i<8;i++)										// 8 Displayzeilen
				{
					P0Icon[i].status=TypePit.status;
					lcd_icon_Ver(P0Icon[i].status+ROTATE, 30 ,P0Icon[i].PosY+20 , BRIGHT_CYAN, BLACK);
				  if((P0Icon[i].status&1)==0)	
					{
						TouchP0[i]=0;	
						TouchAllP0 = 0;
					}
					else 
					{
						TouchP0[i]=1;	
						TouchAllP0 = 0xFF;  
					}
				  drawPit(P0SW[i],TouchP0[i]);
				}
			}
			
			for(i=0;i<8;i++)																		// Alle 8 Displayzeilen
			{
			  if(PitCatched(P0Icon[i]))													// In Schaltertypkolonne
				{																									// Zirkular Schaltertyp
					if(P0Icon[i].status<3) P0Icon[i].status++;			// wechseln
					else P0Icon[i].status=0;												// und anzeigen
					lcd_icon_Ver(P0Icon[i].status+ROTATE, 30 ,P0Icon[i].PosY+20 , BRIGHT_CYAN, BLACK);
					
					if((P0Icon[i].status&1)==0)	ClearTouchP0(i);
					else 	SetTouchP0(i);
				  drawPit(P0SW[i],TouchP0[i]);
				}
		   	if(PitCatched(P0SW[i]))														// In Schalterkolonne
				{
					if(P0Icon[i].status<2)													// Wenn Schalter 0:high, 1:low
					{
						if(TouchP0[i]==0) SetTouchP0(i);
						else ClearTouchP0(i);
					}	
					else																						// Taster down 2:high, 3:low
					{
						if(P0Icon[i].status==2)  SetTouchP0(i);
						else 	ClearTouchP0(i);
					}
				  drawPit(P0SW[i],TouchP0[i]);
				}				
			}					

		}
		else 																									// Touch up 
		{
			for(i=0;i<8;i++)
		   	if(PitCatched(P0SW[i]))														// Schalterkolonne
				{
					if(P0Icon[i].status==2)	ClearTouchP0(i);
					if(P0Icon[i].status==3) SetTouchP0(i);
					drawPit(P0SW[i],TouchP0[i]);
				}
		}
	}
}

//-----------------------------------------------------------------------------------
void SetTouchP0(char i)
{
	TouchP0[i]=1;						// Taster 3:high			
	TouchAllP0 |= 1<<i;	
}

//-----------------------------------------------------------------------------------
void ClearTouchP0(char i)
{
	TouchP0[i]=0;						// Taster 3:high			
	TouchAllP0 &= ~(1<<i);	
}

//-----------------------------------------------------------------------------------
void ShowMSBLSB(void)
{
	int i, PosY;	
	for(i=0;i<8;i++) 
	{			
		if(MSBLSBxch) PosY = VertPos[7-i];
		else PosY = VertPos[i];
		P0SW[i].PosY = PosY;
		P1LED[i].PosY = PosY;
		P0Icon[i].PosY = PosY;
		lcd_icon_Ver(P0Icon[i].status+ROTATE, 30 ,P0Icon[i].PosY+20 , BRIGHT_CYAN, BLACK);
		if(ROTATE==0) lcd_printStr_Ver(P1LED[i].caption, 145 ,P1LED[i].PosY+20 , BRIGHT_CYAN, BLACK);
		else 			 lcd_icon_Ver(8+i, 145 , P0Icon[i].PosY+20, BRIGHT_CYAN, BLACK);		// 8..15: Horiz 0..7
	}
}

//-----------------------------------------------------------------------------------
void ShowP0P1Pits(void)
{
	char i, P0Pos = 80, P1Pos = 185;	
 	if(P0P1xch) { P0Pos = 185; P1Pos = 80;} 
	for(i=0;i<8;i++) 
	{
		P0SW[i].PosX = P0Pos;
		P1LED[i].PosX = P1Pos;
	  Rectan_Ver(P0SW[i].PosX-1,P0SW[i].PosY-1,P0SW[i].PosX+P0SW[i].Width,P0SW[i].PosY+P0SW[i].Height, 1, BRIGHT_GREEN, 0);
	  Rectan_Ver(P1LED[i].PosX-1,P1LED[i].PosY-1,P1LED[i].PosX+P1LED[i].Width,P1LED[i].PosY+P1LED[i].Height, 1, YELLOW, 0);
	}
	if(P0P1xch) Headline = Headline2;
	else Headline = Headline1;
	lcd_printStr_Ver(Headline,5,15, BRIGHT_CYAN, BLACK);
	lcd_icon_Ver(16, 148 , 15, BRIGHT_CYAN, BLACK);		// 16: Doppelpfeil zw. IN/OUT
}

//-----------------------------------------------------------------------------------
void TSCPIORefresh(void) 		// In "stm32f10x_it.h" extern deklariert
{																// Gestartet mit SysTick_Config(15000000);
	static char oldP1=0xFF, newP1, diffP1;
	static char oldP0=0xFF, newP0, diffP0;
	int i, Bit;
	
	if(InitDone)
	{
	newP1 = P1;
	diffP1 = newP1^oldP1;
	if(diffP1 != 0)
	{
		Bit = 0x01;
		for(i=0; i<8; i++)
		{
			if((diffP1&Bit)!=0) 
			{
				drawPit(P1LED[i], P1&(1<<i));
//				if((newP1&Bit)==0) drawPit(P1LED[i],0);
//				else drawPit(P1LED[i],1);
			}
			Bit <<= 1;
		}
		oldP1 = newP1;
	}
	
	newP0 = P0;
	diffP0 = newP0^oldP0;
	if(diffP0 != 0)
	{
		Bit = 0x01;
		for(i=0; i<8; i++)
		{
			if((diffP0&Bit)!=0) 
			{
				drawPit(P0SW[i], P0&(1<<i));
//				if((newP0&Bit)==0) drawPit(P0SW[i],0);
//				else drawPit(P0SW[i],1);
			}
			Bit <<= 1;
		}
		oldP0 = newP0;
	}
}
}

//-----------------------------------------------------------------------------------
void plot_tab_Ver1(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned long color)
{
  char rx,ry;
	  
  for(ry=0;ry<h;ry++)
  { 
    GLCD_Write_Command(0x20);  																//Command Set Adddress Hor(X)
    GLCD_Write_Data(x);  																	//Sent X_address for CGRAM 
		GLCD_Write_Command(0x21);  																//Command Set Address Ver(Y)
    GLCD_Write_Data(y+ry);  																//Sent Y_address for CGRAM 			
    GLCD_Write_Command(0x22);  																//Command Write Data RGB
		for(rx=0;rx<w;rx++) 
			GLCD_Write_Data(color);  																//write data
  }
}

//-----------------------------------------------------------------------------------
void plotFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned long color)
{
  char rx,ry;	  
  for(ry=0;ry<h;ry++)
  { 
    GLCD_Write_Command(0x20);  																//Command Set Adddress Hor(X)
    GLCD_Write_Data(x);  																//Sent X_address for CGRAM 
	  GLCD_Write_Command(0x21);  																//Command Set Address Ver(Y)
    GLCD_Write_Data(y+ry);  																	//Sent Y_address for CGRAM 			
    GLCD_Write_Command(0x22);  																//Command Write Data RGB	
	  for(rx=0;rx<w;rx++) 
      GLCD_Write_Data(color);  																//write data
  }
}

//-----------------------------------------------------------------------------------
void drawPit(TPit Pit, uint8_t Stat)
{
	if(Stat == 0)		plotFilledRect(Pit.PosX, Pit.PosY, Pit.Width, Pit.Height, Pit.OFFColor);
	else						plotFilledRect(Pit.PosX, Pit.PosY, Pit.Width, Pit.Height, Pit.ONColor);
}

//-----------------------------------------------------------------------------------
uint8_t PitCatched(TPit Pit)
{  
	if ((X>=Pit.PosX && X<=Pit.PosX+Pit.Width) && (Y>=Pit.PosY && Y<=Pit.PosY+Pit.Height))
	{    							
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------------
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_ms( uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0)
  {
  }
}

//-----------------------------------------------------------------------------------
void SysTick_Handler(void)		// In stm32f10x_it.c auskommentiert und 
{															// hier definiert
  TimingDelay_Decrement();
}

//-----------------------------------------------------------------------------------
/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void) // alle 10ms aus SysTick_Handler
{
	static int RefreshTime = 0;
	if(RefreshTime-- == 0)					// Refresh alle 5x10ms

	{
		TSCTouchHandle();
		TSCPIORefresh();
		RefreshTime = 25;
	}	
  if (TimingDelay != 0x00)    TimingDelay--;
 }
//-----------------------------------------------------------------------------------
