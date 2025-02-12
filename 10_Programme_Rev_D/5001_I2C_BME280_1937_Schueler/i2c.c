/** @file I2C.c
*  @brief  C File for STM32F107 MCB32 Applications 
*          Read STM App Note AN2824 REV 3 and REV 4 for more information \n
*
* 
*  @author   	rma / Cityline/TBZ  REV D implementation
*  @date      	12.9.2015   
*  @version 		1.0    
*  @todo    -
*  @see 
*  @note    	 Code tested 
*  @link 1      http://wiibrew.org/wiki/Wiimote/Extension_Controllers     // for WII Controller and Nunchuk
*               https://acassis.wordpress.com/2015/06/26/using-the-nintendo-wii-nunchuk-joystick-in-the-stm32f4discovery-running-nuttx/ 
*  @warning -
*  @bug 				No known bugs.
*/

#include <stm32f10x.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "global_directives.h"				// used for conditional compiling 

#include "I2C.h"

#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

/* 
 *  See AN2824 STM32F10xxx I2C optimized examples
 *
 *  This code implements polling based solution
 *
 */

/**
 *  Names of events used in stdperipheral library
 *
 *      I2C_EVENT_MASTER_MODE_SELECT                          : EV5    // Read APP Note REV3
 *      I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6     
 *      I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
 *      I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
 *     
 **/



/*
 *  Read process is documented in RM008
 *
 *   There are three cases  -- read  1 byte  AN2824 Figure 2 
 *                             read  2 bytes AN2824 Figure 2
 *                             read >2 bytes AN2824 Figure 1
 */  

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf,uint32_t nbyte, uint8_t SlaveAddress)
{
  __IO uint32_t Timeout = 0;

  //    I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors 

  if (!nbyte)
    return Success;



  // Wait for idle I2C interface

  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

  // Enable Acknowledgement, clear POS flag

  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

  // Intiate Start Sequence (wait for EV5

  I2C_GenerateSTART(I2Cx, ENABLE);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

  // Send Address

  I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

  // EV6
  
  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));

  if (nbyte == 1)
    {

      // Clear Ack bit      

      I2C_AcknowledgeConfig(I2Cx, DISABLE);       

      // EV6_1 -- must be atomic -- Clear ADDR, generate STOP

      __disable_irq();
      (void) I2Cx->SR2;                           
      I2C_GenerateSTOP(I2Cx,ENABLE);      
      __enable_irq();

      // Receive data   EV7

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
      *buf++ = I2C_ReceiveData(I2Cx);

    }
  else if (nbyte == 2)
    {
      // Set POS flag

      I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);

      // EV6_1 -- must be atomic and in this order

      __disable_irq();
      (void) I2Cx->SR2;                           // Clear ADDR flag
      I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit
      __enable_irq();

      // EV7_3  -- Wait for BTF, program stop, read data twice

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

      __disable_irq();
      I2C_GenerateSTOP(I2Cx,ENABLE);
      *buf++ = I2Cx->DR;
      __enable_irq();

      *buf++ = I2Cx->DR;

    }
  else 
    {
      (void) I2Cx->SR2;                           // Clear ADDR flag
      while (nbyte-- != 3)
	{
	  // EV7 -- cannot guarantee 1 transfer completion time, wait for BTF 
          //        instead of RXNE

	  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF)); 
	  *buf++ = I2C_ReceiveData(I2Cx);
	}

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  

      // EV7_2 -- Figure 1 has an error, doesn't read N-2 !

      I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit

      __disable_irq();
      *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
      I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
      __enable_irq();

      *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

      // wait for byte N

      Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
      *buf++ = I2C_ReceiveData(I2Cx);

      nbyte = 0;

    }

  // Wait for stop

  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
  return Success;

 errReturn:

  // Any cleanup here
  return Error;

}

/*
 * Write buffer of bytes -- AN2824 Figure 3
* I2C_TypeDef* I2Cx:   Choose Interface 
* const uint8_t* buf:  Pointer to buffer with content
* uint32_t nbyte:      how many bytes must transfered
* uint8_t SlaveAddress: Address of  Device
 */

Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf,  uint32_t nbyte, uint8_t SlaveAddress)
{
    __IO uint32_t Timeout = 0;

    /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
    //    I2Cx->CR2 |= I2C_IT_ERR;

	if (nbyte)
		{
			Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

			// Intiate Start Sequence

			I2C_GenerateSTART(I2Cx, ENABLE);
			Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

			// Send Address  EV5

				I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
				Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

				// EV6

				// Write first byte EV8_1

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



/** ****************************************
* @function	I2C_LowLevel_Init()
* @brief
* @note
* @param 1 I2C_TypeDef* I2Cx   I2C1   or I2C2  see Ref Manual for Pinassignment
* @param 1 int ClockSpeed      probably for Nunchuk 10000 (start with this)
* @param 1 int OwnAddress
* @param 1
*
* *****************************************/
void I2C_LowLevel_Init(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;

    // Enable GPIOB clocks

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Configure I2C clock and GPIO 

    GPIO_StructInit(&GPIO_InitStructure);

	

    if (I2Cx == I2C1){

        /* I2C1 clock enable */

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  // Clock I2C
			#ifdef Remapp_I2C1  
			 /* Enable the I2C1 Pins Software Remapping */
			 // MCB32 KIT(STM32F107VCT6) Hardware
       // I2C1(Remap) = PB8:SCL1
       //             = PB9:SDA1
  		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // is needed if remapping is used
        GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
        /* I2C1 SDA and SCL configuration */
			  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;				//Remapped Pins: SCL (PB6),SDA (PB7): Alternate Function, Open Drain
			#else 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			#endif
  			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // GPIO_Mode_AF_PP = Alternate funktion Push Pull // Original GPIO_Mode_AF_OD;     					// Open Drain, I2C bus pulled high externally  
		    GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* I2C1 Reset */

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

      }
      else {

        /* I2C2 clock enable */

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

        /* I2C2 SDA and SCL configuration */

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;			//SCL (PB10),SDA (PB10): Alternate Function, Open Drain
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // GPIO_Mode_AF_PP = Alternate funktion Push Pull // Original GPIO_Mode_AF_OD;     					// Open Drain, I2C bus pulled high externally  
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* I2C2  Reset */

        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

      }

    /* Configure I2Cx                */

    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = OwnAddress;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;

    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE); 
}
