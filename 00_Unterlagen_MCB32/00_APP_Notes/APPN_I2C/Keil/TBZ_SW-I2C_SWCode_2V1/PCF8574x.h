 
//============================================================================= 
//    Cityline AG   AG,  8630 Rueti, Switzerland 
//============================================================================= 
// Project   :  PCF8574x Sample Code (V1.1) 
// File      :  PCF8574x.h (V1.1) 
// Author    :  RMA 
// Date      :  19.4.2016 
// Controller:  STM32F107VC 
// IDE       :  µVision V5.10.0.2 
// Compiler  :  Keil-Default 
// Brief     :  PCF8574x Layer: Definitions of commands and functions for 
//              I2C-Expander access. 
//============================================================================= 
 
#ifndef PCF8574x_H 
#define PCF8574x_H 
 
//-- Includes ----------------------------------------------------------------- 
#include "system.h" 
#include "i2c_hal.h"

 
//-- Enumerations ------------------------------------------------------------- 
// Sensor Commands 
//typedef enum{ 
//  CMD_READ_SERIALNBR  = 0x3780, // read serial number 
//  CMD_READ_STATUS     = 0xF32D, // read status register 
//  CMD_CLEAR_STATUS    = 0x3041, // clear status register 
//  CMD_HEATER_ENABLE   = 0x306D, // enabled heater 
//  CMD_HEATER_DISABLE  = 0x3066, // disable heater 
//  CMD_SOFT_RESET      = 0x30A2, // soft reset 
//  	CMD_SOFT_RESET_PCF  = 0x0000, // soft reset 
//  CMD_MEAS_CLOCKSTR_H = 0x2C06, // measurement: clock stretching, high repeatability 
//  CMD_MEAS_CLOCKSTR_M = 0x2C0D, // measurement: clock stretching, medium repeatability 
//  CMD_MEAS_CLOCKSTR_L = 0x2C10, // measurement: clock stretching, low repeatability 
//  CMD_MEAS_POLLING_H  = 0x2400, // measurement: polling, high repeatability 
//  CMD_MEAS_POLLING_M  = 0x240B, // measurement: polling, medium repeatability 
//  CMD_MEAS_POLLING_L  = 0x2416, // measurement: polling, low repeatability 
//  CMD_MEAS_PERI_05_H  = 0x2032, // measurement: periodic 0.5 mps, high repeatability 
//  CMD_MEAS_PERI_05_M  = 0x2024, // measurement: periodic 0.5 mps, medium repeatability 
//  CMD_MEAS_PERI_05_L  = 0x202F, // measurement: periodic 0.5 mps, low repeatability 
//  CMD_MEAS_PERI_1_H   = 0x2130, // measurement: periodic 1 mps, high repeatability 
//  CMD_MEAS_PERI_1_M   = 0x2126, // measurement: periodic 1 mps, medium repeatability 
//  CMD_MEAS_PERI_1_L   = 0x212D, // measurement: periodic 1 mps, low repeatability 
//  CMD_MEAS_PERI_2_H   = 0x2236, // measurement: periodic 2 mps, high repeatability 
//  CMD_MEAS_PERI_2_M   = 0x2220, // measurement: periodic 2 mps, medium repeatability 
//  CMD_MEAS_PERI_2_L   = 0x222B, // measurement: periodic 2 mps, low repeatability 
//  CMD_MEAS_PERI_4_H   = 0x2334, // measurement: periodic 4 mps, high repeatability 
//  CMD_MEAS_PERI_4_M   = 0x2322, // measurement: periodic 4 mps, medium repeatability 
//  CMD_MEAS_PERI_4_L   = 0x2329, // measurement: periodic 4 mps, low repeatability 
//  CMD_MEAS_PERI_10_H  = 0x2737, // measurement: periodic 10 mps, high repeatability 
//  CMD_MEAS_PERI_10_M  = 0x2721, // measurement: periodic 10 mps, medium repeatability 
//  CMD_MEAS_PERI_10_L  = 0x272A, // measurement: periodic 10 mps, low repeatability 
//  CMD_FETCH_DATA      = 0xE000, // readout measurements for periodic mode 
//  CMD_R_AL_LIM_LS     = 0xE102, // read alert limits, low set 
//  CMD_R_AL_LIM_LC     = 0xE109, // read alert limits, low clear 
//  CMD_R_AL_LIM_HS     = 0xE11F, // read alert limits, high set 
//  CMD_R_AL_LIM_HC     = 0xE114, // read alert limits, high clear 
//  CMD_W_AL_LIM_HS     = 0x611D, // write alert limits, high set 
//  CMD_W_AL_LIM_HC     = 0x6116, // write alert limits, high clear  
//  CMD_W_AL_LIM_LC     = 0x610B, // write alert limits, low clear 
//  CMD_W_AL_LIM_LS     = 0x6100, // write alert limits, low set 
//  CMD_NO_SLEEP        = 0x303E, 
//}etCommands; 
 
//// Measurement Repeatability 
//typedef enum{ 
//  REPEATAB_HIGH,   // high repeatability 
//  REPEATAB_MEDIUM, // medium repeatability 
//  REPEATAB_LOW,    // low repeatability 
//}etRepeatabilityPCF; 
// 
//// Measurement Mode 
//typedef enum{ 
//  MODE_CLKSTRETCH, // clock stretching 
//  MODE_POLLING,    // polling 
//}etModePCF; 
// 
//typedef enum{ 
//  FREQUENCY_HZ5,  //  0.5 measurements per seconds 
//  FREQUENCY_1HZ,  //  1.0 measurements per seconds 
//  FREQUENCY_2HZ,  //  2.0 measurements per seconds 
//  FREQUENCY_4HZ,  //  4.0 measurements per seconds 
//  FREQUENCY_10HZ, // 10.0 measurements per seconds 
//}etFrequencyPCF; 
 
//-- Typedefs ----------------------------------------------------------------- 
// Status-Register PCF8574N
typedef union { 
  u16t u16; 
  struct{ 
    #ifdef LITTLE_ENDIAN  // bit-order is little endian 
    u16t Port0_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port1_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port2_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port3_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port4_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port5_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port6_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port7_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Reserve1      : 1; // reserved 
    u16t Reserve2      : 1; // reserved 
    u16t Reserve3      : 6; // reserved 		
    #else                 // bit-order is big endian 
    u16t Reserve3      : 6; // reserved 
    u16t Reserve2      : 1; // reserved 
    u16t Reserve1      : 1; // reserved 
    u16t Port7_Set     : 1; // Port0 of PCF8574 is set (Means 1)  
    u16t Port6_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port5_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port4_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port3_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port2_Set     : 1; // Port0 of PCF8574 is set (Means 1) 
    u16t Port1_Set     : 1; // Port0 of PCF8574 is set (Means 1) 		
    u16t Port0_Set     : 1; // Port0 of PCF8574 is set (Means 1) 	
    #endif 
  }bit; 
} regStatusPCF; 
 

//============================================================================= 
// Initializes the I2C bus for communication with the I2C Bus Expander PCF8574N.  
//----------------------------------------------------------------------------- 
// input: i2cAddress    I2C address, 0x70 ADDR pin low (W) / 0x45 ADDR pin high (RD)
//----------------------------------------------------------------------------- 
 void PCF8574N_Init(u8t i2cAddress);

//============================================================================= 
// Sets the I2C address. 
//----------------------------------------------------------------------------- 
// input: i2cAddress    I2C address, 0x70 ADDR pin low / 0x71 ADDR pin high 
//----------------------------------------------------------------------------- 
// void SHT3X_SetI2cAdr(u8t i2cAddress); 
void PCF8574N_SetI2cAdr(u8t i2cAddress) ; 
 

//============================================================================= 
// Reads the serial number from I2C CHIP PCF8574N if available. 
//----------------------------------------------------------------------------- 
// input: serialNumber  pointer to serialNumber 
// 
// return: error:       ACK_ERROR      = no acknowledgment from sensor 
//                      CHECKSUM_ERROR = checksum mismatch 
//                      TIMEOUT_ERROR  = timeout 
//                      NO_ERROR       = no error 
//----------------------------------------------------------------------------- 
etError PCF8574N_ReadSerialNumber(u32t* serialNumber) ;
 
//============================================================================= 
// Reads a Byte from I2C CHIP PCF8574N.
//----------------------------------------------------------------------------- 
// input: readPCFByte  pointer to a Byte "readPCFByte" 
// 
// return: error:       ACK_ERROR      = no acknowledgment from sensor 
//                      CHECKSUM_ERROR = checksum mismatch 
//                      TIMEOUT_ERROR  = timeout 
//                      NO_ERROR       = no error 
//-----------------------------------------------------------------------------  
 etError PCF8574N_ReadByte(u8t* readPCFByte);
 

//============================================================================= 
// Write a DataByte to PCF8574N. 
//----------------------------------------------------------------------------- 
// input:               Byte 
//
// return: error:       ACK_ERROR      = no acknowledgment from sensor 
//                      CHECKSUM_ERROR = checksum mismatch 
//                      TIMEOUT_ERROR  = timeout 
//                      NO_ERROR       = no error 
//-----------------------------------------------------------------------------  
etError PCF8574N_WriteByte(u8t txByte) ;

 
//============================================================================= 
// Starts periodic Readout. 
//----------------------------------------------------------------------------- 
// input: repeatability repeatability for the measurement [low, medium, high] 
//        frequency     measurement frequency [0.5, 1, 2, 4, 10] Hz 
// 
// return: error:       ACK_ERROR      = no acknowledgment from sensor 
//                      CHECKSUM_ERROR = checksum mismatch 
//                      TIMEOUT_ERROR  = timeout 
//                      PARM_ERROR     = parameter out of range 
//                      NO_ERROR       = no error 
//-----------------------------------------------------------------------------  
     
//etError PCF8574N_StartPeriodicMeasurment(etRepeatability repeatability,etFrequency frequency); 
 
//============================================================================= 
// Calls the soft reset mechanism that forces the PCF8574N into a well-defined 
// state without removing the power supply. 5ms Delay after Reset
//----------------------------------------------------------------------------- 
// return: error:       ACK_ERROR      = no acknowledgment from sensor 
//                      CHECKSUM_ERROR = checksum mismatch 
//                      TIMEOUT_ERROR  = timeout 
//                      NO_ERROR       = no error 
//----------------------------------------------------------------------------- 
etError PCF8574N_SoftReset(void) ;
 

//============================================================================= 
// Resets the PCF by pulling down the reset pin. 
//----------------------------------------------------------------------------- 
void PCF8574N_HardReset(void);

	
 
#endif 


// ENd of File
