#include <avr/io.h>
#include <avr/cpufunc.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "Color.h"
#include "IntToRGB.c"

int main(void)
{
	// Initialize the display
	DisplayInit();
	// All pixels white (background)
	DisplayOn();
	InitScrolling();
	DisplayGradient();
	
	unsigned int i = 0;
	
	
	while (1)
	{
		
		MoveDisplay();
		DrawBins();
		_delay_ms(130);
		UpdateDisplayPointer(40);
	}	
}