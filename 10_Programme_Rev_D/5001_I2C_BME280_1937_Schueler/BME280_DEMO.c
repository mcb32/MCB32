/**
* @file   	BME280_I2C.c
* @autor  	rma / Cityline AG
* @date   	2.4.2018 / 11.9.2019
* @version 	1.1
* @note   	BME280 I2C Prototyp 
* @note   	BME280 chip from Bosch
* @note   	Remapping of I2C bus: YES  
*           default: PB6 - I2C1_SCL; PB7 - I2C1_SDA
*           remap:   PB8 - I2C1_SCL; PB9 - I2C1_SDA
* @link     https://github.com/BoschSensortec/BME280_driver
* @note     
* @install	11.9.2019: Connect as following listed:
*           VCC of BME280 to:  PB(8..15)  Pin 9
*						GND of BME280 to:  PB(8..15)  Pin 10
*						SCL of BME280 to:  PB(8..15)  Pin 1  (= PB8 remapped)
*						SDA of BME280 to:  PB(8..15)  Pin 2  (= PB9 remapped)
* @install	11.9.2019: Communication. Connect Hyperterminal with 192000 Baud  
*                                     to Uart1 of MCB32 and you will see the values.
*************************************************************/
#include <stm32f10x.h>								// uC-Typ bei uVision4
#include "TouchP0P1.h"								// P0-,P1-,Touchscreen 
#include "global_directives.h"				    // used for conditional compiling 
#include "delays_cz.h"
#include "i2c.h"
#include ".\printf_embedded\my_printf.c"     // small printf implementation without float
#include ".\I2C_BME280\bme280_I2C.c"   	   // BOSCH Files from BOSCH Github
#include ".\I2C_BME280\bme280_CZ.h"   		   // BOSCH Files from BOSCH Github
#include "debug.h"                            // Switch is defined in debug.h       #define printf_oFF      								
// Define IC_BME280_ClockSpeed of   BME280  / Bosch Sensor for Hum, Temp and pressure
#define IC_BME280_ClockSpeed    100000     			// 400 KHz is possible
/**\name I2C addresses see file bme280_defs_CZ.h */

//initialization and reset instructions 
// const uint8_t inst1[] = {0x00, 0xFF};					// also used for single Byte transfer to PCF8574A
uint8_t BME_RX_Buffer[40] = {0};				// Buffer for Data from BME280
uint8_t TempTXBuf[4]= { 0};                      // Buffer for commands to BME280 

// 
// Setup Function
void setup(void)
{
	// Touch
	InitTouchScreen();    // Init MCB32 Touch
  DelayInit();
  // Initialize I2C busses 
  I2C_LowLevel_Init(I2C1, IC_BME280_ClockSpeed, (BME280_I2C_ADDR_PRIM <<1));   // Address ist left aligned
	DelayInit();
	// for ebugging Infos via USART1 and Printf
	USARTInit(1, 0); // USART1 ohne Intr. USART1: 19200Bd, 1,8,1,n //PCLK2 mit 72MHz max (MCB32 nach Reset)
	
}

// ----------------------------------------------------------------------------
// Variables 
char wertausgabe[30];

//----------------------------------------------------------
int main (void)						// Hauptprogramm
{
  // BME280 Setup
	uint8_t settings_sel;
	struct bme280_data comp_data;
   struct bme280_dev dev;
	int8_t rslt = BME280_OK;
	// Add properties to struct
   dev.dev_id = BME280_I2C_ADDR_PRIM;
   dev.intf = BME280_I2C_INTF;
   dev.read = user_i2c_read;
   dev.write = user_i2c_write;
   dev.delay_ms = user_delay_ms;
	// End of BME280 setup
	
	
 	setup();     // Set the MCB32 HW and the Touch for the Application
   printAt ( 1, "BME280 DemoProg. 1937B" );        // with Bosch Lib 
	rslt = bme280_init(&dev);    // Init BME280: reads the chip-id and calibration data from the sensor.
  
	// for Demonstration purposes: Now check the user_i2C_read()   function. 
	rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID_ADDR,  BME_RX_Buffer, 1);
	if( rslt==TRUE )
	{
		printAt ( 19, "There was an Error 4 in Reading ");
	}
	else{
		printAt ( 19, "BNE280 found after Init (4) ");
		// No Error
	}
  printAt ( 6, "Start Reading from Sensor: " );  printDec (-2,rslt);
  printAt ( 4, "Debug via USART 1 is ON");
  #ifdef printf_on  
	  my_printf("\nDebug via USART 1 is ON\n");     // Test of my_printf
  #endif

// ---------------------------------------------
	
//  /* Recommended mode of operation: Indoor navigation */	
//  dev.settings.osr_h = BME280_OVERSAMPLING_1X;
//  dev.settings.osr_p = BME280_OVERSAMPLING_16X;
//  dev.settings.osr_t = BME280_OVERSAMPLING_2X;
//  dev.settings.filter = BME280_FILTER_COEFF_16;

//  Now read the data in forced Mode with 25Samples /s and send result to Touch and to RS232
    stream_sensor_data_forced_mode(&dev);              // Never come back from this call

// \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  Investigation area from the dev. phase
//  See File BME280_Development.c   for more Details  and how to use the sensor.


}








