/*
 * mic_module_test.c
 *
 * Created: 24-May-18 15:48:18
 * Author : madslund
 */ 

#include <avr/io.h>
#include "mic.h"

int main(void)
{
	float freq = 0, spl = 0;
	initRecorder();
    
    while (1) 
    {
		record();
		analyze();
		freq = getTopFrequency();
		spl = getSoundPressureLevel();
		//PORTB = ~PORTB;		// Disable if display is connected
    }
}

