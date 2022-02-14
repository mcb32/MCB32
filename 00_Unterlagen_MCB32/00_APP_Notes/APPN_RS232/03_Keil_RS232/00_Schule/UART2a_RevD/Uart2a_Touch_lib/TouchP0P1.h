
/** @file TouchP0P1.h 
*  @brief  Zugriffsfunktionen zur HW des Touch- und Grafikdisplays. 
*
*  Es sind untenstehende 4 Headerdateien zusammengefasst,
*  damit die Lernenden nur die eine TouchP0P1.h kopieren
*  und einbinden muessen.
*  #include "P0P1Shell.h"					
*  #include "TouchGrafik.h"					
*  #include "TouchPrint.h"					
*  #include "Peripherie.h"	
*  
*  Siehe auch: P0P1Shell.h  unten
* 
*  @author   rw  / BZU   Main Design 15.11.14
*  @author   rma / tbz  REV D implementation
*  @date     15.11.14   
*  @version  1.0D   // D= HardwareVersion für Display (D= ILI9341)     
*  @todo    -
*  @see 
*  @note    Code ist nur fuer Version D  mit Display Chip ILI 9341 gueltig
*  @warning -
*  @bug No known bugs.
*/
//=================================Includes===================================== 

#ifndef __TouchP0P1_h
#define __TouchP0P1_h

/** ********************************************************************
*   P0P1Shell.h  /  15.11.14  /  rw  /  bzu
*   -  Variante: PAL als Input ausgeschlagen, zuviel interne Peripherie
*   -  enthaelt die Pin-Definitionen und das Bitbanding zum 8 Bit P0/P1- 
*      Handling als Uebergangsstufe vom 8Bit-uC auf den 32Bit-uC.
*   -  enthaelt zudem die Header zu den Funktionen zur Darstellung und 
*      Touchaufnahme von P0 und P1 auf dem Grafiktouchdisplay.
*   -  Touch-Eingabe parallel zur Pin-Eingabe GPIOA0..7 oder GPIOC0..7
*      Touch-Ausgabe parallel zur Pin-Ausgabe LEDs an GPIOE8..15
*   -------------------------------------------------------------------
*   Touchscreen parallel zu Pin-Ein/Ausgabe GPIOC0..7, GPIOE8..15
*   ------------------------------------------------------------------- */
#ifndef __P0P1Shell_h
#define __P0P1Shell_h
//*********************************************************************

#define HOR 0									
#define VER 1

/** @brief  Define für dMCB32_LibVersion: 
*
*   Date (yearweek) Hardwareversion motherboard (Hxx) HardwareVersion Display (Dxx) R (=Release_Status ->Released_Alpha_Beta).
*
*  @author   rma / tbz  REV D implementation
*  @date     20.12.2014  
************************** */
#define dMCB32_LibVersion "1450-H01D02-R"     // 1450-H01D02-R = 20.12.2014


// Touch enthaelt den Eingabezustand der Bits vom Display um mit der
// physischen Porteingabe AND-verknuepft zu werden
extern unsigned char TouchP0[8], TouchAllP0;// = {1,1,1,1,1,1,1,1};
// ----------- Bit Banding fuer read/write auf gleichen Bezeichner -----
// Define base address of peripheral bit-band
// #define PERIPH_BASE 0x40000000
// Define base address of peripheral alias band
// #define PERIPH_BB_BASE 0x42000000
// Convert PERI address to alias region
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (((uint32_t)a)-PERIPH_BASE)*32 + (b*4)))
/*
// In dieser Variante nicht mit dem Touchdisplay verknuepft
#define P0_0 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,0)))) 
#define P0_1 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,1)))) 
#define P0_2 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,2)))) 
#define P0_3 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,3)))) 
#define P0_4 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,4)))) 
#define P0_5 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,5)))) 
#define P0_6 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,6)))) 
#define P0_7 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,7)))) 
*/
// In dieser Variante mit dem Touchdisplay verknuepft
// BZU-Kompatibiltaet zu 1Bit Manipulation: P0_0 .. P0_7 Input von GPIOC0..7
// Alternativ zu P0_x sind die naeheren Pinbezeichner PC_x verwendbar
#define P0_0 (((unsigned char)TouchP0[0])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,0))))) 
#define P0_1 (((unsigned char)TouchP0[1])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,1)))))
#define P0_2 (((unsigned char)TouchP0[2])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,2))))) 
#define P0_3 (((unsigned char)TouchP0[3])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,3))))) 
#define P0_4 (((unsigned char)TouchP0[4])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,4))))) 
#define P0_5 (((unsigned char)TouchP0[5])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,5))))) 
#define P0_6 (((unsigned char)TouchP0[6])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,6))))) 
#define P0_7 (((unsigned char)TouchP0[7])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,7)))))

#define PC_0 (((unsigned char)TouchP0[0])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,0))))) 
#define PC_1 (((unsigned char)TouchP0[1])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,1)))))
#define PC_2 (((unsigned char)TouchP0[2])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,2))))) 
#define PC_3 (((unsigned char)TouchP0[3])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,3))))) 
#define PC_4 (((unsigned char)TouchP0[4])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,4))))) 
#define PC_5 (((unsigned char)TouchP0[5])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,5))))) 
#define PC_6 (((unsigned char)TouchP0[6])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,6))))) 
#define PC_7 (((unsigned char)TouchP0[7])&(*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,7)))))

// In dieser Variante mit dem Touchdisplay verknuepft
// Alternativ zu P1_0..7 sind die HW-naeheren Pinbezeichner PE_8..15 verwendbar
// BZU-Kompatibiltaet zu 1Bit Manipulation: P1_0 .. P1_7 Output nach GPIOE8..15
#define P1_0 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,8)))) 
#define P1_1 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,9)))) 
#define P1_2 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,10)))) 
#define P1_3 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,11)))) 
#define P1_4 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,12)))) 
#define P1_5 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,13)))) 
#define P1_6 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,14)))) 
#define P1_7 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,15)))) 

// BZU-Kompatibiltaet zu 1Bit Manipulation: P1_0 .. P1_7 Output nach GPIOE8..15
#define PE_8 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,8)))) 
#define PE_9 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,9)))) 
#define PE_10 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,10)))) 
#define PE_11 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,11)))) 
#define PE_12 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,12)))) 
#define PE_13 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,13)))) 
#define PE_14 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,14)))) 
#define PE_15 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOE->ODR,15)))) 

// OnBoard Controlstick 5Bit InByte PD_11 .. PD_15 von GPIOD high
#define PD_11 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOD->IDR,11)))) 
#define PD_12 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOD->IDR,12)))) 
#define PD_13 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOD->IDR,13)))) 
#define PD_14 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOD->IDR,14))))
#define PD_15 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOD->IDR,15))))

// OnBoard Controlstick 5Bit InByte PD_11 .. PD_15 von GPIOD high
#define PA_0  (*((volatile unsigned char *)(BITBAND_PERI(&GPIOA->IDR,0)))) 
#define PC_13 (*((volatile unsigned char *)(BITBAND_PERI(&GPIOC->IDR,13)))) 

// BZU-Kompatibiltaet zu 8Bit InByte P0 von GPIOC low und OutByte P1 nach GPIOE high
#define P0  (((unsigned char)TouchAllP0)&(*((volatile unsigned char *)(&(GPIOC->IDR))))) // In Bit0..7 als Byte
#define P1  (*((volatile unsigned char *)(&(GPIOE->ODR))+1)) // Out Bit8..15 als Byte
#define PCL  (((unsigned char)TouchAllP0)&(*((volatile unsigned char *)(&(GPIOC->IDR))))) // In Bit0..7 als Byte
#define PEH  (*((volatile unsigned char *)(&(GPIOE->ODR))+1)) // Out Bit8..15 als Byte
#define PD_High (*((volatile unsigned char *)(&(GPIOD->IDR))+1)) // In Bit8..15 als Byte fuer CtrlStick


//----- Funktionsheader zu P0P1Touch.c bezw. P0P1Touch.lib -----
void InitTouchP0P1(char* StartType);		// Einzig oeffentlich
void InitP0P1(void);											// Alle intern/privat
void delay_ms(unsigned long nTime);	 					//Delay ms
/** @brief  Funktion: MCB32_LibVersion
*
*   Gibt die Version der aktuellen Lib mit HW Status zurück
*   @author rma / 20.12.2014
*/
// const char * MCB32_LibVersion(void)
//{
//    return dMCB32_LibVersion;        // Return Pointer to String LibVersion
//}


	
//------ Parameterpruefung bei CMSIS Einsatz -----------
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
#endif 		// __P0P1Shell_h 

//******************************************************************
//******************************************************************


//****************************************************************
//  TouchGrafik.h  /  15.11.14  /  rw  /  bzu
//  Headerdatei zu TouchGrafik.c:
//  enthaelt die Touchscreen Farbdefinitionen 
//  und die Prototypen der Touch- und Grafikfunktionen 
//****************************************************************

#ifndef __TouchGrafik_h
#define __TouchGrafik_h

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------*/
#include "stm32f10x.h"

/******************************/
/* Define Color Code For GLCD */
/* Color = RRRR'RGGG'GGGB'BBBB */
/******************************/
#define no_bg           	0x0001			// No Color Back Ground
#define BLACK           	0x0000
#define WHITE           	0xFFFF
#define RED             	0x8000
#define GREEN           	0x0400
#define DARK_GREEN       	0x1C03			// weber
#define BLUE            	0x0010
#define YELLOW          	0xFFF0
#define DARK_YELLOW  			0x8403			// weber
#define CYAN            	0x0410
#define MAGENTA         	0x8010
#define BROWN           	0xFC00
#define OLIVE	        		0x8400

#define BRIGHT_RED      	0xF800
#define BRIGHT_GREEN    	0x07E0
#define BRIGHT_BLUE     	0x001F
#define BRIGHT_YELLOW   	0xFFE0
#define BRIGHT_CYAN     	0x07FF
#define BRIGHT_MAGENTA  	0xF81F

#define LIGHT_GRAY      	0x8410
#define LIGHT_BLUE      	0x841F
#define LIGHT_GREEN     	0x87F0
#define LIGHT_CYAN      	0x87FF
#define LIGHT_RED       	0xFC10
#define LIGHT_MAGENTA   	0xFC1F

#define DARK_GRAY       	0x4208
#define GRAY0       			0xE71C
#define GRAY1         		0xC618
#define GRAY2           	0xA514
#define GRAY3           	0x630C
#define GRAY4           	0x4208
#define GRAY5	        	0x2104
#define	GRAY6	        	0x3186	

#define	BLUE0	        	0x1086	
#define	BLUE1       		0x3188 
#define	BLUE2          		0x4314
#define BLUE3         		0x861C	

#define	CYAN0	        	0x3D34
#define	CYAN1           	0x1DF7		

#define	GREEN0          	0x0200	
#define	GREEN1          	0x0208	

//------------ Color For Build Button -------------
#define BT_RED          	1
#define BT_GREEN        	2
#define BT_BLUE         	3
#define	BT_YELLOW       	4
#define BT_WHITE        	5
#define BT_CYAN         	6
#define BT_MAGENTA      	7
#define BT_BROWN        	8

#define BT_LRED         	9
#define BT_LGREEN       	10
#define BT_LBLUE        	11
#define	BT_LYELLOW      	12
#define BT_LWHITE       	13
#define BT_LCYAN        	14
#define BT_LMAGENTA     	15
#define BT_LBROWN       	16

///** 
//  * @brief  LCD Control pins  REV C
//  */ 
//#define LCD_CS_PIN          GPIO_Pin_8                  // PC8 = CS# GLCD
//#define LCD_CS_GPIO_PORT    GPIOC                       
//#define LCD_CS_GPIO_CLK     RCC_APB2Periph_GPIOC  

//#define LCD_BL_PIN          GPIO_Pin_7                  // PD7 = BL GLCD
//#define LCD_BL_GPIO_PORT    GPIOD                       
//#define LCD_BL_GPIO_CLK     RCC_APB2Periph_GPIOD 
/** 
  * @brief  LCD Control pins  REV D
  */ 
#define LCD_CS_GPIO_PIN        GPIO_Pin_8                  																				// PC8 = CS# GLCD
#define LCD_CS_GPIO_PORT       GPIOC                       
#define LCD_CS_GPIO_CLK        RCC_APB2Periph_GPIOC  

#define LCD_BL_GPIO_PIN        GPIO_Pin_7                  																				// PD7 = BL GLCD
#define LCD_BL_GPIO_PORT       GPIOD                       
#define LCD_BL_GPIO_CLK        RCC_APB2Periph_GPIOD 

///** 
//  * @brief  LCD SPI Interface pins   REV C
//  */ 
//#define LCD_SPI_SCK_PIN     GPIO_Pin_10                 
//#define LCD_SPI_MISO_PIN    GPIO_Pin_11                 
//#define LCD_SPI_MOSI_PIN    GPIO_Pin_12                 
//#define LCD_SPI_GPIO_PORT   GPIOC                       
//#define LCD_SPI_GPIO_CLK    RCC_APB2Periph_GPIOC  
//#define LCD_SPI			    SPI3
//#define LCD_SPI_CLK		    RCC_APB1Periph_SPI3
/** 
  * @brief  LCD SPI Interface pins REV D
  */ 
#define LCD_SPI_SCK_PIN        GPIO_Pin_10                 
#define LCD_SPI_MISO_PIN       GPIO_Pin_11                 
#define LCD_SPI_MOSI_PIN       GPIO_Pin_12       
#define LCD_SPI_GPIO_PORT      GPIOC                       
#define LCD_SPI_GPIO_CLK       RCC_APB2Periph_GPIOC  
#define LCD_SPI			       		 SPI3
#define LCD_SPI_CLK		       	 RCC_APB1Periph_SPI3

///** 
//  * @brief  TCS Control pins (ADS7846:Touch Sensor)  REV C
//  */ 
//// PE3(PEN#) <--
//// PE4(MISO) <--
//// PE5(MOSI) -->
//// PE6(CS#)	 -->
//// PE7(SCK)	 --> 
//#define TCS_GPIO_CLK        RCC_APB2Periph_GPIOE  
//#define TCS_GPIO_PORT      	GPIOE
//#define TCS_PEN_PIN         GPIO_Pin_3                  // PE3 = PEN# Touch Sensor                       
//#define TCS_MISO_PIN        GPIO_Pin_4                  // PE4 = MISO Touch Sensor
//#define TCS_MOSI_PIN        GPIO_Pin_5                  // PE5 = MOSI Touch Sensor
//#define TCS_CS_PIN          GPIO_Pin_6                  // PE6 = CS# Touch Sensor
//#define TCS_SCK_PIN         GPIO_Pin_7                  // PE7 = SCK Touch Sensor

/** 
  * @brief  TCS Control pins (ADS7846:Touch Sensor)  REV D
  */ 
// PE3(PEN#) <--
// PE4(MISO) <--
// PE5(MOSI) -->
// PE6(CS#)	 -->
// PE7(SCK)	 --> 
#define TCS_GPIO_CLK        RCC_APB2Periph_GPIOE  
#define TCS_GPIO_PORT      	GPIOE
#define TCS_PEN_PIN         GPIO_Pin_3                    // PE3 = PEN# Touch Sensor                 																					// PE3 = PEN# Touch Sensor                       
#define TCS_MISO_PIN        GPIO_Pin_4                  																					// PE4 = MISO Touch Sensor
#define TCS_MOSI_PIN        GPIO_Pin_5                  																					// PE5 = MOSI Touch Sensor
#define TCS_CS_PIN          GPIO_Pin_6                  																					// PE6 = CS# Touch Sensor
#define TCS_SCK_PIN         GPIO_Pin_7 

// PEN#(PE3)   REV D
#define TCS_PEN_PORT        GPIOE
#define TCS_PEN_CLK         RCC_APB2Periph_GPIOE
#define TCS_PEN_PORT_SOURCE GPIO_PortSourceGPIOE
#define TCS_PEN_PIN         GPIO_Pin_3					
#define TCS_PEN_PIN_SOURCE  GPIO_PinSource3
#define TCS_PEN_EXTI_LINE   EXTI_Line3
#define TCS_PEN_IRQn        EXTI3_IRQn 

//// PEN#(PE3)  REV C
//#define TCS_PEN_PORT        GPIOE
//#define TCS_PEN_CLK         RCC_APB2Periph_GPIOE
//#define TCS_PEN_PORT_SOURCE GPIO_PortSourceGPIOE
//#define TCS_PEN_PIN         GPIO_Pin_3					
//#define TCS_PEN_PIN_SOURCE  GPIO_PinSource3
//#define TCS_PEN_EXTI_LINE   EXTI_Line3
//#define TCS_PEN_IRQn        EXTI3_IRQn 

//// REV c defines
//#define GLCD_CS_LOW()     	GPIO_ResetBits(LCD_CS_GPIO_PORT, LCD_CS_PIN)
//#define GLCD_CS_HIGH()    	GPIO_SetBits(LCD_CS_GPIO_PORT, LCD_CS_PIN)
//#define GLCD_BL_OFF()     	GPIO_ResetBits(LCD_BL_GPIO_PORT, LCD_BL_PIN)
//#define GLCD_BL_ON()    	GPIO_SetBits(LCD_BL_GPIO_PORT, LCD_BL_PIN)
//#define TCS_CS_LOW()     	GPIO_ResetBits(TCS_GPIO_PORT, TCS_CS_PIN)
//#define TCS_CS_HIGH()    	GPIO_SetBits(TCS_GPIO_PORT, TCS_CS_PIN)

// REV D defines
#define GLCD_CS_LOW()     	GPIO_ResetBits(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN)
#define GLCD_CS_HIGH()    	GPIO_SetBits(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN)
#define GLCD_BL_OFF()     	GPIO_ResetBits(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN)
#define GLCD_BL_ON()    		GPIO_SetBits(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN)
#define TCS_CS_LOW()     		GPIO_ResetBits(TCS_GPIO_PORT, TCS_CS_PIN)
#define TCS_CS_HIGH()    		GPIO_SetBits(TCS_GPIO_PORT, TCS_CS_PIN)

#define VER 1
#define HOR 0
	
void setScreenDir (char dir);
char getScreenDir (void);
void clearScreen(long color);
char getTSCtouched(void);
unsigned int getTSCy(void);
unsigned int getTSCx(void);

//// REV C
///* Function Control Graphic LCD ET-TFT240320TP-3.2 Rev.B (Driver SPFD5408A)*/
//void Initial_GLCD_Hardware(void);	
//// Only REV C  unsigned char GLCD_SPI_Read_Write(unsigned char DataByte);
//void GLCD_Write_Command(unsigned char GLCD_Command);
//void GLCD_Write_Data(unsigned int GLCD_Data);
//unsigned char TCS_SPI_Write(unsigned char DataByte);
//void TCS_Average_X_Y(unsigned char num);

/** @defgroup STM3210C_EVAL_LCD_Exported_Functions REV D
  * @{
  */ 
  /* Function Control Graphic LCD ET-TFT240320TP-3.2 Rev.D (Driver ILI 9341)*/
void Initial_GLCD_Hardware(void);	
// Only REV C unsigned char GLCD_SPI_Read_Write(unsigned char DataByte);
void GLCD_Write_Command(unsigned char GLCD_Command);   // NEW REV D
void GLCD_Write_Data16(unsigned int GLCD_DataWord);   // NEW REV D
void GLCD_SetX(uint16_t Xpos);                       // NEW REV D
void GLCD_SetY(uint16_t Ypos);                       // NEW REV D
void GLCD_SetCursor(uint16_t Xpos, uint16_t Ypos);    // NEW REV D
char Get_Status_Touch(void);   // NEW REV D
// Only REV C  void GLCD_Write_Data(unsigned int GLCD_Data);
void GLCD_Write_Data(unsigned char GLCD_DataByte);      // NEW REV D
unsigned char TCS_SPI_Write(unsigned char DataByte);

void TCS_Average_X_Y(unsigned char num);

void bg_color(unsigned int bg_color);    // NEW REV D
void dec_to_ascii(unsigned int num);    // NEW REV D
// REV D end

//REV C
//void InitTouchScreen(void);
//void plotFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned long color);
// REV D
void InitTouchScreen(void);
// rma 7.11.2014 void plotFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned long color); // rma 1.11.2014  
// REV D Version  plotFilledRect  // rma 1.11.2014
void plotFilledRect(uint16_t x1, uint16_t y1, uint16_t width, uint16_t high, unsigned long color); 
   



////Start of Horizontal
//void Initial_GLCD_Hor(void);
//void Touch_Calibrate_Hor(void);
//void TCS_SPI_ReadPos(void);
void getTSCxy(void);
//void plotDot(unsigned long Xadd,unsigned long Yadd,unsigned long color);
//void plot_mark_Hor(long x1,long y1,long x2,long y2,long color);
//void line(long x_1,long y_1,long x_2,long y_2,char line_type,long color);
//void circle(long CX,long CY,char Radius,char tick,long color,char fill);
//void ellipse(unsigned int h,unsigned int k, long rx,long ry,char tick,long color,char fill);
//void rectan(long x1,long y1,long x2,long y2, char tick,long color,char fill);
//void plotFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned long color);
//void lcd_printStr_Hor(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
//void plot_tab_Hor(unsigned long color);       
//void plot_picture_Hor(const char *bmp,long xad,long yad,long width,long high); 
// //End of Horizontal
//Start of Horizontal
void Initial_GLCD_Hor(void);
void TCS_Set_Matrix_Hor(void); // NEW REV D eingefuegt
void Touch_Calibrate_Hor(void); // NEW REV D eingefuegt
void TCS_SPI_ReadPos(void);    // REV C von RW
void TCS_SPI_Read_Hor(void);  // NEW REV D eingefuegt
void TCS_Get_Point_Hor(char num);  // NEW REV D eingefuegt
void TSC_getXY(void);   // REV C von RW
unsigned int getTSC_Y(void);
unsigned int getTSC_X(void);
// REV C Version: void plotDot(unsigned long Xadd,unsigned long Yadd,unsigned long color);
void plotDot(unsigned int Xadd,unsigned int Yadd,unsigned int color);
void plot_dot_Hor(unsigned int Xadd,unsigned int Yadd,unsigned int color);  // NEW REV D eingefuegt
void plot_mark_Hor(long x1,long y1,long x2,long y2,long color);   // NEW REV D eingefuegt

void Line_Hor(int x_1,int y_1,int x_2,int y_2,unsigned char line_type,unsigned int color);  
void line(int x_1,int y_1,int x_2,int y_2,unsigned char line_type,unsigned int color);
// REV C Version void line(long x_1,long y_1,long x_2,long y_2,char line_type,long color);

// void circle(long CX,long CY,char Radius,char tick,long color,char fill);  
void Circle_Hor(int CX,int CY,unsigned char Radius,unsigned tick,unsigned int color,unsigned char fill); // NEW REV D eingefuegt

// void ellipse(unsigned int h,unsigned int k, long rx,long ry,char tick,long color,char fill);
// void Ellipse_Hor(unsigned int h,unsigned int k,long rx,long ry,unsigned char tick,unsigned int color,unsigned char fill);

// REV C Vewrsion: void rectan(long x1,long y1,long x2,long y2, char tick,long color,char fill);
void rectan(int x1,int y1,int x2,int y2,unsigned char tick,unsigned int color,unsigned char fill); // from REV D orig file Rectan_VER()

void Rectan_Hor(int x1,int y1,int x2,int y2,unsigned char tick,unsigned int color,unsigned char fill);

// void plotFilledRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, unsigned long color);

// REV C Version: void lcd_printStr_Hor(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
void lcd_printStr_Hor(char *str,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color);
void lcd_print3Cha_Hor(char ch1,char ch2,char ch3,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color);

// REV C Version:  void plot_tab_Hor(unsigned long color);    
void plot_tab_Hor(unsigned int color);       


// REV C Version: void plot_picture_Hor(const char *bmp,long xad,long yad,long width,long high); 
void plot_picture_Hor(char *bmp,unsigned int xad,unsigned int yad,unsigned int width,unsigned int high);  

//End of Horizontal


//// REV C 
////Start of Vertical
//void Initial_GLCD_Vertikal(void);
//void TCS_Set_Matrix_Ver(void);
//void Touch_Calibrate_Ver(void);
//void plot_mark_Ver(long x1,long y1,long x2,long y2,long color);
//void text_7x11(char row,long adx,long ady,long fg_clr,long bg_clr);
//void textxy(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
//void lcd_printStr_Ver(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
//void plot_tab_Ver(unsigned long color);
//void plot_picture_Ver(const char *bmp,long xad,long yad,long width,long high);  
//void lcd_icon_Ver(char row,long adx,long ady,long fg_clr,long bg_clr);
////End of Vertical  REV C

//Start of Vertical  REV D
void Initial_GLCD_Vertikal(void);
void TCS_Set_Matrix_Ver(void);
void Touch_Calibrate_Ver(void);
// REV C Version: void plot_mark_Ver(long x1,long y1,long x2,long y2,long color);
void plot_mark_Ver(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);

// REV C Version: void text_7x11(char row,long adx,long ady,long fg_clr,long bg_clr);
void text_7x11(unsigned char row,int adx,int ady,unsigned int fg_clr,unsigned int bg_clr);

void textxy(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
// REV C Version: void lcd_printStr_Ver(char *str,long cur_x,long cur_y, long fg_color,long bg_color);
void lcd_printStr_Ver(char *str,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color);

// REV C Version: void plot_tab_Ver(unsigned long color);
void plot_tab_Ver(unsigned int color);
// REV C Version: void plot_picture_Ver(const char *bmp,long xad,long yad,long width,long high);  
void plot_picture_Ver(const char *bmp,unsigned int xad,unsigned int yad,unsigned int width,unsigned int high);  

void lcd_icon_Ver(char row,long adx,long ady,long fg_clr,long bg_clr);
// the following lines are from REV D document  
// to do: check if necessary
void Line_Ver(int x_1,int y_1,int x_2,int y_2,unsigned char line_type,unsigned int color);
// REV C: void circle(long CX,long CY,char Radius,char tick,long color,char fill)
void Circle_Ver(int CX,int CY,unsigned char Radius,unsigned char tick,unsigned int color,unsigned char fill); // From REV D File
void circle(int CX,int CY,unsigned char Radius,unsigned char tick,unsigned int color,unsigned char fill); // From REV D File but new name


void Ellipse_Ver(unsigned int h,unsigned int k,long rx,long ry,unsigned char tick,unsigned int color,unsigned char fill);
void ellipse(unsigned int h,unsigned int k,long rx,long ry,unsigned char tick,unsigned int color,unsigned char fill);  // from REV D File new name

void Rectan_Ver(int x1,int y1,int x2,int y2,unsigned char tick,unsigned int color,unsigned char fill);
void lcd_print3Cha_Ver(char ch1,char ch2,char ch3,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color);
void button_build_Ver(unsigned int px,unsigned int py,unsigned int width,unsigned int high,unsigned int c_bt,char *tx,unsigned char num_tx,unsigned int c_tx);
//End of Vertical   REV D




#ifdef __cplusplus
}
#endif

//******************************************************************
#endif // __TouchGrafik_h

//*********************************************************************
//  Peripherie.h  /  26.9.14  /  rw  /  bzu
//  Sammlung vereinfachter Peripheriefunktionen,
//  Parameterbeschreibung siehe unten und Doku 'MinimalPeripherie'
//*********************************************************************
#ifndef __Peripherie_h
#define __Peripherie_h

void GPIOInit(char Port[], char Dir[]);
void GPIOPutByte(char Port[], char Byte);
char GPIOGetByte(char Port[]);
void ADCInit(char Nr, char Pin[]);								
char ADCGetVal(char Nr);												
void DACInit(char Nr);
void DACPutVal(char Nr, char Val);
void TimerInit(char Nr, int Time100us, char IRQPrio);	
int  TimerGetTime(char Nr);
char TimerGetFlag(char Nr);														
void CounterInit(char Nr, char Pin[], char UpDn, char IRQPrio);			
int  CounterGetCount(char Nr);											
void CounterPutCount(char Nr, int Val);
void USARTInit(char Nr, char IRQPrio);
void USARTWrite(char Nr, char c);
char USARTRead(char Nr);
char USARTtoRead(char Nr);
void ExtIRQInit(char Pin[], char Flanke, char IRQPrio);
void IRQClearFlag(char Pin[]);

#endif // __Peripherie_h

//****************************************************************
//  TouchPrint.h  /  15.11.14  /  rw  /  bzu
//  Headerdatei zu Printfunktionen der BZU
//  mit und ohne P0P1Touch verwendbar, aber zwingend mit TouchGrafik
//******************************************************************
#ifndef __TouchPrint_h
#define __TouchPrint_h

void setTextScreen(char dir);
void setTextcolor(long col);
void printAt(char n, char *txt);
void printLn(char *txt);
void print(char *txt);
void printBin(char n, long num);
void printHex(unsigned char n, long num);
void printDec(int len,  long num);
void PrintHeadline(char n);

//*******************************************************************
#endif // __TouchPrint_h

#endif // __TouchP0P1_h

