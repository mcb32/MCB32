/**
* @file   	BME280_I2C.c
* @autor  	rma / Cityline AG
* @date   	2.4.2018
* @version 	1915.1
* @note   	BME280 I2C User I2C Routines for the BME280 Structs (idea from Bosch Git)
* @note   	BME280 chip from Bosch
* @note   	
* @link     https://github.com/BoschSensortec/BME280_driver
* @note     
*************************************************************/

#include <stm32f10x.h>								// uC-Typ bei uVision4
#include "..\TouchP0P1.h"								// P0-,P1-,Touchscreen 
#include "..\i2c.h"
#include "..\delays_cz.h"
//#include <stdio.h>										// lib for sprintf   .......
//#include <string.h>
//#include <stdlib.h>
//#include "global_directives.h"				// used for conditional compiling 
#include ".\bme280_CZ.h"   							// BOSCH Files from BOSCH Github
#include "..\debug.h"
//----------------------------------------------------------------------------
// Insert Functions for BME280 from Samples Files Bosch
void user_delay_ms(uint32_t period)
{
  DelayMs(period);     // from delay_cz.lib
}

/**
* @function: user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID,  BME_RX_Buffer, 1);
* @note  From Bosch Template proposal 
* @note  The parameter dev_id can be used as a variable to store the I2C address of the device
* @note  Data on the bus should be like
* @note  |------------+---------------------|
* @note  | I2C action | Data                |
* @note  |------------+---------------------|
* @note  | Start      | -                   |
* @note  | Write      | (reg_addr)          |
* @note  | Stop       | -                   |
* @note  | Start      | -                   |
* @note  | Read       | (reg_data[0])       |
* @note  | Read       | (....)              |
* @note  | Read       | (reg_data[len - 1]) |
* @note  | Stop       | -                   |
* @note  |------------+---------------------|
 */
// example: error = user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID,  BME_RX_Buffer, 1);
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    
	int8_t rslt = 0;                                   // Return 0 for Success, non-zero for failure
	if (len!=0)
	{
    uint8_t I2C_TX_DataBuf[] = {00, 00};						                  // TX Databuffer 
		uint8_t I2C_RX_DataBuf[10] = { 0 };						                    // RX Databuffer 
		uint8_t clen=0;                                                  // how many bytes are comming in calculated via len
    I2C_TX_DataBuf[0] =  reg_addr;                                   // set Registeraddress of BME280 in TX DataBuff 
	  I2C_Write(I2C1, I2C_TX_DataBuf , 1, (dev_id <<1));               // Write on I2C Address dev_id  the Address of the Register from I2C_DataBuf
	  delay_us(10);
		do{
			I2C_Read(I2C1, I2C_RX_DataBuf, 1, (BME280_I2C_ADDR_PRIM <<1)); 
			reg_data[clen]=I2C_RX_DataBuf[0];                                 // move actual received byte to BME280buffer
		  clen=clen+1;    // next pointer for received data
			delay_us(2);
		} while(clen != (len));                // loop until  number of len bytes are received. We are starting with buffer[0] --> len-1
		return rslt;														// No Error as feedback
	}else{
		rslt=1;                                 // Error 1 => len=0
		return rslt;
	}
// Now read the Data
}
// ------------------------------------------------------ end of user_i2c_read

// ------------------------------------------------------ Begin of user_i2c_write
/** From Bosch Template proposal 
* @function I2C_Write(I2C1, TempTXBuf , 2, (BME280_I2C_ADDR_PRIM<<1));  
* @parameter The parameter dev_id can be used as a variable to store the I2C address of the device
* @note Data on the bus should be like
* @note  |------------+---------------------|
* @note  | I2C action | Data                |
* @note  |------------+---------------------|
* @note  | Start      | -                   |
* @note  | Write      | (reg_addr)          |
* @note  | Write      | (reg_data[0])       |
* @note  | Write      | (....)              |
* @note  | Write      | (reg_data[len - 1]) |
* @note  | Stop       | -                   |
* @note  |------------+---------------------|
 */
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
  int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
	uint8_t BME_TX_Buffer[20] = {0};				// TX Buffer for Data to  BME280  (not more than 20)
	uint8_t li =0;

	if (len!=0)
 {
	 BME_TX_Buffer[0] =  reg_addr;                        // set Registeraddress of BME280 in TX DataBuff 
	 for(li=0;li<len;li++)    // 
	  {
	   BME_TX_Buffer[1+li] = reg_data[li];                 // copy tx data of reg_addr from external buffer *reg_data  to local tx buffer  BME_TX_Buffer
	  }
		I2C_Write(I2C1, BME_TX_Buffer , (len+1), (dev_id<<1));    // BME280_I2C_ADDR_PRIM = dev_id  (len+1 because of Registeradress in [0]
		delay_us(10);

	  return rslt;    // no Error all Data are sent
 }else
 {
	  return (rslt+11);      // Error in TX
 }
 // Exit Point to caller
}
// ------------------------------------------------------ end of user_i2c_write
// Sample function for Demonstration purposes
// Changed for MCB32 based and  TouchLib.
void print_sensor_data(struct bme280_data *comp_data)
{
	#ifdef printf_on
   float ftemp = ((float)comp_data->temperature)/100; 
	 float fpress = ((float)comp_data->pressure)/10000; 
	 float fhumi = ((float)comp_data->humidity)/1000; 
	 char strtemp[10];char strhumi[10];char strpress[10];
  
	#endif
	printAt ( 12, "SensorData Temp: " ); printDec(  4,(comp_data->temperature));
	printAt ( 13, "SensorData Humi: " ); printDec(  4,(comp_data->humidity));
	printAt ( 14, "SensorData Pres: " ); printDec(  4,(comp_data->pressure));
	#ifdef printf_on
	 sprintf ( strtemp, "%2.1f", ftemp );sprintf ( strhumi, "%2.1f", fhumi );sprintf ( strpress, "%3.1f", fpress );
	 my_printf("temp %s, press %s, hum %s\r\n",strtemp, strpress, strhumi);
	 // my_printf("temp %d, press %d, hum %d\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
	#endif
//#ifdef BME280_FLOAT_ENABLE
//  // printf("temp %0.2f, p %0.2f, hum %0.2f\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
//#else
//  // printf("temp %ld, p %ld, hum %ld\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
//#endif
}


// from Linux Sample
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
  int8_t rslt;
  uint8_t settings_sel;
  struct bme280_data comp_data;

  /* Recommended mode of operation: Indoor navigation */
  dev->settings.osr_h = BME280_OVERSAMPLING_1X;
  dev->settings.osr_p = BME280_OVERSAMPLING_16X;
  dev->settings.osr_t = BME280_OVERSAMPLING_2X;
  dev->settings.filter = BME280_FILTER_COEFF_16;

  settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

  rslt = bme280_set_sensor_settings(settings_sel, dev);

  // printf("Temperature, Pressure, Humidity\r\n");
  /* Continuously stream sensor data */
  // while (1) for demonstration purposes onöly
  while (1) {
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    /* Wait for the measurement to complete and print data @25Hz */
    dev->delay_ms(BME280_STANDBY_TIME_1000_MS);
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
    print_sensor_data(&comp_data);
  }
   return rslt;
}

// ---------------------------------------------------- Ende of Functions

