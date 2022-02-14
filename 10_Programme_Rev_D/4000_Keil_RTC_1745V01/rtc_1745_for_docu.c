//-------------------------------------------------
// rtc_1745.c   10.11.2017 / rma  / TBZ REV D
// Die SW zeigt die Funktion des RTC Moduls. 
// Mit dem Joy-Stick kann durch drücken eine Zeit gesetzt werden. Muss unten programmiert werden.
// bei eingesetzer Batteri läuft die Uhr weiter
// Achtung unter Projekt/Options-> C/C++ -> preprocessor Symboles -> Define: USE_STDPERIPH_DRIVER   -> eingeben
//-------------------------------------------------
// #include <stm32f10x_cl.h>		// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0-, P1-Definition
#include <stdio.h>
#include <string.h>
#include <stm32f10x_rcc.h>       //  reset and clock control (RCC)  Chapter 8
#include <stm32f10x_pwr.h>       // Power control (PWR)  Chapter 5
#define update_every_x_second 2  // Period for Time Update on Screen
/* Private variables ----------------------------------------------------------------------------------------------*/
static __IO uint32_t TimingDelay =0;
__IO uint32_t TimeDisplay = update_every_x_second;
char uart_buf[512];   
// Globals  for Horus, minutes, seconds
uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
uint32_t THH = 0, TMM = 0, TSS = 0;
uint32_t TimeVar;
/* Private variables end here -------------------------------------------------------------------------------------*/

/* Private functions ----------------------------------------------------------------------------------------------*/
void EXTI0_IRQHandler(void);  // EXTI0_IRQHandler
void RTC_IRQHandler(void);
/* Private functions end here -------------------------------------------------------------------------------------*/




int main(void) 							// Hauptprogramm
{
	/* Setup STM32 system (clock, PLL and Flash configuration) */
   // SystemInit();    // This function should be used only after reset.
   /* Setup SysTick Timer for 1 msec interrupts -> in Touchlib */

   InitTouchP0P1("1");					// P0P1-Touchscreen ON  / Setup SysTick Timer
   setScreenDir ( HOR );
  
   
	// rma new Block for NVIC RTC configuration , could be a function
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
	}
	
	// Ports initialize:   
   RCC->APB2ENR |= 1<<2;    		   // Enable GPIOA clock, Input       
   RCC->APB2ENR |= 1<<6;    		   // Enable GPIOE clock, LEDs         
   GPIOA->CRL   &= 0xFFFFFF00;      // Configure the GPIOA 0..7 
   GPIOA->CRL   |= 0x00000088;      // Pullup Input PA_0, PA_1
	// Interrupt setup/config:   
   RCC->APB2ENR |= 1;	  			   // Alternate Funct. AFIO ON
	AFIO ->EXTICR[0] = 0x00;		   // Pin0 von PA0 auf EXTI0
	EXTI->IMR  |= 1;						// EXTI0 ON-Maske ON
	EXTI->RTSR |= 1;						// Steigende Triggerflanke
	EXTI->FTSR |= 1;						// Fallende Triggerflanke
	NVIC->ISER[0]  |= 1<<6;			   // Enable IntrNr 6 für EXTI0

   // RTC Configuration -------------------------------------------------------------------------------------------

   /* Enable PWR and BKP clocks : see 7.3.8 in REF Manual Bit 27 BKPEN*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  
   /* Allow access to BKP Domain: see 6.1 and 5.4.1 */
  PWR_BackupAccessCmd(ENABLE);  // set DBP bit in the Power control register (PWR_CR) 5.4.1
  /* Reset Backup Domain */
  // Deinitializes the BKP peripheral registers to their default reset values.
  // see 7.3.9 and 7.1.3 in REF Manual Set/Reset Bit 16  / Backup domain reset
  // RCC_BackupResetCmd(ENABLE);  // Setzt die internen Register zurück. Also auch die Zeit
  // RCC_BackupResetCmd(DISABLE); // beide Zeilen ausblenden wenn die Zeoit weiterlaufen soll
  /* Enable LSE : Configures the External Low Speed oscillator (LSE): see 7.3.9 */
  RCC_LSEConfig(RCC_LSE_ON);   //  see 7.3.9 
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  /* Select External Low Speed oscillator (LSE) as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();  // See 18.4.2 RTC_CRL RSF Bit 
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();   // see 18.4.2 wait until RTOFF flag is set 
     /* Enable the RTC Second : see chapter 18ff*/
  RTC_ITConfig(RTC_IT_SEC, ENABLE);     // see 18.4.1 RTC Register
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
     /* Set RTC prescaler: set RTC period to 1sec: see 18.4.3 RTC prescaler load register */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  // end of RTC Configuration -------------------------------------------------------------------------------------
 
  // Adjust time by values entered by the user via SW
    
  if (PD_15==!1) {       //Wenn Stick gedrückt wird dann Setup Time = 14:00:00
     Tmp_HH = 17;           // siehe auch RCC_BackupResetCmd(ENABLE/DESABLE);
     Tmp_MM = 10; 
     Tmp_SS = 0;   
     RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);     // Enter Config Mode see 18.4.2 ff
     RTC_WaitForLastTask(); 
  }
  // Wait until last write operation on RTC registers has finished 
  RTC_WaitForLastTask();     
  
  // Change the current time 
  // RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);      // Enter Config Mode see 18.4.2 ff
  // Wait until last write operation on RTC registers has finished 
  // RTC_WaitForLastTask();

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);      // SECIE second IR enable see 18.4.1

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  /* Clear reset flags: Control/status register (RCC_CSR) see 8.3.10, Set RMVF bit to clear the reset flags */
  RCC_ClearFlag();                     // Clears the RCC reset flags.
   
  // RTC Configuration end here ------------------------------------------------------------------------------------ 
   
   while(1)	 								   // Endlosschlaufe
	{	
/* If 1s has passed */
    if (TimeDisplay == 0)              // TimeDisplay  is handled by   RTC_IRQHandler()
    {
	  TimeVar = RTC_GetCounter();

	  /* Compute  hours */
      THH = TimeVar / 3600;

      /* Compute minutes */
      TMM = (TimeVar % 3600) / 60;

      /* Compute seconds */
      TSS = (TimeVar % 3600) % 60;
      // low level method for writng the time to touchscreen: 
      // printAt ( 2, "Time: " ); printDec ( 2, THH );print(":");printDec ( 2, TMM );print(":");printDec ( 2, TSS );
      sprintf(uart_buf,"Time: %0.2d:%0.2d:%0.2d", THH, TMM, TSS);
      printAt(1,""); printLn ( uart_buf );
      TimeDisplay = update_every_x_second;              // is handled by   RTC_IRQHandler()
    } 
  }
								
} 

// local Functions used for this DEmo -------------------------------------------------------------------------------
// 

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
    // if(P1_7==1){P1_7=0;}else{P1_7=1;}
     /* Toggle Point on screen after each second */
    if(RTC_GetCounter()%2==0){circle(220,10,3,1,GREEN,1);}else{circle(220,10,3,1,RED,1);}

    /* Enable time update */
    TimeDisplay = TimeDisplay-1 ;   // base ist "update_every_x_second"

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x00015180)
    {
      RTC_SetCounter(0x0);  // Enters Config Mode see 18.4.2 
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
  }
}




// Thema: EXTI0_IRQHandler  um bei gedrückter Taste "Wackeup (PA0??)" zu reagieren
// siehe startup_stm32f10x_cl.s für die Namen der handler
void EXTI0_IRQHandler(void)		// Vorgeg. Handlername
{											// Zuerst: Loesche
	EXTI->PR |= 1;						// Intr Flag 1 = Pin0
	P1++;									// Zaehler erhoehen
}



