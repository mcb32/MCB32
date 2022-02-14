/** @file  gragik_lib.h
*  @brief  Header File für Defines, Variablen und prototypen  (MCB32)
* 
*         
*  @author  mal  
*  @date    28.3.2015
*  @version 1.0    
*  @bug     No known bugs. ...
*/
//=================================Includes===================================== 

#ifndef  _generic_lib_h // nachdem das *.h fFile hier zum ersten mal geladen wird soll sich der Präprozessor das EInbinden merken
#define _generic_lib_h
 // in anderen Files    #include "grafik_lib.h"         // Defines, Variablen und Prototypen


#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen und Grafik
#include <math.h>             // lib für Sinus
#include <stdlib.h>						// lib für rand()




#define PI 3.14159f // Konstante PI
#define WORD    unsigned int
#define uchar   unsigned char
#define ON     1
#define OFF    0 

// void ldelay_ms(unsigned int uidelayms);   // local_Delay_in_millisekunden
void ldelay_ms(unsigned int uidelayms)   // local_Delay_in_millisekunden
{
	unsigned int uit, uita;
	for(uita=uidelayms;uita>0;uita--)
	{
		for(uit=8000;uit>0;uit--);    // 12000 = 1mS, 8000 aus Messung im lokalen Kontext
	}
}

#endif


