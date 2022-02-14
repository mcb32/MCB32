/**
  ******************************************************************************
  * @file    IOExpander/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    06/19/2009
  * @brief   Header for main.c module
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "et_stm32f_arm_kit_glcd.h"    // rma für Zeitversuch 05112013
#include "et_stm32f_arm_kit.h"
#include "et_stm32f_arm_kit_lcd.h"
//#include "et_stm32f_arm_kit_i2c.h"
//#include "et_stm32f_arm_kit_uart.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BUTTON_MODE_GPIO
//#define BUTTON_MODE_EXTI

#ifdef BUTTON_MODE_GPIO
  #define BUTTON_MODE    Mode_GPIO
#else
  #define BUTTON_MODE    Mode_EXTI
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(vu32 nTime);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
