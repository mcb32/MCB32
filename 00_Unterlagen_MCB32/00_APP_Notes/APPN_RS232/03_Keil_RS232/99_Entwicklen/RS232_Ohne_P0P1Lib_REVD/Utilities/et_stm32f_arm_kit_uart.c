/**
  ******************************************************************************
  * @file    et_atm32f_arm_kit_uart.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides a set of functions needed to manage the
  *          communication between I2C peripheral and I2C M24CXX EEPROM.
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

/* Includes ------------------------------------------------------------------*/
#include "et_stm32f_arm_kit.h"
#include "et_stm32f_arm_kit_uart.h"

void ET_STM32_USART_Init(void)
{
  USART_InitTypeDef USART_InitStructure;

  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 19200; //9600;                    //rma 10.10.2015 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  //Initial USART1
  ET_STM32_COM_Init(COM1, &USART_InitStructure);
  
  //Initial USART2
  ET_STM32_COM_Init(COM2, &USART_InitStructure);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar (int uart) 
{
  if (uart == 1)
  {
    /* Loop until the USARTz Receive Data Register is not empty */
    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
    {
    }

    return (USART_ReceiveData(EVAL_COM1));
  }
  else
  {
    /* Loop until the USARTz Receive Data Register is not empty */
    while(USART_GetFlagStatus(EVAL_COM2, USART_FLAG_RXNE) == RESET)
    {
    }

    return (USART_ReceiveData(EVAL_COM2));
  }
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar_nb (int uart) 
{
  if (uart == 1)
  {
    /* Verify the USART Receive Data */
    if (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) != RESET)
    {
      return (USART_ReceiveData(EVAL_COM1));
    }
    else
    {
      return 0;
    }
  }
  else
  {
    /* Verify the USART Receive Data */
    if (USART_GetFlagStatus(EVAL_COM2, USART_FLAG_RXNE) != RESET)
    {
      return (USART_ReceiveData(EVAL_COM2));
    }
    else
    {
      return 0;
    }
  }
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar (int uart, int c) 
{
  if (uart == 1)
  {
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
    {}

    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(EVAL_COM1,c);
  }
  else
  {
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(EVAL_COM2, USART_FLAG_TC) == RESET)
    {}

    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(EVAL_COM2,c);

    return c;
  }

  return c;
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int  SER_getChar1(void)
{
  /* Loop until the USARTz Receive Data Register is not empty */
  while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
  {
  }

  return (USART_ReceiveData(EVAL_COM1));
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int  SER_getChar2(void)
{
  /* Loop until the USARTz Receive Data Register is not empty */
  while(USART_GetFlagStatus(EVAL_COM2, USART_FLAG_RXNE) == RESET)
  {
  }

  return (USART_ReceiveData(EVAL_COM2));
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non blocking read)
 *----------------------------------------------------------------------------*/
int  SER_getChar1_nb(void)
{
  /* Verify the USART Receive Data */
  if (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) != RESET)
  {
    return (USART_ReceiveData(EVAL_COM1));
  }
  else
  {
    return 0;
  }
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non blocking read)
 *----------------------------------------------------------------------------*/
int  SER_getChar2_nb(void)
{
  /* Verify the USART Receive Data */
  if (USART_GetFlagStatus(EVAL_COM2, USART_FLAG_RXNE) != RESET)
  {
    return (USART_ReceiveData(EVAL_COM2));
  }
  else
  {
    return 0;
  }
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar1 (int c) 
{
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1,c);
 
  return c;
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar2 (int c) 
{
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM2, USART_FLAG_TC) == RESET)
  {}

  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM2,c);

  return c;
}

/*************************************************************************
* Function Name     : putsUART1
* Description       : This function puts the data string to be transmitted 
*                     into the transmit buffer (till NULL character)
* Parameters        : unsigned int * address of the string buffer to be 
*                     transmitted 
* Return Value      : None
*********************************************************************/
void printUART1(unsigned int *buffer)
{
  char * temp_ptr = (char *) buffer;
  int c;

  while(*temp_ptr != '\0')	 	// End of String
  {
    c = *temp_ptr++;

	if (c == '\n')  			// \n = CR = Enter
    {
	  SER_putChar1 (0x0D);  	// Enter
    }
	else if(c=='\r')			// \r = LF = Line Feed
	{
	  SER_putChar1 (0x0A); 		// Line Feed
	}
	else
	{	
      SER_putChar1 (c); 		// Character
	}
  }
}

/*************************************************************************
* Function Name     : putsUART2
* Description       : This function puts the data string to be transmitted 
*                     into the transmit buffer (till NULL character)
* Parameters        : unsigned int * address of the string buffer to be 
*                     transmitted 
* Return Value      : None
*********************************************************************/
void printUART2(unsigned int *buffer)
{
  char * temp_ptr = (char *) buffer;
  int c;

  while(*temp_ptr != '\0')		// End of String
  {
    c = *temp_ptr++;

	if (c == '\n')  			// \n = CR = Enter
    {
	  SER_putChar2 (0x0D);  	// Enter
    }
	else if(c=='\r')		   	// \r = LF = Line Feed
	{
	  SER_putChar2 (0x0A); 		// Line Feed
	}	
	else
	{
      SER_putChar2 (c); 		// Character
	}
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
