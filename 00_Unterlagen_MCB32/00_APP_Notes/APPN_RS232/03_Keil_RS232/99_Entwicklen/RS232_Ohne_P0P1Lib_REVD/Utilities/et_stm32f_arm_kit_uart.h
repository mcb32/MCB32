/**
  ******************************************************************************
  * @file    et_stm32f_arm_kit_uart.h 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Header for i2c_ee.c module
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __ET_STM32F_ARM_KIT_UART_H
#define __ET_STM32F_ARM_KIT_UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ET_STM32_USART_Init(void);
int  SER_getChar(int uart);
int  SER_getChar_nb(int uart);
int  SER_putChar(int uart, int c);
int  SER_getChar1(void);
int  SER_getChar2(void);
int  SER_getChar1_nb(void);
int  SER_getChar2_nb(void);
int  SER_putChar1(int c);
int  SER_putChar2(int c);
void printUART1(unsigned int *buffer);
void printUART2(unsigned int *buffer);

#endif /* __ET_STM32F_ARM_KIT_UART_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


