/**
  ******************************************************************************
  * @file    et_stm32f_arm_kit.c
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides firmware functions to manage Leds, push-buttons 
  *          and COM ports available on STM32 Evaluation Boards from STMicroelectronics.
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
//rma #include "stm32_eth.h"

/** @addtogroup Utilities
  * @{
  */ 
  
/** @defgroup STM32_EVAL 
  * @brief This file provides firmware functions to manage Leds, push-buttons 
  *        and COM ports available on STM32 Evaluation Boards from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32_EVAL_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Defines
  * @{
  */ 

//#define TIMEOUT_MAX    0xFFF; /*<! The value of the maximal timeout for I2C waiting loops */

/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

//uint32_t TimeOut = TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */

//static uint16_t IOE_TS_Read_X(void);
//static uint16_t IOE_TS_Read_Y(void);
//static uint16_t IOE_TS_Read_Z(void);

//static void EVAL_GPIO_Config(void);
//static void EVAL_I2C_Config(void);
//static void EVAL_EXTI_Config(void);


// Private Variables 
// ET-STM32F ARM KIT(STM32F107VCT6) Hardware Kit 
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED0_GPIO_PORT, 
                                 LED1_GPIO_PORT, 
								 LED2_GPIO_PORT,
								 LED3_GPIO_PORT, 
                                 LED4_GPIO_PORT, 
								 LED5_GPIO_PORT,
								 LED6_GPIO_PORT,
                                 LED7_GPIO_PORT};

const uint16_t GPIO_PIN[LEDn] = {LED0_GPIO_PIN, 
                                 LED1_GPIO_PIN, 
								 LED2_GPIO_PIN,
								 LED3_GPIO_PIN, 
                                 LED4_GPIO_PIN, 
								 LED5_GPIO_PIN,
								 LED6_GPIO_PIN,
                                 LED7_GPIO_PIN};

const uint32_t GPIO_CLK[LEDn] = {LED0_GPIO_CLK, 
					             LED1_GPIO_CLK, 
								 LED2_GPIO_CLK,
								 LED3_GPIO_CLK, 
					             LED4_GPIO_CLK, 
								 LED5_GPIO_CLK,
								 LED6_GPIO_CLK,
                                 LED7_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_PORT, 
                                      TAMPER_BUTTON_PORT,
									  UP_BUTTON_PORT, 
                                      LEFT_BUTTON_PORT,
									  DOWN_BUTTON_PORT, 
                                      RIGHT_BUTTON_PORT,
                                      SELECT_BUTTON_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN, 
                                      TAMPER_BUTTON_PIN,
                                      UP_BUTTON_PIN, 
                                      LEFT_BUTTON_PIN,
									  DOWN_BUTTON_PIN, 
                                      RIGHT_BUTTON_PIN,
                                      SELECT_BUTTON_PIN}; 

const uint32_t BUTTON_CLK[BUTTONn] = {WAKEUP_BUTTON_CLK, 
                                      TAMPER_BUTTON_CLK,
                                      UP_BUTTON_CLK, 
                                      LEFT_BUTTON_CLK,
									  DOWN_BUTTON_CLK, 
                                      RIGHT_BUTTON_CLK,
                                      SELECT_BUTTON_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {WAKEUP_BUTTON_EXTI_LINE,
                                            TAMPER_BUTTON_EXTI_LINE, 
											UP_BUTTON_EXTI_LINE,
                                            LEFT_BUTTON_EXTI_LINE,
											DOWN_BUTTON_EXTI_LINE,
                                            RIGHT_BUTTON_EXTI_LINE,
                                            SELECT_BUTTON_EXTI_LINE};

const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {WAKEUP_BUTTON_PORT_SOURCE,
                                              TAMPER_BUTTON_PORT_SOURCE, 
											  UP_BUTTON_PORT_SOURCE,
                                              LEFT_BUTTON_PORT_SOURCE, 
											  DOWN_BUTTON_PORT_SOURCE,
                                              RIGHT_BUTTON_PORT_SOURCE, 
                                              SELECT_BUTTON_PORT_SOURCE};
								 
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {WAKEUP_BUTTON_PIN_SOURCE,
                                             TAMPER_BUTTON_PIN_SOURCE, 
											 UP_BUTTON_PIN_SOURCE,
                                             LEFT_BUTTON_PIN_SOURCE, 
											 DOWN_BUTTON_PIN_SOURCE,
                                             RIGHT_BUTTON_PIN_SOURCE, 
                                             SELECT_BUTTON_PIN_SOURCE}; 

const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_IRQn, 
                                       TAMPER_BUTTON_IRQn,
									   UP_BUTTON_IRQn, 
                                       LEFT_BUTTON_IRQn,
									   DOWN_BUTTON_IRQn, 
                                       RIGHT_BUTTON_IRQn,
                                       SELECT_BUTTON_IRQn};

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED0
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void ET_STM32_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED0
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void ET_STM32_LED_On(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led]; 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED0
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  * @retval None
  */
void ET_STM32_LED_Off(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED0
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7 
  * @retval None
  */
void ET_STM32_LED_Toggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

/**
  * @brief  Write Byte to LED.
  * @param  value(byte). 
  * @retval None
  */
void ET_STM32_LED_Write(uint8_t value)
{
  GPIO_WriteBit(GPIO_PORT[LED0], GPIO_PIN[LED0], (value & 0x01) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED1], GPIO_PIN[LED1], (value & 0x02) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED2], GPIO_PIN[LED2], (value & 0x04) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED3], GPIO_PIN[LED3], (value & 0x08) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED4], GPIO_PIN[LED4], (value & 0x10) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED5], GPIO_PIN[LED5], (value & 0x20) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED6], GPIO_PIN[LED6], (value & 0x40) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[LED7], GPIO_PIN[LED7], (value & 0x80) ? Bit_SET : Bit_RESET);
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be one of following parameters:   
  *     @arg Button_WAKEUP: Wakeup Push Button
  *     @arg Button_TAMPER: Tamper Push Button  
  *     @arg Button_KEY: Key Push Button 
  *     @arg Button_RIGHT: Joystick Right Push Button 
  *     @arg Button_LEFT: Joystick Left Push Button 
  *     @arg Button_UP: Joystick Up Push Button 
  *     @arg Button_DOWN: Joystick Down Push Button
  *     @arg Button_SEL: Joystick Sel Push Button
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg Mode_GPIO: Button will be used as simple IO 
  *     @arg Mode_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability  
  * @retval None
  */
void ET_STM32_PB_Init(Button_TypeDef Button, Button_Mode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable Button GPIO clock */
  RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == Mode_EXTI)
  {
    /* Connect Button EXTI Line to Button GPIO Pin */
    GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);  

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(Button != Button_WAKEUP)
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    }
    else
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    }
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:    
  *     @arg Button_WAKEUP: Wakeup Push Button
  *     @arg Button_TAMPER: Tamper Push Button  
  *     @arg Button_UP: Joystick Up Push Button   
  *     @arg Button_LEFT: Joystick Left Push Button   
  *     @arg Button_DOWN: Joystick Down Push Button
  *     @arg Button_RIGHT: Joystick Right Push Button 
  *     @arg Button_SELECT: Joystick Select Push Button    
  * @retval The Button GPIO pin value.
  */
uint32_t ET_STM32_PB_GetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}


/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void ET_STM32_COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  if (COM == COM1)
  {
	/* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(EVAL_COM1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

    /* Enable the USART1 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(EVAL_COM1_CLK, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EVAL_COM1_GPIO, &GPIO_InitStructure);
    
    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EVAL_COM1_GPIO, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(EVAL_COM1, USART_InitStruct);
    
    /* Enable USART */
    USART_Cmd(EVAL_COM1, ENABLE);
  }
  else
  {
    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(EVAL_COM2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(EVAL_COM2_CLK, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM2_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EVAL_COM2_GPIO, &GPIO_InitStructure);
    
    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM2_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EVAL_COM2_GPIO, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(EVAL_COM2, USART_InitStruct);
    
    /* Enable USART */
    USART_Cmd(EVAL_COM2, ENABLE);
  }
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures ADC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ET_STM32_ADC_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* ADC Channel14 config --------------------------------------------------------*/
  /* Relative to STM3210C-EVAL Board   */
  /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//PC4 = ADC
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
}

/**
  * @brief  Configures the SD Card : SPI interface.
  * @param  None
  * @retval None
  */
void ET_STM32_SD_SPI_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* SD_SPI_PORT and SD_CS_PORT Periph clock enable */
  RCC_APB2PeriphClockCmd(SD_SPI_GPIO_PORT_CLOCK | 
                         SD_CS_GPIO_PORT_CLOCK | \
                         RCC_APB2Periph_AFIO, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  /* Configure SD_SPI pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_PIN_SCK | 
                                SD_SPI_PIN_MISO | 
								SD_SPI_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SD_SPI_PORT, &GPIO_InitStructure);

  /* Configure CS pin */
  GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SD_CS_PORT, &GPIO_InitStructure);

  /* SD_SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

  /* Clock speed = fPCLK1 / 256 = 280 kHz at 72 MHz PCLK1 clk. */
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SD_SPI, &SPI_InitStructure);

  /* SD_SPI enable */
  SPI_Cmd(SD_SPI, ENABLE);
}

/**
  * @brief  Configures the DP83848V : Ethernet RMII Interface.
  * @param  None
  * @retval None
  */
//void ET_STM32_ETH_Configuration(void)
//{
//  vu32 Value = 0;
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  ETH_InitTypeDef   ETH_InitStructure;
//  
//  /* Enable ETHERNET Clock  */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC    | 
//                        RCC_AHBPeriph_ETH_MAC_Tx |
//                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

//  // Start Initial GPIO For Interface DP83848V
//  // Initial ET-STM32F ARM KIT Ethernet Interface = RMII Mode
//  // PA1  <- ETH_RMII_REF_CLK(Default)
//  // PA2  -> ETH_RMII_MDIO(Default)
//  // PA8  -> MCO(Default)
//  // PB11 -> ETH_RMII_TXEN(Default)
//  // PB12 -> ETH_RMII_TXD0(Default)
//  // PB13 -> ETH_RMII_TXD1(Default)
//  // PC1  -> ETH_RMII_MDC(Default)
//  // PD8  <- ETH_RMII_RXDV(Remap)
//  // PD9  <- ETH_RMII_RXD0(Remap)
//  // PD10 <- ETH_RMII_RXD1(Remap)

//  // Configure PA2 as alternate function push-pull 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//PA2 = ETH_RMII_MDIO
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  // Configure PC1 as alternate function push-pull 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//PC1 = ETH_RMII_MDC
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);

//  // Configure PB11, PB12 and PB13 as alternate function push-pull 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |			//PB11 = ETH_RMII_TXEN 
//                                GPIO_Pin_12 | 		  	//PB12 = ETH_RMII_TXD0
//								GPIO_Pin_13;			//PB13 = ETH_RMII_TXD1
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
//  // Configure PA1 as input 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//PA1 = ETH_RMII_REF_CLK
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  // ETHERNET pins remapp in ET-STM32F ARM KIT board: RX_DV and RxD[1:0] 
//  // PD8=CRS_DV(RMII Remap)
//  // PD9=RXD0(RMII Remap)
//  // PD10=RXD1(RMII Remap)
//  GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);

//  // Configure PD8, PD9, PD10 as input 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | 			//PD8  = ETH_RMII_RX_DV
//  							    GPIO_Pin_9 | 			//PD9  = ETH_RMII_RXD0
//								GPIO_Pin_10;			//PD10 = ETH_RMII_RXD1
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//  
//  /* Start of Config MCO Clock = 50MHz on PA8 */
//  // Configure MCO (PA8) as alternate function push-pull 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 			//PA8 = MCO
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  // set PLL3 clock output to 50MHz (25MHz / 5 * 10 = 50MHz) 
//  RCC_PLL3Config(RCC_PLL3Mul_10);

//  // Enable PLL3 
//  RCC_PLL3Cmd(ENABLE);

//  // Wait till PLL3 is ready 
//  while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET){}

//  // Get clock PLL3 clock on PA8 pin 
//  RCC_MCOConfig(RCC_MCO_PLL3CLK); 
//  /*End of Initial MCO Clock = 50MHz on PA8	*/
//  
//  //Initial Ethernet Interface = RMII Mode      
//  GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);
//   
//  /* Reset ETHERNET on AHB Bus */
//  ETH_DeInit();
//  
//  /* Software reset */ 
//  ETH_SoftwareReset();
//  
//  /* Wait for software reset */
//  while(ETH_GetSoftwareResetStatus()==SET);

//  /* ETHERNET Configuration ------------------------------------------------------*/
//  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
//  ETH_StructInit(&ETH_InitStructure);
//  
//  /* Fill ETH_InitStructure parametrs */
//  /*------------------------   MAC   -----------------------------------*/
//  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;  

//  //ETH_InitStructure.ETH_Speed = ETH_Speed_100M;  //Test 100 MHz

//  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;              

//  //ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;  //Test

//  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;                                                                                  
//  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;                                                                                                                                                                        
//  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;                                                                                                       
//  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;      
//  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;                                                             
//  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;      
//  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;                        

//  /* Configure ETHERNET */
//  Value = ETH_Init(&ETH_InitStructure, PHY_ADDRESS); 
//}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void ET_STM32_RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  // BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
