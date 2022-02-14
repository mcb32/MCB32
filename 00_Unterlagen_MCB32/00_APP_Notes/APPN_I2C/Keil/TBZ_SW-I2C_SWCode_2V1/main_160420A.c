//============================================================================= 
//    Cityline AG   AG,  8630 Rueti, Switzerland 
//============================================================================= 
// Project   :  PCF8574N and SHT3x Sample Code (based on Sensirion AG) (V2.0) 
// File      :  main.c (V2.0) 
// Author    :  MAL 
// Date      :  18.04.2016 
// Controller:  STM32F107VC 
// IDE       :  µVision V5.10.0.x 
// Compiler  :  Keil Default
// Brief     :  This code shows how to implement the basic commands for the 
//              SHT3x sensor and the I2C Bus Expander PCF8574N chip. 
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

//-- Defines for controlling compiler ----------------------------------------- 
#define SHT3x_USED 1          // Means the Chip is used  inside this project
#define MCB32D_USED 1            // Means the SW is running on a MCB32 HW (www.mcb32.ch)
//-- Static function prototypes ----------------------------------------------- 
// look in File "static_lib.h"
 
//----------------------------------------------------------------------------- 
int main(void) 
{ 
  etError   error;       // error code 
  u32t      serialNumber;// serial number 
	u8t				readPCFByte; //  Place for 1 Byte

	#ifdef SHT3x_USED
	regStatus status;      // sensor status 
  ft        temperature; // temperature [°C] 
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
   
  PCF8574N_Init(0x70); // Address: 0x44 = Sensor on EvalBoard connector 
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
    LedBlueOn(); 
		error = NO_ERROR; 
     
    // loop while no error 
    while(error == NO_ERROR) 
    { 
      PCF8574N_WriteByte(0xFF);
      // error |= PCF8574N_ReadStatus(&status.u16); 
			error = PCF8574N_ReadByte(&readPCFByte);
      if(error != NO_ERROR) break; 
       
      // check if the reset bit is set after a reset or power-up 
			
//      if(status.bit.ResetDetected)  
//      { 
//        //override default temperature and humidity alert limits (red LED) 
//        error = SHT3X_SetAlertLimits( 70.0f,  50.0f,  // high set:   RH [%], T [°C] 
//                                      68.0f,  48.0f,  // high clear: RH [%], T [°C] 
//                                      32.0f,  -2.0f,  // low clear:  RH [%], T [°C] 
//                                      30.0f,  -4.0f); // low set:    RH [%], T [°C] 
//        if(error != NO_ERROR) break; 
//     
//         
//        // clear reset and alert flags 
//        error = SHT3X_ClearAllAlertFlags(); 
//        if(error != NO_ERROR) break; 
//         
//        //start periodic measurement, with high repeatability and 1 measurements per second 
//        error = SHT3X_StartPeriodicMeasurment(REPEATAB_HIGH, FREQUENCY_1HZ); 
//        if(error != NO_ERROR) break; 
//         
//        //switch green LED on 
//        LedGreenOn(); 
//      } 
         
      // read measurment buffer 
//      error = SHT3X_ReadMeasurementBuffer(&temperature, &humidity); 
      if(error == NO_ERROR) 
      { 
        // flash blue LED to signalise new temperature and humidity values  Sample Code for SHT3x 
    
     
        LedBlueOn(); 
        DelayMicroSeconds(10000); 
        LedBlueOff(); 
      } 
      else if (error == ACK_ERROR) 
      { 
        // there were no new values in the buffer -> ignore this error 
        error = NO_ERROR; 
      } 
      else break; 
       
      // read heater status 
      heater = status.bit.HeaterStatus ? TRUE : FALSE; 
       
      // if the user button is not pressed ... 
      if(ReadUserButton() == 0) 
      {  
         // ... and the heater is on 
         if(heater) 
         { 
           // switch off the sensor internal heater 
           error |= SHT3X_DisableHeater(); 
           if(error != NO_ERROR) break; 
         } 
      } 
      else 
      // if the user button is pressed ... 
      { 
         // ... and the heater is off 
         if(!heater) 
         { 
           // switch on the sensor internal heater 
           error |= SHT3X_EnableHeater(); 
           if(error != NO_ERROR) break; 
         } 
      } 
       
      // wait 100ms 
      DelayMicroSeconds(100000); 
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
			// SHT3X_HardReset(); 
    } 
     
    // flash green LED to signalise an error 
    LedGreenOn(); 
    DelayMicroSeconds(10000); 
    LedGreenOff(); 
  } 
}  
   


// ENd of File
