/************************************************************
  File name: "TFTdriver.c"

  Driver for "ITDB02 320 x 240 TFT display module, Version 2"
  mounted at "ITDB02 Arduino Mega2560 Shield".
  Display controller = ILI 9341.
  
  Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

  Connections:
  DB15-DB8:   PORT A
  DB7-DB0:    PORT C

  RESETx:     PORT G, bit 0
  CSx:        PORT G, bit 1
  WRx:        PORT G, bit 2
  RS (=D/Cx): PORT D, bit 7

  Henning Hargaard, February 14, 2018
************************************************************/  
#include <avr/io.h> 
#include <avr/cpufunc.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"

// Data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW  PORTC

// Control port definitions:
#define WR_PORT PORTG
#define WR_BIT 2
#define DC_PORT PORTD
#define DC_BIT  7  //"DC" signal is at the shield called RS
#define CS_PORT PORTG
#define CS_BIT  1
#define RST_PORT PORTG
#define RST_BIT 0

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

// ILI 9341 data sheet, page 238
void WriteCommand(unsigned int command)
{
	
	WR_PORT &= ~(1<<WR_BIT);
	//apparently the upper byte is obsolete
	DATA_PORT_HIGH = command >> 8; 
	DATA_PORT_LOW = command;
	
	DC_PORT &= ~(1<<DC_BIT);
	
	CS_PORT &= ~(1<<CS_BIT);
	
	_NOP();
	
	WR_PORT |= (1<<WR_BIT);
	
	_NOP();			
}

// ILI 9341 data sheet, page 238
void WriteData(unsigned int data)
{
	DATA_PORT_HIGH = data >> 8;
	DATA_PORT_LOW = data;
	
	DC_PORT |= (1<<DC_BIT);
		
	CS_PORT &= ~(1<<CS_BIT);
	
	WR_PORT &= ~(1<<WR_BIT);
	
	_NOP();
	
	WR_PORT |= (1<<WR_BIT);
	
	_NOP();
}

// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////

// Initializes (resets) the display
void DisplayInit()
{
	//set up ports
	DDRA = 0xff;
	DDRC = 0xff;
	DDRG |= 0b00000111;
	DDRD |= 0b10000000;
	
	PORTG |=0b00000111;
	PORTD |=0b10000000;
	RST_PORT &= ~(1<<RST_BIT);
	
	_delay_us(500); //11ms
	
	RST_PORT |= (1<<RST_BIT);
	
	CS_PORT |= (1<<CS_BIT);
	WR_PORT |= (1<<WR_BIT);
	
	_delay_ms(130);
	
	SleepOut();
	
	DisplayOn();
	
	MemoryAccessControl(0b00001000);
	
	InterfacePixelFormat(0b00000101);
	
}

void AllPixelsOff()
{
	WriteCommand(0x22);
}

void AllPixelsOn()
{
	WriteCommand(0x23);	
}

void DisplayOff()
{
	WriteCommand(0x28);
}

void DisplayOn()
{
	WriteCommand(0x29);
}

void SleepOut()
{
	WriteCommand(0x11);
}

void MemoryAccessControl(unsigned char parameter)
{
	WriteCommand(0x36);
	WriteData(parameter);
}

void InterfacePixelFormat(unsigned char parameter)
{
	WriteCommand(0x3A);
	WriteData(parameter);
}

void MemoryWrite()
{
	WriteCommand(0x2C);
}

// Red 0-31, Green 0-63, Blue 0-31
//
void WritePixel(unsigned char Red, unsigned char Green, unsigned char Blue)
{ 	
	if (Red < 32 && Green < 64 && Blue < 32)
	{
		WriteData((Red << 11) | (Green << 5) | Blue);
	}
	
}

// Set Column Address (0-239), Start > End
void SetColumnAddress(unsigned int Start, unsigned int End)
{
		WriteCommand(0x2A);
		WriteData(Start>>8);
		WriteData(Start);
		WriteData(End>>8);
		WriteData(End);
}

// Set Page Address (0-239), Start > End
void SetPageAddress(unsigned int Start, unsigned int End)
{
		WriteCommand(0x2B);
		WriteData(Start>>8);
		WriteData(Start);
		WriteData(End>>8);
		WriteData(End);		
}

// Fills rectangle with specified colour
// (0,0) = Upper left corner. X horizontal (0-239) , Y vertical (0-339).
// Height (1-240) vertical. Width (1-320) horizontal.
// R-G-B = 5-6-5 bits.
void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	SetPageAddress(StartX, (Width+StartX));
	SetColumnAddress(StartY, (Height+StartY));	
	
	MemoryWrite();
	for (unsigned long i = 0 ; i < (unsigned long)Width*Height ; i++)
	{
		WritePixel(Red,Green,Blue);
	}
}