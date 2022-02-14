//============================================================================= 
//    Cityline AG   AG,  8630 Rueti, Switzerland 
//============================================================================= 
// Project   :  PCF8574x Sample Code (V2.1) 
// File      :  PCF8574x.c (V2.1) 
// Author    :  Mal
// Date      :  6-Mai-2015 
// Controller:  STM32F100RB 
// IDE       :  µVision V5.10.0.x 
// Compiler  :  Keil Default
// Brief     :  I2C Bus Expander Layer: Implementation of functions for Chip access. 
// @ref      :  Based on Sensirion AG I2C COde for SHT3x Chips
//============================================================================= 
 
//-- Includes ----------------------------------------------------------------- 
#include "PCF8574x.h" 
#include "sht3x.h"                  // ToDo some etCommands are used from this file (Bug)
#include "i2c_hal.h" 
 
//-- Defines ------------------------------------------------------------------ 
//// Generator polynomial for CRC 
//#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001 
// 
//============================================================================= 
// IO-Pins                            /* -- adapt the defines for your uC -- */ 
//----------------------------------------------------------------------------- 
// Reset on port B, bit 12 
#define RESET_LOW()  (GPIOB->BSRR = 0x10000000) // set Reset to low 
#define RESET_HIGH() (GPIOB->BSRR = 0x00001000) // set Reset to high   on port B, bit 12 
 
// Alert on port B, bit 10 
#define ALERT_READ   (GPIOB->IDR  & 0x0400)     // read Alert 
//============================================================================= 
 
//-- Global variables --------------------------------------------------------- 
static u8t _i2cAddress; // I2C Address 
//-- Static function prototypes ----------------------------------------------- 
static etError PCF8574N_StartWriteAccess(void) ;
static etError PCF8574N_StartReadAccess(void) ;  // new RMA
static void PCF8574N_StopAccess(void) ;
static etError PCF8574N_WriteCommand(etCommands command) ;
static etError PCF8574N_Read1Byte(u16t* data, etI2cAck finaleAckNack, u8t timeout) ;

//-----------------------------------------------------------------------------
void PCF8574N_Init(u8t i2cAddress)          /* -- adapt the init for your uC -- */ 
{ 
  // init I/O-pins 
  RCC->APB2ENR |= 0x00000008;  // I/O port B clock enabled 
   
  // Alert on port B, bit 10 
  GPIOB->CRH   &= 0xFFFFF0FF;  // set floating input for Alert-Pin 
  GPIOB->CRH   |= 0x00000400;  //  
   
   
  // Reset on port B, bit 12 
  GPIOB->CRH   &= 0xFFF0FFFF;  // set push-pull output for Reset pin 
  GPIOB->CRH   |= 0x00010000;  // 
  RESET_LOW(); 
   
  I2c_Init(); // init I2C 
  PCF8574N_SetI2cAdr(i2cAddress); 
   
  // release reset 
  RESET_HIGH(); 
}
 
//----------------------------------------------------------------------------- 
void PCF8574N_SetI2cAdr(u8t i2cAddress) 
{ 
  _i2cAddress = i2cAddress; 
} 

// --------------------------------------------------------------
// not really useful this function because PCF8574 has no serial number :-)
  etError PCF8574N_ReadSerialNumber(u32t* serialNumber) 
{ 
  etError error; // error code 
  u16t serialNumWords[2]; 
  error = PCF8574N_StartWriteAccess( );  // new Rma 
  if(error == NO_ERROR) error = PCF8574N_StartReadAccess(); 
  if(error == NO_ERROR) error = PCF8574N_Read1Byte(&serialNumWords[0], ACK, 100);    // I2c_ReadByte()	
   
  return error; 
} 
etError PCF8574N_ReadByte(u8t* readPCFByte) 
{ 
  etError error; // error code 
  u16t readPCFBytes[2]; 
  error = PCF8574N_StartReadAccess(); // new Rma generate Startcondition
  // if(error == NO_ERROR) error = PCF8574N_Read1Byte(&readPCFBytes[0], ACK, 100);    // I2c_ReadByte()	
	// Attention with NACK or ACK -->User decides what's needed depending of Chip and Master behavior
	// The master can forget to send a ACK if he has read the byte. 
	// Therefor "PCF8574N_Read1Byte(&readPCFBytes[0], NACK, 100);" is OK
	if(error == NO_ERROR) error = PCF8574N_Read1Byte(&readPCFBytes[0], NACK, 100);    // I2c_ReadByte()	
  PCF8574N_StopAccess(); 
  *readPCFByte = (readPCFBytes[0]);  
  return error; 
}  
                             

//----------------------------------------------------------------------------- 
etError PCF8574N_SoftReset(void) 
{ 
  etError error; // error code 
  error = PCF8574N_StartWriteAccess(); 
  // write reset command 
   error |= PCF8574N_WriteCommand(CMD_SOFT_RESET_PCF);  // 
  PCF8574N_StopAccess(); 
   
  // if no error, wait 0.5ms after reset 
  if(error == NO_ERROR) DelayMicroSeconds(500); 
  return error; 
} 
 

//----------------------------------------------------------------------------- 
void PCF8574N_HardReset(void) 
{ 
  // set reset low 
  RESET_LOW(); 
 
  // wait 2 ms 
  DelayMicroSeconds(2000); 
   
  // release reset 
  RESET_HIGH(); 
   
  // wait 1 ms after reset 
  DelayMicroSeconds(1000); 
}  
                              

//----------------------------------------------------------------------------- 
static etError PCF8574N_StartWriteAccess(void) 
{ 
  etError error; // error code 
 
  // write a start condition 
  I2c_StartCondition(); 
 
  // write the sensor I2C address with the write flag 
  error = I2c_WriteByte(_i2cAddress); 
 
  return error; 
} 

//----------------------------------------------------------------------------- 
static etError PCF8574N_StartReadAccess(void) 
{ 
  etError error; // error code 
 
  // write a start condition 
  I2c_StartCondition(); 
 
  // write the sensor I2C address with the read flag 
  error = I2c_WriteByte(_i2cAddress | 0x01); 
 
  return error; 
} 
 


//----------------------------------------------------------------------------- 
static void PCF8574N_StopAccess(void) 
{ 
  // write a stop condition 
  I2c_StopCondition(); 
}  


//----------------------------------------------------------------------------- 
static etError PCF8574N_WriteCommand(etCommands command) 
{ 
  etError error; // error code 
 
  // write the upper 8 bits of the command to the sensor 
  error  = I2c_WriteByte(command); 
 
//  // write the lower 8 bits of the command to the sensor 
//  error |= I2c_WriteByte(command & 0xFF); 
 
  return error; 
} 

//----------------------------------------------------------------------------- 
etError PCF8574N_WriteByte(u8t txByte) 
{ 
  etError error; // error code 
	error = PCF8574N_StartWriteAccess(); 
  error  = I2c_WriteByte(txByte); 
	PCF8574N_StopAccess();
  return error; 
} 

//----------------------------------------------------------------------------- 
static etError PCF8574N_Read1Byte(u16t* data, etI2cAck finaleAckNack, 
                                      u8t timeout) 
{  
     
  etError error;    // error code 
  u8t     bytes[1]; // read data array 
//  error = I2c_ReadByte(&bytes[0], ACK, 0);  
	error = I2c_ReadByte(&bytes[0], finaleAckNack, timeout);  
  *data = (bytes[0]);   // only 1 Byte with PCF    
  return error; 
}



// ENd of File
