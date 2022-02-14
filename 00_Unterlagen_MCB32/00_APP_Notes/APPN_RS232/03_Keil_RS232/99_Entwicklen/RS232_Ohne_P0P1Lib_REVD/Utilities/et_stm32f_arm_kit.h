/**
  ******************************************************************************
  * @file    et_stm32f_arm_kit.h
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Header file for stm32f_arm_kit.c module.
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
#ifndef __ET_STM32F_ARM_KIT_H
#define __ET_STM32F_ARM_KIT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
                 
// MCB32  Hardware Board
// 1). LED[0..7] = PE[8..15]
#define LEDn                        8

#define LED0_GPIO_PORT              GPIOE
#define LED0_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED0_GPIO_PIN               GPIO_Pin_8
  
#define LED1_GPIO_PORT              GPIOE
#define LED1_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED1_GPIO_PIN               GPIO_Pin_9
  
#define LED2_GPIO_PORT              GPIOE
#define LED2_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED2_GPIO_PIN               GPIO_Pin_10
  
#define LED3_GPIO_PORT              GPIOE
#define LED3_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED3_GPIO_PIN               GPIO_Pin_11

#define LED4_GPIO_PORT              GPIOE
#define LED4_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED4_GPIO_PIN               GPIO_Pin_12
  
#define LED5_GPIO_PORT              GPIOE
#define LED5_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED5_GPIO_PIN               GPIO_Pin_13
  
#define LED6_GPIO_PORT              GPIOE
#define LED6_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED6_GPIO_PIN               GPIO_Pin_14
  
#define LED7_GPIO_PORT              GPIOE
#define LED7_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define LED7_GPIO_PIN               GPIO_Pin_15
  
// MCB32  Hardware Board
// 2). Switch = PA0(Wakeup)
//            = PC13(Tamper)
//            = PD11(Joy-Up)
//            = PD12(Joy-Left)
//            = PD13(Joy-Down)
//            = PD14(Joy-Right)
//            = PD15(Joy-Select)

#define BUTTONn                     7	

// Switch Wakeup(PA0)
#define WAKEUP_BUTTON_PORT          GPIOA
#define WAKEUP_BUTTON_CLK           RCC_APB2Periph_GPIOA
#define WAKEUP_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOA
#define WAKEUP_BUTTON_PIN           GPIO_Pin_0					
#define WAKEUP_BUTTON_PIN_SOURCE    GPIO_PinSource0
#define WAKEUP_BUTTON_EXTI_LINE     EXTI_Line0
#define WAKEUP_BUTTON_IRQn          EXTI0_IRQn 

// Switch Tamper(PC13)
#define TAMPER_BUTTON_PORT          GPIOC
#define TAMPER_BUTTON_CLK           RCC_APB2Periph_GPIOC
#define TAMPER_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOC
#define TAMPER_BUTTON_PIN           GPIO_Pin_13					
#define TAMPER_BUTTON_PIN_SOURCE    GPIO_PinSource13
#define TAMPER_BUTTON_EXTI_LINE     EXTI_Line13
#define TAMPER_BUTTON_IRQn          EXTI15_10_IRQn 

// Joy Up(PD11)
#define UP_BUTTON_PORT             	GPIOD
#define UP_BUTTON_CLK              	RCC_APB2Periph_GPIOD
#define UP_BUTTON_PORT_SOURCE      	GPIO_PortSourceGPIOD
#define UP_BUTTON_PIN              	GPIO_Pin_11					
#define UP_BUTTON_PIN_SOURCE       	GPIO_PinSource11
#define UP_BUTTON_EXTI_LINE        	EXTI_Line11
#define UP_BUTTON_IRQn             	EXTI15_10_IRQn 

// Joy Left(PD12)
#define LEFT_BUTTON_PORT            GPIOD
#define LEFT_BUTTON_CLK             RCC_APB2Periph_GPIOD
#define LEFT_BUTTON_PORT_SOURCE     GPIO_PortSourceGPIOD
#define LEFT_BUTTON_PIN             GPIO_Pin_12
#define LEFT_BUTTON_PIN_SOURCE      GPIO_PinSource12
#define LEFT_BUTTON_EXTI_LINE       EXTI_Line12
#define LEFT_BUTTON_IRQn            EXTI15_10_IRQn 

// Joy Down(PD13)
#define DOWN_BUTTON_PORT            GPIOD
#define DOWN_BUTTON_CLK             RCC_APB2Periph_GPIOD
#define DOWN_BUTTON_PORT_SOURCE     GPIO_PortSourceGPIOD
#define DOWN_BUTTON_PIN             GPIO_Pin_13				
#define DOWN_BUTTON_PIN_SOURCE      GPIO_PinSource13
#define DOWN_BUTTON_EXTI_LINE       EXTI_Line13
#define DOWN_BUTTON_IRQn            EXTI15_10_IRQn 

// Joy Right(PD14)
#define RIGHT_BUTTON_PORT           GPIOD
#define RIGHT_BUTTON_CLK            RCC_APB2Periph_GPIOD
#define RIGHT_BUTTON_PORT_SOURCE    GPIO_PortSourceGPIOD
#define RIGHT_BUTTON_PIN            GPIO_Pin_14					
#define RIGHT_BUTTON_PIN_SOURCE     GPIO_PinSource14
#define RIGHT_BUTTON_EXTI_LINE      EXTI_Line14
#define RIGHT_BUTTON_IRQn           EXTI15_10_IRQn 

// Joy Select(PD15)
#define SELECT_BUTTON_PORT          GPIOD
#define SELECT_BUTTON_CLK           RCC_APB2Periph_GPIOD
#define SELECT_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOD
#define SELECT_BUTTON_PIN           GPIO_Pin_15					
#define SELECT_BUTTON_PIN_SOURCE    GPIO_PinSource15
#define SELECT_BUTTON_EXTI_LINE     EXTI_Line15
#define SELECT_BUTTON_IRQn          EXTI15_10_IRQn 

// MCB32 (STM32F107VCT6) Hardware Kit 
// 3). UART1(Remap) = PB7:RX1
//                  = PB6:TX1
#define EVAL_COM1                   USART1		 				//COM1 = USART1
#define EVAL_COM1_GPIO              GPIOB						//USART1 Port = PB
#define EVAL_COM1_CLK               RCC_APB2Periph_USART1		//Enable USART1 Clock
#define EVAL_COM1_GPIO_CLK          RCC_APB2Periph_GPIOB		//Enable PB Clock
#define EVAL_COM1_RxPin             GPIO_Pin_7					//RX1=PB7
#define EVAL_COM1_TxPin             GPIO_Pin_6					//TX1=PB6

//     UART2(Remap) = PD6:RX2
//                  = PD5:TX2
#define EVAL_COM2                   USART2						//COM2 = USART2
#define EVAL_COM2_GPIO              GPIOD                       //USART2 Port = PD 
#define EVAL_COM2_CLK               RCC_APB1Periph_USART2		//Enable UART2 Clock
#define EVAL_COM2_GPIO_CLK          RCC_APB2Periph_GPIOD		//Enable PD Clock
#define EVAL_COM2_RxPin             GPIO_Pin_6					//RX2=PD6
#define EVAL_COM2_TxPin             GPIO_Pin_5					//TX2=PD5

// MCB32 (STM32F107VCT6) Hardware Kit 
// 4). I2C1(Remap)  = PB8:SCL1
//                  = PB9:SDA1
#define I2C_EE             			I2C1
#define I2C_EE_CLK         			RCC_APB1Periph_I2C1
#define I2C_EE_GPIO        			GPIOB
#define I2C_EE_GPIO_CLK    			RCC_APB2Periph_GPIOB
#define I2C_EE_SCL         			GPIO_Pin_8
#define I2C_EE_SDA         			GPIO_Pin_9
#define I2C_EE_Speed              	400000
#define I2C_EE_SLAVE_ADDRESS7     	0xA0


// MCB32 (STM32F107VCT6) Hardware Kit
// SPI(SD Card) Interface
// PC5 = CS#
// PA5 = SPI1:SCK
// PA6 = SPI1:MISO
// PA7 = SPI1:MOSI
#define SD_SPI                      SPI1
#define SD_SPI_PORT                 GPIOA
#define SD_SPI_GPIO_PORT_CLOCK      RCC_APB2Periph_GPIOA
#define SD_SPI_PIN_SCK              GPIO_Pin_5			
#define SD_SPI_PIN_MISO             GPIO_Pin_6			
#define SD_SPI_PIN_MOSI             GPIO_Pin_7		   	

#define SD_CS_PORT                  GPIOC
#define SD_CS_GPIO_PORT_CLOCK       RCC_APB2Periph_GPIOC
#define SD_CS_PIN                   GPIO_Pin_5		  	
#define SD_CS_LOW()     			GPIO_ResetBits(SD_CS_PORT, SD_CS_PIN)
#define SD_CS_HIGH()    			GPIO_SetBits(SD_CS_PORT, SD_CS_PIN)

// MCB32 (STM32F107VCT6) Hardware Kit
// Ethernet Interface = RMII Mode
#define RMII_MODE       		/* Used "RMII" mode for MCB32  Board */
#define DP83848_PHY        		/* Chip Ethernet Interface Pins Mapped on Board */
#define PHY_ADDRESS    	0x01 	/* Relative to MCB32  Hardware Board */
#define ETH_RXBUFNB     8 
#define ETH_TXBUFNB     2 

typedef enum 
{
  LED0 = 0,
  LED1 = 1,
  LED2 = 2,
  LED3 = 3,
  LED4 = 4,
  LED5 = 5,
  LED6 = 6,
  LED7 = 7
} Led_TypeDef;

typedef enum 
{  
  Button_WAKEUP = 0,
  Button_TAMPER = 1,
  Button_UP = 2,
  Button_LEFT = 3,
  Button_DOWN = 4,
  Button_RIGHT = 5,
  Button_SELECT = 6
} Button_TypeDef;

typedef enum 
{  
  Mode_GPIO = 0,
  Mode_EXTI = 1
} Button_Mode_TypeDef;

//typedef enum 
//{ 
//  JOY_NONE = 0,
//  JOY_UP = 1,
//  JOY_LEFT = 2,
//  JOY_SELECT = 3,
//  JOY_RIGHT = 4,
//  JOY_DOWN = 5
//} JOY_State_TypeDef;

typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;
 
// Exported Functions
void ET_STM32_LED_Init(Led_TypeDef Led);
void ET_STM32_LED_On(Led_TypeDef Led);
void ET_STM32_LED_Off(Led_TypeDef Led);
void ET_STM32_LED_Toggle(Led_TypeDef Led);
void ET_STM32_LED_Write(uint8_t value);

void ET_STM32_PB_Init(Button_TypeDef Button, Button_Mode_TypeDef Button_Mode);
uint32_t ET_STM32_PB_GetState(Button_TypeDef Button);
void ET_STM32_COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void ET_STM32_ADC_Configuration(void);
void ET_STM32_SD_SPI_Configuration(void);
// rma void ET_STM32_ETH_Configuration(void);
void ET_STM32_RTC_Configuration(void);

#ifdef __cplusplus
}
#endif

#endif /* __ET_STM32F_ARM_KIT_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
