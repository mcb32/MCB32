/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "main.h"

//USB_MSD
///* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_it.h"
//#include "usb_lib.h"
//#include "usb_istr.h"
//#include "usb_pwr.h"

//#ifdef STM32F10X_HD
//#include "sdcard.h"
//#endif /* STM32F10X_HD */

//USB_MSD

extern void TimingDelay_Decrement(void);
extern __IO uint32_t TimeDisplay;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Toggle LED0 */
    ET_STM32_LED_Toggle(LED1);   // RMA toggle LEd 

    /* Enable time update */
    TimeDisplay = 1;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x00015180)
    {
      RTC_SetCounter(0x0);
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
  }
}

/*******************************************************************************
* Function Name  : OTG_FS_IRQHandler
* Description    : This function handles USB-On-The-Go FS global interrupt request.
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void OTG_FS_IRQHandler(void)
//{
//  STM32_PCD_OTG_ISR_Handler(); 
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  //if(EXTI_GetITStatus(WAKEUP_BUTTON_EXTI_LINE) != RESET)
  //{
  //  /* Toggle LED0 */
  //  ET_STM32_LED_Toggle(LED0);
  //
  //  EXTI_ClearITPendingBit(WAKEUP_BUTTON_EXTI_LINE);
  //}
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  //Tamper=PC13
  //if( (EXTI_GetITStatus(TAMPER_BUTTON_EXTI_LINE) != RESET) && (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==0) )
  //if(EXTI_GetITStatus(TAMPER_BUTTON_EXTI_LINE) != RESET)
  //{
  //  /* Toggle LED1 */
  //  ET_STM32_LED_Toggle(LED1);
  //  EXTI_ClearITPendingBit(TAMPER_BUTTON_EXTI_LINE);
  //} 

  //Joy-Up=PD11 
  //if(EXTI_GetITStatus(UP_BUTTON_EXTI_LINE) != RESET)
  //{
  //  /* Toggle LED3 */
  //  STM_EVAL_LEDToggle(LED3);
  //  EXTI_ClearITPendingBit(UP_BUTTON_EXTI_LINE);
  //} 

  //Joy-Left=PD12
  //if(EXTI_GetITStatus(LEFT_BUTTON_EXTI_LINE) != RESET)
  //{
  //  /* Toggle LED4 */
  //  STM_EVAL_LEDToggle(LED4);
  //  EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
  //} 

  //Joy-Down=PD13
  //if( (EXTI_GetITStatus(DOWN_BUTTON_EXTI_LINE) != RESET) && (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13)==0) )
  //if(EXTI_GetITStatus(DOWN_BUTTON_EXTI_LINE) != RESET) 
  //{
  //  /* Toggle LED7 */
  //  ET_STM32_LED_Toggle(LED7);
  //  EXTI_ClearITPendingBit(DOWN_BUTTON_EXTI_LINE);
  //} 

  //Joy-Right=PD14
  //if(EXTI_GetITStatus(RIGHT_BUTTON_EXTI_LINE) != RESET)
  //{
  //  /* Toggle LED6 */
  //  STM_EVAL_LEDToggle(LED6);
  //  EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);
  //} 

  //Joy-Select=PD15
  //if(EXTI_GetITStatus(SELECT_BUTTON_EXTI_LINE) != RESET)
  //{
  //  /* Toggle LED5 */
  //  STM_EVAL_LEDToggle(LED5);
  //  EXTI_ClearITPendingBit(SELECT_BUTTON_EXTI_LINE);
  //} 
   
}

/**
  * @}
  */ 


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles ETH interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{

}

/**
  * @brief  This function handles ETH  Weke-up interrupt request.
  * @param  None
  * @retval None
  */
void ETH_WKUP_IRQHandler(void)
{

}
/**
  * @}
  */ 



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
