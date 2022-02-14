//============================================================================= 
//    Cityline AG   AG,  8630 Rueti, Switzerland 
//============================================================================= 
// Project   :  PCF8574N and SHT3x Sample Code (based on Sensirion AG) (V2.0) 
// File      :  static_lib.h (V2.1) 
// Author    :  MAL 
// Date      :  18.04.2016 
// Controller:  STM32F107VC 
// IDE       :  µVision V5.10.0.x 
// Compiler  :  Keil Default
// Brief     :  Static functions linked to HW used in Main
// 
// Porting to a different microcontroller (uC): 
//   - the definitions of basic types may have to be changed  in typedefs.h 
//   - adapt the button and led functions for your platform   in main.c 
//   - adapt the port functions / definitions for your uC     in i2c_hal.h/.c 
//   - adapt the timing of the delay function for your uC     in system.c 
//   - adapt the SystemInit()                                 in system.c 
//   - change the uC register definition file <stm32f10x.h>   in system.h 
//============================================================================= 
#ifndef __static_lib_H 
#define __static_lib_H

//-- Includes ----------------------------------------------------------------- 

#include "system.h" 

//-- Defines for controlling compiler ----------------------------------------- 
// #define SHT3x_USED 1          // Means the Chip is used  inside this project
#define MCB32D_USED 1            // Means the SW is running on a MCB32 HW (www.mcb32.ch)
//-- Static function prototypes ----------------------------------------------- 
static void EvalBoardPower_Init(void); 
static void Led_Init(void); 
static void UserButton_Init(void); 
static void LedBlueOn(void); 
static void LedBlueOff(void); 
static void LedGreenOn(void); 
static void LedGreenOff(void); 
static u8t ReadUserButton(void); 
  
 
//----------------------------------------------------------------------------- 
static void EvalBoardPower_Init(void)    /* -- adapt this code for your platform -- */ 
{ 
  RCC->APB2ENR |= 0x00000040;  // I/O port (port E 0x00000040) (B =0x00000008) clock enabled 
   
  GPIOE->CRH   &= 0x0FFFFFFF;  // set push-pull output for Vdd & GND pins (0x0FFF0FFF = orig and Port GPIOB .....!!!)
  GPIOE->CRH   |= 0x10000000;  // (0x1000100 = orig)
   
  GPIOE->BSRR = 0x00008000;    // set Bit PE15 to LOW (LED ON) // set Vdd to High, set GND to Low  (rma orig: 0x08008000)
} 
 
//----------------------------------------------------------------------------- 
static void Led_Init(void)               /* -- adapt this code for your platform --*/ 
{ 
 #ifdef MCB32D_USED  
	RCC->APB2ENR |= 0x00000040;  // I/O port E clock enabled  Ref Manual Page 113
  GPIOE->CRH   &= 0x00000000;  // set general purpose output mode for LEDs  (orig : FFFFFF00)
  GPIOE->CRH   |= 0x11111111;  // 01: Output mode, max speed 10 MHz. and 00: General purpose output push-pull
  GPIOE->BSRR   = 0xFFFF0000;  // LEDs off (Orig: 0x03000000 but LED are inverted on MCB32)
	#else
	// write here code for your board
	#endif
}
 
//----------------------------------------------------------------------------- 
static void UserButton_Init(void)        /* -- adapt this code for your platform -- */ 
{ 
   #ifdef MCB32D_USED 
	RCC->APB2ENR |= 0x00000004;  // I/O port A clock enabled 
  GPIOA->CRH   &= 0xFFFFFFF0;  // set general purpose input mode for User Button 
  GPIOA->CRH   |= 0x00000004;  // 
	#else
	// write here code for your board
	#endif
} 
 
//----------------------------------------------------------------------------- 
static void LedBlueOn(void)              /* -- adapt this code for your platform -- */ 
{ 
  GPIOE->BSRR = 0x00000100; 
} 
 
//----------------------------------------------------------------------------- 
static void LedBlueOff(void)             /* -- adapt this code for your platform -- */ 
{ 
  GPIOE->BSRR = 0x01000000; 
} 
 
//----------------------------------------------------------------------------- 
static void LedGreenOn(void)             /* -- adapt this code for your platform -- */ 
{ 
  GPIOE->BSRR = 0x00000200; 
} 
 
//----------------------------------------------------------------------------- 
static void LedGreenOff(void)            /* -- adapt this code for your platform -- */ 
{ 
  GPIOE->BSRR = 0x02000000; 
} 
 
//-----------------------------------------------------------------------------  
 
   

     
static u8t ReadUserButton(void)          /* -- adapt this code for your platform -- */ 
{ 
#ifdef MCB32D_USED  
	return (GPIOA->IDR & 0x00000001);   // MCB32 environment WakeUp Switch
#else
	return (GPIOA->IDR & 0x00000001);   // Sensirion environment
#endif
	
} 
#endif


// ENd of File
