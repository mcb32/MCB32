/**
******************************************************************************
* @file    GPIO_3_joystick_buttons.c
* @author  MCB32 Application Note. R.Malacarne
* @version V1.15.15a
* @date    29.04.2015
* @brief   MCB32 IO Setup via Registerprogrammierung (APP-Note).
*          Schalter an PA[0], PC13 sowie Joystick PD15..11 werden gelesen und an LED PE[8..15] angezeigt. 
*          In dieser Version wird die minimale Konfiguration  der Programmierumgebung genutzt. 
*          Im  Run-Time Environment Manager (RTEM) ist nur das Minimum an
*          nötigen Files ausgewählt. 
* 
******************************************************************************/

#include <stm32f10x.h>

/**
	* @name   void system_init()
*   @brief 	Lokale System Init Funktion. 
*          	Die eigentliche SystemInit() Funktion
*          	wird gleich nach dem Reset im Reset_Handler aufgerufen. 
*          	(siehe system_stm32f10x.c und startup_stm32f10x_cl.s)
*          	OnBoard Buttons In PA_0 .. PC_13 
*          	 PA_0   Button_0  
*          	 PC_13  Button_1
*           Controlstick
*          	 StickSelect = 	PD_15 
*          	 StickDown 	= 	PD_14; 
*          	 StickLeft 	= 	PD_13; 
*          	 StickUp 	  = 	PD_12; 
*          	 StickRight 	= 	PD_11;
* @param  None
* @retval None      
*/

void system_init()
	{
		RCC->APB2ENR |=  1 <<  2;     // Enable GPIOA clock, Button0         
    RCC->APB2ENR |=  1 <<  4;     // Enable GPIOC clock, P0 und Button1          
    RCC->APB2ENR |=  1 <<  5;     // Enable GPIOD clock, ControlStick          
    RCC->APB2ENR |=  1 <<  6;     // Enable GPIOE clock, P1         
		
// GPIO --------------------------------------------------------------
		GPIOC->ODR    = 0xDFFF;				// Configure GPIOCL Input pullup. PC13 = PullDown auch wenn Floating gewählt ist.
    GPIOC->CRH   &= 0xFF0FFFFF;		// OnBoard Button1 (PC_13)  
    GPIOC->CRH   |= 0x00400000;		// Floating Input 1 Switch Button1

    GPIOD->ODR    = 0xFFFF;        // setze nun PullUp. für GPIOD11..15 ext. ControlStick
    GPIOD->CRH   &= 0x00000FFF;    // Configure the GPIOD (Bit 15-11) for 
    GPIOD->CRH   |= 0x88888000;    // Pullup Input 8 für ControlSwitch 

 		GPIOA ->CRL   = 0x88888888;		// PA7..0 als Input. Nun noch definieren ob wir Pull-Up oder Pull Down wollen. 
		GPIOA ->ODR   = 0x00FE;				// PA7..1 mit Pullup, PA0 mit PullDown wegen Button 0
		
		GPIOE->CRH    = 0x11111111;		// PE15-8 Output, Push Pull 10MHz
	}

/* Main                  */
int main(void){
	unsigned int Portbuffer=0;
	system_init();                // spezifischer System Init für die aktuelle Applikation

	while(1)
	{
		Portbuffer = ((GPIOA->IDR)&0x0001);											// lese Port A mit seinem Schalter Button_0
		Portbuffer = Portbuffer | (((GPIOC->IDR)&0x2000)>>12);  // Lese PC13 und schiebe es auf Bit1 (Button_1)
		Portbuffer = Portbuffer | (((GPIOD->IDR)&0xF800)>>8);   // Lese nun Controlstick  PD15..11 dazu. LED7...3  von PE15...7 sind der Stick		
		GPIOE->ODR = (0x00FF&Portbuffer)<<8;   									// setzte alle unnötigen Bits auf 0
		
	}
}  


/******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, CITYLINE AG SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2015 Cityline AG</center></h2>
******************************************************************************
*/

 
