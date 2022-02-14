// MCB32_I2C.c  /  12.9.2015 / rma / Cityline AG
// I2C Prototyp 
// 
//---------------------------------------------------
#include <stm32f10x.h>			// uC-Typ bei uVision4
#include "TouchP0P1.h"				// P0-,P1-,Touchscreen 
// #include "I2C_LowLevel_Init.h"

#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout -- == 0) goto errReturn;}

#define NUNCHUK_ADDRESS 0xA4
#define Status uint8_t							// used for better Reading :-)
const uint8_t buf[] = {0xf0, 0x55};
const uint8_t buf2[] = {0xfb, 0x00};

uint8_t Success = 0;
uint8_t Error = 1;

// -------------------------------
void I2C_LowLevel_Init(I2C_TypeDef* I2Cx, int ClockSpeed , int OwnAddress)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		I2C_InitTypeDef I2C_InitStructure;
		// Enable GPIOB clocks
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		// Configure I2C clock and GPIO
		GPIO_StructInit(&GPIO_InitStructure);
		if (I2Cx == I2C1){
		/* I2C1 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE);
		/* I2C1 SDA and SCL configuration */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_Init(GPIOB , &GPIO_InitStructure);
		/* I2C1 Reset */
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1 , ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1 , DISABLE);
		}
		else { // I2C2 ...}
		/* Configure I2Cx */
		I2C_StructInit(&I2C_InitStructure);
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 = OwnAddress;
		I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress =
		I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
		I2C_Init(I2Cx, &I2C_InitStructure);
		I2C_Cmd(I2Cx, ENABLE);
		}
}
// ---------------------------------------

Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbyte , uint8_t SlaveAddress) 
	{
		__IO uint32_t Timeout = 0;
		if (nbyte)
		{
			Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
			// Intiate Start Sequence
			I2C_GenerateSTART(I2Cx, ENABLE);
			Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
			// Send Address EV5
			I2C_Send7bitAddress(I2Cx, SlaveAddress ,
			I2C_Direction_Transmitter);
			Timed(!I2C_CheckEvent(I2Cx,
			I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
			// EV6 Write first byte EV8_1
			I2C_SendData(I2Cx, *buf++);
			while (--nbyte) {
				// wait on BTF
				Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
				I2C_SendData(I2Cx, *buf++);
				}
		  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
		  I2C_GenerateSTOP(I2Cx, ENABLE);
		  Timed(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
		}
		return Success;
		errReturn:
		return Error;
}




//----------------------------------------------------------
int main (void)						// Hauptprogramm
{
	// Init
	I2C_LowLevel_Init(I2C1, 50000 , 0x034);      //  Only Sample. Adapt it to your HW
	I2C_Write(I2C1, buf, 2, NUNCHUK_ADDRESS);
  I2C_Write(I2C1, buf2, 2, NUNCHUK_ADDRESS);


}