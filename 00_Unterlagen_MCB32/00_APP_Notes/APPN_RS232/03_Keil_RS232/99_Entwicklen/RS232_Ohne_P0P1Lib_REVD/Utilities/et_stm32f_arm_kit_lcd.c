/**
  ******************************************************************************
  * @file    et_stm32f_arm_kit_lcd.c
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
  *          Liquid Crystal Display Module of STM3210C-EVAL board.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "et_stm32f_arm_kit_lcd.h"
#include "fonts.h"

// GLCD Controller RevD. : ILI9341
// GLCD Display : Library ST Style
// GLCD:ILI9341
// X = 0...239(240)
// Y = 0...319(320)  
// Y = Y+1
// ===================================
// |X0,Y0                   X0,Y319|#|
// |                               |#|
// |                               |#|
// |                               |#|
// |                               |#|
// |                               |#|
// |                               |#|
// |X239,Y0               X239,Y319|#|
// ===================================

// extern void delay_ms(__IO uint32_t nTime);   // warum grossgeschrieben rma_28012015
extern void delay_ms(__IO uint32_t nTime);
static  __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void ET_STM32_LCD_Init(void)
{
  /* Setups the LCD */
  LCD_Setup();
}

/**
  * @brief  Setups the LCD.
  * @param  None
  * @retval None
  */
void LCD_Setup(void)
{ 
  long cnt ;
	
  /* Configure the LCD Control pins --------------------------------------------*/
  LCD_GPIO_Config();
  GPIO_WriteBit(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, Bit_SET);
  
  /* Configure the LCD_SPI interface ----------------------------------------------*/
  LCD_SPIConfig();
  delay_ms(50); /* Delay 50 ms */
	
  //-------------- Start GLCD --------------
  LCD_WriteCommand(0x01);   																																			//Software reset
  delay_ms(5);
  LCD_WriteCommand(0x28);    																																			//display off
    
  LCD_WriteCommand(0xCF);
  LCD_WriteData(0x00);
  LCD_WriteData(0x83);
  LCD_WriteData(0x30);
      
  LCD_WriteCommand(0xED);
  LCD_WriteData(0x64);
  LCD_WriteData(0x03);
  LCD_WriteData(0x12);
  LCD_WriteData(0x81);
  
  LCD_WriteCommand(0xE8);
  LCD_WriteData(0x85);
  LCD_WriteData(0x01);
  LCD_WriteData(0x79);
  
  LCD_WriteCommand(0xCB);
  LCD_WriteData(0x39);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x00);
  LCD_WriteData(0x34);
  LCD_WriteData(0x02);
  
  LCD_WriteCommand(0xF7);
  LCD_WriteData(0x20);
  
  LCD_WriteCommand(0xEA);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  
  //---------------- Power Control -------------------   
  LCD_WriteCommand(0xC0);    																																			//Power Control 1
  LCD_WriteData(0x26);
   
  LCD_WriteCommand(0xC1);    																																			//Power Control2
  LCD_WriteData(0x11);

  //----------------- VCOM --------------------
  LCD_WriteCommand(0xC5);    																																			//VCOM Control 1
  LCD_WriteData(0x35);
  LCD_WriteData(0x3E);

  LCD_WriteCommand(0xC7);    																																			//VCOM Control 2
  LCD_WriteData(0xBE);

  //----------------- Memory access Control ---------------
  	
	//1010 1000
	//MY  = 1 : Row Address Order
	//MX  = 0
	//MV  = 1
	//ML  = 0  
	
	//BGR = 1 : Color = BGR
	//MH  = 0 : Horizontal Refresh
	//0
	//0
	LCD_WriteCommand(0x36);    																																			//Memory access Control 
  LCD_WriteData(0xA8);    																																				//my,mx=1,mv,ml,BGR,mh
	//==============================================================================================
	
  LCD_WriteCommand(0x3A);    																																			//Pixel format set
  LCD_WriteData(0x55);   																																					//16 bit/Pixel

  //---------------------- Frame Rate ----------------------
  LCD_WriteCommand(0xB1);    																																			//Frame rate
  LCD_WriteData(0x00);
  LCD_WriteData(0x1B);    																																				//default

  //---------------------- Gamma -------------------
  LCD_WriteCommand(0xF2);    																																			//3Gamma Function Disable 
  LCD_WriteData(0x08);  
   
  LCD_WriteCommand(0x26);    
  LCD_WriteData(0x01);    																																				// Gamma Set 4 gamma curve 01/02/04/08
   
  LCD_WriteCommand(0xE0);    																																			//Positive gamma correction    
  LCD_WriteData(0x1F);   
  LCD_WriteData(0x1A);     
  LCD_WriteData(0x18);   
  LCD_WriteData(0x0A);     
  LCD_WriteData(0x0F);   
  LCD_WriteData(0x06);     
  LCD_WriteData(0x45);   
  LCD_WriteData(0x87);     
  LCD_WriteData(0x32);   
  LCD_WriteData(0x0A);     
  LCD_WriteData(0x07);   
  LCD_WriteData(0x02);     
  LCD_WriteData(0x07);   
  LCD_WriteData(0x05);     
  LCD_WriteData(0x00);   

  LCD_WriteCommand(0xE1);    																																			//Negamma correction    
  LCD_WriteData(0x00);   
  LCD_WriteData(0x25);     
  LCD_WriteData(0x27);   
  LCD_WriteData(0x05);     
  LCD_WriteData(0x10);   
  LCD_WriteData(0x09);     
  LCD_WriteData(0x3A);   
  LCD_WriteData(0x78);     
  LCD_WriteData(0x4D);   
  LCD_WriteData(0x05);     
  LCD_WriteData(0x18);   
  LCD_WriteData(0x0D);     
  LCD_WriteData(0x38);   
  LCD_WriteData(0x3A);     
  LCD_WriteData(0x1F);   
  
  //------------------ SET DDRAM Address ------------------
	LCD_WriteCommand(0x2A);    																																			//Column Set(X=0..319)   
	LCD_WriteData(0x00);    																																				//Strart address(H) = 0x0000
  LCD_WriteData(0x00);    																																				//Strart address(L) = 0x0000(0)	
	LCD_WriteData(0x01);    																																				//End address(H) = 0x01
  LCD_WriteData(0x3F);    																																				//End address(L) = 0x3F(319)
  		
	LCD_WriteCommand(0x2B);    																																			//Page Address Set(Y=0..239)    
	LCD_WriteData(0x00);    																																				//Strart address(H) = 0x00
  LCD_WriteData(0x00);    																																				//Strart address(L) = 0x00(0)	
	LCD_WriteData(0x00);    																																				//End address(H) = 0x00
  LCD_WriteData(0xEF);    																																				//End address(L) = 0xEF(239)  
	   
  LCD_WriteCommand(0xB7);    																																			//Entry mode set    
  LCD_WriteData(0x07);    																																				//Normal display

  //-------------------- Display -----------------
  LCD_WriteCommand(0xB6);    																																			//Display Function Control  
  LCD_WriteData(0x0A);   
  LCD_WriteData(0xC2);     //0x82,0xE2,0xC2
  LCD_WriteData(0x27);   
  LCD_WriteData(0x00);     

  LCD_WriteCommand(0x11);    																																			//Sleep Out       
  delay_ms(100);

  //------- Clear the contents of the display buffer. --------
  LCD_WriteCommand(0x2C);    																																			//Memmory Write  
  for(cnt = 0;cnt<((long)240*320);cnt++)
  LCD_WriteData16(Black);    																																			//Back-Ground Color Black
	
  LCD_WriteCommand(0x29);    																																			//Display On       
  delay_ms(100);
  
  GPIO_WriteBit(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, Bit_SET);   																		//Back Light ON     
}

/**
  * @brief  Configures LCD control lines in Output Push-Pull mode.
  * @param  None
  * @retval None
  */
void LCD_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(LCD_CS_GPIO_CLK | LCD_BL_GPIO_CLK | LCD_SPI_GPIO_CLK, ENABLE);

  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = LCD_CS_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStructure);  
	
  /* Configure BL in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = LCD_BL_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_BL_GPIO_PORT, &GPIO_InitStructure);  
	
	/* Configure SCK3 in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure);  
	
	/* Configure MOSI3 in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure); 
	
	GPIO_WriteBit(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, Bit_RESET);
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_BL_GPIO_PIN, Bit_RESET);
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_RESET);
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_MOSI_PIN, Bit_RESET);
}

/**
  * @brief  Configures the LCD_SPI interface.
  * @param  None
  * @retval None
  */
void LCD_SPIConfig(void)
{
  SPI_InitTypeDef    SPI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(LCD_SPI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);

  /* Enable SPI clock  */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  
  /* Configure SPI pins: SCK3, MISO3 and MOSI3 */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN | LCD_SPI_MISO_PIN | LCD_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure);  
  SPI_I2S_DeInit(LCD_SPI);
  
  /* SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_Init(LCD_SPI, &SPI_InitStructure);

  /* SPI enable */
  SPI_Cmd(LCD_SPI, ENABLE);
}

/**
  * @brief  LCD Write Command 8Bit
  * @param  GLCD_Data: Byte Command
  * @retval None
  */
void LCD_WriteCommand(uint8_t GLCD_Command)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Swap SCK3,MOSI3 = GPIO Output Mode
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure);
	//
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_MOSI_PIN, Bit_RESET);																	// MOSI = Low(Command)	  	
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_RESET);																		// SCK = Low(Default)
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, Bit_RESET);																		// CS = Low
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_SET);																		  // SCK = High(Strobe D/C)
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_RESET);																		// SCK = Low(Default)
	//
	//Swap SCK3,MOSI3	= SPI Function
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure);
  //	
	//Start Send Command Byte to SPI 
  SPI_I2S_SendData(LCD_SPI, GLCD_Command);																												// Byte Command 
  while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);
	//
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, Bit_SET);																			// CS = High
}

/**
  * @brief  LCD Write Data 8Bit
  * @param  GLCD_Data: Byte Data
  * @retval None
  */
void LCD_WriteData(uint8_t GLCD_Data)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Swap SCK3,MOSI3 = GPIO Output Mode
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure);
	//
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_MOSI_PIN, Bit_SET);																  	// MOSI = High(Data)	  	
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_RESET);																		// SCK = Low(Default)
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, Bit_RESET);																		// CS = Low
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_SET);																		  // SCK = High(Strobe D/C)
	GPIO_WriteBit(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN, Bit_RESET);																		// SCK = Low(Default)
	//
	//Swap SCK3,MOSI3	= SPI Function
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(LCD_SPI_GPIO_PORT, &GPIO_InitStructure);
	//
	//Start Send Data Byte to SPI 
  SPI_I2S_SendData(LCD_SPI, GLCD_Data);																														// Byte Data
  while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);
	//
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, Bit_SET);																			// CS = High
}

/**
  * @brief  LCD Write Data 16Bit
  * @param  GLCD_Data16: Word Data
  * @retval None
  */
void LCD_WriteData16(uint16_t GLCD_Data16)
{ 
	LCD_WriteData(GLCD_Data16>>8);
  LCD_WriteData(GLCD_Data16);
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @retval None
  */
void LCD_ClearLine(uint8_t Line)
{
  LCD_DisplayStringLine(Line, "                    ");
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{	
  uint32_t index = 0;
 
  LCD_SetCursor(0,0); 			
	LCD_WriteCommand(0x2C);    																																			//Memmory Write  																											// Memory Write							
  for(index = 0; index < ((long)240*320); index++)
  {
		LCD_WriteData16(Color);
  }
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{  
	LCD_WriteCommand(0x2B);    																																		//Page address Set(X=0..239)    
	LCD_WriteData((char)(Xpos >> 8) & 0xFF); 																											//Strart address(H) 
  LCD_WriteData((char)(Xpos & 0xFF));  																													//Strart address(L) 		
	
	LCD_WriteCommand(0x2A);    																																		//Column Set(Y=0..319)
  LCD_WriteData((char)(Ypos >> 8) & 0xFF);  																										//Strart address(H)
  LCD_WriteData((char)(Ypos & 0xFF));  																													//Strart address(L)	
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0;
  uint16_t Xaddress = 0;
   
  Xaddress = Xpos;
  
  LCD_SetCursor(Xaddress, Ypos);
  
  for(index = 0; index < 24; index++)
  {
		LCD_WriteCommand(0x2C);
    for(i = 0; i < 16; i++)
    {
      if((c[index] & (1 << i)) == 0x00)
      {
        LCD_WriteData16(BackColor);
      }
      else
      {
        LCD_WriteData16(TextColor);
      }
    }   
    Xaddress++;
    LCD_SetCursor(Xaddress, Ypos);
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint8_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_DrawChar(Line, Column, &ASCII_Table[Ascii * 24]);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint8_t Line, uint8_t *ptr)
{
  uint32_t i = 0;
	uint16_t refcolumn = 0;
	
  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 20))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
		
    /* Decrement the column position by 16 */
		refcolumn += 16;
		
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief  Displays a line.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: line length.
  * @param  Direction: line direction.
  *   This parameter can be one of the following values: Vertical or Horizontal.
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
	uint32_t i = 0;
    
  if(Direction == Horizontal)
  { 
		LCD_SetCursor(Xpos, Ypos);
    LCD_WriteCommand(0x2C);
    for(i = 0; i < Length; i++)
    {
      LCD_WriteData16(TextColor);
    }   
  }
  else
  {		
    for(i = 0; i < Length; i++)
    {
			LCD_SetCursor(Xpos, Ypos);
			LCD_WriteCommand(0x2C);
      LCD_WriteData16(TextColor);
      Xpos++;      
    }
  }
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
  LCD_DrawLine(Xpos, Ypos, Width, Horizontal);
  LCD_DrawLine((Xpos + Height), Ypos, Width, Horizontal);
  
  LCD_DrawLine(Xpos, Ypos, Height, Vertical);
  LCD_DrawLine(Xpos, (Ypos+Width), Height, Vertical);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
  /* Display On */
  LCD_WriteCommand(0x29); /* 262K color and display ON */ 
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{
  /* Display Off */
  LCD_WriteCommand(0x28);
}

/**
  * @brief  Configures the Touch Sensor interface.
  * @param  None
  * @retval None
  */
void ET_STM32_TCS_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(TCS_GPIO_CLK, ENABLE);

  /* Configure CS in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = TCS_CS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(TCS_GPIO_PORT, &GPIO_InitStructure);  

  /* Configure MOSI in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = TCS_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(TCS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SCK in Output Push-Pull mode */
  GPIO_InitStructure.GPIO_Pin = TCS_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(TCS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure PEN as input floating */
  GPIO_InitStructure.GPIO_Pin = TCS_PEN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(TCS_GPIO_PORT, &GPIO_InitStructure);  

  /* Configure MISO as input floating */
  GPIO_InitStructure.GPIO_Pin = TCS_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(TCS_GPIO_PORT, &GPIO_InitStructure); 
}

/**
  * @brief  Writes to Touch Sensor register.
  * @param  TCS_Data : address of the selected register.
  * @retval None
  */
unsigned char TCS_SPI_Write_Byte(unsigned char DataByte)
{
  unsigned char Bit,result = 0 ;				 	   

  for (Bit = 0; Bit < 8; Bit++)              																											// 8 Bit Write
  {		
    GPIO_WriteBit(TCS_GPIO_PORT, TCS_SCK_PIN, Bit_SET); 																					// Start Clock  High
    
		//--------- write Data ---------
		if((DataByte & 0x80)== 0x80)
		{	
		  GPIO_WriteBit(TCS_GPIO_PORT, TCS_MOSI_PIN, Bit_SET); 																				// Set bit data = 1
		}	
		else
		{	
		  GPIO_WriteBit(TCS_GPIO_PORT, TCS_MOSI_PIN, Bit_RESET);  																		// Clear bit data = 0
		}	
		   	
		GPIO_WriteBit(TCS_GPIO_PORT, TCS_SCK_PIN, Bit_RESET);    																			// Clock low
    
		DataByte <<= 1;	 																																							// Shift Next Bit Data
      
	  //---------- Read Data -------
	  result <<= 1;	
	 											  	  
		if (GPIO_ReadInputDataBit(TCS_GPIO_PORT,TCS_MISO_PIN) == Bit_SET)
    {
		  result |= 0x01;
		}
	}
	return (result);                    
}

/**
  * @brief  Return Touch Screen X position value
  * @param  None
  * @retval X position.
  */
uint16_t TCS_Read_X(void)
{
  int32_t x, xr;

  long tcs_adc_X ,tcs_adc_Y;
  unsigned char buf_data[4]={0,0,0,0};
  
  if (GPIO_ReadInputDataBit(TCS_GPIO_PORT,TCS_PEN_PIN) == Bit_RESET)
  {
    GPIO_WriteBit(TCS_GPIO_PORT, TCS_CS_PIN, Bit_RESET);																					// Enable Touch Interface
	
    buf_data[0] = TCS_SPI_Write_Byte(0xD0);																												// Write Command Measure X-Position 
	  buf_data[0] = TCS_SPI_Write_Byte(0x00);																												// Read ADC data X-Position (7-bit byte High) data: 0ddddddd	(bit) 
	  buf_data[1] = TCS_SPI_Write_Byte(0x90);																												// Write Command Measure Y-Position ; Read ADC data X-Position (5-bit byte Low)  data:ddddd000(bit)
	  buf_data[2] = TCS_SPI_Write_Byte(0x00);																												// Read ADC data Y-Position(7-bit byte High) data: 0ddddddd (bit)
	  buf_data[3] = TCS_SPI_Write_Byte(0x00);																												// Read ADC data Y-Position(5-bit byte Low)  data: ddddd000 (bit)
  
	  tcs_adc_X  = buf_data[0];																																			// Mark ADC Data X-Position 12 bit
	  tcs_adc_X  = tcs_adc_X << 5;
	  tcs_adc_X |= buf_data[1] >> 3;
	  tcs_adc_X  = tcs_adc_X & 0x00000FFF;
	 
	  tcs_adc_Y  = buf_data[2];   																																	// Mark ADC Data Y-Position 12 bit
	  tcs_adc_Y  = tcs_adc_Y << 5;   																																// Shift 7 bit High
	  tcs_adc_Y |= buf_data[3] >> 3;   																															// Shift 5 bit low
	  tcs_adc_Y  = tcs_adc_Y & 0x00000FFF;																													// total ADC data 12 bit
	
	  GPIO_WriteBit(TCS_GPIO_PORT, TCS_CS_PIN, Bit_SET);																						// Disable Touch Interface			
	
    x = 4095-tcs_adc_Y;  																																					// ADC 12 bit :LCD Start Landscape	
  }
  else
  {
    x = 0;
  }
  
  /* first correction */
  xr =  (x * 320) >> 12;
 
  /* second correction */
  xr = ((xr * 32)/29) - 17;
  
  if(xr <= 0)
  {
    xr = 0;
  }

  return (uint16_t)(xr); 
}

/**
  * @brief  Return Touch Screen Y position value
  * @param  None
  * @retval Y position.
  */
uint16_t TCS_Read_Y(void)
{
  int32_t y, yr;

  //long dif_adc_X,dif_adc_Y;

  long tcs_adc_X ,tcs_adc_Y;
  unsigned char buf_data[4]={0,0,0,0};

  if (GPIO_ReadInputDataBit(TCS_GPIO_PORT,TCS_PEN_PIN) == Bit_RESET)
  {    
    GPIO_WriteBit(TCS_GPIO_PORT, TCS_CS_PIN, Bit_RESET);																					// Enable Touch Interface
	
    buf_data[0] = TCS_SPI_Write_Byte(0xD0);																												// Write Command Measure X-Position 
	  buf_data[0] = TCS_SPI_Write_Byte(0x00);																												// Read ADC data X-Position (7-bit byte High) data: 0ddddddd	(bit) 
	  buf_data[1] = TCS_SPI_Write_Byte(0x90);																												// Write Command Measure Y-Position ; Read ADC data X-Position (5-bit byte Low)  data:ddddd000(bit)
	  buf_data[2] = TCS_SPI_Write_Byte(0x00);																												// Read ADC data Y-Position(7-bit byte High) data: 0ddddddd (bit)
	  buf_data[3] = TCS_SPI_Write_Byte(0x00);																												// Read ADC data Y-Position(5-bit byte Low)  data: ddddd000 (bit)

	  tcs_adc_X  = buf_data[0];																																			// Mark ADC Data X-Position 12 bit
	  tcs_adc_X  = tcs_adc_X << 5;
	  tcs_adc_X |= buf_data[1] >> 3;
	  tcs_adc_X  = tcs_adc_X & 0x00000FFF;
	 
	  tcs_adc_Y  = buf_data[2];   																																	// Mark ADC Data Y-Position 12 bit
	  tcs_adc_Y  = tcs_adc_Y << 5;   																																// Shift 7 bit High
	  tcs_adc_Y |= buf_data[3] >> 3;   																															// Shift 5 bit low
	  tcs_adc_Y  = tcs_adc_Y & 0x00000FFF;																													// total ADC data 12 bit
	
	  GPIO_WriteBit(TCS_GPIO_PORT, TCS_CS_PIN, Bit_SET);																						// Disable Touch Interface				

    y = tcs_adc_X;	 																																							// ADC 12 bit :LCD Start Landscape	
  }
  else
  {
    y = 0;
  }
  
  yr= (y * 240) >> 12;
  yr = ((yr * 240) / 217) - 12;
  
  if(yr <= 0)
  {
    yr = 0;
  }

  return (uint16_t)(yr); 
}

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
