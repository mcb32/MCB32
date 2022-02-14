/** @file I2C.h 
*  @brief  Header File for I2C.C Lib File
*          Read STM App Note AN2824 REV 3 and REV 4 for more information \n
*
* 
*  @author   		rma / Cityline  REV D implementation
*  @date      	12.9.2015   
*  @version 		1.0    
*  @todo    -
*  @see 
*  @note    		Code tested ?
*  @warning -
*  @bug 				No known bugs.
*/

// Check for Lib to prevent multiple reading
#ifndef I2C_H
#define I2C_H

#include <stm32f10x_i2c.h>

typedef enum {Error = 0, Success = !Error } Status;

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf, uint8_t SlaveAddress);
Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);
void I2C_LowLevel_Init(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress);

#endif
