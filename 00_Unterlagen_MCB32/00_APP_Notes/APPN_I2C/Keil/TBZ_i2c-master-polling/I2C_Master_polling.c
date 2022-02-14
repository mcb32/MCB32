/** @file I2C_Master_polling.c
*  @brief  Versuch mit I2C Bus am MCB32  
*  @Attention: attach a device or at least Pullup resistor to the Pin SDA an SCK. Because of Open Drain Output
*              The actual example uses Pin PB6 and PB7 which are also used for USART1. But the pins could be remapped to PB8 and PB9 
* 
*  @author rma / TBZ / REV D implmentation
*  @date 13.7.2016 / rma (1.1)
*  @version 1.1    
*  @bug No known bugs.   USE in flash: USE_STDPERIPH_DRIVER
*  @note		Demonstration of I2C communication with an PCF8574A I2C Buss Expander from TI
*           See SCPS069F.pdf for more details about the CHIP. 
*/
//=================================Includes=====================================
#include <stm32f10x.h>								// Mikrocontrollertyp
#include "LIB_REVD\TouchP0P1.h"				// P0/P1,8Bit,Touchscreen   
// include "math.h"
//#include "stm32f10x_rcc.h"					// desabled if defined in CMSIS setup
//#include "stm32f10x_gpio.h"					// desabled if defined in CMSIS setup
//#include "stm32f10x_i2c.h"					// desabled if defined in CMSIS setup
#include "delay.h"										// Lib for Delay(s)				
#include "I2C_polling.h"							// I2C Lib , polling
#include <stdio.h>										// lib for sprintf   .......

// Defines
#define PCF8574A   					0x70
#define SLAVE_ADDRESS_WR	PCF8574A					// Implement here the Device Address for Writing
#define SLAVE_ADDRESS_RD (PCF8574A |0x01)		// Implement here the Device Address for Reading

// Variables 
char wertausgabe[30];
uint8_t receivedByte;
	
// **************** Main ***********************
int main(void)
{
	DelayInit();
	InitTouchScreen ();					//  Touchscreen ON. 
	printAt ( 2, "I2C Demo Programm Rev D" );
	
	// Initialize I2C
	i2c_init();
	
	while (1)
	{
		// Write 0x00 to slave (turn on LED blinking)
		i2c_write(SLAVE_ADDRESS_WR, 0x00);
		DelayMs(5);
		// Read LED blinking status (off/on)
		i2c_read(SLAVE_ADDRESS_RD, &receivedByte);
		
		// Display LED blinking status
		if (receivedByte == 0)
		{
			printAt ( 12, "LED Blinking  ON" );
	    sprintf(wertausgabe,"Received-Byte    I: %d", receivedByte);
			printAt ( 13, wertausgabe );
		}
		else if (receivedByte >= 1)
		{
			printAt ( 12, "LED Blinking OFF" );
      sprintf(wertausgabe,"Received-Byte  II: %d", receivedByte);
		  printAt ( 14, wertausgabe );		
		}
		DelayMs(2000);
		
		//Write 0xFF to slave (turn off LED blinking)
		i2c_write(SLAVE_ADDRESS_WR, 0xFF);
		DelayMs(5);
		// Read LED blinking status (off/on)
		i2c_read(SLAVE_ADDRESS_RD, &receivedByte);
		// Display LED blinking status
  
		if (receivedByte == 0)
		{
			printAt ( 12, "LED Blinking  ON" );
      sprintf(wertausgabe,"Received-Byte III: %d", receivedByte);
		  printAt ( 14, wertausgabe );			

		}
		else if (receivedByte >= 1)
		{
			printAt ( 12, "LED Blinking OFF" );
      sprintf(wertausgabe,"Received-Byte  IV: %d", receivedByte);
		  printAt ( 14, wertausgabe );		
		}
		DelayMs(2000);
	}
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
