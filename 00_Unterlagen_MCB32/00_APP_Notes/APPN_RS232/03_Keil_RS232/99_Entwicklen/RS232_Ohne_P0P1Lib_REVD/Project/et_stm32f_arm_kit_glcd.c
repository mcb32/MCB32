/**
  ******************************************************************************
  * @file    et_stm32f_arm_kit_glcd.c
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
#include "et_stm32f_arm_kit_glcd.h"
#include "math.h"
#include <stdio.h>

/******************************************************
 *			 Font 7x11(English)				          *
 *											          *
 * -ASCII fonts from 0x20 ~ 0x7F(DEC 32 ~ 126)     	  *
 *  composed of following characters 			      *
 *                                                    * 
 * -XSize : 7  pixcels							      *
 * -YSize : 11 pixcels						       	  *
 *											       	  *
 *   ....... <- b0, Blank, Not used				      *
 *   ....... <- b1, Blank, Not used				      *
 *   ....... <- b2, Blank, Not used				      *
 *   ....... <- b3, Blank, Not used				      *
 *   ....... <- b4, Blank, Not used				      *
 *   ..End.. <- b5(2nd byte)					      *
 *   ..OO... <- b6(2nd byte)					      *
 *   ..OO... <- b7(2nd byte)					      *
 *   ..OO... <- b0(1st byte)					      *
 *   ..OO... <- b1(1st byte)					      *
 *   ..OO... <- b2(1st byte)					      *
 *   ..OO... <- b3(1st byte)					      *
 *   ..OO... <- b4(1st byte)				 	      *
 *   ..OO... <- b5(1st byte)					      *
 *   ..OO... <- b6(1st byte)					      *
 *   .Start..<- b7(1st byte)					      *
 *												      *
 ******************************************************/
//7x11 ascii font, each character takes 14 bytes, total 1330 bytes, draw from left bottom to left top
const char ascii_7x11[95][14] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f}, //0x20 space
  {0x00, 0x00, 0x07, 0xc0, 0xdf, 0xe0, 0xdf, 0xe0, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x1f}, //0x21 !
  {0x00, 0x00, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x1f}, //0x22 "
  {0x00, 0x00, 0x1b, 0x00, 0x7f, 0xc0, 0x1b, 0x00, 0x7f, 0xc0, 0x1b, 0x00, 0x00, 0x1f}, //0x23 #
  {0x13, 0x00, 0x27, 0x80, 0x24, 0x80, 0x7f, 0xc0, 0x24, 0x80, 0x38, 0x80, 0x19, 0x1f}, //0x24 $
  {0x30, 0x80, 0x19, 0x40, 0x2d, 0x40, 0x56, 0x80, 0x53, 0x00, 0x21, 0x80, 0x00, 0x1f}, //0x25 %
  {0x38, 0x00, 0x7d, 0x80, 0x47, 0xc0, 0x5e, 0x40, 0x7b, 0xc0, 0x21, 0x80, 0x58, 0x1f}, //0x26 &
  {0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f}, //0x27 '
  {0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0xc0, 0x60, 0x80, 0x20, 0x00, 0x1f}, //0x28 (
  {0x00, 0x00, 0x80, 0x20, 0xc0, 0x60, 0x7f, 0xc0, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x1f}, //0x29 )
  {0x00, 0x00, 0x24, 0x80, 0x3f, 0x80, 0x0e, 0x00, 0x3f, 0x80, 0x24, 0x80, 0x00, 0x1f}, //0x2a *
  {0x04, 0x00, 0x04, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x1f}, //0x2b +
  {0x00, 0x00, 0x80, 0x00, 0xe0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f}, //0x2c ,
  {0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x1f}, //0x2d -
  {0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f}, //0x2e .
  {0xc0, 0x00, 0xf0, 0x00, 0x3c, 0x00, 0x0f, 0x00, 0x03, 0xc0, 0x00, 0xe0, 0x00, 0x3f}, //0x2f /
  {0x7f, 0xc0, 0xff, 0xe0, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0xff, 0xe0, 0x7f, 0xdf}, //0x30 0
  {0x00, 0x00, 0x80, 0x80, 0x80, 0xc0, 0xff, 0xe0, 0xff, 0xe0, 0x80, 0x00, 0x00, 0x1f}, //0x31 1
  {0xe0, 0xc0, 0xf0, 0xe0, 0x98, 0x20, 0x8c, 0x20, 0x84, 0x20, 0x87, 0xe0, 0x83, 0xdf}, //0x32 2
  {0x60, 0xc0, 0xe0, 0xe0, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0xff, 0xe0, 0x7b, 0xdf}, //0x33 3
  {0x38, 0x00, 0x3e, 0x00, 0x27, 0x80, 0x21, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0x20, 0x1f}, //0x34 4
  {0x83, 0xe0, 0x83, 0xe0, 0x82, 0x20, 0x82, 0x20, 0xc2, 0x20, 0x7e, 0x20, 0x3c, 0x3f}, //0x35 5
  {0x7f, 0x00, 0xff, 0xc0, 0x82, 0x60, 0x82, 0x20, 0x82, 0x20, 0xfe, 0x20, 0x7c, 0x3f}, //0x36 6
  {0x00, 0x20, 0x00, 0x20, 0xe0, 0x20, 0xfc, 0x20, 0x1f, 0x20, 0x03, 0xe0, 0x00, 0xff}, //0x37 7
  {0x7b, 0xc0, 0xff, 0xe0, 0x86, 0x20, 0x86, 0x20, 0x8c, 0x20, 0xff, 0xe0, 0x7b, 0xdf}, //0x38 8
  {0x03, 0xc0, 0x87, 0xe0, 0x84, 0x20, 0xc4, 0x20, 0x64, 0x20, 0x3f, 0xe0, 0x1f, 0xdf}, //0x39 9
  {0x00, 0x00, 0x00, 0x00, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x00, 0x00, 0x00, 0x1f}, //0x3a :
  {0x00, 0x00, 0x00, 0x00, 0xb1, 0x80, 0xf1, 0x80, 0x71, 0x80, 0x00, 0x00, 0x00, 0x1f}, //0x3b ;
  {0x04, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x60, 0xc0, 0xc0, 0x60, 0x80, 0x3f}, //0x3c <
  {0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x1f}, //0x3d =
  {0x80, 0x20, 0xc0, 0x60, 0x60, 0xc0, 0x31, 0x80, 0x1b, 0x00, 0x0e, 0x00, 0x04, 0x1f}, //0x3e >
  {0x01, 0xc0, 0x01, 0xe0, 0x00, 0x20, 0xd8, 0x20, 0xdc, 0x20, 0x07, 0xe0, 0x03, 0xdf}, //0x3f ?
  {0x7f, 0x40, 0x83, 0x20, 0x83, 0x20, 0xff, 0x20, 0x80, 0x60, 0xff, 0xe0, 0x7f, 0xdf}, //0x40 @
  {0xff, 0x00, 0xff, 0x80, 0x08, 0xc0, 0x08, 0x60, 0x08, 0xc0, 0xff, 0x80, 0xff, 0x1f}, //0x41 A
  {0xff, 0xe0, 0xff, 0xe0, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0xff, 0xe0, 0x7b, 0xdf}, //0x42 B
  {0x7f, 0xc0, 0xff, 0xe0, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0xe0, 0xe0, 0x60, 0xdf}, //0x43 C
  {0xff, 0xe0, 0xff, 0xe0, 0x80, 0x20, 0x80, 0x20, 0xc0, 0x60, 0x7f, 0xc0, 0x3f, 0x9f}, //0x44 D
  {0xff, 0xe0, 0xff, 0xe0, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x84, 0x20, 0x80, 0x3f}, //0x45 E
  {0xff, 0xe0, 0xff, 0xe0, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x00, 0x3f}, //0x46 F
  {0x7f, 0xc0, 0xff, 0xe0, 0x80, 0x20, 0x80, 0x20, 0x88, 0x20, 0xf8, 0xe0, 0xf8, 0xdf}, //0x47 G
  {0xff, 0xe0, 0xff, 0xe0, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0xff, 0xe0, 0xff, 0xff}, //0x48 H
  {0x00, 0x00, 0x80, 0x20, 0xff, 0xe0, 0xff, 0xe0, 0x80, 0x20, 0x00, 0x00, 0x00, 0x1f}, //0x49 I
  {0x60, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xff, 0xe0, 0x7f, 0xff}, //0x4a J
  {0xff, 0xe0, 0xff, 0xe0, 0x04, 0x00, 0x1f, 0x00, 0x7b, 0xc0, 0xe0, 0xe0, 0x80, 0x3f}, //0x4b K
  {0xff, 0xe0, 0xff, 0xe0, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x1f}, //0x4c L
  {0xff, 0xe0, 0xff, 0xe0, 0x00, 0x80, 0x07, 0x00, 0x00, 0x80, 0xff, 0xe0, 0xff, 0xff}, //0x4d M
  {0xff, 0xe0, 0xff, 0xe0, 0x03, 0x80, 0x0e, 0x00, 0x38, 0x00, 0xff, 0xe0, 0xff, 0xff}, //0x4e N
  {0x7f, 0xc0, 0xff, 0xe0, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0xff, 0xe0, 0x7f, 0xdf}, //0x4f O
  {0xff, 0xe0, 0xff, 0xe0, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x07, 0xe0, 0x03, 0xdf}, //0x50 P
  {0x7f, 0xc0, 0xff, 0xe0, 0x90, 0x20, 0xb0, 0x20, 0xe0, 0x20, 0x7f, 0xe0, 0xff, 0xdf}, //0x51 Q
  {0xff, 0xe0, 0xff, 0xe0, 0x04, 0x20, 0x04, 0x20, 0x0c, 0x20, 0xff, 0xe0, 0xf3, 0xdf}, //0x52 R
  {0x43, 0xc0, 0xc7, 0xe0, 0x86, 0x20, 0x84, 0x20, 0x8c, 0x20, 0xfc, 0x60, 0x78, 0x5f}, //0x53 S
  {0x00, 0x20, 0x00, 0x20, 0xff, 0xe0, 0xff, 0xe0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x1f}, //0x54 T
  {0x7f, 0xe0, 0xff, 0xe0, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xff, 0xe0, 0x7f, 0xff}, //0x55 U
  {0x1f, 0xe0, 0x3f, 0xe0, 0x60, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x3f, 0xe0, 0x1f, 0xff}, //0x56 V
  {0x1f, 0xe0, 0xff, 0xe0, 0xe0, 0x00, 0x1c, 0x00, 0xe0, 0x00, 0xff, 0xe0, 0x1f, 0xff}, //0x57 W
  {0xc0, 0x60, 0xf1, 0xe0, 0x3f, 0x80, 0x0e, 0x00, 0x3f, 0x80, 0xf1, 0xe0, 0xc0, 0x7f}, //0x58 X
  {0x03, 0xe0, 0x07, 0xe0, 0xfc, 0x00, 0xfc, 0x00, 0x07, 0xe0, 0x03, 0xe0, 0x00, 0x1f}, //0x59 Y
  {0xc0, 0x20, 0xf0, 0x20, 0xbc, 0x20, 0x8f, 0x20, 0x83, 0xa0, 0x80, 0xe0, 0x80, 0x7f}, //0x5a Z
  {0x00, 0x00, 0xff, 0xe0, 0xff, 0xe0, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x00, 0x1f}, //0x5b [
  {0x00, 0x20, 0x00, 0xe0, 0x03, 0xc0, 0x0f, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0xc0, 0x1f}, /*0x5c \*/
  {0x00, 0x00, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0xff, 0xe0, 0xff, 0xe0, 0x00, 0x1f}, //0x5d ]
  {0x00, 0x80, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x1f}, //0x5e ^
  {0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x1f}, //0x5f _
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x60, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x1f}, //0x60 `
  {0x70, 0x00, 0xf9, 0x00, 0x89, 0x00, 0x89, 0x00, 0x89, 0x00, 0xff, 0x00, 0xfe, 0x1f}, //0x61 a
  {0xff, 0xe0, 0xff, 0xe0, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0xff, 0x00, 0x7e, 0x1f}, //0x62 b
  {0x7e, 0x00, 0xff, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0xc3, 0x00, 0x42, 0x1f}, //0x63 c
  {0x7e, 0x00, 0xff, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0xff, 0xe0, 0xff, 0xff}, //0x64 d
  {0x7e, 0x00, 0xff, 0x00, 0x91, 0x00, 0x91, 0x00, 0x91, 0x00, 0x9f, 0x00, 0x5e, 0x1f}, //0x65 e
  {0x04, 0x00, 0xff, 0xc0, 0xff, 0xe0, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x00, 0x1f}, //0x66 f
  {0x9e, 0x00, 0xbf, 0x00, 0xa1, 0x00, 0xa1, 0x00, 0xa1, 0x00, 0xff, 0x00, 0x7f, 0x1f}, //0x67 g
  {0xff, 0xe0, 0xff, 0xe0, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0xff, 0x00, 0xfe, 0x1f}, //0x68 h
  {0x81, 0x00, 0x81, 0x00, 0xff, 0x60, 0xff, 0x60, 0x80, 0x00, 0x80, 0x00, 0x00, 0x1f}, //0x69 i
  {0x80, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0xff, 0x60, 0x7f, 0x60, 0x00, 0x1f}, //0x6a j
  {0xff, 0xe0, 0xff, 0xe0, 0x18, 0x00, 0x3c, 0x00, 0x66, 0x00, 0xc3, 0x00, 0xc1, 0x1f}, //0x6b k
  {0x80, 0x20, 0x80, 0x20, 0xff, 0xe0, 0xff, 0xe0, 0x80, 0x00, 0x80, 0x00, 0x00, 0x1f}, //0x6c l
  {0xff, 0x00, 0xff, 0x00, 0x01, 0x00, 0x3f, 0x00, 0x01, 0x00, 0xff, 0x00, 0xfe, 0x1f}, //0x6d m
  {0xff, 0x00, 0xff, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0xff, 0x00, 0xfe, 0x1f}, //0x6e n
  {0x7e, 0x00, 0xff, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0xff, 0x00, 0x7e, 0x1f}, //0x6f o
  {0xff, 0x00, 0xff, 0x00, 0x21, 0x00, 0x21, 0x00, 0x21, 0x00, 0x3f, 0x00, 0x1e, 0x1f}, //0x70 p
  {0x1e, 0x00, 0x3f, 0x00, 0x21, 0x00, 0x21, 0x00, 0x21, 0x00, 0xff, 0x00, 0xfe, 0x1f}, //0x71 q
  {0x01, 0x00, 0xff, 0x00, 0xff, 0x00, 0x04, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x1f}, //0x72 r
  {0x8e, 0x00, 0x9f, 0x00, 0x99, 0x00, 0x99, 0x00, 0x99, 0x00, 0xf9, 0x00, 0x71, 0x1f}, //0x73 s
  {0x01, 0x00, 0x01, 0x00, 0x7f, 0xc0, 0xff, 0xc0, 0x81, 0x00, 0x81, 0x00, 0x81, 0x1f}, //0x74 t
  {0x7f, 0x00, 0xff, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xff, 0x00, 0xff, 0x1f}, //0x75 u
  {0x1f, 0x00, 0x3f, 0x00, 0x60, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x3f, 0x00, 0x1f, 0x1f}, //0x76 v
  {0x3f, 0x00, 0xff, 0x00, 0xc0, 0x00, 0x3c, 0x00, 0xc0, 0x00, 0xff, 0x00, 0x3f, 0x1f}, //0x77 w
  {0xe3, 0x00, 0xf7, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0xf7, 0x00, 0xe3, 0x1f}, //0x78 x
  {0x8f, 0x00, 0x9f, 0x00, 0x90, 0x00, 0x90, 0x00, 0x90, 0x00, 0xff, 0x00, 0x7f, 0x1f}, //0x79 y
  {0xc1, 0x00, 0xe1, 0x00, 0xb1, 0x00, 0x99, 0x00, 0x8d, 0x00, 0x87, 0x00, 0x83, 0x1f}, //0x7a z
  {0x00, 0x00, 0x04, 0x00, 0x7f, 0xc0, 0xe0, 0xe0, 0x80, 0x20, 0x80, 0x00, 0x00, 0x1f}, //0x7b {
  {0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f}, //0x7c |
  {0x00, 0x00, 0x80, 0x20, 0xe0, 0xe0, 0x7f, 0xc0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x5f}, //0x7d }
  {0x00, 0x60, 0x00, 0x20, 0x00, 0x60, 0x00, 0xc0, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x5f}  //0x7e ~
};

long dif_adc_X,dif_adc_Y;
long buf_adc_X[100],buf_adc_Y[100];
long ave_adc_X,ave_adc_Y;
long tcs_ave_X[3];																							// Keep Value adc Touch Point X
long tcs_ave_Y[3];																							// Keep Value adc Touch Point Y
long dis_XD_Hor[3] = {32,287,160}; 															// Value refer Point X at 10% of Display(X=320:0-319) 3 Position
long dis_YD_Hor[3] = {215,120,24}; 															// Value refer Point Y at 10% of Display(Y=240:0-239) 3 Position
long dis_XD_Ver[3] = {24,215,120}; 															// Value refer Point X at 10% of Display(X=240:0-239) 3 Position
long dis_YD_Ver[3] = {32,160,287}; 															// Value refer Point Y at 10% of Display(Y=320:0-319) 3 Position
extern long divider,An,Bn,Cn,Dn,En,Fn,X,Y;											// Valiable for keep coefficient Calibrat and position touch Screen
extern char num1,num2,num3,num4;
extern void delay_ms(__IO uint32_t nTime);	 										// Delay mS

/******************************************/
/* Config Hardware Interface GLCD & Touch */
/* (LCD Driver SPFD5408A + Touch ADS7846) */
/* GLCD(ET-TFT240320P) SPI Mode Interface */
/* -> P0.6(GPIO:Out)    = CS GLCD         */
/* -> P0.7(SCK1:SSP1)   = SCL	GLCD	  */
/* -> P0.8(MISO1:SSP1)  = SDO GLCD		  */
/* -> P0.9(MOSI1:SSP1)  = SDI	GLCD	  */
/* -> P4.28(GPIO:Out)   = BL GLCD		  */
/* Touch GLCD : ADS7846 SPI Interface     */
/* -> P1.20(SCK0:SSP0)  = DCLK ADS7846    */
/* -> P1.21(GPIO:Out)   = CS ADS7846	  */
/* -> P1.23(MISO0:SSP0) = DOUT ADS7846	  */
/* -> P1.24(MOSI0:SSP0) = DIN ADS7846	  */
/* -> P0.21(GPIO:In)    = PENIRQ ADS7846  */
/******************************************/
void Initial_GLCD_Hardware()
{
  SPI_InitTypeDef    SPI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  //Start of GLCD GPIO Initial  
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
	//End of GLCD GPIO Initial  
	
	//Start of GLCD SPI Initial
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
  //End of GLCD SPI Initial
	
	//Start of Touch Screen Initial
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
	//End of Touch Screen Initial
}

/******************************************************/
/* Write Address Command(Index Reg.)(Use Device ID=0) */
/******************************************************/
void GLCD_Write_Command(unsigned char GLCD_Command)
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
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, Bit_SET);				
}
 
/***************************************/
/* Write data to LCD (Use Device ID=0) */
/***************************************/
void GLCD_Write_Data(unsigned char GLCD_DataByte)
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
  SPI_I2S_SendData(LCD_SPI, GLCD_DataByte);																												// Byte Data
  while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);
	//
	GPIO_WriteBit(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, Bit_SET);		
}

/***************************************/
/* Write data to LCD (Use Device ID=0) */
/***************************************/
void GLCD_Write_Data16(unsigned int GLCD_DataWord)
{	
  GLCD_Write_Data(GLCD_DataWord >> 8);      																											// Data 8 bit High 
  GLCD_Write_Data(GLCD_DataWord);           																											// Data 8 bit Low 
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void GLCD_SetX(uint16_t Xpos)
{  
	GLCD_Write_Command(0x2A);    																																		//Column Set(X)	
	GLCD_Write_Data((char)(Xpos >> 8) & 0xFF); 																											//Strart address(H) 
  GLCD_Write_Data((char)(Xpos & 0xFF));  																													//Strart address(L) 			
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void GLCD_SetY(uint16_t Ypos)
{  
	GLCD_Write_Command(0x2B);    																																		//Page address Set(Y)    
  GLCD_Write_Data((char)(Ypos >> 8) & 0xFF);  																										//Strart address(H)
  GLCD_Write_Data((char)(Ypos & 0xFF));  																													//Strart address(L)	
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void GLCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{  
	GLCD_Write_Command(0x2A);    																																		//Column Set(X)	
	GLCD_Write_Data((char)(Xpos >> 8) & 0xFF); 																											//Strart address(H) 
  GLCD_Write_Data((char)(Xpos & 0xFF));  																													//Strart address(L) 		
	
	GLCD_Write_Command(0x2B);    																																		//Page address Set(Y)    
  GLCD_Write_Data((char)(Ypos >> 8) & 0xFF);  																										//Strart address(H)
  GLCD_Write_Data((char)(Ypos & 0xFF));  																													//Strart address(L)	
}

/********************************************/
/* Function Check Status Press Touch Screen */
/* Return Value : 0 = Not Touched screen    */
/*                1 = Touched screen        */
/********************************************/
char Get_Status_Touch(void)
{		  																	// Press Touch screen
  if (GPIO_ReadInputDataBit(TCS_GPIO_PORT,TCS_PEN_PIN) == Bit_SET)
  {
    return 0 ;			  																																						// Not Press
  }
  else
  {
    return 1 ;	
  }
}

/***************************************************************************/
/*    Function SPI Write and Read Data 1 Byte from Touch Screen ADS7846    */
/***************************************************************************/
/* Parameter    : DataByte = data or command control ADS7846 (1Byte)       */ 
/* Return Value : Return value adc from touched times 1 byte Pass Function */         
/***************************************************************************/
unsigned char TCS_SPI_Write(unsigned char DataByte)			  	
{
  unsigned char Bit,result = 0 ;				 	   

  for (Bit = 0; Bit < 8; Bit++)              																											// 8 Bit Write
  {		
    GPIO_WriteBit(TCS_GPIO_PORT, TCS_SCK_PIN, Bit_SET); 																					// Clock  High(Prepared Write Data)

	// Write Data to MOSI Pin
	if((DataByte & 0x80)== 0x80)  																																	// MSB First
	{
	  GPIO_WriteBit(TCS_GPIO_PORT, TCS_MOSI_PIN, Bit_SET); 																					// Set Bit Data(MOSI) = 1
	}
	else
	{
	  GPIO_WriteBit(TCS_GPIO_PORT, TCS_MOSI_PIN, Bit_RESET);  																			// Reset Bit Data(MOSI) = 0
	}														  	
		   	
	GPIO_WriteBit(TCS_GPIO_PORT, TCS_SCK_PIN, Bit_RESET);    																				// Clock Low(Strobe Data & Read)
	DataByte <<= 1;	 																																								// Shift Next Bit Data
      
	// Read Data From MISO Pin
	result <<= 1;		 											  	  
	if (GPIO_ReadInputDataBit(TCS_GPIO_PORT,TCS_MISO_PIN) == Bit_SET)
    {
	  result |= 0x01;
	}
  }
  return (result);
}

/************************************************************************************/
/* Function Calculate average of measurements,by cut value max and min measurement. */
/************************************************************************************/
/* Parameter    : num = number times of measurement adc from touch screen           */
/* Return value : ave_adc_X = Keep result average adc X-Position			        */
/*                ave_adc_Y = Keep result average adc Y-Position                    */     
/************************************************************************************/
void TCS_Average_X_Y(unsigned char num)				
{
  unsigned char i;
  unsigned long sum_x, sum_y;
  unsigned long min_x, min_y, max_x, max_y;

  sum_x=0;sum_y=0;min_x=0xffff;min_y=0xffff,max_x=0;max_y=0;
  for(i=0;i<num;i++)
  {
    sum_x += buf_adc_X[i];
	sum_y += buf_adc_Y[i];

	if(buf_adc_X[i]< min_x)		                        																							// search for minimum X value
	{
	  min_x = buf_adc_X[i];
    }
	else if (buf_adc_X[i]> max_x)	                    																							// search for maximum X value
	{
	  max_x = buf_adc_X[i];
	}

    if(buf_adc_Y[i]< min_y)	  	                        																					// search for minimum Y value
	{
	  min_y = buf_adc_Y[i];
	}
	else if(buf_adc_Y[i]> max_y)	  	                																							// search for maximum Y value
	{
	  max_y = buf_adc_Y[i];
	}
  }
  ave_adc_X = (sum_x - min_x - max_x ) / (num-2); 																								// update x average value
  ave_adc_Y = (sum_y - min_y - max_y ) / (num-2);	    																						// update y average value
}

/******************************************************/
/*   Function Set  Background color or Clear Screen   */
/******************************************************/
/* Parameter : bg_color =  BackGround color of Screen */
/******************************************************/
void bg_color(unsigned int bg_color)
{	
  long cnt;
	
	GLCD_SetCursor(0,0);  
  GLCD_Write_Command(0x2C);    																																		// Cmm Write Data to LCD
  for(cnt=0; cnt<((long)240*320); cnt++)
  {
    GLCD_Write_Data16(bg_color) ;    																															// Back-Ground Color Black
  } 
}

/*****************************************/
/* Convert Dec to Ascii 3 digit(000-999) */
/*****************************************/        
void dec_to_ascii(unsigned int num)
{
  num1 = (num/100) + 0x30;																																				//Digit-100
  num  =  num%100;
  num2 = (num/10) + 0x30;																																					//Digit-10
  num3 =  (num%10) + 0x30;																																				//Digit-1	 
}
/*****************************************/
/* Convert Dec to Ascii 4 digit(0000-9999) */
/*****************************************/        
void dec_to_ascii_4digit(long num)
{
  num1 = (num/1000) + 0x30;		//Digit-1000
  num  =  num%1000;
  num2 = (num/100) + 0x30;		//Digit-100
	num  =  num%100;
  num3 =  (num/10) + 0x30;		//Digit-10
  num4 =	(num%10) + 0x30;		//Digit-1	 
}

/********************************/
/* Initial GLCD : ET-TFT240320P */
/*      (Driver SPFD5408A)      */
/********************************/
void Initial_GLCD_Hor(void)
{
  unsigned long cnt;
	
  Initial_GLCD_Hardware();
	GLCD_Write_Command(0x01);   																																		//Software reset
  delay_ms(5);
  GLCD_Write_Command(0x28);    																																		//display off
    
  GLCD_Write_Command(0xCF);
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x83);
  GLCD_Write_Data(0x30);
    
  GLCD_Write_Command(0xED);
  GLCD_Write_Data(0x64);
  GLCD_Write_Data(0x03);
  GLCD_Write_Data(0x12);
  GLCD_Write_Data(0x81);
	
  GLCD_Write_Command(0xE8);
  GLCD_Write_Data(0x85);
  GLCD_Write_Data(0x01);
  GLCD_Write_Data(0x79);

  GLCD_Write_Command(0xCB);
  GLCD_Write_Data(0x39);
  GLCD_Write_Data(0x2C);
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x34);
  GLCD_Write_Data(0x02);

  GLCD_Write_Command(0xF7);
  GLCD_Write_Data(0x20);

  GLCD_Write_Command(0xEA);
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x00);

  //---------------- Power Control ------------------- 
  GLCD_Write_Command(0xC0);    																																		//Power Control 1
  GLCD_Write_Data(0x26);
   
  GLCD_Write_Command(0xC1);    																																		//Power Control2
  GLCD_Write_Data(0x11);

  //----------------- VCOM --------------------
  GLCD_Write_Command(0xC5);    																																		//VCOM Control 1
  GLCD_Write_Data(0x35);
  GLCD_Write_Data(0x3E);

  GLCD_Write_Command(0xC7);    																																		//VCOM Control 2
  GLCD_Write_Data(0xBE);

  //----------------- Memory access Control ---------------
  GLCD_Write_Command(0x36);    																																		//Memory access Control 
  GLCD_Write_Data(0x68);    																																			//my,mx=1,mv=1,ml,BGR,mh
   
  GLCD_Write_Command(0x3A);    																																		//Pixel format set
  GLCD_Write_Data(0x55);   																																				//16 bit/Pixel

  //---------------------- Frame Rate ----------------------
  GLCD_Write_Command(0xB1);    																																		//Frame rate
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x1B);    																																			//default

  //---------------------- Gamma -------------------
  GLCD_Write_Command(0xF2);    																																		//3Gamma Function Disable 
  GLCD_Write_Data(0x08);  
   
  GLCD_Write_Command(0x26);    
  GLCD_Write_Data(0x01);    																																			// Gamma Set 4 gamma curve 01/02/04/08
   
  GLCD_Write_Command(0xE0);    																																		//Positive gamma correction    
  GLCD_Write_Data(0x1F);   
  GLCD_Write_Data(0x1A);     
  GLCD_Write_Data(0x18);   
  GLCD_Write_Data(0x0A);     
  GLCD_Write_Data(0x0F);   
  GLCD_Write_Data(0x06);     
  GLCD_Write_Data(0x45);   
  GLCD_Write_Data(0x87);     
  GLCD_Write_Data(0x32);   
  GLCD_Write_Data(0x0A);     
  GLCD_Write_Data(0x07);   
  GLCD_Write_Data(0x02);     
  GLCD_Write_Data(0x07);   
  GLCD_Write_Data(0x05);     
  GLCD_Write_Data(0x00);   
  
  GLCD_Write_Command(0xE1);    																																		//Negamma correction    
  GLCD_Write_Data(0x00);   
  GLCD_Write_Data(0x25);     
  GLCD_Write_Data(0x27);   
  GLCD_Write_Data(0x05);     
  GLCD_Write_Data(0x10);   
  GLCD_Write_Data(0x09);     
  GLCD_Write_Data(0x3A);   
  GLCD_Write_Data(0x78);     
  GLCD_Write_Data(0x4D);   
  GLCD_Write_Data(0x05);     
  GLCD_Write_Data(0x18);   
  GLCD_Write_Data(0x0D);     
  GLCD_Write_Data(0x38);   
  GLCD_Write_Data(0x3A);     
  GLCD_Write_Data(0x1F);   

  //------------------ SET DDRAM Address ------------------
  GLCD_Write_Command(0x2A);    																																		//Column Set (X)   
  GLCD_Write_Data(0x00);    																																			//Strart address(H) = 0x00
  GLCD_Write_Data(0x00);    																																			//Strart address(L) = 0x00(0)
  GLCD_Write_Data(0x01);    																																			//End address(H) = 0x01
  GLCD_Write_Data(0x3F);    																																			//End address(L) = 0x3F(319)
  
  GLCD_Write_Command(0x2B);    																																		//Page address Set(Y)    
  GLCD_Write_Data(0x00);    																																			//Strart address(H) = 0x0000
  GLCD_Write_Data(0x00);    																																			//Strart address(L) = 0x0000(0)
  GLCD_Write_Data(0x00);    																																			//End address(H) = 0x00
  GLCD_Write_Data(0xEF);    																																			//End address(L) = 0xEF(239)
 
  GLCD_Write_Command(0xB7);    																																		//Entry mode set    
  GLCD_Write_Data(0x07);    																																			//Normal display

  //-------------------- Display -----------------
  GLCD_Write_Command(0xB6);    																																		//Display Function Control  
  GLCD_Write_Data(0x0A);   
  GLCD_Write_Data(0x82);     
  GLCD_Write_Data(0x27);   
  GLCD_Write_Data(0x00);     

  GLCD_Write_Command(0x11);    																																		//Sleep Out       
  delay_ms(100);

  //------- Clear the contents of the display buffer. --------
  GLCD_Write_Command(0x2C);    																																		//Memmory Write  
  for(cnt = 0; cnt<((long)240*320); cnt++)
	{
    GLCD_Write_Data16(BLACK);    																																	// Back-Ground Color Black
	}	
    
  GLCD_Write_Command(0x29);    																																		//Display On       
  delay_ms(100);
  
  GLCD_BL_ON(); 																																									//Back Light ON     
  delay_ms(20);    																																								//Delay for 20ms, which is equivalent to two frames.
}

/************************************************************/
/*     Function Calculate Matrix Find value Coeffcient      */
/************************************************************/
/* Parameter    : None                                      */
/* Return Value : Keep value Coeffcient for use calculate   */                           
/*                X-Y-Position in variable divider,An,Bn,Cn */                   
/*                Dn,En,Fn                                  */
/************************************************************/
void TCS_Set_Matrix_Hor()
{
  divider = ((tcs_ave_X[0]-tcs_ave_X[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
	          ((tcs_ave_X[1]-tcs_ave_X[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

  An = ((dis_XD_Hor[0]-dis_XD_Hor[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
	  	 ((dis_XD_Hor[1]-dis_XD_Hor[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

	Bn = ((tcs_ave_X[0]-tcs_ave_X[2])*(dis_XD_Hor[1]-dis_XD_Hor[2]))-
			 ((dis_XD_Hor[0]-dis_XD_Hor[2])*(tcs_ave_X[1]-tcs_ave_X[2]));
			
	Cn = (tcs_ave_X[2]*dis_XD_Hor[1]-tcs_ave_X[1]*dis_XD_Hor[2])*tcs_ave_Y[0]+
	     (tcs_ave_X[0]*dis_XD_Hor[2]-tcs_ave_X[2]*dis_XD_Hor[0])*tcs_ave_Y[1]+
			 (tcs_ave_X[1]*dis_XD_Hor[0]-tcs_ave_X[0]*dis_XD_Hor[1])*tcs_ave_Y[2];
	    
	Dn = ((dis_YD_Hor[0]-dis_YD_Hor[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
			 ((dis_YD_Hor[1]-dis_YD_Hor[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

	En = ((tcs_ave_X[0]-tcs_ave_X[2])*(dis_YD_Hor[1]-dis_YD_Hor[2]))-
			 ((dis_YD_Hor[0]-dis_YD_Hor[2])*(tcs_ave_X[1]-tcs_ave_X[2]));

  Fn = (tcs_ave_X[2]*dis_YD_Hor[1]-tcs_ave_X[1]*dis_YD_Hor[2])*tcs_ave_Y[0]+
	     (tcs_ave_X[0]*dis_YD_Hor[2]-tcs_ave_X[2]*dis_YD_Hor[0])*tcs_ave_Y[1]+
			 (tcs_ave_X[1]*dis_YD_Hor[0]-tcs_ave_X[0]*dis_YD_Hor[1])*tcs_ave_Y[2];
}

/************************************************************/
/*              Function Calibrate Touch Green              */
/* Touch at Point + on Screen 3 Point(3 step) for Caribrate */
/* Parameter    : None                                      */
/* Return value : None                                      */
/*              											*/
/************************************************************/
void Touch_Calibrate_Hor(void)	
{									
  char n,touch_mark_point=0;

  plot_mark_Hor(25,215,32,222,BRIGHT_RED);                                  											// Plot Signe + for Position Calibrate Touch Point1
  lcd_printStr_Hor("<Touch Screen Calibratetion>",45,164,BRIGHT_BLUE,BLACK);   										// Plot Text	Color Bright blue
  lcd_printStr_Hor("        STEP 1 of 3         ",45,144,BRIGHT_BLUE,BLACK);  
  lcd_printStr_Hor(" Click on center of '+' icon",45,104,WHITE,BLACK);        										// Plot Text	Color Black
  lcd_printStr_Hor(" for Touch screen calibrate ",45,84,WHITE,BLACK);       

  while(touch_mark_point != 3)	        																													// Number Mark Point Calibrate = 3 Point  
  {
    if(Get_Status_Touch())                 																												// return (1) if touch screen is detected.
    {	  
	  for(n=0;n<50;n++)
	  {
	    TCS_SPI_Read_Hor();            																															// Read portrait(ADC X,Y) by touch value from ADS7846 and update dif_adc_X & dif_adc_Y
		buf_adc_X[n] = dif_adc_X;  																																		// General buffer for averaging and filter touch screen adc read outs.
	    buf_adc_Y[n] = dif_adc_Y;  																																	// General buffer for averaging and filter touch screen adc read outs.
	  }
	  TCS_Average_X_Y(50);             																															// Take average out of 50 touch measurements, excluding max and min measurement.

	  tcs_ave_X[touch_mark_point] =  ave_adc_X;
	  tcs_ave_Y[touch_mark_point] =  ave_adc_Y;
				 
      touch_mark_point++;            																															// touch_cal_status : 0=xy1 cal, 1=xy2 cal, 2=xy3 cal, 3=calibration success. 0xff=calibration not performed.	

	  while(Get_Status_Touch());        																														// Waite until not Touch screen 
      switch(touch_mark_point)																																		// Select Caribrate Next Step 2 and 3 
	  {
	    case 1: bg_color(BLACK);                                                        						// Clear Screen
				plot_mark_Hor(280,120,287,127,BRIGHT_RED);                                 	 							// Plot Signe + for Position Calibrate Touch Point2

                lcd_printStr_Hor("<Touch Screen Calibratetion>",45,164,BRIGHT_BLUE,BLACK);  		// Plot Text Color Bright blue
                lcd_printStr_Hor("        STEP 2 of 3         ",45,144,BRIGHT_BLUE,BLACK);  
                lcd_printStr_Hor(" Click on center of '+' icon",45,104,WHITE,BLACK);        // Plot Text Color white
                lcd_printStr_Hor(" for Touch screen calibrate ",45,84,WHITE,BLACK);         
		        break;

		case 2:	bg_color(BLACK);                                                         	//Clear Screen
		        plot_mark_Hor(153,24,160,31,BRIGHT_RED);                                    //Plot Signe + for Position Calibrate Touch Point3
						
				lcd_printStr_Hor("<Touch Screen Calibratetion>",45,164,BRIGHT_BLUE,BLACK);  //Plot Text	Color Bright blue
                lcd_printStr_Hor("        STEP 3 of 3         ",45,144,BRIGHT_BLUE,BLACK);   
                lcd_printStr_Hor(" Click on center of '+' icon",45,104,WHITE,BLACK);        //Plot Text	Color white
                lcd_printStr_Hor(" for Touch screen calibrate ",45,84,WHITE,BLACK);          
				break;

		case 3: bg_color(BLACK);                                                         	//Clear Screen

				lcd_printStr_Hor("<Touch Screen Calibratetion>",45,159,BRIGHT_BLUE,BLACK);  //Plot Text	Color Bright blue
                lcd_printStr_Hor("          Succeded!         ",45,119,BRIGHT_GREEN,BLACK);  
                lcd_printStr_Hor("   -Tap Screen for exit-    ",45,79,WHITE,BLACK);          
                       
				while(!Get_Status_Touch());
				delay_ms(100);
				bg_color(BLACK);                                                         	// Clear Screen
	            break;
	  }		 
	}
    delay_ms(100);	
  }															 
  TCS_Set_Matrix_Hor(); 																	// Calculate Coefficient for use find Position Address real on Display
	
  //You can Keep Value data in valiabie devider,An,Bn,Cn,Dn,En,Fn to E2Prom  so that not 
  //come back	 Caribrate.
/*
  printf("\n\r divider = %d",divider) ;
  printf("\n\r      An = %d",An) ;
  printf("\n\r      Bn = %d",Bn) ;
  printf("\n\r      Cn = %d",Cn) ;
  printf("\n\r      Dn = %d",Dn) ;
  printf("\n\r      En = %d",En) ;
  printf("\n\r      Fn = %d",Fn) ; 
*/

}

/*****************************************************************/ 
/* Function Read X-Y-Position ADC Touch Screen-12 bit (ADS7846)  */
/* Parameter    : None                                           */
/* Return Value : dif_adc_X = Keep Result ADC X-Position(12 bit) */
/*                dif_adc_Y = Keep result ADC Y-Position(12 bit) */                      
/*****************************************************************/
void TCS_SPI_Read_Hor(void)
{
  long tcs_adc_X ,tcs_adc_Y;
  unsigned char buf_data[4]={0,0,0,0};

  if(Get_Status_Touch())
  {
    TCS_CS_LOW();																			// Enable Touch Interface

    buf_data[0] = TCS_SPI_Write(0xD0);														// Write Command Measure X-Position 
	buf_data[0] = TCS_SPI_Write(0x00);														// Read ADC data X-Position (7-bit byte High) data: 0ddddddd	(bit) 
	buf_data[1] = TCS_SPI_Write(0x90);														// Write Command Measure Y-Position ; Read ADC data X-Position (5-bit byte Low)  data:ddddd000(bit)
	buf_data[2] = TCS_SPI_Write(0x00);														// Read ADC data Y-Position(7-bit byte High) data: 0ddddddd (bit)
	buf_data[3] = TCS_SPI_Write(0x00);														// Read ADC data Y-Position(5-bit byte Low)  data: ddddd000 (bit)

	tcs_adc_X  = buf_data[0];																// Mark ADC Data X-Position 12 bit
	tcs_adc_X  = tcs_adc_X << 5;
	tcs_adc_X |= buf_data[1] >> 3;
	tcs_adc_X  = tcs_adc_X & 0x00000FFF;
	 
	tcs_adc_Y  = buf_data[2];   															// Mark ADC Data Y-Position 12 bit
	tcs_adc_Y  = tcs_adc_Y << 5;   															// Shift 7 bit High
	tcs_adc_Y |= buf_data[3] >> 3;   														// Shift 5 bit low
	tcs_adc_Y  = tcs_adc_Y & 0x00000FFF;													// total ADC data 12 bit
	
	TCS_CS_HIGH();																			// Disable Touch Interface			

    //Result	 	 
    dif_adc_X = 4095-tcs_adc_Y;  															// ADC 12 bit :LCD Start Landscape
	dif_adc_Y = tcs_adc_X;	
  }
}

/*******************************************************************/
/*          Function Get Position Address real of Display          */
/*******************************************************************/
/* Parameter    : num = Number times for Measure adc from Touched  */ 
/* Return Value : X = Keep value address X-Position		           */
/*                Y = Keep value address Y-Position                */ 
/*******************************************************************/
void TCS_Get_Point_Hor(char num)
{
  char nm; 
	
  nm=0;
  while(Get_Status_Touch() && nm<num)	  													// Measure touch x,y 10 times if the Touch kept pressed,
  {
    TCS_SPI_Read_Hor();                     												// Read value ADC Touch X-Y 
	buf_adc_X[nm]=	dif_adc_X; 																// keep value ADC Touch-X
	buf_adc_Y[nm]=	dif_adc_Y; 																// keep value ADC Touch-Y

 	nm++; 
  }

  if(nm==num)	                      														// if the touch xy successfuly collected,
  {	
    TCS_Average_X_Y(num); 																	// take average out of 10 touch measurements, excluding max and min measurement.
			    
    dif_adc_X = ave_adc_X; 																	// keep average ADC Touch-X
	dif_adc_Y = ave_adc_Y; 																	// keep average ADC Touch-Y
               
	X = ((An*dif_adc_X)+(Bn*dif_adc_Y)+Cn)/divider; 
	Y = ((Dn*dif_adc_X)+(En*dif_adc_Y)+Fn)/divider;
  }
}

/*****************************************************/
/*              Function Plot Dot 1 Pixel            */
/*****************************************************/
/* Parameter : Xadd,Yadd = Position X,Y for Plot Dot */
/*             color = Color of dot                  */
/*****************************************************/
void plot_dot_Hor(unsigned int Xadd,unsigned int Yadd,unsigned int color)
{	
  GLCD_SetCursor(Xadd,Yadd);            
  GLCD_Write_Command(0x2C);     //Command Write Data RGB
  GLCD_Write_Data16(color);     //write data
}

/****************************************************/
/*               Function Plot Signe+               */
/****************************************************/
/* Paramiter : x1,y1 = Position start Line '-'(Hor) */
/*                     plot from Left to Right      */
/*             x2,y2 = Position start Line '|'(Ver) */
/*                     plot from botton to top      */
/****************************************************/
void plot_mark_Hor(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
  unsigned char i ;
	unsigned int y = y2;

  //---- Line- ----
  //set_x_addr(x1);     																																						// Start Adddress Hor(X)=0
  //set_y_addr(y1);     																																						// Start Adddress Ver(Y)=0
	GLCD_SetCursor(x1,y1);
    
  GLCD_Write_Command(0x2C);    																																		// Command Write Data RGB   
	for(i=0; i<15; i++) 
	{
    GLCD_Write_Data16(color);
	}	

	//------Line | --------
  for(i=0; i<15; i++) 
  {   
    //set_x_addr(x2);     																																					// Set Adddress Hor(X)
    //set_y_addr(y);     																																						// Set Adddress Ver(Y)
		GLCD_SetCursor(x2,y);

    GLCD_Write_Command(0x2C);     																																// Command Write Data RGB   
    GLCD_Write_Data16(color);    
    y = y-1;   
  }
}
 
/****************************************************************************/
/**                Function Print Text 1 Charecter size 7x11                */
/****************************************************************************/
/* Parameter : row      = Ascii Code (Position buffer keep text)		    */
/*             adx,ady  = Position X,Y for begin plot text by will 			*/
/*                        begin plot from bottom left to top left   		*/
/*             fg_clr   = Color of text										*/
/*             bg_clr   = Color background of text(if bg_clr = no_bg or 1=	*/
/*                        non color background)								*/
/****************************************************************************/
void text_7x11_Hor(unsigned char row,int adx,int ady,unsigned int fg_clr,unsigned int bg_clr)
{
  int ax,ay;
  unsigned char m,n,tx;
     
	ax = adx;
	ay = ady; 
	row = row-0x20;

  //------- Print Text 1 Charecter(data 14 Byte) -------------	  
	for(m=0; m<14; m++)
	{
    //---Sent data byte1=8 bit----
    tx = ascii_7x11[row][m];  //Read data Ascii
		for(n=0; n<8; n++)		                     //Loop Sent data  1 byte(8bit)
		{
	    if(tx & 0x80)				                 //if data bit7 = 1 ,Plot Color area Charecter
			{              
			  plot_dot_Hor(ax,ay,fg_clr);
			}
		  else						                 //if data bit7 = 0 ,Plot Color area back ground Charecter
			{
			  if(bg_clr != 1)     
        {
			    plot_dot_Hor(ax,ay,bg_clr);
			  }
			}
		  tx <<= 1;   //Shift Right data 1 bit
		  ay   = ay+1;   //Increment Y-address
		} 
		m++;   //Increment Next pointter byte Data 

		//------- Sent data byte2=3 bit -----
		tx = ascii_7x11[row][m];  //Read data byte2
	
		for(n=0; n<3; n++)			                 //Loop sent data byte2 = 3 bit
		{						                  
	    if(tx & 0x80)				                 //if data bit7 = 1 ,Plot Color area Charecter
			{              
		    plot_dot_Hor(ax,ay,fg_clr);
			}
		  else						                 //if data bit7 = 0 ,Plot Color area back ground Charecter
			{
			  if(bg_clr != 1)     
        {
			    plot_dot_Hor(ax,ay,bg_clr);
			  }
			}

		  tx <<= 1;   //Shift Right data 1 bit
		  ay = ay+1;   //Increment Y-address
		} 

		ax = ax+1;   //Complet sent data 2 byte(11bit) Increment X-Address
		ay = ady;   //Set Position Y-address old
	}	

	//----Fill Back ground Color Position space between Charecter 1 Colum -------
  if(bg_clr != 1)     
  {
	  for(n=0; n<11; n++)
	  {
	    plot_dot_Hor(ax,ay,bg_clr);
	    ay = ay+1;   //Increment Y-Address
	  }
  }
}

/********************************************************/
/*               Function Draw Line 		     		*/
/********************************************************/
/* Function : Line(x_1,y_1,x_2,y_2,line_type);	        */
/*												     	*/
/* Input  : x_1,y_1    = Start Coordinate Line			*/
/*          x_2,y_2    = End Coordinate Line			*/
/*          line_type  = Value Thick of Line (1-100)    */     
/*	        color      = Color of line			        */
/********************************************************/
void Line_Hor(int x_1,int y_1,int x_2,int y_2,unsigned char line_type,unsigned int color)
{
  unsigned char tick;
  int x1=x_1,xx1=x_1;
  int y1=y_1;  
  int x2=x_2,xx2=x_2;
  int y2=y_2;
	int dx,dy,inc_dec,two_dx,two_dy,two_dy_dx,two_dx_dy,x,y,p;

  if(x_1>x_2)
	{
	  x1=x_2;
	  y1=y_2;

	  x2=x_1;
	  y2=y_1;
	}

  dx  = fabs(x2-x1);
  dy  = fabs(y2-y1);

	if(y2>=y1)
	{	
	  inc_dec = 1;
	}	
	else
	{	
		inc_dec = -1;
	}	

  if(dx>dy)
	{
	  two_dy=(2*dy)         ;
	  two_dy_dx=(2*(dy-dx)) ;
	  p=((2*dy)-dx)         ;

	  x=x1 ;
	  y=y1 ;

	  while(x<=x2)
		{
		  for(tick=0;tick<line_type;tick++)
			{	   
			  if(xx1==xx2)
				{	
			    plot_dot_Hor(x+tick,y,color)   ;
				}	
			  else
				{	
			    plot_dot_Hor(x,y+tick,color)   ;
				}	
			}

		  x++              ;

		  if(p<0)
			{	
		    p += two_dy   ;
			}	
		  else
		  {
			  y += inc_dec   ;
			  p += two_dy_dx ;
		  }
		}  //while
	}	 //if

  else
	{
	  two_dx=(2*dx);
	  two_dx_dy=(2*(dx-dy));
	  p=((2*dx)-dy);

	  x=x1;
	  y=y1;

	  while(y!=y2)
		{
		  for(tick=0;tick<line_type;tick++)
		  {		     
			  if(xx1==xx2)					    
				{	
			    plot_dot_Hor(x+tick,y,color)       ;
				}	
			  else
				{	
			    plot_dot_Hor(x,y+tick,color)       ;
				}	  
		  }
		  y+=inc_dec;

		  if(p<0)
			{	
		    p+=two_dx;
			}	
		  else
		  {
			  x++             ;
			  p += two_dx_dy  ;
		  }
		}  //while
	}	//else
}	    //end.

/*******************************************************************************/
/*                         Function Draw Circle 					           */
/*******************************************************************************/   
/* Function : Circle(CX,CY,Radius,tick,color,fill)		                       */
/* Input    : CX,CY       = Center Point of Circle							   */
/*            Radius      = radius of Circle					               */
/*            tick(1-100) = bold of line (have effect when fill=0)	           */
/*            color       = color of Circle						   	           */
/*            fill        = 0: No fill color in Circle,1:Fill color in Circle  */
/*******************************************************************************/
void Circle_Hor(int CX,int CY,unsigned char Radius,unsigned tick,unsigned int color,unsigned char fill)
{
  int tswitch=0                ;
  unsigned char d,x=0,y=0,bd   ;

  d = CY-CX             ;
	y = Radius            ;
	tswitch = 3-(2*Radius);

	while(x<=y)	
	{
	  if(fill==1)
	  {
		  Line_Hor(CX-x,CY+y,CX+x,CY+y,1,color) ;
		  Line_Hor(CX-x,CY-y,CX+x,CY-y,1,color) ;
		  Line_Hor(CX-y,CY+x,CX+y,CY+x,1,color) ;
		  Line_Hor(CX-y,CY-x,CX+y,CY-x,1,color) ;
		}
	  else
		{
		  for(bd=0;bd<tick;bd++)
		  {
		    plot_dot_Hor((CX+x)+bd,(CY+y)+bd,color)     ;
		    plot_dot_Hor((CX+x)+bd,(CY-y)+bd,color)     ;

		    plot_dot_Hor((CX-x)+bd,(CY+y)+bd,color)     ;
		    plot_dot_Hor((CX-x)+bd,(CY-y)+bd,color)     ; 

		    plot_dot_Hor((CY+y-d)+bd,(CY+x)+bd,color)   ;
		    plot_dot_Hor((CY+y-d)+bd,(CY-x)+bd,color)   ;

		    plot_dot_Hor((CY-y-d)+bd,(CY+x)+bd,color)   ;
		    plot_dot_Hor((CY-y-d)+bd,(CY-x)+bd,color)   ;
		  }
	  }

	  if(tswitch<0)
		{	
	    tswitch += (4*x)+6     ;
		}	
	  else
	  {
      tswitch += 4*(x-y)+10  ;
		  y--;
	  }
    x++;
  }              //while
}
  
/********************************************************************************/
/*                           Function Draw Ellipse 							    */
/********************************************************************************/   
/* Function : Ellipse(h,k,rx,ry,tick,color,fill)								*/
/* Input    : h,k         = Center Point of Ellipes  				            */
/*            rx,ry       = radius of Ellipse in X,Y axis			            */
/*            tick(1-100) = bold of line (have effect when fill=0)	            */
/*            color       = color of Ellipse						   	        */
/*            fill        = 0: No fill color in Ellipes,1:Fill color in Ellipes */
/********************************************************************************/ 
void Ellipse_Hor(unsigned int h,unsigned int k,long rx,long ry,unsigned char tick,unsigned int color,unsigned char fill)
{
  unsigned char bd       ;
  long  ax  =(rx*rx)     ;
  long   by  =(ry*ry)    ;
  float  ax2 =(ax*2)     ;
  float  by2 =(by*2)     ;
  long   x   = 0         ;
  long   y   = ry        ;
  float  fx  = 0         ;
  float  fy  = (ax2*ry)  ;
  float     p=(int)(by-(ax*ry)+(0.25*ax)+0.5);

	if(fill==1)
	{
		 Line_Hor(h-x,k+y,h+x,k+y,1,color) ;
		 Line_Hor(h-x,k-y,h+x,k-y,1,color) ;
	}
	else
	{
		 for(bd=0;bd<tick;bd++)
		 {
       plot_dot_Hor((h+x)+bd,(k+y)+bd,color)      ;
		   plot_dot_Hor((h+x)+bd,(k-y)+bd,color)      ;
		   plot_dot_Hor((h-x)+bd,(k-y)+bd,color)      ;
		   plot_dot_Hor((h-x)+bd,(k+y)+bd,color)      ;
		 }
	}		   
	
  while(fx<fy)
	{
	  x++;
	  fx+=by2;

	  if(p<0)
		{	
		  p+=(fx+by);
		}	
	  else
		{
      y--;
		  fy-=ax2;
		  p+=(fx+by-fy);
		}

		if(fill==1)
	  {
		  Line_Hor(h-x,k+y,h+x,k+y,1,color) ;
		  Line_Hor(h-x,k-y,h+x,k-y,1,color) ;
	  }
	  else
	  {
		  for(bd=0;bd<tick;bd++)
		  {		  
		    plot_dot_Hor((h+x)+bd,(k+y)+bd,color)      ;
		    plot_dot_Hor((h+x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Hor((h-x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Hor((h-x)+bd,(k+y)+bd,color)      ;
		  }
		}
	}

  p=(int)((by*(x+0.5)*(x+0.5))+(ax*(y-1)*(y-1))-(ax*by)+0.5);

  while(y>0)
	{
	  y--;
	  fy-=ax2;

	  if(p>=0)
		{	
		  p+=(ax-fy);
		}	
	  else
		{
		  x++;
		  fx+=by2;
		  p+=(fx+ax-fy);
		}

		if(fill==1)
	  {
		  Line_Hor(h-x,k+y,h+x,k+y,1,color) ;
		  Line_Hor(h-x,k-y,h+x,k-y,1,color) ;
	  }
	  else
	  {
		  for(bd=0;bd<tick;bd++)
		  {
		    plot_dot_Hor((h+x)+bd,(k+y)+bd,color)      ;
		    plot_dot_Hor((h+x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Hor((h-x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Hor((h-x)+bd,(k+y)+bd,color)      ;
		  }
		}   //else   
	}	 
}
 
/************************************************************************************/
/*                          Function Draw a rectangle 			                    */
/************************************************************************************/
/* Function : Rectan(x1,y1,x2,y2,tick,color,fill)									*/	
/* Inputs   : (x1, y1)    = Start Point coordinate Rectan							*/
/*            (x2, y2)    = End Point coordinate  Rectan							*/
/*            tick(1-100) = bold of line(have effect when fill=0					*/
/*            color       =  color of rectangle									    */
/*            fill        = 0: No fill color in rectangle,1:Fill color in rectangle */
/*       																		    */
/*            (x1,y1) -------------												    */
/*                   |             |											    */
/*                   |             |											    */
/*                   |             |											    */
/*                   |             |											    */
/*                    -------------	(x2,y2)										    */
/*																					*/
/************************************************************************************/																						  
void Rectan_Hor(int x1,int y1,int x2,int y2,unsigned char tick,unsigned int color,unsigned char fill)
{
  int xmin,xmax,ymin,ymax,i ;
  unsigned char tk = tick-1 ;
	
  if(fill==1)
	{
	  if(x1 < x2)                            //  Find x min and max
    {
      xmin = x1;
      xmax = x2;
		}
		else
		{
			xmin = x2;
			xmax = x1;
		}
			
		if(y1 < y2)                            // Find the y min and max
		{
		  ymin = y1;
			ymax = y2;
		}
		else
		{
		  ymin = y2;
			ymax = y1;
		}
			
		for(; xmin<=xmax;xmin++)
		{
		  for(i=ymin; i<=ymax; i++)
			plot_dot_Hor(xmin,i,color)      ;
		}
	}
	else	// No-Fill
	{
	  Line_Hor(x1,y1-tk,x2,y1-tk,tick,color) ;  //Line Ho.r Top
	  Line_Hor(x1,y2,x2,y2,tick,color)       ;  //Line Hor. Botton
	  Line_Hor(x1,y1,x1,y2,tick,color)       ;  //Line Ver. Left
	  Line_Hor(x2-tk,y1,x2-tk,y2,tick,color) ;  //Line Ver. Right
	}      		 
}

/**********************************************************************/
/*                         Function Print String                      */
/**********************************************************************/
/* Parameter : *str = Charecter ASCII (String)					      */
/*             cur_x,cur_y = Position X,Y for begin plot text by will */
/*                           begin plot from bottom left to top left  */
/*             fg_color = color of Text								  */
/*             bg_color = color Background of text                    */											
/**********************************************************************/
void lcd_printStr_Hor(char *str,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color)
{
  unsigned char i;

	for (i=0; str[i] != '\0'; i++)
	{					 
	 text_7x11_Hor(str[i],cur_x,cur_y,fg_color,bg_color);
	 cur_x += 8 ; 
		
	}
}
 
/***********************************/
/* Function Print Text 3 Charecter */
/***********************************/
void lcd_print3Cha_Hor(char ch1,char ch2,char ch3,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color)
{	  			 
  text_7x11_Hor(ch1,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8 ; 
  text_7x11_Hor(ch2,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8 ; 
  text_7x11_Hor(ch3,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8 ;  	
}

/*************************************/
/*         Function Plot TAB         */
/*************************************/
/* Parameter : color = color for tab */
/*************************************/	     
void plot_tab_Hor(unsigned int color)
{
  unsigned char rx,ry;
	  
	for(ry=0;ry<26;ry++)
	{ 
		GLCD_SetCursor(309,226-ry);
		
    GLCD_Write_Command(0x2C);  //Command Write Data RGB	
	  for(rx=0;rx<6;rx++) 
		{
      GLCD_Write_Data16(color);  //write data
		}	
	}
}


/*************************************************************************/
/*                    Function build Button (Draw-rectang)               */
/*    Start Plot button from  botton left to botton Right of button      */  
/*************************************************************************/
/*																	     */
/* Inputs : px,py      = Position X,Y  begin build  button               */
/*          width,high = width and high of button(unit Pixel)            */
/*                       for width not exceed 50 pixel will good         */
/*          c_bt       = Color of button                                 */
/*          *tx        = ASCII Charecter on Button                       */
/*                       1 Charecter width = 9pixel. Number pixel of	 */
/*                       charecter total must less pixel width of button */  
/*                     												     */
/*          num_tx     = Number Charecter at display on button           */
/*          c_tx       = Color charecter on button                       */
/*       																 */
/*          |<--- width --->|								    	     */
/*           -------------->   ---									     */
/*          |           end |	 ^									     */
/*          |               |	 |									     */
/*          |               |	high								     */
/*          |start          |	 |									     */
/*   (px,py) -------------->	---									     */
/*																	     */
/*************************************************************************/
void button_build_Hor(unsigned int px,unsigned int py,unsigned int width,unsigned int high,unsigned int c_bt,char *tx,unsigned char num_tx,unsigned int c_tx)
{
  unsigned int x1,y1,x2,y2,ay,tpx;
  unsigned char tab,n;
  unsigned int cbt1=0,cbt2=0,sub=0;
  
  switch(c_bt)
	{
	  case BT_RED   :
		                c_bt = 0xF800; //Color 
					    cbt1 = c_bt; //color start Max
					    cbt2 = 0x9800; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0800; //decrement color
		break;

		case BT_GREEN :
		                c_bt = 0x07C0; //Color 
					    cbt1 = c_bt; //color start Max
					    cbt2 = 0x04D0; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0040; //decrement color		         
		break;

	  case BT_BLUE   :
		                c_bt = 0x001F; //Color 
					    cbt1 = c_bt; //color start Max
					    cbt2 = 0x0014; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0001; //decrement color
		break;

    case BT_YELLOW :
		               	c_bt = 0xFFC0; //Color 
					    cbt1 = c_bt; //color start Max
					    cbt2 = 0x9CC0; //color last  Min(cbt1-(sub*12))
					    sub  = 0x0840; //decrement color
		break;

		case BT_WHITE  :
		                c_bt = 0xFFFF; //Color 
					    cbt1 = c_bt; //color start Max
					    cbt2 = 0x9CF3; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0841; //decrement color
		break;

	  case BT_CYAN  :
		                c_bt = 0x87FF; //Color 
					    cbt1 = c_bt; //color start Max
					    cbt2 = 0x24F3; //color last  Min(cbt1-(sub*12))
					    sub  = 0x0841; //decrement color
		break;

	 	case BT_MAGENTA :
		                 c_bt = 0xFC1F; //Color 
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0x9913; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0841; //decrement color
		break;

	  case BT_BROWN :
		                 c_bt = 0xEC00; //Color 
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0x8900; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0840; //decrement color
		break;
	
//-----------------------------------------------------------------------------------

	  case BT_LRED  :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0xFCF3; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0041; //decrement color
		break;	
				 
	  case BT_LGREEN :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0x9FF3; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0801; //decrement color
		break;	
				 
	  case BT_LBLUE :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0x9CFF; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0840; //decrement color
		break;

	  
		case BT_LYELLOW :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0xFFC3; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0001; //decrement color
		break;
         
		case BT_LWHITE :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0xFFDE; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0021; //decrement color
		break;

		case BT_LCYAN :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0x9FFF; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0800; //decrement color
		break;
						
		case BT_LMAGENTA :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0xFCFF; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0040; //decrement color
		break;	
				 
	  case BT_LBROWN :
		                 c_bt = 0xFFFF;
					     cbt1 = c_bt; //color start Max
					     cbt2 = 0xFE73; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0021; //decrement color
		break;												
  }


  x1   = px;    
	y1   = py;
  x2   = px+width; 
  y2   = py+high-1;
  

  high  = high/2;
  ay    = py+high;
  tab   = high/12;
	
	if(tab==0)
	{	
	   tab=1;
	}	
	else
	{
	  if((high%12)>3)
		{	
	    tab = tab+1;
		}	
	}

  //---------Start Plot halft button from botton to top ---------
  while(y1<=ay)
  {
    for(n=0;n<tab;n++)
    {
      Line_Hor(x1,y1,x2,y1,1,cbt1);
      y1++;
	   
	    if(y1 >= ay)
			{	
	      n=n+20;
			}	
    }
    cbt1 = cbt1-sub;
     
	  if(cbt1<cbt2)
		{	
      cbt1=cbt2;
		}	
  }

  //---------Plot halft button from top to botton---------
  cbt1 = c_bt; 

  while(y2>=ay)
  {
    for(n=0;n<tab;n++)
    {
      Line_Hor(x1,y2,x2,y2,1,cbt1);
      y2--;
	 
	    if(y2 <= ay)
			{	
	      n=n+20;
			}	
    }
    cbt1 = cbt1-sub;
     
	  if(cbt1<cbt2)
		{	
      cbt1=cbt2;
		}	
  }

  //----------write Text ---------
	
  tpx = (width-(num_tx*8))/2;
	tpx = tpx+px;
		
	py = (py+high)-6;
	

	for (n=0; tx[n] != '\0'; n++)
	{					 
	  text_7x11_Hor(tx[n],tpx,py,c_tx,no_bg);
	  tpx += 9; 		
	}
}

/*********************************************************************/
/*         Function Plot Picture(BMP) to Display                     */
/*  Start Plot data of picture from  top left to top Right           */
/*********************************************************************/
/*  Inputs : *bmp       = Name reg. array keep data of picture       */
/*           xad,yad    = Position X,Y for begin plot data picture   */
/*           width,high = Size Picture unit Pixel                    */
/*********************************************************************/
void plot_picture_Hor(char *bmp,unsigned int xad,unsigned int yad,unsigned int width,unsigned int high)     
{
  unsigned long pt=0,picture;
  unsigned int hi,wid;
  unsigned int xad1,yad1;

  xad1 = xad;
	yad1 = yad;

	for(hi=0;hi<high;hi++)		                    //Number X-Pixel(High) of Picture
	{      		
    GLCD_SetY(yad1);  //Set Adddress Ver(Y)  

		for(wid=0;wid<width;wid++)	                //number X-pixel(Width) of Picture  
		{
			 GLCD_SetX(xad1);  //Set Adddress Hor(X)
		   				 
			 GLCD_Write_Command(0x2C);  //Command Write data 	
			 	  
			 picture = (bmp[pt+1]&0xFF);  //Read data picture byte High
			 picture =	picture <<8;
			  
			 picture|= (bmp[pt]&0xFF);  //read data picture Byte low

       GLCD_Write_Data16(picture);
			 pt = pt+2;  //Increment pointer of data picture 2 byte
			 
			 xad1++;			 
		 }

		 yad1--;
		 xad1 = xad;
	}
}

/********************************/
/* Initial GLCD : ET-TFT240320P */
/*      (Driver SPFD5408A)      */
/********************************/
void Initial_GLCD_Ver(void)
{
  unsigned long cnt;
  
  GLCD_Write_Command(0x01);   //Software reset
  delay_ms(5);
  GLCD_Write_Command(0x28);    //display off
      
  GLCD_Write_Command(0xCF);
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x83);
  GLCD_Write_Data(0x30);
      
  GLCD_Write_Command(0xED);
  GLCD_Write_Data(0x64);
  GLCD_Write_Data(0x03);
  GLCD_Write_Data(0x12);
  GLCD_Write_Data(0x81);
  
  GLCD_Write_Command(0xE8);
  GLCD_Write_Data(0x85);
  GLCD_Write_Data(0x01);
  GLCD_Write_Data(0x79); 
  
  GLCD_Write_Command(0xCB);
  GLCD_Write_Data(0x39);
  GLCD_Write_Data(0x2C);
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x34);
  GLCD_Write_Data(0x02);
  
  GLCD_Write_Command(0xF7);
  GLCD_Write_Data(0x20);
  
  GLCD_Write_Command(0xEA);
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x00);

  //---------------- Power Control -------------------
  GLCD_Write_Command(0xC0);    //Power Control 1
  GLCD_Write_Data(0x26);
   
  GLCD_Write_Command(0xC1);    //Power Control2
  GLCD_Write_Data(0x11);

  //----------------- VCOM --------------------
  GLCD_Write_Command(0xC5);    //VCOM Control 1
  GLCD_Write_Data(0x35);
  GLCD_Write_Data(0x3E);

  GLCD_Write_Command(0xC7);    //VCOM Control 2
  GLCD_Write_Data(0xBE);

  //----------------- Memory access Control ---------------
  GLCD_Write_Command(0x36);    //Memory access Control 
  GLCD_Write_Data(0x48);    //my,mx=1,mv,ml,BGR,mh
   
  GLCD_Write_Command(0x3A);    //Pixel format set
  GLCD_Write_Data(0x55);    //16 bit/Pixel

  //---------------------- Frame Rate ----------------------
  GLCD_Write_Command(0xB1);    //Frame rate
  GLCD_Write_Data(0x00);
  GLCD_Write_Data(0x1B);    //default

  //---------------------- Gamma -------------------
  GLCD_Write_Command(0xF2);    //3Gamma Function Disable 
  GLCD_Write_Data(0x08);  
   
  GLCD_Write_Command(0x26);    
  GLCD_Write_Data(0x01);    // Gamma Set 4 gamma curve 01/02/04/08
   
  GLCD_Write_Command(0xE0);    //Positive gamma correction    
  GLCD_Write_Data(0x1F);   
  GLCD_Write_Data(0x1A);     
  GLCD_Write_Data(0x18);   
  GLCD_Write_Data(0x0A);     
  GLCD_Write_Data(0x0F);   
  GLCD_Write_Data(0x06);     
  GLCD_Write_Data(0x45);   
  GLCD_Write_Data(0x87);     
  GLCD_Write_Data(0x32);   
  GLCD_Write_Data(0x0A);     
  GLCD_Write_Data(0x07);   
  GLCD_Write_Data(0x02);     
  GLCD_Write_Data(0x07);   
  GLCD_Write_Data(0x05);     
  GLCD_Write_Data(0x00);   

  GLCD_Write_Command(0xE1);    //Negamma correction    
  GLCD_Write_Data(0x00);   
  GLCD_Write_Data(0x25);     
  GLCD_Write_Data(0x27);   
  GLCD_Write_Data(0x05);     
  GLCD_Write_Data(0x10);   
  GLCD_Write_Data(0x09);     
  GLCD_Write_Data(0x3A);   
  GLCD_Write_Data(0x78);     
  GLCD_Write_Data(0x4D);   
  GLCD_Write_Data(0x05);     
  GLCD_Write_Data(0x18);   
  GLCD_Write_Data(0x0D);     
  GLCD_Write_Data(0x38);   
  GLCD_Write_Data(0x3A);     
  GLCD_Write_Data(0x1F);   

  //------------------ SET DDRAM Address ------------------
  GLCD_Write_Command(0x2A);    //Column Set (X)   
  GLCD_Write_Data(0x00);    //Strart address(H) = 0x00
  GLCD_Write_Data(0x00);    //Strart address(L) = 0x00(0)
  GLCD_Write_Data(0x00);    //End address(H) = 0x00
  GLCD_Write_Data(0xEF);    //End address(L) = 0xEF(239)

  GLCD_Write_Command(0x2B);    //Page address Set(Y)    
  GLCD_Write_Data(0x00);    //Strart address(H) = 0x0000
  GLCD_Write_Data(0x00);    //Strart address(L) = 0x0000(0)
  GLCD_Write_Data(0x01);    //End address(H) = 0x01
  GLCD_Write_Data(0x3F);    //End address(L) = 0x3F(319)
 
  GLCD_Write_Command(0xB7);    //Entry mode set    
  GLCD_Write_Data(0x07);    //Normal display

  //-------------------- Display -----------------
  GLCD_Write_Command(0xB6);    //Display Function Control  
  GLCD_Write_Data(0x0A);   
  GLCD_Write_Data(0x82);     
  GLCD_Write_Data(0x27);   
  GLCD_Write_Data(0x00);     

  GLCD_Write_Command(0x11);    //Sleep Out       
  delay_ms(100);

  //------- Clear the contents of the display buffer. --------
  GLCD_Write_Command(0x2C);                //Memmory Write  
  for(cnt=0; cnt<((long)240*320); cnt++)
	{
    GLCD_Write_Data16(BLACK);                // Back-Ground Color Black
	}	
    
  GLCD_Write_Command(0x29);    //Display On       
  delay_ms(100);
  
  GLCD_BL_ON();    //Back Light ON   
  delay_ms(20);    //Delay for 20ms, which is equivalent to two frames.
}

/************************************************************/
/*     Function Calculate Matrix Find value Coeffcient      */
/************************************************************/
/* Parameter    : None                                      */
/* Return Value : Keep value Coeffcient for use calculate   */                           
/*                X-Y-Position in variable divider,An,Bn,Cn */                   
/*                Dn,En,Fn                                  */
/************************************************************/
void TCS_Set_Matrix_Ver()
{
  divider = ((tcs_ave_X[0]-tcs_ave_X[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
	        ((tcs_ave_X[1]-tcs_ave_X[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

       An = ((dis_XD_Ver[0]-dis_XD_Ver[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
			((dis_XD_Ver[1]-dis_XD_Ver[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

	   Bn = ((tcs_ave_X[0]-tcs_ave_X[2])*(dis_XD_Ver[1]-dis_XD_Ver[2]))-
			((dis_XD_Ver[0]-dis_XD_Ver[2])*(tcs_ave_X[1]-tcs_ave_X[2]));
			
	   Cn = (tcs_ave_X[2]*dis_XD_Ver[1]-tcs_ave_X[1]*dis_XD_Ver[2])*tcs_ave_Y[0]+
		    (tcs_ave_X[0]*dis_XD_Ver[2]-tcs_ave_X[2]*dis_XD_Ver[0])*tcs_ave_Y[1]+
			(tcs_ave_X[1]*dis_XD_Ver[0]-tcs_ave_X[0]*dis_XD_Ver[1])*tcs_ave_Y[2];
	    
	   Dn = ((dis_YD_Ver[0]-dis_YD_Ver[2])*(tcs_ave_Y[1]-tcs_ave_Y[2]))-
			((dis_YD_Ver[1]-dis_YD_Ver[2])*(tcs_ave_Y[0]-tcs_ave_Y[2]));

	   En = ((tcs_ave_X[0]-tcs_ave_X[2])*(dis_YD_Ver[1]-dis_YD_Ver[2]))-
			((dis_YD_Ver[0]-dis_YD_Ver[2])*(tcs_ave_X[1]-tcs_ave_X[2]));

	   Fn = (tcs_ave_X[2]*dis_YD_Ver[1]-tcs_ave_X[1]*dis_YD_Ver[2])*tcs_ave_Y[0]+
		    (tcs_ave_X[0]*dis_YD_Ver[2]-tcs_ave_X[2]*dis_YD_Ver[0])*tcs_ave_Y[1]+
			(tcs_ave_X[1]*dis_YD_Ver[0]-tcs_ave_X[0]*dis_YD_Ver[1])*tcs_ave_Y[2];
}

/************************************************************/
/*              Function Calibrate Touch Green              */
/* Touch at Point + on Screen 3 Point(3 step) for Caribrate */
/* Parameter    : None                                      */
/* Return value : None                                      */
/*              											*/
/************************************************************/
void Touch_Calibrate_Ver(void)	
{									
  char n,touch_mark_point=0;

  plot_mark_Ver(17,32,24,25,BRIGHT_RED); 													//Plot Signe + for Position Calibrate Touch Point1
  lcd_printStr_Ver("<Touch Screen Calibratetion>",8,110,BRIGHT_BLUE,BLACK); 				//Plot Text	Color Bright blue
  lcd_printStr_Ver("        STEP 1 of 3         ",8,130,BRIGHT_BLUE,BLACK);
  lcd_printStr_Ver(" Click on center of '+' icon",8,200,WHITE,BLACK); 						//Plot Text	Color Black
  lcd_printStr_Ver(" for Touch screen calibrate ",8,220,WHITE,BLACK);

  while(touch_mark_point != 3)																//Number Mark Point Calibrate = 3 Point  
  {
    if(Get_Status_Touch())        															//return (1) if touch screen is detected.
	{	  
	  for(n=0;n<50;n++)
	  {
	    TCS_SPI_Read_Ver();																	//Read portrait(ADC X,Y) by touch value from ADS7846 and update dif_adc_X & dif_adc_Y
		buf_adc_X[n] = dif_adc_X;															//General buffer for averaging and filter touch screen adc read outs.
		buf_adc_Y[n] = dif_adc_Y;															//General buffer for averaging and filter touch screen adc read outs.
	  }
	  TCS_Average_X_Y(50);																	//Take average out of 50 touch measurements, excluding max and min measurement.

	  tcs_ave_X[touch_mark_point]=  ave_adc_X;
	  tcs_ave_Y[touch_mark_point]=  ave_adc_Y;
				 
	  touch_mark_point++;																	//touch_cal_status;	//0=xy1 cal, 1=xy2 cal, 2=xy3 cal, 3=calibration success. 0xff=calibration not performed.
		
	  while(Get_Status_Touch());        													//Waite until not Touch screen ,

	  switch(touch_mark_point)																//Select Caribrate Next Step 2 and 3 
	  {
	    case 1: bg_color(BLACK);															//Clear Screen
				plot_mark_Ver(208,160,215,153,BRIGHT_RED);									//Plot Signe + for Position Calibrate Touch Point2

                lcd_printStr_Ver("<Touch Screen Calibratetion>",8,110,BRIGHT_BLUE,BLACK); 	//Plot Text	Color Bright blue
                lcd_printStr_Ver("        STEP 2 of 3         ",8,130,BRIGHT_BLUE,BLACK);
                lcd_printStr_Ver(" Click on center of '+' icon",8,200,WHITE,BLACK);	   		//Plot Text	Color white
                lcd_printStr_Ver(" for Touch screen calibrate ",8,220,WHITE,BLACK);
				break;

		case 2:	bg_color(BLACK); 															//Clear Screen
			    plot_mark_Ver(113,287,120,280,BRIGHT_RED);									//Plot Signe + for Position Calibrate Touch Point3
						
				lcd_printStr_Ver("<Touch Screen Calibratetion>",8,110,BRIGHT_BLUE,BLACK);	//Plot Text	Color Bright blue
                lcd_printStr_Ver("        STEP 3 of 3         ",8,130,BRIGHT_BLUE,BLACK);
                lcd_printStr_Ver(" Click on center of '+' icon",8,200,WHITE,BLACK);	   		//Plot Text	Color white
                lcd_printStr_Ver(" for Touch screen calibrate ",8,220,WHITE,BLACK);
				break;

		case 3: bg_color(BLACK); 															//Clear Screen

				lcd_printStr_Ver("<Touch Screen Calibratetion>",8,110,BRIGHT_BLUE,BLACK);	//Plot Text	Color Bright blue
                lcd_printStr_Ver("          Succeded!         ",8,150,BRIGHT_GREEN,BLACK);
                lcd_printStr_Ver("   -Tap Screen for exit-    ",8,190,WHITE,BLACK);
                       
				while(!Get_Status_Touch());  
				delay_ms(100);
				bg_color(BLACK); 															//Clear Screen
				break;
	  }			 
	}
	delay_ms(100);	
  }																			 
  TCS_Set_Matrix_Ver();																		//Calculate Coefficient for use find Position Address real on Display
	
  //You can Keep Value data in valiabie devider,An,Bn,Cn,Dn,En,Fn to E2Prom  so that not 
  //come back	 Caribrate.
/*  printf("\n\r divider = %d",divider) ;
	printf("\n\r      An = %d",An) ;
	printf("\n\r      Bn = %d",Bn) ;
	printf("\n\r      Cn = %d",Cn) ;
    printf("\n\r      Dn = %d",Dn) ;
	printf("\n\r      En = %d",En) ;
	printf("\n\r      Fn = %d",Fn) ;
*/

}

/*****************************************************************/ 
/* Function Read X-Y-Position ADC Touch Screen-12 bit (ADS7846)  */
/* Parameter    : None                                           */
/* Return Value : dif_adc_X = Keep Result ADC X-Position(12 bit) */
/*                dif_adc_Y = Keep result ADC Y-Position(12 bit) */                      
/*****************************************************************/
void TCS_SPI_Read_Ver(void)
{
  long tcs_adc_X ,tcs_adc_Y;
  unsigned char buf_data[4]={0,0,0,0};

  if(Get_Status_Touch())
  {
    TCS_CS_LOW();																			// Enable Touch Interface

    buf_data[0] = TCS_SPI_Write(0xD0);														// Write Command Measure X-Position 
	buf_data[0] = TCS_SPI_Write(0x00);														// Read ADC data X-Position (7-bit byte High) data: 0ddddddd	(bit) 
	buf_data[1] = TCS_SPI_Write(0x90);														// Write Command Measure Y-Position ; Read ADC data X-Position (5-bit byte Low)  data:ddddd000(bit)
	buf_data[2] = TCS_SPI_Write(0x00);														// Read ADC data Y-Position(7-bit byte High) data: 0ddddddd (bit)
	buf_data[3] = TCS_SPI_Write(0x00);														// Read ADC data Y-Position(5-bit byte Low)  data: ddddd000 (bit)

	tcs_adc_X  = buf_data[0];																// Mark ADC Data X-Position 12 bit
	tcs_adc_X  = tcs_adc_X << 5;
	tcs_adc_X |= buf_data[1] >> 3;
	tcs_adc_X  = tcs_adc_X & 0x00000FFF;
	 
	tcs_adc_Y  = buf_data[2];   															// Mark ADC Data Y-Position 12 bit
	tcs_adc_Y  = tcs_adc_Y << 5;   															// Shift 7 bit High
	tcs_adc_Y |= buf_data[3] >> 3;   														// Shift 5 bit low
	tcs_adc_Y  = tcs_adc_Y & 0x00000FFF;													// total ADC data 12 bit
	
	TCS_CS_HIGH();																			// Disable Touch Interface			

	//Vertical Result
	dif_adc_X = tcs_adc_X;																	//ADC 12 bit :LCD Start Portrait
	dif_adc_Y = 4095-tcs_adc_Y;
  }
}

/*******************************************************************/
/*          Function Get Position Address real of Display          */
/*******************************************************************/
/* Parameter    : num = Number times for Measure adc from Touched  */ 
/* Return Value : X = Keep value address X-Position		           */
/*                Y = Keep value address Y-Position                */ 
/*******************************************************************/
void TCS_Get_Point_Ver(char num)
{
  char nm; 
	
  nm=0;
  while(Get_Status_Touch() && nm<num)	  													// Measure touch x,y 10 times if the Touch kept pressed,
  {
    TCS_SPI_Read_Ver();                     												// Read value ADC Touch X-Y 
	buf_adc_X[nm]=	dif_adc_X; 																// keep value ADC Touch-X
	buf_adc_Y[nm]=	dif_adc_Y; 																// keep value ADC Touch-Y

 	nm++; 
  }

  if(nm==num)	                      														// if the touch xy successfuly collected,
  {	
    TCS_Average_X_Y(num); 																	// take average out of 10 touch measurements, excluding max and min measurement.
			    
    dif_adc_X = ave_adc_X; 																	// keep average ADC Touch-X
	dif_adc_Y = ave_adc_Y; 																	// keep average ADC Touch-Y
               
	X = ((An*dif_adc_X)+(Bn*dif_adc_Y)+Cn)/divider; 
	Y = ((Dn*dif_adc_X)+(En*dif_adc_Y)+Fn)/divider;
  }
}

/*****************************************************/
/*              Function Plot Dot 1 Pixel            */
/*****************************************************/
/* Parameter : Xadd,Yadd = Position X,Y for Plot Dot */
/*             color = Color of dot                  */
/*****************************************************/
void plot_dot_Ver(unsigned int Xadd,unsigned int Yadd,unsigned int color)
{	        
	GLCD_SetCursor(Xadd,Yadd);
  GLCD_Write_Command(0x2C);      //Command Write Data RGB
  GLCD_Write_Data16(color);      //write data
}


/****************************************************/
/*               Function Plot Signe+               */
/****************************************************/
/* Paramiter : x1,y1 = Position start Line '-'(Hor) */
/*                     plot from Left to Right      */
/*             x2,y2 = Position start Line '|'(Ver) */
/*                     plot from botton to top      */
/****************************************************/
void plot_mark_Ver(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
  unsigned char i;
  unsigned int y = y2;
  
  //---- Line- ----   
	//set_x_addr(x1);	  //Start Adddress Hor(X)=0
 	//set_y_addr(y1);	  //Start Adddress Ver(Y)=0
	GLCD_SetCursor(x1,y1);
	 
	GLCD_Write_Command(0x2C);    //Command Write Data RGB   
	for(i=0; i<15; i++)
	{
    GLCD_Write_Data16(color);
	}	

	//------Line | --------
  for(i=0;i<15;i++) 
	{	
    //set_x_addr(x2);	    //Set Adddress Hor(X)
    //set_y_addr(y);		//Set Adddress Ver(Y)
		GLCD_SetCursor(x2,y);

		GLCD_Write_Command(0x2C);     //Command Write Data RGB   
    GLCD_Write_Data16(color);
		
		y = y+1;	  
	}
}

/****************************************************************************/
/**                Function Print Text 1 Charecter size 7x11                */
/****************************************************************************/
/* Parameter : row      = Ascii Code (Position buffer keep text)		    */
/*             adx,ady  = Position X,Y for begin plot text by will 			*/
/*                        begin plot from bottom left to top left   		*/
/*             fg_clr   = Color of text										*/
/*             bg_clr   = Color background of text(if bg_clr = no_bg or 1=	*/
/*                        non color background)								*/
/****************************************************************************/
void text_7x11_Ver(unsigned char row,int adx,int ady,unsigned int fg_clr,unsigned int bg_clr)
{
  int ax,ay                  ;
  unsigned char m,n,tx       ;
    
	ax = adx;
	ay = ady; 
	row = row-0x20;

  //------- Print Text 1 Charecter(data 14 Byte) -------------  
	for(m=0;m<14;m++)
	{
	  //---Sent data byte1=8 bit----
    tx = ascii_7x11[row][m]; //Read data Ascii
		 
		for(n=0;n<8;n++)		                        //Loop Sent data  1 byte(8bit)
		{
	    if(tx & 0x80)				                    //if data bit7 = 1 ,Plot Color area Charecter
			{              
			  plot_dot_Ver(ax,ay,fg_clr);
			}
		  else						                    //if data bit7 = 0 ,Plot Color area back ground Charecter
			{
			  if(bg_clr != 1)     
        {
			    plot_dot_Ver(ax,ay,bg_clr);
			  } 
			}

		  tx <<= 1                                   ;  //Shift Right data 1 bit
		  ay   = ay-1                                ;  //Decrement Y-address
		} 
		m++;  //Increment Next pointter byte Data 


		//------- Sent data byte2=3 bit -----
		tx =  ascii_7x11[row][m];  //Read data byte2
	
		for(n=0;n<3;n++)			                     //Loop sent data byte2 = 3 bit
		{						                      
	    if(tx & 0x80)				                     //if data bit7 = 1 ,Plot Color area Charecter
			{              
			  plot_dot_Ver(ax,ay,fg_clr);
			}
		  else						                     //if data bit7 = 0 ,Plot Color area back ground Charecter
			{
			  if(bg_clr != 1)     
        {
			    plot_dot_Ver(ax,ay,bg_clr);
			  }
			}

		  tx <<= 1;  //Shift Right data 1 bit
		  ay = ay-1;  //Decrement Y-address
		} 

		ax = ax+1;  //Complet sent data 2 byte(11bit) Increment X-Address
		ay = ady;  //Set Position Y-address old
	}	

	//----Fill Back ground Color Position space between Charecter 1 Colum ------- 
  if(bg_clr != 1)     
  {
	  for(n=0;n<11;n++)
	  {
	    plot_dot_Ver(ax,ay,bg_clr);
		  ay = ay-1;   //Incre ment Y-Address
	  }
  }
}

/********************************************************/
/*               Function Draw Line 		     		*/
/********************************************************/
/* Function : Line(x_1,y_1,x_2,y_2,line_type);	        */
/*												     	*/
/* Input  : x_1,y_1    = Start Coordinate Line			*/
/*          x_2,y_2    = End Coordinate Line			*/
/*          line_type  = Value Thick of Line (1-100)    */     
/*	        color      = Color of line			        */
/********************************************************/
void Line_Ver(int x_1,int y_1,int x_2,int y_2,unsigned char line_type,unsigned int color)
{
  unsigned char tick           ;
  int  x1=x_1,xx1=x_1          ;
  int y1=y_1                   ;
  int x2=x_2,xx2=x_2           ;
  int y2=y_2                   ;
	int dx,dy,inc_dec,two_dx,two_dy,two_dy_dx,two_dx_dy,x,y,p  ;

  if(x_1>x_2)
	{
    x1=x_2 ;
	  y1=y_2 ;
	  x2=x_1 ;
	  y2=y_1 ;
	}
  dx  = fabs(x2-x1) ;
  dy  = fabs(y2-y1) ;
	if(y2>=y1)
	{	
	     inc_dec = 1    ;
	}	
	else
	inc_dec = -1   ;

  if(dx>dy)
	{
    two_dy=(2*dy)         ;
	  two_dy_dx=(2*(dy-dx)) ;
	  p=((2*dy)-dx)         ;

	  x=x1 ;
	  y=y1 ;

	  while(x<=x2)
		{
      for(tick=0;tick<line_type;tick++)
			{	   
			  if(xx1==xx2)
				{	
			    plot_dot_Ver(x+tick,y,color)   ;
				}	
			  else
				{	
			    plot_dot_Ver(x,y+tick,color)   ;
				}				  
			}
		  x++              ;
		  if(p<0)
			{	
		    p += two_dy   ;
			}	
		  else
		  {
			  y += inc_dec   ;
			  p += two_dy_dx ;
		  }
		}  //while
	}	 //if
  else
	{
	  two_dx=(2*dx)            ;
	  two_dx_dy=(2*(dx-dy))    ;
	  p=((2*dx)-dy)            ;

	  x=x1                     ;
	  y=y1                     ;

	  while(y!=y2)
		{
		  for(tick=0;tick<line_type;tick++)
		  {		     
			  if(xx1==xx2)					    
				{	
			    plot_dot_Ver(x+tick,y,color)       ;
				}	
			  else
				{	
			    plot_dot_Ver(x,y+tick,color)       ;
				}	  
		  }
		  y+=inc_dec         ;

		  if(p<0)
			{	
		    p+=two_dx       ;
			}	
		  else
		  {
			  x++              ;
			  p += two_dx_dy   ;
		  }
	  }  //while
	}	//else
}	    //end.

/*******************************************************************************/
/*                         Function Draw Circle 					           */
/*******************************************************************************/   
/* Function : Circle(CX,CY,Radius,tick,color,fill)		                       */
/* Input    : CX,CY       = Center Point of Circle							   */
/*            Radius      = radius of Circle					               */
/*            tick(1-100) = bold of line (have effect when fill=0)	           */
/*            color       = color of Circle						   	           */
/*            fill        = 0: No fill color in Circle,1:Fill color in Circle  */
/*******************************************************************************/
void Circle_Ver(int CX,int CY,unsigned char Radius,unsigned char tick,unsigned int color,unsigned char fill)
{
  int tswitch=0   ;
  unsigned char d,x=0,y=0,bd   ;

  d = CY-CX ;
	y = Radius ;
	tswitch = 3-(2*Radius);

	while(x<=y)	
	{
	  if(fill==1)
	  {
		  Line_Ver(CX-x,CY+y,CX+x,CY+y,1,color) ;
		  Line_Ver(CX-x,CY-y,CX+x,CY-y,1,color) ;
		  Line_Ver(CX-y,CY+x,CX+y,CY+x,1,color) ;
		  Line_Ver(CX-y,CY-x,CX+y,CY-x,1,color) ;
		}
	  else
		{
		  for(bd=0;bd<tick;bd++)
		  {
		    plot_dot_Ver((CX+x)+bd,(CY+y)+bd,color)     ;
		    plot_dot_Ver((CX+x)+bd,(CY-y)+bd,color)     ;

		    plot_dot_Ver((CX-x)+bd,(CY+y)+bd,color)     ;
		    plot_dot_Ver((CX-x)+bd,(CY-y)+bd,color)     ;

		    plot_dot_Ver((CY+y-d)+bd,(CY+x)+bd,color)   ;
		    plot_dot_Ver((CY+y-d)+bd,(CY-x)+bd,color)   ;

		    plot_dot_Ver((CY-y-d)+bd,(CY+x)+bd,color)   ;
		    plot_dot_Ver((CY-y-d)+bd,(CY-x)+bd,color)   ;
		  }
	  }

	  if(tswitch<0)
		{	
      tswitch += (4*x)+6     ;
		}	
	  else
	  {
      tswitch += 4*(x-y)+10  ;
		  y--;
	  }
    x++;
  }              //while	
}
  
/********************************************************************************/
/*                           Function Draw Ellipse 							    */
/********************************************************************************/   
/* Function : Ellipse(h,k,rx,ry,tick,color,fill)								*/
/* Input    : h,k         = Center Point of Ellipes  				            */
/*            rx,ry       = radius of Ellipse in X,Y axis			            */
/*            tick(1-100) = bold of line (have effect when fill=0)	            */
/*            color       = color of Ellipse						   	        */
/*            fill        = 0: No fill color in Ellipes,1:Fill color in Ellipes */
/********************************************************************************/ 
void Ellipse_Ver(unsigned int h,unsigned int k,long rx,long ry,unsigned char tick,unsigned int color,unsigned char fill)
{
  unsigned char bd       ;
  long  ax  =(rx*rx)     ;
  long   by  =(ry*ry)    ;
  float  ax2 =(ax*2)     ;
  float  by2 =(by*2)     ;

  long   x   = 0         ;
  long   y   = ry        ;

  float  fx  = 0         ;
  float  fy  = (ax2*ry)  ;

  float     p=(int)(by-(ax*ry)+(0.25*ax)+0.5);


	if(fill==1)
	{
	  Line_Ver(h-x,k+y,h+x,k+y,1,color) ;
		Line_Ver(h-x,k-y,h+x,k-y,1,color) ;
	}
	else
	{
    for(bd=0;bd<tick;bd++)
		{
      plot_dot_Ver((h+x)+bd,(k+y)+bd,color)      ;
		  plot_dot_Ver((h+x)+bd,(k-y)+bd,color)      ;
		  plot_dot_Ver((h-x)+bd,(k-y)+bd,color)      ;
		  plot_dot_Ver((h-x)+bd,(k+y)+bd,color)      ;
		}
  }
			   
  while(fx<fy)
	{
    x++;
	  fx+=by2;

	  if(p<0)
		{	
		  p+=(fx+by);
		}	
	  else
		{
		  y--;
		  fy-=ax2;
		  p+=(fx+by-fy);
		}

		if(fill==1)
	  {
		  Line_Ver(h-x,k+y,h+x,k+y,1,color) ;
		  Line_Ver(h-x,k-y,h+x,k-y,1,color) ;
	  }
	  else
	  {
		  for(bd=0;bd<tick;bd++)
		  {		  
		    plot_dot_Ver((h+x)+bd,(k+y)+bd,color)      ;
		    plot_dot_Ver((h+x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Ver((h-x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Ver((h-x)+bd,(k+y)+bd,color)      ;
		  }
		}
	}
  p=(int)((by*(x+0.5)*(x+0.5))+(ax*(y-1)*(y-1))-(ax*by)+0.5);
  while(y>0)
	{
	  y--                           ;
	  fy-=ax2                       ;

	  if(p>=0)
		{	
		  p+=(ax-fy)                     ;
		}	
	  else
		{
		  x++                         ;
		  fx+=by2                     ;
		  p+=(fx+ax-fy)               ;
		}

		if(fill==1)
	  {
		  Line_Ver(h-x,k+y,h+x,k+y,1,color) ;
		  Line_Ver(h-x,k-y,h+x,k-y,1,color) ;
	  }
	  else
	  {
		  for(bd=0;bd<tick;bd++)
		  {
		    plot_dot_Ver((h+x)+bd,(k+y)+bd,color)      ;
		    plot_dot_Ver((h+x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Ver((h-x)+bd,(k-y)+bd,color)      ;
		    plot_dot_Ver((h-x)+bd,(k+y)+bd,color)      ;
		  }
		}   //else	    
	} 
}

/************************************************************************************/
/*                          Function Draw a rectangle 			                    */
/************************************************************************************/
/* Function : Rectan(x1,y1,x2,y2,tick,color,fill)									*/	
/* Inputs   : (x1, y1)    = Start Point coordinate Rectan							*/
/*            (x2, y2)    = End Point coordinate  Rectan							*/
/*            tick(1-100) = bold of line(have effect when fill=0					*/
/*            color       =  color of rectangle									    */
/*            fill        = 0: No fill color in rectangle,1:Fill color in rectangle */
/*       																		    */
/*            (x1,y1) -------------												    */
/*                   |             |											    */
/*                   |             |											    */
/*                   |             |											    */
/*                   |             |											    */
/*                    -------------	(x2,y2)										    */
/*																					*/
/************************************************************************************/																						  
void Rectan_Ver(int x1,int y1,int x2,int y2,unsigned char tick,unsigned int color,unsigned char fill)
{
  unsigned int xmin,xmax,ymin,ymax,i ;
  unsigned char tk = tick-1  ;
    
  if(fill==1)
  {
    if(x1 < x2)                            //  Find x min and max
    {
      xmin = x1;
      xmax = x2;
    }
    else
    {
      xmin = x2;
      xmax = x1;
    }
         
    if(y1 < y2)                            // Find the y min and max
    {
      ymin = y1 ;
      ymax = y2 ;
    }
    else
    {
      ymin = y2 ; 
      ymax = y1 ;
    }
         
    for(; xmin<=xmax;xmin++)
    {
      for(i=ymin; i<=ymax; i++)
      plot_dot_Ver(xmin,i,color)  ;
    }
  }
    else   // No-Fill
     {
       Line_Ver(x1,y1-tk,x2,y1-tk,tick,color) ; 
       Line_Ver(x1,y2,x2,y2,tick,color)       ;
       Line_Ver(x1,y1,x1,y2,tick,color)       ;
       Line_Ver(x2-tk,y1,x2-tk,y2,tick,color) ;
     }      
       
}

/**********************************************************************/
/*                         Function Print String                      */
/**********************************************************************/
/* Parameter : *str = Charecter ASCII (String)					      */
/*             cur_x,cur_y = Position X,Y for begin plot text by will */
/*                           begin plot from bottom left to top left  */
/*             fg_color = color of Text								  */
/*             bg_color = color Background of text                    */											
/**********************************************************************/
void lcd_printStr_Ver(char *str,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color)
{
  unsigned char i;

	for (i=0; str[i] != '\0'; i++)
	{					 
	  text_7x11_Ver(str[i],cur_x,cur_y,fg_color,bg_color) ;
	  cur_x += 8                                      ; 		
	}
}

/***********************************/
/* Function Print Text 3 Charecter */
/***********************************/
void lcd_print3Cha_Ver(char ch1,char ch2,char ch3,int cur_x,int cur_y,unsigned int fg_color,unsigned int bg_color)
{	  			 
  text_7x11_Ver(ch1,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8 ; 
  text_7x11_Ver(ch2,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8 ; 
  text_7x11_Ver(ch3,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8 ;  	
}

/***********************************************/
/* Function Print Text 4 Character rma_21122013*/
/***********************************************/
void lcd_print4Cha_Ver(char ch1,char ch2,char ch3,char ch4,
                       long cur_x,long cur_y,long fg_color,long bg_color)
{	  			 
  text_7x11_Ver(ch1,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8; 
  text_7x11_Ver(ch2,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8; 
  text_7x11_Ver(ch3,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8;  	
	text_7x11_Ver(ch4,cur_x,cur_y,fg_color,bg_color);
  cur_x += 8;  	
}

/*************************************/
/*         Function Plot TAB         */
/*************************************/
/* Parameter : color = color for tab */
/*************************************/	     
void plot_tab_Ver(unsigned int color)
{
  unsigned char rx,ry;
	  
	for(ry=0;ry<6;ry++)
	{ 	  
		GLCD_SetCursor(200,309+ry); 	 	     
	  GLCD_Write_Command(0x2C);  //Command Write Data RGB	
	  for(rx=0;rx<26;rx++)
		{
      GLCD_Write_Data16(color);  //write data
		}	
	}
}

/*************************************************************************/
/*                    Function build Button (Draw-rectang)               */
/*    Start Plot button from  botton left to botton Right of button      */  
/*************************************************************************/
/*																	     */
/* Inputs : px,py      = Position X,Y  begin build  button               */
/*          width,high = width and high of button(unit Pixel)            */
/*                       for width not exceed 50 pixel will good         */
/*          c_bt       = Color of button                                 */
/*          *tx        = ASCII Charecter on Button                       */
/*                       1 Charecter width = 9pixel. Number pixel of	 */
/*                       charecter total must less pixel width of button */  
/*                     												     */
/*          num_tx     = Number Charecter at display on button           */
/*          c_tx       = Color charecter on button                       */
/*       																 */
/*          |<--- width --->|								    	     */
/*           -------------->   ---									     */
/*          |           end |	 ^									     */
/*          |               |	 |									     */
/*          |               |	high								     */
/*          |start          |	 |									     */
/*   (px,py) -------------->	---									     */
/*																	     */
/*************************************************************************/
void button_build_Ver(unsigned int px,unsigned int py,unsigned int width,unsigned int high,unsigned int c_bt,char *tx,unsigned char num_tx,unsigned int c_tx)      
{
  unsigned int x1,y1,x2,y2,ay,tpx  ;
  unsigned char tab,n              ;
  unsigned int cbt1=0,cbt2=0,sub=0 ;

  switch(c_bt)
	{
    case BT_RED   :
		                c_bt = 0xF800    	; //Color 
					    cbt1 = c_bt         ; //color start Max
					    cbt2 = 0x9800       ; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0800       ; //decrement color
		break;

		case BT_GREEN :
		                c_bt = 0x07C0    	; //Color 
					    cbt1 = c_bt         ; //color start Max
					    cbt2 = 0x04D0       ; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0040       ; //decrement color		         
		break;

	  case BT_BLUE   :
		                c_bt = 0x001F    	; //Color 
					    cbt1 = c_bt         ; //color start Max
					    cbt2 = 0x0014       ; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0001       ; //decrement color
		break;

    case BT_YELLOW :
		               	c_bt = 0xFFC0    	; //Color 
					    cbt1 = c_bt         ; //color start Max
					    cbt2 = 0x9CC0       ; //color last  Min(cbt1-(sub*12))
					    sub  = 0x0840       ; //decrement color
		break;

		case BT_WHITE  :
		                c_bt = 0xFFFF    	; //Color 
					    cbt1 = c_bt         ; //color start Max
					    cbt2 = 0x9CF3       ; //color last  Min (cbt1-(sub*12))
					    sub  = 0x0841       ; //decrement color
		break;

	  case BT_CYAN  :
		                c_bt = 0x87FF    	; //Color 
					    cbt1 = c_bt         ; //color start Max
					    cbt2 = 0x24F3       ; //color last  Min(cbt1-(sub*12))
					    sub  = 0x0841       ; //decrement color
		break;


	  case BT_MAGENTA :
		                 c_bt = 0xFC1F     	 ; //Color 
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0x9913       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0841       ; //decrement color
		break;

	  case BT_BROWN :
		                 c_bt = 0xEC00    	; //Color 
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0x8900       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0840       ; //decrement color
		break;
	
//-----------------------------------------------------------------------------------

	  case BT_LRED  :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0xFCF3       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0041       ; //decrement color
		break;	
				 
	  case BT_LGREEN :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0x9FF3       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0801       ; //decrement color
		break;	
				 
	  case BT_LBLUE :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0x9CFF       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0840       ; //decrement color
		break;

		case BT_LYELLOW :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0xFFC3       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0001       ; //decrement color
		break;
         
		case BT_LWHITE :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0xFFDE      ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0021       ; //decrement color
		break;

		case BT_LCYAN :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0x9FFF       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0800       ; //decrement color
		break;
						
		case BT_LMAGENTA :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0xFCFF       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0040       ; //decrement color
		break;	
				 
	  case BT_LBROWN :
		                 c_bt = 0xFFFF       ;
					     cbt1 = c_bt         ; //color start Max
					     cbt2 = 0xFE73       ; //color last  Min(cbt1-(sub*12))
					     sub  = 0x0021       ; //decrement color
		break;												
  }

  x1   = px           ;    
	y1   = py           ;
  x2   = px+width     ; 
  y2   = py-high+1    ;
  
  high  = high/2      ;
  ay    = py-high     ;
  tab   = high/12     ;
	
	if(tab==0)
	{	
	  tab=1            ;
	}	
	else
	{
	  if((high%12)>3)
		{	
	    tab = tab+1     ;
		}	
	}
  //---------Start Plot halft button from botton to top ---------
  
  while(y1>=ay)
  {
    for(n=0;n<tab;n++)
    {
      Line_Ver(x1,y1,x2,y1,1,cbt1) ;
      y1--                     ;
	   
	    if(y1 <= ay)
			{	
	      n=n+20                  ;
			}	
    }
    cbt1 = cbt1-sub           ;
     
	  if(cbt1<cbt2)
		{	
      cbt1=cbt2               ;
		}	
  }
  
  //---------Plot halft button from top to botton---------

  cbt1 = c_bt                   ; 
  while(y2<=ay)
  {
    for(n=0;n<tab;n++)
    {
      Line_Ver(x1,y2,x2,y2,1,cbt1) ;
      y2++                     ;
	 
	    if(y2 >= ay)
			{	
	      n=n+20                 ;
			}	
    }
    cbt1 = cbt1-sub            ;
     
	  if(cbt1<cbt2)
		{	
      cbt1=cbt2                ;
		}	
  }

  //----------write Text ---------
	
  tpx = (width-(num_tx*8))/2  ;
	tpx = tpx+px                ;
		
	py = (py-high)+6            ;
	
	for(n=0; tx[n] != '\0'; n++)
	{					 
	  text_7x11_Ver(tx[n],tpx,py,c_tx,no_bg) ;
	  tpx += 9                           ; 		
	}
}

/*********************************************************************/
/*         Function Plot Picture(BMP) to Display                     */
/*  Start Plot data of picture from  top left to top Right           */
/*********************************************************************/
/*  Inputs : *bmp       = Name reg. array keep data of picture       */
/*           xad,yad    = Position X,Y for begin plot data picture   */
/*           width,high = Size Picture unit Pixel                    */
/*********************************************************************/
void plot_picture_Ver(char *bmp,unsigned int xad,unsigned int yad,unsigned int width,unsigned int high)        
{
  unsigned long pt=0,picture;
  unsigned int hi,wid;
  unsigned int xad1,yad1;

	xad1 = xad;
	yad1 = yad;

	for(hi=0;hi<high;hi++)		                   //Number Y-Pixel(High) of Picture
	{      		
    GLCD_SetY(yad1);  //Set Adddress Ver(Y)
	
    for(wid=0;wid<width;wid++)	               //number X-pixel(Width) of Picture  
		{
      GLCD_SetX(xad1);  //Start Adddress Hor(X)=0		   				 
			GLCD_Write_Command(0x2C);  //Command Write data 				 	   
      picture = bmp[pt+1];  //Read data picture byte High 
			picture =	picture <<8;  
			picture|= bmp[pt];  //read data picture Byte low
      GLCD_Write_Data16(picture);
			pt = pt+2;  //Increment pointer of data picture 2 byte			    
			xad1++;
		}
		yad1++;
		xad1 = xad;
	}
}

  
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
