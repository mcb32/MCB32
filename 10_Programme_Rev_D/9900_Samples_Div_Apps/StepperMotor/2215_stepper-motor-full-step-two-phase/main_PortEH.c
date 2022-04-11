//-----------------------------------------------------
// main_PortEH.c   / 10.2.2017  / rma / TBZ 
// Protoyp für Schrittmotoren
// Hal programmierung ohne Touch Lib
//-----------------------------------------------------

/**
    * @note:  Stepermotor Verdrahtung / Wiring
* Stepper Motor Tabelle Vorwärts CCW, die Farben beschreiben die Motordrahtfarben
* also: Blue ist mit IN1 und dann P1_0 verbunden, 
*       Pink ist mit IN2 und dann P1_1 verbunden usw. 

*              Stepp --> 1     2     3     4     5     6     7     8
*      ---------------|------------------------------------------------
*      P1_3 / OR   4  |  1     1     0     0     0     0     0     1
*      P1_2 / Yel  3  |  0     1     1     1     0     0     0     0      
*      P1_1 / PIN  2  |  0     0     0     1     1     1     0     0
*      P1_0 / BLU  1  |  0     0     0     0     0     1     1     1
*      ------------------------------------------------------------------
*      Hexcode for P1:  08    0C     04    06    02    03    01    09    
*      Der HExCode wird P1 von Links nach rechts zugewiesen damit fahren wir CCW
*/      

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void delay(unsigned int nCount);

GPIO_InitTypeDef GPIO_InitStruct;
int cycle = 0;

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	// One shaft revolution CW using full step mode (two-phase on)
	for (cycle = 0; cycle < 512; cycle++)
	{
		GPIO_Write(GPIOE, 0x300);
		delay(5);
		GPIO_Write(GPIOE, 0x600);
		delay(5);
		GPIO_Write(GPIOE, 0xC00);
		delay(5);
		GPIO_Write(GPIOE, 0x900);
		delay(5);
	}
	
	delay(1000);
	
	// One shaft revolution CCW using full step mode (two-phase on)
	for (cycle = 0; cycle < 512; cycle++)
	{
		GPIO_Write(GPIOE, 0x900);
		delay(5);
		GPIO_Write(GPIOE, 0xC00);
		delay(5);
		GPIO_Write(GPIOE, 0x600);
		delay(5);
		GPIO_Write(GPIOE, 0x300);
		delay(5);
	}
	GPIO_Write(GPIOE, 0x0000);   // Motor OFF
	while (1)
	{
	}
}

void delay(unsigned int nCount)
{
	unsigned int i, j;
	
	for (i = 0; i < nCount; i++)
		for (j = 0; j < 0x2AFF; j++);
}
