/*
 * TFTdriver.c
 *
 * Created: 23-05-2018 23:26:37
 *  Author: rdixe
 */ 

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
	DATA_PORT_HIGH = command >> 8;
	DATA_PORT_LOW = command;
	DC_PORT &= ~(1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
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
	DDRG |= 0b00000111;
	DDRD |= 0b10000000;
	
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	PORTG |= 0b00000111;
	PORTD |= 0b10000000;
	
	RST_PORT &= ~(1<<RST_BIT);
	
	_delay_us(500);
	RST_PORT |= ~(1<<RST_BIT);
	CS_PORT |= ~(1<<CS_BIT);
	WR_PORT |= ~(1<<WR_BIT);
	
	_delay_ms(130);
	
	SleepOut();
	
	DisplayOn();
	
	MemoryAccessControl(0b00011000);
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
void WritePixel(unsigned char Red, unsigned char Green, unsigned char Blue)
{
	WriteData((Red<<11) | (Green<<5) | Blue);
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

// Set Page Address (0-319), Start > End
void SetPageAddress(unsigned int Start, unsigned int End)
{
	WriteCommand(0x2B);
	WriteData(Start>>8);
	WriteData(Start);
	WriteData(End>>8);
	WriteData(End);
}

void DrawLine(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	SetPageAddress(StartX, (StartX+Width));
	SetColumnAddress(StartY, StartY);
	MemoryWrite();
	for (unsigned long i=0; i<(unsigned long)Width; i++)
	{
		WritePixel(Red,Green,Blue);
	}
}

void InitScrolling()
{
	WriteCommand(0x33);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x18);
	WriteData(0x00);
	WriteData(0x28);
}

void MoveDisplay()
{	
	WriteCommand(0x37);
	WriteData(vsp>>8);
	WriteData(vsp);
}

vsp = 0;
invvsp = 40;

void UpdateDisplayPointer(unsigned int width)
{
	vsp += width;	invvsp -= width;	if (vsp == 320){
		vsp = 40;
	}
	
	if (invvsp == 0){
		invvsp = 280;
	}
}

