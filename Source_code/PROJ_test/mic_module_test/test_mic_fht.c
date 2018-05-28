/*
 * mic_module_test.c
 *
 * Created: 24-May-18 15:48:18
 * Author : madsl
 */ 

#include <avr/io.h>
#include "mic.h"

int main(void)
{
	float freq = 0, spl = 0;
	initRecorder();
    /* Replace with your application code */
    while (1) 
    {
		record();
		analyze();
		freq = getTopFrequency();
		spl = getSoundPressureLevel();
		//PORTB = ~PORTB;		// Disable if display is connected
    }
}

