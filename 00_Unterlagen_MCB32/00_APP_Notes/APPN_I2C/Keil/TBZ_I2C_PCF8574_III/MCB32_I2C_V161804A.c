// MCB32_I2C.c  /  12.9.2015 / rma / Cityline AG
// I2C Prototyp 
// 
//---------------------------------------------------

#include <stm32f10x.h>			// uC-Typ bei uVision4
#include "TouchP0P1.h"				// P0-,P1-,Touchscreen 
#include "i2c.h"
#include "delays_cz.h"
#include <stdio.h>										// lib for sprintf   .......


// Define I2C Address
//#define NUNCHUK_ADDRESS 0xA4
//#define EEPROM_24LC256_ADDRESS   0xA0          // 1 0 1 0 "A2" "A1" "A0" "R/!W"   // A2..A0 are open means 0. R/!W must be set druing operation
//#define I2C1_SLAVE_ADDRESS7  0x38    					// I2C Adresse Sensor
//#define I2C1_Own_ADDRESS7  0x00    						// eigene I2C Adresse 
//#define ClockSpeed    400000     							// 400 KHz 
//#define Nunchuk_ClockSpeed    400000     			// 400 KHz 
// Defines
#define PCF8574A   					0x70
#define PCF8574A_ClockSpeed    100000     			// 400 KHz 
#define PCF8574A_ADDRESS_WR	PCF8574A					// Implement here the Device Address for Writing
#define PCF8574A_ADDRESS_RD (PCF8574A |0x01)		// Implement here the Device Address for Reading

// #define Status uint8_t							// used for better Reading :-)
//uint8_t Success = 0;
//uint8_t Error = 1;
//Nunchuck initialization and reset instructions 
const uint8_t inst1[] = {0x00, 0xFF}; 
const uint8_t inst2[] = {0xFF, 0x00}; 
const uint8_t reset[] = {0xFF}; 
// EEPROM Init 
const uint8_t buf1[] = {0xf0, 0x55};
const uint8_t buf2[] = {0xfb, 0x00};

void delay_us (uint32_t /*uint*/ duration_us);
// Variables 
char wertausgabe[30];
uint8_t receivedByte;


//----------------------------------------------------------
int main (void)						// Hauptprogramm
{
	//Nunchuck data arrays 
  uint8_t data1[6] = {0}; 

  // Initialize I2C busses 
  I2C_LowLevel_Init(I2C1, PCF8574A_ClockSpeed, PCF8574A_ADDRESS_WR); 
//     I2C_LowLevel_Init(I2C2, 10000, NUNCHUK_ADDRESS);    // if 2 Nunchuks are available
 
	  // Initialize the PCF8574A 
		I2C_Write(I2C1, reset, 1, PCF8574A_ADDRESS_WR); 

	
  // Initialize the nunchuck(s) 
//  I2C_Write(I2C1, inst1, 2, NUNCHUK_ADDRESS); 
//  I2C_Write(I2C1, inst2, 2, NUNCHUK_ADDRESS); 
	
 
//     I2C_Write(I2C2, inst1, 2, NUNCHUK_ADDRESS); 				 	// if 2 Nunchuks are available
//     I2C_Write(I2C2, inst2, 2, NUNCHUK_ADDRESS);    			// if 2 Nunchuks are available

	
	    // Init  EEPROM
//			I2C_LowLevel_Init(I2C1, ClockSpeed , I2C1_Own_ADDRESS7);  //  Only Sample. Adapt it to your HW
//			I2C_Write(I2C1, buf1 , 2, EEPROM_24LC256_ADDRESS&0xFE);   // 0xFE means Writing to CHIP
//			I2C_Write(I2C1, buf2, 2, EEPROM_24LC256_ADDRESS&0xFE);


//InitTouchP0P1("0");	
DelayInit();
InitTouchScreen();
printAt ( 2, "I2C DemoProg. No polling RevD" );
	// Demo
while (1) 
	{ 
		I2C_Write(I2C1, inst1, 1, PCF8574A_ADDRESS_WR); 
		DelayMs(5);
		// delay_us(10000); 									// If InitTouchxx("1");
   // Read LED blinking status (off/on)
		I2C_Read(I2C1, data1, 1, PCF8574A_ADDRESS_RD);
		// i2c_read(SLAVE_ADDRESS_RD, &receivedByte);
		// Display LED blinking status
		receivedByte=data1[0];
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
		I2C_Write(I2C1, inst2, 1, PCF8574A_ADDRESS_WR); 
		// i2c_write(SLAVE_ADDRESS_WR, 0xFF);
		DelayMs(5);
		// Read LED blinking status (off/on)
		I2C_Read(I2C1, data1, 1, PCF8574A_ADDRESS_RD);
		// i2c_read(SLAVE_ADDRESS_RD, &receivedByte);
		// Display LED blinking status
		receivedByte=data1[0];
		// i2c_read(SLAVE_ADDRESS_RD, &receivedByte);
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








