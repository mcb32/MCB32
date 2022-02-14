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
* @note     BME280 developig prototyp. With the help of this code i found a way how the sensor is working
*           This code is valid for trying around.
*************************************************************/

#include <stm32f10x.h>								// uC-Typ bei uVision4
#include "TouchP0P1.h"								// P0-,P1-,Touchscreen 
#include "i2c.h"
#include "delays_cz.h"
#include <stdio.h>										// lib for sprintf   .......
#include <string.h>
#include <stdlib.h>
#include "global_directives.h"				// used for conditional compiling 
#include ".\I2C_BME280\bme280_CZ.h"


// Define I2C Address
#define PCF8574A   					0x40                    // !!!Setzte hier den richtigen Wert ein
#define PCF8574A_ClockSpeed    100000     			// 400 KHz 
#define PCF8574A_ADDRESS_WR	PCF8574A						// Implement here the Device Address for Writing
#define PCF8574A_ADDRESS_RD (PCF8574A |0x01)		// Implement here the Device Address for Reading


// Define I2C Address   BME280  / Bosch Sensor for Hum, Temp and pressure
#define IC_BME280_ClockSpeed    100000     			// 400 KHz is possible
//#define IC_BME280_ADDRESS_WR	PCF8574A					// Implement here the Device Address for Writing
//#define IC_BME280_ADDRESS_RD (PCF8574A |0x01)		// Implement here the Device Address for Reading
// Define I2C Address   BME280  / Bosch Sensor for Hum, Temp and pressure
/**\name I2C addresses see file bme280_defs_CZ.h */
// #define BME280_I2C_ADDR_PRIM              UINT8_C(0x76)
// #define BME280_I2C_ADDR_SEC               UINT8_C(0x77)

//initialization and reset instructions 
const uint8_t inst1[] = {0x00, 0xFF};					// also used for single Byte transfer to PCF8574A
const uint8_t inst2[] = {0xFF, 0x00}; 
const uint8_t reset[] = {0xFF};								// 0xFF Set PCF8574A Pins to 1 
const uint8_t read_ID[] = {0xD0, 00};						  // 0xD0 = Registeraddress of ID Register 
      uint8_t BME_RX_Buffer[40] = {0};				// Buffer for Data from BME280
			
uint8_t SoftResetTXBuf[2]= {0xE0,0xB6};           // softreset command as default
uint8_t TempTXBuf[4]= { 0};                      // Buffer for commands to BME280 
// Insert Functions for BME280 from Samples Files Bosch
void user_delay_ms(uint32_t period)
{
  DelayMs(period);     // from delay_cz.lib
}
// ----------------------------------------------------------------------------

// From Bosch Template proposal 
/*
 * function: user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID,  BME_RX_Buffer, 1);


     * The parameter dev_id can be used as a variable to store the I2C address of the device
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
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
// From Bosch Template proposal 
// I2C_Write(I2C1, TempTXBuf , 2, (BME280_I2C_ADDR_PRIM<<1));  
    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
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

// from Linux sample
void print_sensor_data(struct bme280_data *comp_data)
{
	
	printAt ( 12, "SensorData Temp: " );
	printDec(  4,(comp_data->temperature));
	printAt ( 13, "SensorData Humi: " );
	printDec(  4,(comp_data->humidity));
	printAt ( 14, "SensorData Pres: " );
	printDec(  4,(comp_data->pressure));
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
  while (1) {
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    /* Wait for the measurement to complete and print data @25Hz */
    dev->delay_ms(50);
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
    print_sensor_data(&comp_data);
  }
  return rslt;
}



// ---------------------------------------------------- Ende of Functions





// Variables 
char wertausgabe[30];
uint8_t receivedByte;
unsigned int blinking_frequency = 1;        // Value in HZ
unsigned int blinking_delay = 0;          // Value in ms


//----------------------------------------------------------
int main (void)						// Hauptprogramm
{
	uint8_t settings_sel;
  struct bme280_data comp_data;
	
	
	// data arrays for receiving Data
  uint8_t data1[6] = {0}; 
	// BME280
	struct bme280_dev dev;
	int8_t rslt = BME280_OK;
	// Touch
	InitTouchScreen();
  DelayInit();
	
  // Initialize I2C busses 
  I2C_LowLevel_Init(I2C1, IC_BME280_ClockSpeed, (BME280_I2C_ADDR_PRIM <<1));   // Address ist left aligned
	DelayInit();
	// Initialize the BME280 
	// I2C_Write(I2C1, reset , 1, BME280_I2C_ADDR_PRIM);              // Not a real function, only to check what happens onto the I2C Bus. I can see somet....
  //   I2C_Write(I2C1, read_ID , 1, (BME280_I2C_ADDR_PRIM <<1));     // Not a real function, only to check what happens onto the I2C Bus. I can see somet....
	//   delay_us(10);
  //	   I2C_Read(I2C1, data1, 1, (BME280_I2C_ADDR_PRIM <<1)); receivedByte=data1[0];
  //	if(data1[0]==0x60){
  //		printAt ( 2, "Chip ID found: BME280 ");
  //	}
	// Now check the user_i2C_read()   function. 
	rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID_ADDR,  BME_RX_Buffer, 1);
	if( rslt==TRUE )
	{
		printAt ( 4, "There was an Error in Reading ");
	}
	else{
		// printAt ( 4, "No Error in Reading ");
		// No Error
	}
	
	
	// Write Softreset to BME280 for debugging
       
//	I2C_Write(I2C1, TempTXBuf , 2, (BME280_I2C_ADDR_PRIM<<1));                 // Not a real function, only to check what happens onto the I2C Bus. I can see somet....
  //   I2C_Write(I2C1, read_ID , 1, (BME280_I2C_ADDR_PRIM <<1));     // Not a real function, only to check what happens onto the I2C Bus. I can see somet....
	//   delay_us(10);
  //	   I2C_Read(I2C1, data1, 1, (BME280_I2C_ADDR_PRIM <<1)); receivedByte=data1[0];
  //	if(data1[0]==0x60){
  //		printAt ( 2, "Chip ID found: BME280 ");
  //	}
	// Now check the user_i2C_read()   function. 	
	
	// I2C_Read(I2C1,data1,1,BME280_I2C_ADDR_PRIM);
	//InitTouchP0P1("0");	
	//DelayInit();

	
// ______________________________________________________________________

  //Linux will not work here
//  if ((fd = open(argv[1], O_RDWR)) < 0) {
//    printf("Failed to open the i2c bus %s", argv[1]);
//    exit(1);
//  }
//  if (ioctl(fd, I2C_SLAVE, 0x76) < 0) {
//    printf("Failed to acquire bus access and/or talk to slave.\n");
//    exit(1);
//  }
  dev.dev_id = BME280_I2C_ADDR_PRIM;
  dev.intf = BME280_I2C_INTF;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.delay_ms = user_delay_ms;

  rslt = bme280_init(&dev);
	// Now check the user_i2C_read()   function. 
	rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID_ADDR,  BME_RX_Buffer, 1);
	if( rslt==TRUE )
	{
		printAt ( 19, "There was an Error in Reading ");
	}
	else{
		printAt ( 19, "BNE280 found after Init 1 ");
		// No Error
	}
  printAt ( 6, "Start Reading from Sensor: " );  printDec (-2,rslt);


// ---------------------------------------------
	
	printAt ( 1, "BME280 DemoProg. Bosch Lib" );
	
	// stream_sensor_data_forced_mode(&dev);
//	#ifdef Remapp_I2C1
//	 printAt ( 3, "I2C Remapped to PB8, PB9" );
//	#else
//	 printAt ( 3, "I2C at PB6, PB7" );
//	#endif
//	// Demo
	blinking_delay= (int)(1000/ blinking_frequency);		// delay in ms
	
	
	
	



//  /* Recommended mode of operation: Indoor navigation */
//	
//  dev.settings.osr_h = BME280_OVERSAMPLING_1X;
//  dev.settings.osr_p = BME280_OVERSAMPLING_16X;
//  dev.settings.osr_t = BME280_OVERSAMPLING_2X;
//  dev.settings.filter = BME280_FILTER_COEFF_16;

//  settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

//  rslt = bme280_set_sensor_settings(settings_sel, &dev);  // Fehler ?? Hier schreibt der Bus eine 1 auf die Adresse

  // printf("Temperature, Pressure, Humidity\r\n");
  /* Continuously stream sensor data */
//    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);
//    /* Wait for the measurement to complete and print data @25Hz */
//    dev.delay_ms(50);
//		DelayMs(50);
//		
// OK Set now the Sensor to a measuring Mode:
// Reg F4: Clear alle Measur crtl bits Write 00 to F4
// Reg F2: Controlls Hum  osrs_h[2:0]: 
// Reg F3: Controlls Status measuring[0]   and im_update[0] 
// Reg F4: Controlls Hum  osrs_t[2:0]  osrs_p[2:0]   mode[1:0]
// Reg F5: Controlls config  t_sb[2:0]  filter[2:0]   spi3w_en[0]: 
 
    TempTXBuf[0] = 0x00; 		
		rslt= user_i2c_write(BME280_I2C_ADDR_PRIM, 0xF4, TempTXBuf, 1);   // Writes F4 00 to BME: Clears all ctrl_meas Bits
    TempTXBuf[0] = 0xB0; 		
		rslt= user_i2c_write(BME280_I2C_ADDR_PRIM, 0xF5, TempTXBuf, 1);   // Writes to BME: sets config to 1000ms, Filterkoef 4 and no SPI
		TempTXBuf[0] = 0x01; 		
		rslt= user_i2c_write(BME280_I2C_ADDR_PRIM, 0xF2, TempTXBuf, 1);   // Writes to BME: sets ctrl_hum to oversampling *1
		TempTXBuf[0] = 0x67; 		
		rslt= user_i2c_write(BME280_I2C_ADDR_PRIM, 0xF4, TempTXBuf, 1);   // Writes to BME: sets ctrl_meas to oversamp *4, oversamp*4 and forced mode
		rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, BME280_CHIP_ID_ADDR,  BME_RX_Buffer, 1);
	  if( rslt==TRUE )
	  {
		  printAt ( 15, "There was an Error in Reading ");
	  }
	   else{
	  	printAt ( 15, "BNE280 found after Init in forced Mode ");
		// No Error
	}
// End of Init with Success



while (1) 
	{ 
		
		rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, 0xF3,  BME_RX_Buffer, 1);
	  if( rslt==TRUE )
	   {
	   	printAt ( 16, "Error during measure ");
	   }
	   else{
			
       rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, 0xFD,  BME_RX_Buffer, 2);
			 printAt ( 16, "FD hum: "); printBin(8,BME_RX_Buffer[0]);print("/"); printBin(8,BME_RX_Buffer[1]);
			 
	  	// No Error
	   }
		 DelayMs(20);
		 rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, 0xF4,  BME_RX_Buffer, 1);
	  if( rslt==TRUE )
	   {
	   	printAt ( 17, "There was an Error in Reading ");
	   }
	   else{
			 printAt ( 17, "F4: "); printBin(8,BME_RX_Buffer[0]);
	  	// No Error
	   }
		 DelayMs(20);
		  rslt = user_i2c_read(BME280_I2C_ADDR_PRIM, 0xF5,  BME_RX_Buffer, 1);
	  if( rslt==TRUE )
	   {
	   	printAt ( 18, "There was an Error in Reading ");
	   }
	   else{
			 printAt ( 18, "F5: "); printBin(8,BME_RX_Buffer[0]);
	  	// No Error
	   }
		 DelayMs(20);
		
		
//		// turn LED ON
//		I2C_Write(I2C1, inst1, 1, PCF8574A_ADDRESS_WR); 
//		DelayMs(1);
//		// Read LED blinking status (off/on)
//		I2C_Read(I2C1, data1, 1, PCF8574A_ADDRESS_RD); receivedByte=data1[0];
//		// Display LED blinking status
//		if (receivedByte == 0)
//		{
//			printAt ( 12, "LED Blinking  ON" );
//	    sprintf(wertausgabe,"Received-Byte    I: %d", receivedByte);
//			printAt ( 13, wertausgabe );
//		}
//		else if (receivedByte >= 1)
//		{
//			printAt ( 12, "LED Blinking OFF" );
//      sprintf(wertausgabe,"Received-Byte  II: %d", receivedByte);
//		  printAt ( 14, wertausgabe );		
//		}
//	  DelayMs(blinking_delay);                  		// delay_us(10000); 									// If InitTouchxx("1");
//		//turn LED OFF
//		I2C_Write(I2C1, inst2, 1, PCF8574A_ADDRESS_WR); 
//		DelayMs(1);
//		// Read LED blinking status (off/on)
//		I2C_Read(I2C1, data1, 1, PCF8574A_ADDRESS_RD); 		receivedByte=data1[0];
//		// Display LED blinking status
//		if (receivedByte == 0)
//		{
//			printAt ( 12, "LED Blinking  ON" );
//      sprintf(wertausgabe,"Received-Byte III: %d", receivedByte);
//		  printAt ( 14, wertausgabe );			

//		}
//		else if (receivedByte >= 1)
//		{
//			printAt ( 12, "LED Blinking OFF" );
//      sprintf(wertausgabe,"Received-Byte  IV: %d", receivedByte);
//		  printAt ( 14, wertausgabe );		
//		}
//		DelayMs(blinking_delay);

	}
}








