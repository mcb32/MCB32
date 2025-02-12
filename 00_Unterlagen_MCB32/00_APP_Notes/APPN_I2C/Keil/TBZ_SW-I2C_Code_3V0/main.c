//============================================================================= 
//    Cityline AG   AG,  8630 Rueti, Switzerland 
//============================================================================= 
// Project   :  PCF8574N and SHT3x Sample Code (based on Sensirion AG) (V3.0) 
// File      :  main.c (V3.0) 
// Author    :  MAL 
// Date      :  18.04.2016  // 15.3.2019 (I2C adress Output)
// Controller:  STM32F107VC 
// IDE       :  �Vision V5.10.0.x 
// Compiler  :  Keil Default
// Brief     :  This code shows how to implement the basic commands for the 
//              SHT3x sensor and the I2C Bus Expander PCF8574N chip with the
//							help of bit-banging (SW/HW Emulation of I2C)
//              Due to compatibility reasons the I2C interface is implemented 
//              as "bit-banging" on normal I/O's. This code is written for an 
//              easy understanding and is neither optimized for speed nor code 
//              size. (Sensirion AG)
// 
// Porting to a different microcontroller (uC): 
//   - the definitions of basic types may have to be changed  in typedefs.h 
//   - adapt the button and led functions for your platform   in main.c 
//   - adapt the port functions / definitions for your uC     in i2c_hal.h/.c 
//   - adapt the timing of the delay function for your uC     in system.c 
//   - adapt the SystemInit()                                 in system.c 
//   - change the uC register definition file <stm32f10x.h>   in system.h 
//============================================================================= 
 
//-- Includes ----------------------------------------------------------------- 
#include <stm32f10x.h>			  // uC-Typ bei uVision4
#include "system.h" 
#include "sht3x.h" 
#include "PCF8574x.h" 
#include "TouchP0P1.h"				// P0-,P1-,Touchscreen 
#include "static_lib.h"
#include <stdio.h>

//-- Defines for controlling compiler ----------------------------------------- 
// #define SHT3x_USED 1          // Means the Chip is used  inside this project
#define MCB32D_USED 1            // Means the SW is running on a MCB32 HW (www.mcb32.ch)
//-- Static function prototypes ----------------------------------------------- 
// look in File "static_lib.h"
 
//----------------------------------------------------------------------------- 
int main(void) 
{ 
  etError   error;       	// error code 
  u32t      serialNumber;	// serial number 
	u8t				readPCFByte; 	//  Place for 1 Byte
	char wertausgabe[40];		// used for sprintf operations as buffer

	#ifdef SHT3x_USED
	regStatus status;      // sensor status 
  ft        temperature; // temperature [�C] 
  ft        humidity;    // relative humidity [%RH] 
  bt        heater;      // heater, false: off, true: on 
	#endif
	
   
  SystemInit(); 
  Led_Init();          // Switch LED PE8 and PE9 OFF
  UserButton_Init();  
     
  EvalBoardPower_Init();  // Switch LED PE15 ON
	InitTouchScreen();
	printAt ( 2, "I2C DemoProg. Soft-I2C PortB" );
  printAt ( 3, "SCL=PB13, SDA=PB14" );
	printAt ( 4, "I2C Adress is 0x40" );
  // There are different PCF8574 Chips therefore also diff. addresses 0x70, 0x40 .... 
  PCF8574N_Init(0x40); // Address: 0x44 = Sensor on EvalBoard connector 
                    //          0x45 = Sensor on EvalBoard 
	
  // wait 1ms after power on 
  DelayMicroSeconds(1000);     
	printAt ( 5, "Make: Soft_Reset ..........." );
	PCF8574N_SoftReset(); // Write Adress and a Byte (ox00) to the PCF8574
	DelayMicroSeconds(100);   
	PCF8574N_WriteByte(0xFF);
	printAt ( 7, "Done: Write Data ..........." );	
	error = PCF8574N_ReadByte(&readPCFByte);   // Dummy Read 
	printAt ( 6, "Done: Read Data ............" );	
  PCF8574N_WriteByte(0xA5);
	error = PCF8574N_ReadByte(&readPCFByte);   // Dummy Read 
  PCF8574N_WriteByte(0x5A);
	error = PCF8574N_ReadByte(&readPCFByte);   // Dummy Read 
		DelayMicroSeconds(100); 
	error = PCF8574N_ReadByte(&readPCFByte);   // Dummy Read 
//  error = SHT3x_ReadSerialNumber(&serialNumber); 
  if(error != NO_ERROR){} // do error handling here 
      
// loop forever 
  while(1) 
  { 
    LedBlueOff(); 
		error = NO_ERROR; 
     
    // loop while no error 
    while(error == NO_ERROR) 
    { 
			error = PCF8574N_WriteByte(0x00);
	    sprintf(wertausgabe,"Error ..... I  : %d", error);
			printAt ( 12, wertausgabe );
		  DelayMicroSeconds(500000); 
			error = PCF8574N_WriteByte(0xFF);
			sprintf(wertausgabe,"Error ..... II : %d", error);
			printAt ( 13, wertausgabe );
			DelayMicroSeconds(500000); 
      // read status register 
      if(error != NO_ERROR) break; 
       
      
      // read IO of PCF8574 
      error = PCF8574N_ReadByte(&readPCFByte);   // Dummy Read 
			sprintf(wertausgabe,"Error ..... III: %d", error);
			printAt ( 14, wertausgabe );
      if(error == NO_ERROR) 
      { 
        // flash blue LED to signalise no error in communication  
        LedBlueOn(); 
        DelayMicroSeconds(50000); 
        LedBlueOff(); 
				// Delete old Errors 
				sprintf(wertausgabe,"Error .ACK.  IV: %d", error);
			  printAt ( 15, wertausgabe );
				sprintf(wertausgabe,"                     ");
			  printAt ( 16, wertausgabe );
				sprintf(wertausgabe,"Byte  .read... : %x", readPCFByte);
			  printAt ( 18, wertausgabe );
      } 
      else 
				if (error == ACK_ERROR) 
        { 
        // there was an ACK error 
        // error = NO_ERROR; 
				sprintf(wertausgabe,"Error .ACK.  IV: %d", error);
			  printAt ( 15, wertausgabe );
				break;
        }
				else
					{
					  sprintf(wertausgabe,"Error .....   V: %d", error);
			      printAt ( 15, wertausgabe );
						break;
					}
   
      // if the user button is not pressed ... 
      if(ReadUserButton() == 0) 
      {  
         // ... and the heater is on 
				sprintf(wertausgabe,"Button ..... I : %d", ReadUserButton());
			  printAt ( 19, wertausgabe );
    
      } 
      else 
      // if the user button is pressed ... 
      { 
         // ... and the heater is on 
				sprintf(wertausgabe,"Button pressed : %d", ReadUserButton());
			  printAt ( 19, wertausgabe );
				// ... and the heater is off 
 
      } 
       
      // wait 1ms 
      DelayMicroSeconds(1000); 
    } 
     
    // in case of an error ... 
     
    // ... switch green and blue LED off 
    LedGreenOff(); 
    LedBlueOff(); 
     
    // ... try first a soft reset ... 
//    error = SHT3X_SoftReset(); 
    error = PCF8574N_SoftReset(); 
    // ... if the soft reset fails, do a hard reset 
    if(error != NO_ERROR) 
    { 
      PCF8574N_HardReset();
			sprintf(wertausgabe,"Error .....  VI: %d", error);
			printAt ( 16, wertausgabe );
			// SHT3X_HardReset(); 
    } else
		{
		  sprintf(wertausgabe,"Error .....  VI: %d", error);
			printAt ( 16, wertausgabe );
		}
     
    // flash green LED to signalise an error 
    LedGreenOn(); 
    DelayMicroSeconds(800000); 
    LedGreenOff(); 
  } 
}  
   


// ENd of File
