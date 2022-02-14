/**
* @file   	MCB32_I2C.c
* @autor  	rma / Cityline AG
* @date   	2.4.2018
* @version 	1.1
* @note   	I2C Prototyp 
* @note   	PCF8574 I2C Bus Expander is used
* @note   	Remapping of I2C bus: YES  
*           default: PB6 - I2C1_SCL; PB7 - I2C1_SDA
*           remap:   PB8 - I2C1_SCL; PB9 - I2C1_SDA
* @link     http://users.ece.utexas.edu/~valvano/embed/chap11/chap11.htm   // präproc. directives
*************************************************************/


#include <stm32f10x.h>								// uC-Typ bei uVision4
#include "TouchP0P1.h"								// P0-,P1-,Touchscreen 
#include "i2c.h"
#include "delays_cz.h"
#include <stdio.h>										// lib for sprintf   .......
#include "global_directives.h"				// used for conditional compiling 

// Define I2C Address
#define PCF8574A   					                     // !!!Setzte hier den richtigen Wert ein
#define PCF8574A_ClockSpeed    100000     			// 400 KHz 
#define PCF8574A_ADDRESS_WR	PCF8574A						// Implement here the Device Address for Writing
#define PCF8574A_ADDRESS_RD (PCF8574A |0x01)		// Implement here the Device Address for Reading

//initialization and reset instructions 
const uint8_t inst1[] = {0x00, 0xFF};					// also used for single Byte transfer to PCF8574A
const uint8_t inst2[] = {0xFF, 0x00}; 
const uint8_t reset[] = {0xFF};								// 0xFF Set PCF8574A Pins to 1 


// Variables 
char wertausgabe[30];
uint8_t receivedByte;
unsigned int blinking_frequency = 1;        // Value in HZ
unsigned int blinking_delay = 0;          // Value in ms


//----------------------------------------------------------
int main (void)						// Hauptprogramm
{
	// data arrays for receiving Data
  uint8_t data1[6] = {0}; 

  // Initialize I2C busses 
  I2C_LowLevel_Init(I2C1, PCF8574A_ClockSpeed, PCF8574A_ADDRESS_WR); 
  // Initialize the PCF8574A 
	I2C_Write(I2C1, reset, 1, PCF8574A_ADDRESS_WR); 
	//InitTouchP0P1("0");	
	DelayInit();
	InitTouchScreen();
	printAt ( 2, "I2C DemoProg. No polling RevD" );
	#ifdef Remapp_I2C1
	 printAt ( 3, "I2C Remapped to PB8, PB9" );
	#else
	 printAt ( 3, "I2C at PB6, PB7" );
	#endif
	// Demo
	blinking_delay= (int)(1000/ blinking_frequency);		// delay in ms
while (1) 
	{ 
		// turn LED ON
		I2C_Write(I2C1, inst1, 1, PCF8574A_ADDRESS_WR); 
		DelayMs(1);
		// Read LED blinking status (off/on)
		I2C_Read(I2C1, data1, 1, PCF8574A_ADDRESS_RD); receivedByte=data1[0];
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
	  DelayMs(blinking_delay);                  		// delay_us(10000); 									// If InitTouchxx("1");
		//turn LED OFF
		I2C_Write(I2C1, inst2, 1, PCF8574A_ADDRESS_WR); 
		DelayMs(1);
		// Read LED blinking status (off/on)
		I2C_Read(I2C1, data1, 1, PCF8574A_ADDRESS_RD); 		receivedByte=data1[0];
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
		DelayMs(blinking_delay);

	}
}








