#include <avr/io.h>
#include <avr/cpufunc.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "Color.h"
#include "IntToRGB.c"
#include "mic.h"


int main(void)
{
	// Initialize the Recorder
	initRecorder();
	// Initialize the display
	DisplayInit();
	// All pixels white (background)
	DisplayOn();
	InitScrolling();
	DisplayGradient();
		
	while (1)
	{	
		record();
		analyze();
		MoveDisplay();
		DrawBins();
		//_delay_ms(130);
		UpdateDisplayPointer(40);
	}	
}