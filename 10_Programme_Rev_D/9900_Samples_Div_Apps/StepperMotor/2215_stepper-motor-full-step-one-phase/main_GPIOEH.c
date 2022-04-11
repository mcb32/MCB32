/**
* @note      full step on Port PE High Pin 8...11 (P1_0....P1_3 on MCB32)
*
*
*
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
	
	// One shaft revolution CW using full step mode (one phase on)
	for (cycle = 0; cycle < 512; cycle++)
	{
		GPIO_Write(GPIOE, 0x0100);
		delay(5);
		GPIO_Write(GPIOE, 0x0200);
		delay(5);
		GPIO_Write(GPIOE, 0x0400);
		delay(5);
		GPIO_Write(GPIOE, 0x0800);
		delay(5);
	}
	
	delay(1000);
	
	// One shaft revolution CCW using full step mode (one phase on)
	for (cycle = 0; cycle < 512; cycle++)
	{
		GPIO_Write(GPIOE, 0x0800);
		delay(5);
		GPIO_Write(GPIOE, 0x0400);
		delay(5);
		GPIO_Write(GPIOE, 0x0200);
		delay(5);
		GPIO_Write(GPIOE, 0x0100);
		delay(5);
	}
	GPIO_Write(GPIOE, 0x0000);   // Steppermotor OFF
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
