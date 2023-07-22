//----------------------------------------------------------------
// Libraryerzeugung  TouchP0P1.lib  / 15.11.14  /  rw  / BZU  
// Target Options / C/C++:  USE_STDPERIPH_DRIVER
// Target Optionen / Output: Create Lib / Name of exec: P0P1Touch
// Manage Runtime-Environment: 
// Device/StdPeriph Drivers/ Framework, GPIO, SPI, RCC, I2C
// in Device/r.Maus-Options: Framework, GPIO, SPI, RCC:
// -- Include in Library
// Es sind nur die 4 Sources: P0P1Shell.c, Touchgrafik.c (Rev D oder C!),
// TouchPrint.c und Peripherie.c einzubinden. Kein Main-File P0P1Count.c!
// Der Header zur Lib ist TouchP0P1.h.
// Die erzeugte Lib ist im Ordner Objects zu finden.

//----------------------------------------------------------------
// P0P1Count.c   / 15.11.14  /  rw  / BZU  
// Zählt am uC-Board MCB32 Display und P1-Port auf und ab.
// gesteuert vom P0-Port am Display oder externen Schaltern.  
//----------------------------------------------------------
//#include <stm32f10x.h>				// Mikrocontrollertyp
#include "TouchP0P1.h"				// P0/P1,8Bit,Touchscreen      

//-------------------------------------------------------------------
int main(void)
{
	long t;	 int ADDat; 	char Zeile[40] = "0000011111";
	unsigned char gpIn = 0x00, gpOut = 0x0F; 
	unsigned char Cnt = 0,  Temp = 0, DADat = 0x00;
	
  SystemInit();
	InitTouchP0P1("0");					// P0P1-Oberfläche OFF
	printAt(1, "I2C-Demo");
	printAt(2, "-------------------");

	I2C_InitI2C(I2C1);
	printAt(6, "1");

	// Init Expander MCP23008	
	I2C_start_ack(I2C1, 0x40, I2C_WRITE);
	I2C_write_ack(I2C1, 0x00);  // SubAdr = DirReg
	I2C_write_ack(I2C1, 0x0F);	// DirReg = Out7..4; IN3..0
	I2C_stop(I2C1);
	I2C_start_ack(I2C1, 0x40, I2C_WRITE);
	I2C_write_ack(I2C1, 0x06);  // SubAdr = PullUpReg
	I2C_write_ack(I2C1, 0x0F);	// PullUpReg = ON wenn Input
	I2C_stop(I2C1);
	I2C_start_ack(I2C1, 0x40, I2C_WRITE);
	I2C_write_ack(I2C1, 0x09);  // SubAdr = GPIOReg
	I2C_write_ack(I2C1, 0xA0);	// GPIOReg = 1010 0000
	I2C_stop(I2C1);

  // EEPROM 24LC02B, 256x8Bit, SCL max 400kHz
  I2C_start_ack(I2C1, 0xA0, I2C_WRITE);
	I2C_write_ack(I2C1, 0x00);  // MemAdr = 0x00 (..0xFF)
	I2C_write_ack(I2C1, 'E');   // 1st Byte PageMode
	I2C_write_ack(I2C1, 'E');   
	I2C_write_ack(I2C1, 'P');   
	I2C_write_ack(I2C1, 'R');   
	I2C_write_ack(I2C1, 'O');   
	I2C_write_ack(I2C1, 'M');   // last Byte SeqMode
	I2C_stop(I2C1);
  for(t=60000; t>0; t--);  		// Zwingend 10ms nach EPROM Write

	//printAt(6, "2");
  while (1)
  {
		// EEPROM 24LC02B,  Adr. 0xA0
    I2C_start_ack(I2C1, 0xA0, I2C_WRITE);
		I2C_write_ack(I2C1, 0x00);  // MemAdr 0x00..0xFF
		I2C_stop(I2C1);
		I2C_start_ack(I2C1, 0xA0, I2C_READ);
		Zeile[0] = I2C_read_ack(I2C1);
		Zeile[1] = I2C_read_ack(I2C1);
		Zeile[2] = I2C_read_ack(I2C1);
		Zeile[3] = I2C_read_ack(I2C1);
		Zeile[4] = I2C_read_ack(I2C1);
		Zeile[5] = I2C_read_nack(I2C1);
		I2C_stop(I2C1);
		Zeile[6] = '\0';
		printAt(4, "EEPROM     =        ");
		printAt(4, "EEPROM     = ");
		print(Zeile);
		
		// Temperatursensor MCP9801, Adr. 0x92
    I2C_start_ack(I2C1, 0x92, I2C_READ);
		Temp = I2C_read_nack(I2C1);
		I2C_stop(I2C1);
		printAt(6, "Temperatur =");
		printDec(6, Temp);
		
		// AD-Wandler MCP3221, Adr. 0x9A
    I2C_start_ack(I2C1, 0x9A, I2C_READ);
		ADDat = I2C_read_ack(I2C1);
		ADDat = (ADDat<<8) + I2C_read_nack(I2C1);
		I2C_stop(I2C1);
		printAt(8, "AD-Dat     =");
		printDec(6, ADDat);

		// DA-Wandler TC1321, Adr. 0x90
    I2C_start_ack(I2C1, 0x90, I2C_WRITE);
		I2C_write_ack(I2C1, 0x00);  // SubAdr = DataReg
		I2C_write_ack(I2C1, DADat);		// DataReg = DADat D9..D2
		I2C_write_ack(I2C1, 0x00);		// DataReg = DADat D1,D0
		I2C_stop(I2C1);
		if(DADat<0x40) DADat += 0x10;
		else DADat = 0x00;
		printAt(10, "DA-Dat     =");
		printDec(6, DADat);
	
		// 8-Bit I/O-Expander MCP23008, Adr. 0x40
		I2C_start_ack(I2C1, 0x40, I2C_WRITE);
		I2C_write_ack(I2C1, 0x09);  // SubAdr = GPIOReg
		I2C_write_ack(I2C1, gpOut);		// GPIOReg = gpio
		I2C_stop(I2C1);
		gpOut ^= 0xF0;
		printAt(14, "GPIO-OUT   =  ");
		printHex(8, gpIn);
	
		// 8-Bit I/O-Expander MCP23008, Adr. 0x40
		I2C_start_ack(I2C1, 0x40, I2C_WRITE);
		I2C_write_ack(I2C1, 0x09);  // SubAdr = GPIOReg
		I2C_stop(I2C1);
    I2C_start_ack(I2C1, 0x40, I2C_READ);
		gpIn = I2C_read_nack(I2C1);
		I2C_stop(I2C1);
		printAt(12, "GPIO-IN    =  ");
		printHex(8, gpIn);
		
		printAt(16, "Count      =  ");
		printDec(4, Cnt++);		
		for(t=120000; t>0; t--);  // Schlaufenverzögerung zB.10ms
  }
}

/*  ] */
/**  
*  @brief  Hilfe zu InitP0P1Touch
*
*    Hilfe zu InitP0P1Touch(1111);
*  -----------------------------
* 1)    InitP0P1Touch (0)   		                    Der Touchscreen bleibt ausgeschaltet,
*                                                  P0 ist als Input, P1 als Output konfiguriert
*
* 2)    InitP0P1Touch (1) ... ( 1  1  1  4 ) ;   		DerTouchscreen wird aktiviert und 	konfiguriert, 
*		                            ¦  ¦  ¦  ¦          optimal mit 1111
*		                            ¦  ¦  ¦  ¦___________  Gemeinsamer Schalter-/Tastertyp (T: 1..4)
*		                            ¦  ¦  ¦______________  Schrift vertikal (0), horizontal (1)
*		                            ¦  ¦_________________  LSB unten/links (0); oben/rechts (1)
*                               ¦____________________  P0 in Mitte (0), aussen (1)
*/
/*  ) */
