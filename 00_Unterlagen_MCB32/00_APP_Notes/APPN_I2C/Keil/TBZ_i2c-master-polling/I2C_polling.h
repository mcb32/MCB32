/**
  ******************************************************************************
  * @file		I2C_polling.h
  * @author	RMA 
  * @date		10.04.2016 
	* @note		new 1.0
  ******************************************************************************
  */
	
#ifndef __I2CPOLLING_H
#define __I2CPOLLING_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
#include "stm32f10x.h"
#define I2Cx_RCC				RCC_APB1Periph_I2C1    	// or RCC_APB1Periph_I2C2
#define I2Cx						I2C1										// or I2C2 
#define I2C_GPIO_RCC		RCC_APB2Periph_GPIOB
#define I2C_GPIO				GPIOB
#define I2C_PIN_SDA			GPIO_Pin_7				// Select the appropriate Pins PB7 I2C1_SDA  // GPIO_Pin_11		 SDA for I2C2
#define I2C_PIN_SCL			GPIO_Pin_6			  // Select the appropriate Pins PB6 I2C1_SCL  // GPIO_Pin_10		 SCL for I2C2

	 
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
void i2c_write(uint8_t address, uint8_t data);
void i2c_read(uint8_t address, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
