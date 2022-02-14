/**
  ******************************************************************************
  * @file		delay_cz.h
  * @author	   RMA 
  * @date		10.04.2016 
  * @note		Re-writen from Project I2C Intro
  ******************************************************************************
  */
	
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

    // Prototypes 
void DelayInit(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
void delay_us (uint32_t /*uint*/ duration_us);    // Verzögerung mit Einlese-Parameter für Zeit ohne Hilfe eines interrupts

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
