
/** @file   2051_Display_ADDA_IRQ-Uebung.c
*  @brief   //Test ADC12 Pin 33 Poti einlesen mittels Interrupt und auf LED Reihe ausgeben
*           
*         
*  @author  mal  
*  @date    9.3.2023
*  @version 1.0    
*  @bug     No known bugs. ...
*/
//=================================Includes===================================== 
#include <stm32f10x.h>




void ADC1_2_IRQHandler(void)
{
		GPIOE->ODR = (ADC1->DR)<<4;													//set LED aus 12Bit->16Bit->higher Byte anzeigen auf LED
//  ADC1->SR &= ~(1<<1); 																//wird automatisch durch auslesen des Registers gelöscht
}



int main(void)
{
unsigned long i;
//-------------------------------------------------------------------------------------------------------------------------------	
//Initialisierung LED-Port
//-------------------------------------------------------------------------------------------------------------------------------	
	RCC->APB2ENR |= 1<<6;																		//GPIOA Clock
	GPIOE->CRH = 0x11111111;																//LED Port (PE15..8) output
//-------------------------------------------------------------------------------------------------------------------------------	
//Initialisierung Alternate Functions/peripheral clock/ADC
//-------------------------------------------------------------------------------------------------------------------------------	
	RCC->APB2ENR = RCC->APB2ENR | 0x00000211;								//set bit0,bit4 and bit 9 	
																													//enable peripheral clock GPIOC (ADC), alternate function, clock ADC1
	RCC->CFGR 		|= 	(2<<14);															//divide by 6 (default divide by 2)
	ADC1->SQR3		= 	(14<<0);															//channel 14 als einzigen Kanal->kein shiften nötig
	ADC1->CR2 		|= 	(7<<17);															//start conversion by sw
	ADC1->CR2 		|= 	(1<<20);															//enable extern trigger (per sw=extern)
	ADC1->CR2 		|= 	(1<<1);																//continous
	ADC1->CR1   	|=	(1<<5);																//EOCIE
  NVIC->ISER[0] |= (1<<18);																//interrupt set enable register for adc
	ADC1->CR2 		|= 	(1<<0);																//ADC ON
//-------------------------------------------------------------------------------------------------------------------------------	
	
  for (i=0;i<1000000;i++)
		;																											//delay between ADC-ON and start conversion->see manual
	ADC1->CR2 |= (1<<22);																		//start conversion
	while(1)																								//forever
	{																												//nothing to do!
	}
}