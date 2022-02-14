/**
  ******************************************************************************
  * @file    Project/Template/main.c 
  * @author  MCD Application Team   / Erweiterung rma 12.3.2014
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Main. mit Erweiterung im Bereich RTC Test. Ausgabe auf LCD (RTC, ADC)
  ******************************************************************************
  * @copy
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 R. Malacarne</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

/* Difference Config TFT240320TP Rev.B & Rev.C */
//Change Code in Function
//File "et_stm32f_arm_kit_lcd.c"
//void LCD_Setup(void)

/* Old Initial For : ET-TFT240320TP-3.2 REV.B */
//LCD_WriteReg(R96,  0x2700); /* Gate Scan Line For ET-TFT240320TP3.2 RevB */

/* New Initial For : ET-TFT240320TP-3.2 REV.C */
//LCD_WriteReg(R96,  0xA700); /* Gate Scan Line For ET-TFT240320TP3.2 RevC */

//TCPIP Web
//#include "stm32_eth.h"
//#include "uip.h"

//SD Card(Micro-SD + EFSL FAT)
//#include "efs.h"
//#include "ls.h"
//SD Card(Micro-SD + EFSL FAT)

//USB_MSD
// #include "usb_lib.h"
#include "et_stm32f_arm_kit_usb.h"
// #include "usb_pwr.h"
extern uint16_t MAL_Init (uint8_t lun);
//USB_MSD

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*---------------LCD Messages ------------------------------------------------*/
#define ETH_MESSAGE1   " STM32 Connectivity " 
#define ETH_MESSAGE2   "     Line Device    " 
#define ETH_MESSAGE3   "WebServer Demo based"
#define ETH_MESSAGE4   "      on uIP        "
#define ETH_MESSAGE5   " IP address is:     "
#define ETH_MESSAGE6   "   192.168.1.234    "	//Config in file "uipopt.h","fsdata.c"

// Ethernet Interface(RMII Mode)
#define RMII_MODE       		/* Used "RMII" mode for ET-STM32F ARM KIT Board */
#define DP83848_PHY        		/* Chip Ethernet Interface Pins Mapped on Board */
#define PHY_ADDRESS    	0x01 	/* Relative to ET-STM32F ARM KIT Hardware Board */
#define ETH_RXBUFNB     8 
#define ETH_TXBUFNB     2 
// Private Variables
long divider,An,Bn,Cn,Dn,En,Fn,X,Y;		
char num1,num2,num3,num4;
unsigned int start_time, stop_time, cycle_count;  // used for Execution Time purposes

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__

  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif /* __GNUC__ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay =0;
__IO uint32_t TimeDisplay = 0;


char uart_buf[512]; 
uint8_t eep_buffer[8];

//Bargraph LED Display
const unsigned long led_graph[8] = {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};

/* Private functions ---------------------------------------------------------*/
void delay_ms(__IO uint32_t nTime);	 //Delay mS

void ET_STM32_RCC_Configuration(void);
void ET_STM32_NVIC_Configuration(void);
void test_gpio(void);		//0
void test_eeprom(void);		//1
void test_adc(void);		//2
void test_adc_mcb32(void);		//2
void test_switch(void);		//3
void test_rtc(void);		//4
void test_rtc_mcb32(void);		//4
void test_glcd_touch(void);	//5
// void test_uart(void);		//6
// void test_SDCard(void);		//7
// void test_usb_msd(void);	//8
// void test_eth(void);		//9
void uIPMain(void);
void hang(void);

// Private Function for RMA for HEX to ASCII Conversion
// from http://stackoverflow.com/questions/5403103/hex-to-ascii-string-conversion

int hex_to_int(char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

int hex_to_ascii(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return high+low;
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  
	int uart_rx;
	// long cr_x,cr_y,x1,y1,x2,y2,s_color=BRIGHT_YELLOW;
       
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(SystemFrequency / 1000))        // Prescaler = 1000 => tick =1mS
  { 
    /* Configuration Error and Capture error */                  // rma_06072014 Wenn der Zähler fehler meldet können die hier abgefangen werden. Configuration Error
    while (1);                 // Bleibt stehen wenn Fehler in SysConfig für Tinmer
  }
  
  /* System Clocks Configuration */
  ET_STM32_RCC_Configuration();
       
  /* NVIC configuration */
  ET_STM32_NVIC_Configuration();

  // Initialize Hardware ET-STM32F ARM KIT(STM32F107VCT6)
  //1) Initial LED
  ET_STM32_LED_Init(LED0);
  ET_STM32_LED_Init(LED1);
  ET_STM32_LED_Init(LED2);
  ET_STM32_LED_Init(LED3);
  ET_STM32_LED_Init(LED4);
  ET_STM32_LED_Init(LED5);
  ET_STM32_LED_Init(LED6);
  ET_STM32_LED_Init(LED7);

  //Default = OFF All LED
  ET_STM32_LED_Off(LED0);
  ET_STM32_LED_Off(LED1);
  ET_STM32_LED_Off(LED2);
  ET_STM32_LED_Off(LED3);
  ET_STM32_LED_Off(LED4);
  ET_STM32_LED_Off(LED5);
  ET_STM32_LED_Off(LED6);
  ET_STM32_LED_Off(LED7);
	// Switch LED x ON
  ET_STM32_LED_On(LED7);  // rma 

	
  //2) Initial Push-Button Switch
  ET_STM32_PB_Init(Button_WAKEUP, BUTTON_MODE);
  ET_STM32_PB_Init(Button_TAMPER, BUTTON_MODE);
  ET_STM32_PB_Init(Button_UP, BUTTON_MODE);
  ET_STM32_PB_Init(Button_LEFT, BUTTON_MODE);
  ET_STM32_PB_Init(Button_DOWN, BUTTON_MODE);
  ET_STM32_PB_Init(Button_RIGHT, BUTTON_MODE);
  ET_STM32_PB_Init(Button_SELECT, BUTTON_MODE);
  
  //3) Initial I2C-EEPROM Interface
  I2C_EE_Init();  

  //4) Initial ADC(ADC14:PC4)
  ET_STM32_ADC_Configuration();

  //5) Initial USART
  ET_STM32_USART_Init(); //USART1,USART2=115200bps,Noparity,8Bit Data,1Stop Bit
 
  //6) Initialize TFT-GLCD 320x240 
  ET_STM32_LCD_Init();

  //7) Initial Touch Sensor ADS7846
  ET_STM32_TCS_Config();

  //8)Initial USB_MSD
  MAL_Config();
//  USB_Init();
  
  // Clear Screen GLCD 
  LCD_Clear(White);
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);     
  LCD_DisplayStringLine(Line0, "                    ");
  LCD_DisplayStringLine(Line1, "     MCB 32 KIT     ");
  LCD_DisplayStringLine(Line2, "STM32F107VCT6-100Pin");
  LCD_DisplayStringLine(Line3, "     WWW.MCB32.CH    ");  
  LCD_DisplayStringLine(Line4, "                    ");
  delay_ms(500);
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
  LCD_DisplayStringLine(Line5, "                    ");
  LCD_DisplayStringLine(Line6, "System Hardware Test");
  LCD_DisplayStringLine(Line7, "--------------------");
  LCD_DisplayStringLine(Line8, "                    ");
  LCD_DisplayStringLine(Line9, "rma/121213  ,HT_V002");
	delay_ms(1500);
  //Start of Menu Test
  /* sprintf(uart_buf,"\n\n\n\n");		
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"**********************************************\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"ET-STM32F ARM KIT(STM32F107VCT6) Hardware Test\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"**********************************************\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"->Ethernet LAN : Webserver Demo(192.168.1.234)\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"->USB Device   : USB Mass Storage(SD Card)\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"**********************************************\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"0. Test GPIO\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"1. Test I2C EEPROM\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"2. Test ADC\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"3. Test PB & JOY SWITCH\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"4. Test RTC\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"5. Test GLCD & TOUCH\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"6. Test UART\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"7. Test Micro-SD Card\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"8. Test USB Device(Mass Storage)\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);		
  sprintf(uart_buf,"9. Test Ethernet LAN\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"**********************************************\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"Select[0...9] For Test\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"**********************************************\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  sprintf(uart_buf,"UART1>Select...");
  printUART1((unsigned int *)uart_buf);
  sprintf(uart_buf,"UART2>Select...");
  printUART2((unsigned int *)uart_buf);
  //End of Menu Test
*/ 
  //Loop Wait Choice Command
  while(1)													// Loop Continue
  {
	//verify UART1 Receive Data
	// uart_rx =  SER_getChar_nb(1);
	uart_rx='4';     // Insert Test Number here  TBZ rma_04112013
	
	if (uart_rx>0)
	{
	  SER_putChar1(uart_rx);
      switch (uart_rx)
	  {			 
	    		   // Test GPIO  
      case '0' : LCD_Clear(White);
								 LCD_DisplayStringLine(Line0, "Test GPIO");
								 test_gpio();								
	               break;

	  			   // Test I2C EEPROM
	    case '1' : LCD_Clear(White);
								 LCD_DisplayStringLine(Line0, "Test I2C EEPROM");
								 test_eeprom();								 
	               break;

				   // Test ADC
	    case '2' : LCD_Clear(White);
								 LCD_DisplayStringLine(Line0, "Test ADC");
			           test_adc();								
	               break;

				   // Test Switch 
      case '3' : LCD_Clear(White);
								 LCD_DisplayStringLine(Line0, "Test Switch");
			           test_switch();								
	               break;

				   // Test RTC
	    case '4' : LCD_Clear(White);
								 LCD_DisplayStringLine(Line0, "MCB32: RTC/ADC");
								 // test_adc_mcb32();									 // rma_21122013 neuer test
								 test_rtc_mcb32();								// rma_21122013 neuer test	mit ADC						
	               break;

				   //Test GLCD
	    case '5' : LCD_Clear(White);
								 LCD_DisplayStringLine(Line0, "Test GLCD");
								 test_glcd_touch(); 		  		 
	               break;

	  			   //Test UART
	     		   //Test Micro-SD Card
	     		   //Test USB Device(Mass Storage)
	    		   //Test Ethernet LAN  
      } 
    }

  /*   // UART2 not used at the moment
	//verify UART2 Receive Data
	uart_rx =  SER_getChar_nb(2);
	if (uart_rx>0)
	{
	  SER_putChar2(uart_rx);
      switch (uart_rx)
	  {			 
	    		   //Test GPIO  
        case '0' : test_gpio();								
	               break;

	  			   // Test I2C EEPROM
	    case '1' : test_eeprom();								 
	               break;

				   // Test ADC		   
		case '2' : test_adc();								
	               break;

				   // Test Switch 
        case '3' : test_switch();								
	               break;

				   // Test RTC
	    case '4' : test_rtc();								
	               break;

				   //Test GLCD
	    case '5' : test_glcd_touch(); 		  		 
	               break;
								 
	  			   //Test UART
	     		   //Test Micro-SD Card
	     		   //Test USB Device(Mass Storage)
	    		   //Test Ethernet LAN  

      } 
    }		
		*/
		
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay_ms(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0)
  {
  }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  Configures the clock
  * @param  None
  * @retval None
  */
void ET_STM32_RCC_Configuration(void)
{
  RCC_ClocksTypeDef RCC_ClockFreq;
  ErrorStatus HSEStartUpStatus;

   /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /***************************************************/
    /* HSE=25MHz, HCLK=72MHz, PCLK2=72MHz, PCLK1=36MHz */
    /***************************************************/

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /*  ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* Configure PLLs */
    /* PPL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PPL1 configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }

  //Initial System Clock Complete Start Initial Peripheral Clock
  RCC_GetClocksFreq(&RCC_ClockFreq);

  //Start of Initial USB Clock input (48MHz)
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
  //End of Initial USB Clock input (48MHz)

  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	


  /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC    | 
                        RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

  /* Enable GPIOs clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |	
                         RCC_APB2Periph_GPIOB | 
						 RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | 
						 RCC_APB2Periph_GPIOE | 
						 RCC_APB2Periph_AFIO, ENABLE);

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void ET_STM32_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  //RTC Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //RTC Interrupt

  //USB_MSD Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  //USB_MSD Interrupt
}

/*******************************/
/* 0 : Test GPIO : 20 Bit GPIO */
/* -> PA3,PA4				   */
/* -> PB0,PB1,PB5			   */
/* -> PB10,PB14,PB15		   */
/* -> PC0,PC2,PC3,PC6,PC7	   */
/* -> PD0,PD1,PD2,PD3,PD4	   */
/* -> PE0,PE2				   */
/*******************************/
void test_gpio(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  // -> PA3,PA4
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_3 | 
                                 GPIO_Pin_4);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // -> PB0,PB1,PB5,PB10,PB14,PB15
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | 
                                 GPIO_Pin_1 | 
								 GPIO_Pin_5 | 
								 GPIO_Pin_10 | 
								 GPIO_Pin_14 | 
                                 GPIO_Pin_15);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // -> PC0,PC2,PC3,PC6,PC7
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | 
                                 GPIO_Pin_2 | 
								 GPIO_Pin_3 | 
								 GPIO_Pin_6 | 								 
                                 GPIO_Pin_7);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // -> PD0,PD1,PD2,PD3,PD4
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | 
                                 GPIO_Pin_1 | 
								 GPIO_Pin_2 | 
								 GPIO_Pin_3 | 								 
                                 GPIO_Pin_4);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // -> PE0,PE2
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 |                                  
                                 GPIO_Pin_2 |
								 GPIO_Pin_8 |                                  
                                 GPIO_Pin_9 |
								 GPIO_Pin_10 |                                  
                                 GPIO_Pin_11 |
								 GPIO_Pin_12 |                                  
                                 GPIO_Pin_13 |
								 GPIO_Pin_14 |                                  
                                 GPIO_Pin_15);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  sprintf(uart_buf,"\n\n\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"GPIO Testing...\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PA3,PA4\n");	
	LCD_DisplayStringLine(Line1, "PA3,PA4...");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PB0,PB1,PB5\n");
  LCD_DisplayStringLine(Line2, "PB0,PB1,PB5...");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PB10,PB14,PB15\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PC0,PC2,PC3,PC6,PC7\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PD0,PD1,PD2,PD3,PD4\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PE0,PE2\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  while(1)
  {
    //Set 
	//===================================================
	//BIT0 |BIT1 |BIT2 |BIT3 |BIT4 |BIT5 |BIT6 |BIT7
    //BIT8 |BIT9 |BIT10|BIT11|BIT12|BIT13|BIT14|BIT15
	//===================================================
	//     |     |     |PA3  |PA4  |	 |	   |
	//===================================================
	//PB0  |PB1  |     |     |     |PB5	 |	   |
	//===================================================
	//     |     |PB10 |     |     |     |PB14 |PB15
	//===================================================
	//PC0  |     |PC2  |PC3  |     |     |PC6  |PC7
	//===================================================
	//PD0  |PD1  |PD2  |PD3  |PD4  |	 |	   |
	//===================================================
	//PE0  |     |PE2  |	 |	   |	 |	   |
	//===================================================
	//PE8  | PE9 |PE10 |PE11 |PE12 |PE13 |PE14 |PE15
	//===================================================

	//===================================================	
	//Step-1 : PB0,PC0,PD0,PE0,PE8
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET); 	  	//PB0
	GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET); 	  	//PC0
	GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_SET); 	  	//PD0
	GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_SET); 	  	//PE0
	GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_SET); 	  	//PE8	
	delay_ms(500);								  	
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);   	//PB0
	GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);   	//PC0
	GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_RESET);   	//PD0
	GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_RESET);   	//PE0
	GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET); 	//PE8
		
	//===================================================
	//Step-2 : PB1,PD1,PE9
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);	  	//PB1
	GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_SET);	  	//PD1
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET); 	  	//PE9
	delay_ms(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);  	//PB1
	GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_RESET);  	//PD1
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET); 	//PE9

	//===================================================
	//Step-3 : PB10,PC2,PD2,PE2,PE10
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);	  	//PB10
	GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);	  	//PC2
	GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET);	  	//PD2
	GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_SET);  	//PE2
	GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_SET);   	//PE10
	delay_ms(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);	//PB10
	GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);	//PC2
	GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_RESET);	//PD2
	GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_RESET);  	//PE2
	GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_RESET);  	//PE10

	//===================================================	
    //Step-4 : PA3,PC3,PD3,PE11
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET); 		//PA3
	GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_SET);		//PC3
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_SET);		//PD3
    GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_SET); 	//PE11
	delay_ms(500);
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET); 	//PA3
	GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);	//PC3
	GPIO_WriteBit(GPIOD, GPIO_Pin_3, Bit_RESET);	//PD3
	GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_RESET);  	//PE11

	//===================================================
	//Step-5 : PA4,PD4,PE12
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);		//PA4
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET);		//PD4
	GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET); 	//PE12
	delay_ms(500);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);	//PA4
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET);	//PD4
	GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET); 	//PE12

	//===================================================
	//Step-6 : PB5,PE13
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);		//PB5
	GPIO_WriteBit(GPIOE, GPIO_Pin_13, Bit_SET);	  	//PE13
	delay_ms(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);	//PB5
	GPIO_WriteBit(GPIOE, GPIO_Pin_13, Bit_RESET);  	//PE13

	//===================================================
	//Step-7 : PB14,PC6,PE14
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);		//PB14	
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);		//PC6
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET); 	//PE14
	delay_ms(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);	//PB14	
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);	//PC6
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_RESET);  	//PE14

	//===================================================
	//Step-8 : PB15,PC7,PE15
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);		//PB15
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);	   	//PC7
	GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_SET);   	//PE15
	delay_ms(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);	//PB15
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);	//PC7
	GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_RESET);  	//PE15
  }
}

/*****************************/
/* 1: Test I2C EEPROM(24C01) */
/*****************************/
void test_eeprom(void)
{
  int i;
  sprintf(uart_buf,"\n\n\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"I2C EEPROM Testing...\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"LED Moving on LED[0..7]\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  /* Test Write I2C EEPROM */
  eep_buffer[0] = 0x01;
  eep_buffer[1] = 0x02;
  eep_buffer[2] = 0x04;
  eep_buffer[3] = 0x08;
  eep_buffer[4] = 0x10;
  eep_buffer[5] = 0x20;
  eep_buffer[6] = 0x40;
  eep_buffer[7] = 0x80;
  I2C_EE_BufferWrite(eep_buffer, 0x00,8);  				// Write 8 Byte From Address 0x00...

  eep_buffer[0] = 0x01;
  eep_buffer[1] = 0x03;
  eep_buffer[2] = 0x07;
  eep_buffer[3] = 0x0F;
  eep_buffer[4] = 0x1F;
  eep_buffer[5] = 0x3F;
  eep_buffer[6] = 0x7F;
  eep_buffer[7] = 0xFF;
  I2C_EE_BufferWrite(eep_buffer, 0x10,8); 				// Write 8 Byte From Address 0x10...
  
  while(1)
  {
    /* Read from I2C EEPROM from EEPROM_ReadAddress1 */
    I2C_EE_BufferRead(eep_buffer,0x00,8); 
	for ( i = 0; i < 8; i++ )							// Read 8 Byte to Buffer 
    {
	  ET_STM32_LED_Write(eep_buffer[i]); 
	  delay_ms(250);
    }

	/* Read from I2C EEPROM from EEPROM_ReadAddress1 */
    I2C_EE_BufferRead(eep_buffer,0x10,8); 
	for ( i = 0; i < 8; i++ )							// Read 8 Byte to Buffer 
    {
	  ET_STM32_LED_Write(eep_buffer[i]); 
	  delay_ms(250);
    }
  }
}

/****************/
/* 2 : Test ADC */
/****************/
void test_adc(void)
{
  int ADCVal = 0;  

  sprintf(uart_buf,"\n\n\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"ADC Testing...\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"Ajust Volume & Display Result on LED Bargraph\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  while (1) 
  {    
    /* obtain adc converted value of potmeter position */
    ADCVal = ADC_GetConversionValue(ADC1);
	ET_STM32_LED_Write(led_graph[ADCVal/512]); 			//Display ADC to Bargraph LED 
  }
}

/****************/
/* 2 : Test ADC MCB32 */
/****************/
void test_adc_mcb32(void)
{
  int ADCVal = 0;  
	ADCVal = ADC_GetConversionValue(ADC1);
  // while ((ADCVal) < 4000)   // rma  es ist ein 12Bit adc (also 4095 ..)
  {    
    /* obtain adc converted value of potmeter position */
    ADCVal = ADC_GetConversionValue(ADC1);
	  ET_STM32_LED_Write(led_graph[ADCVal/512]); 			//Display ADC to Bargraph LED 
  }
}

/*********************************************/
/* 3 : Test Button Switch (BUTTON_MODE_GPIO) */
/*********************************************/
void test_switch(void)
{
  sprintf(uart_buf,"\n\n\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"PB & JOY Switch Testing...\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"Display Switch Status on LED\n");
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  while(1)
  {
    //Wakeup(Toggle Logic:Press=0,Release=1)
    if (ET_STM32_PB_GetState(Button_WAKEUP) != 0)
    {
      ET_STM32_LED_On(LED0);	//Press  
    }
	else
	{
	  ET_STM32_LED_Off(LED0);	//Release 
	}

	//Tamper(Press=0,Release=1)
	if (ET_STM32_PB_GetState(Button_TAMPER) == 0)
    {
      ET_STM32_LED_On(LED1);	//Press
    }
	else
	{
	  ET_STM32_LED_Off(LED1);	//Release
	}

	//Up(Press=0,Release=1)
	if (ET_STM32_PB_GetState(Button_UP) == 0)
    {
      ET_STM32_LED_On(LED3);	//Press
    }
	else
	{
	  ET_STM32_LED_Off(LED3);	//Release
	}

	//Down(Press=0,Release=1)
	if (ET_STM32_PB_GetState(Button_DOWN) == 0)
    {
      ET_STM32_LED_On(LED7);	//Press
    }
	else
	{
	  ET_STM32_LED_Off(LED7);	//Release
	}

	//Left(Press=0,Release=1)
	if (ET_STM32_PB_GetState(Button_LEFT) == 0)
    {
      ET_STM32_LED_On(LED4);	//Press
    }
	else
	{
	  ET_STM32_LED_Off(LED4);	//Release
	}

	//Right(Press=0,Release=1)
	if (ET_STM32_PB_GetState(Button_RIGHT) == 0)
    {
      ET_STM32_LED_On(LED6);	//Press
    }
	else
	{
	  ET_STM32_LED_Off(LED6);	//Release
	}

	//Select(Press=0,Release=1)
	if (ET_STM32_PB_GetState(Button_SELECT) == 0)
    {
      ET_STM32_LED_On(LED5);	//Press
    }
	else
	{
	  ET_STM32_LED_Off(LED5);	//Release
	}
  }
}

/****************/
/* 4 : Test RTC */
/****************/
void test_rtc(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
  uint32_t THH = 0, TMM = 0, TSS = 0;
  uint32_t TimeVar;
	long x1,x2,y1,y2;

  // RTC Configuration 
  ET_STM32_RTC_Configuration();

  // Adjust time by values entred by the user on the hyperterminal 
  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();

  //Setup Time = 00:00:00
  Tmp_HH = 12; 
  Tmp_MM = 0; 
  Tmp_SS = 0; 

  // Change the current time 
  RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);

  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);    // rma 1 sekunden Interrupt wird gestartet

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Clear reset flags */
  RCC_ClearFlag();

  sprintf(uart_buf,"\n\n\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"RTC Testing...\n");	
	
	
	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  while (1)
  {
    /* If 1s has passed */
    if (TimeDisplay == 1)    // rma wird in  RTC_IRQHandler gesetzt wenn eine Sekunde vorbei ist.
    {
	  TimeVar = RTC_GetCounter();

	  /* Compute  hours */
      THH = TimeVar / 3600;

      /* Compute minutes */
      TMM = (TimeVar % 3600) / 60;

      /* Compute seconds */
      TSS = (TimeVar % 3600) % 60;

      sprintf(uart_buf,"Time: %0.2d:%0.2d:%0.2d\n", THH, TMM, TSS);
			LCD_DisplayStringLine(Line2, "MCB32: Time Demo ");
		  // rma LCD_DisplayStringLine(Line3, "Time:%0.2d:%0.2d\n",TMM, TSS");
	// ----------------------------------------- rma Spielwiese
  //	int x1,x2,y1,y2;
	//x1=80+(num2*5);
	// x2=64+(num2*4);
	// y1=180;
	// y2=200;
	// plot_mark_Hor(x1,y1,x2,y2,BLUE); 			//Del Cursot '+' in Position Touch old
	// x1 = x1-(10*num3) ;
	// y1 = y1   ;
	// x2 = x2   ;
	// y2 = y1-(7*num3) ;
	// plot_mark_Hor(x1,y1,x2,y2,RED); 				//Plot Cursot '+' in Position Touch Screen
  // plot_mark_Hor(200,150,200,200,RED);  // Test

	lcd_printStr_Ver("Sekunden: ",100,130,BLACK,no_bg); 	//Plot String Color White
	lcd_printStr_Ver("Minuten : ",100,150,BLACK,no_bg);

	dec_to_ascii(TSS); 															//Convert Seconds to ASCII
	lcd_print3Cha_Ver(num1,num2,num3,175,130,BRIGHT_YELLOW,BLACK); 		//Plot Seconds on GLCD
	dec_to_ascii(TMM); 															//Convert Minutes to ASCII
	lcd_print3Cha_Ver(num1,num2,num3,175,150,BRIGHT_YELLOW,BLACK);  //Plot Minutes on GLCD	

	
		
	// ------------------------------------------
			
      printUART1((unsigned int *)uart_buf);
      printUART2((unsigned int *)uart_buf);

      TimeDisplay = 0;
    }
  }
}

/***********************/
/* 4 : Test RTC MCB32  */
/***********************/
// rma_21122013 mit ADC Test

void test_rtc_mcb32(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
  uint32_t THH = 0, TMM = 0, TSS = 0;
  uint32_t TimeVar;
	long x1,x2,y1,y2;

  // RTC Configuration 
  ET_STM32_RTC_Configuration();

  // Adjust time by values entred by the user on the hyperterminal 
  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();

  //Setup Time = 00:00:00
  Tmp_HH = 12; 
  Tmp_MM = 0; 
  Tmp_SS = 0; 

  // Change the current time 
  RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);

  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);   //rma starte IR für 1 Sekunde

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Clear reset flags */
  RCC_ClearFlag();

  LCD_DisplayStringLine(Line2, "       Zeit/ADC Demo");
  // rma while ((TSS%12)<11)    // rma_21122013 
	while(1)
  {
	   /* If 1s has passed */
    if (TimeDisplay == 1)            // rma TimeDisplay wird in  RTC_IRQHandler gesetzt wenn eine Sekunde vorbei ist.
    {
	     TimeVar = RTC_GetCounter();

	    /* Compute  hours */
      THH = TimeVar / 3600;

      /* Compute minutes */
      TMM = (TimeVar % 3600) / 60;

      /* Compute seconds */
      TSS = (TimeVar % 3600) % 60;

      // sprintf(uart_buf,"Time: %0.2d:%0.2d:%0.2d\n", THH, TMM, TSS);
		
			lcd_printStr_Ver("Sekunden  : ",85,130,BLACK,no_bg); 	//Plot String Color White
			lcd_printStr_Ver("Minuten   : ",85,150,BLACK,no_bg);

			dec_to_ascii(TSS); 															//Convert Seconds to ASCII
			lcd_print3Cha_Ver(num1,num2,num3,180,130,BRIGHT_YELLOW,BLACK); 		//Plot Seconds on GLCD
			dec_to_ascii(TMM); 															//Convert Minutes to ASCII
			lcd_print3Cha_Ver(num1,num2,num3,180,150,BRIGHT_YELLOW,BLACK);  //Plot Minutes on GLCD	
	
	    // ------------------------------------------
			
      TimeDisplay = 0;
    }
		// rma_21122013  ADC Test hier eingefügt damit die Show besser ist. Das heisst der ADC läuft so schnell wie möglich.
		SysTick -> CTRL=0;
	  SysTick ->LOAD=0x00FFFFFF;  // 24bIT
	  SysTick ->VAL =0;
	  SysTick ->CTRL=5;
	  while(SysTick ->VAL !=0){};
	  start_time =SysTick ->VAL;   // Stoppuhr gestartet  rma_05072014
		test_adc_mcb32(); 
		stop_time =SysTick ->VAL;
		     
				 dec_to_ascii_4digit((ADC_GetConversionValue(ADC1)));  // Adjust for Valuerange
			   lcd_printStr_Ver("ADC       : ",85,170,BLACK,no_bg); 	//Plot String Color White
		     lcd_print4Cha_Ver(num1,num2,num3,num4,180,170,BRIGHT_YELLOW,BLACK);  //Plot ADC on GLCD	
	  
		cycle_count=start_time-stop_time;
		cycle_count=cycle_count & 0x00000FFF; // delete unnecessary Bits for conversion to Ascii because we have only some digits.
		dec_to_ascii_4digit(cycle_count);  // First Test, better HEX_toASCII
		lcd_printStr_Ver("Execution: ",85,200,BLACK,no_bg); 	//Plot String Color White
		lcd_print4Cha_Ver(num1,num2,num3,num4,180,200,BRIGHT_YELLOW,BLACK);  //Write Execution Time to CRT
			
    // hex_to_ascii(cycle_count*16, cycle_count);
		// x1 = hex_to_int(cycle_count)*16;  // Bastle am wert für Darstellung
		// x2  = hex_to_int(cycle_count);
		// dec_to_ascii_4digit(x1+x2);
			// so nun testen wir den SysTick auch noch
		SysTick->LOAD=(1677721);   // lade SysTick mit Wert grösser als 230ms. 
	  SysTick->VAL=0;             
		//start_time =((SysTick ->VAL));
		ET_STM32_LED_On(LED5);
		// Stoppuhr gestartet  rma_05072014
		while((SysTick->VAL) !=0)
         {  // Wait 230ms
					 ET_STM32_LED_Off(LED3);
				 };
	   	
		SysTick->LOAD=(1677721);   // lade SysTick mit Wert grösser als 230ms. 
	  SysTick->VAL=0;             
		//start_time =((SysTick ->VAL));
		ET_STM32_LED_Off(LED5);
		// Stoppuhr gestartet  rma_05072014
		while((SysTick->VAL) !=0)
         {  // Wait 230ms
					  ET_STM32_LED_On(LED3);
				 };
		
  }
}



/********************************/
/* 5 : Test GLCD & Touch Sensor */
/********************************/
void test_glcd_touch(void)
{
  //TCS
  uint16_t X;
  uint16_t Y;
  //TCS

  sprintf(uart_buf,"\n\n\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);
  sprintf(uart_buf,"GLCD & Touch Sensor Testing...\n");	
  printUART1((unsigned int *)uart_buf);
  printUART2((unsigned int *)uart_buf);

  //Start of Config GLCD & Touch Sensor
  /* Initialize TFT-GLCD 320x240 */
  ET_STM32_LCD_Init();

  /* Initial Touch Sensor ADS7846 */
  ET_STM32_TCS_Config();

  /* Clear Acreen GLCD */ 
  LCD_Clear(White);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);

  /* Set the LCD Text Color */
  LCD_SetTextColor(White);     
  LCD_DisplayStringLine(Line0, "  ET-STM32 ARM KIT  ");
  LCD_DisplayStringLine(Line1, " Example Touch Demo ");
  LCD_DisplayStringLine(Line2, "used ADS7846N Sensor");
  LCD_DisplayStringLine(Line3, "By......ETT CO.,LTD.");

  /* Leds Control blocks */
  LCD_SetTextColor(Black);
  LCD_DrawRect(120, 310, 40, 60);  //SW0
  LCD_SetTextColor(Blue);
  LCD_DrawRect(120, 230, 40, 60);  //SW1
  LCD_SetTextColor(Blue2);
  LCD_DrawRect(120, 150, 40, 60);  //SW2
  LCD_SetTextColor(Red);
  LCD_DrawRect(120, 70, 40, 60);   //SW3

  LCD_SetTextColor(Magenta);
  LCD_DrawRect(192, 310, 40, 60);  //SW4
  LCD_SetTextColor(Green);
  LCD_DrawRect(192, 230, 40, 60);  //SW5
  LCD_SetTextColor(Cyan);
  LCD_DrawRect(192, 150, 40, 60);  //SW6
  LCD_SetTextColor(Yellow);
  LCD_DrawRect(192, 70, 40, 60);   //SW7

  LCD_SetBackColor(White);
  while(1)
  {
    //Verify Touch Sensor Active
    if (GPIO_ReadInputDataBit(TCS_GPIO_PORT,TCS_PEN_PIN) == Bit_RESET)
    {  
	  X = TCS_Read_X();
	  Y = TCS_Read_Y();

	  //Verify Line1 Block Switch(SW0..SW3)
	  //Ver Position = 120..160 (120+40)
	  if ((Y < 160) && (Y > 120)) 			
	  {
	    LCD_SetTextColor(White);
	    LCD_DisplayStringLine(Line7, "                    ");

	    //Hor Position = 250..310 (250+60)
        if ((X > 250) && (X < 310))	
        {
		  LCD_SetTextColor(Black);
          LCD_DisplayStringLine(Line4, " SW0                ");          
        }

		//Hor Position = 170..230 (170+230)
        else if ((X > 170) && (X < 230)) 	
        {
		  LCD_SetTextColor(Blue);
		  LCD_DisplayStringLine(Line4, "      SW1           ");         
        } 

		//Hor Position = 90..150 (90+60)
        else if ((X > 90) && (X < 150))		
        {
		  LCD_SetTextColor(Blue2);
          LCD_DisplayStringLine(Line4, "           SW2      ");           
        }

	    //Hor Position = 10..70 (10+60)
	    else if ((X > 10) && (X < 70))			
        {
		  LCD_SetTextColor(Red);
		  LCD_DisplayStringLine(Line4, "                SW3 ");          
        }			
	  }

	  //Verify Line2 Block Switch(SW4..SW7)
      //Ver Position = 192..232 (192+40)
	  else if ((Y < 232) && (Y > 192)) 			
	  {
	    LCD_SetTextColor(White);
	      LCD_DisplayStringLine(Line4, "                    ");

	    //Hor Position = 250..310 (250+60)
        if ((X > 250) && (X < 310))	
        {
		  LCD_SetTextColor(Magenta);
          LCD_DisplayStringLine(Line7, " SW4                ");          
        }

		//Hor Position = 170..230 (170+230)
        else if ((X > 170) && (X < 230)) 	
        {
		  LCD_SetTextColor(Green);
		  LCD_DisplayStringLine(Line7, "      SW5           ");         
        } 

		//Hor Position = 90..150 (90+60)
        else if ((X > 90) && (X < 150))		
        {
		  LCD_SetTextColor(Cyan);
          LCD_DisplayStringLine(Line7, "           SW6      ");           
        }

	    //Hor Position = 10..70 (10+60)
	    else if ((X > 10) && (X < 70))			
        {
		  LCD_SetTextColor(Yellow);
		  LCD_DisplayStringLine(Line7, "                SW7 ");          
        }	
	  }
	}
	else
	{
	  LCD_SetTextColor(White);
	  LCD_DisplayStringLine(Line4, "                    ");

	  LCD_SetTextColor(White);
	  LCD_DisplayStringLine(Line7, "                    ");
	}
  }
}

/*****************/
/* Hangup System */
/*****************/
void hang(void)
{
  while(1);
}

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

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
