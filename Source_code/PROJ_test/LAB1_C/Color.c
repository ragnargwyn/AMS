/*
 * Color.c
 *
 * Created: 30-05-2018 16:16:43
 *  Author: rdixe
 */ 
#include <avr/io.h>
#include <avr/cpufunc.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "Color.h"
#include "IntToRGB.c"
#include "DummyFHT.c"

void DisplayGradient()
{
	for(unsigned long a = 0; a < 256; a++)
	{
		DrawLine(0, a, 40, IntToRGB[a][0], IntToRGB[a][1], IntToRGB[a][2]);
	}
}

void DrawBins()
{
	for(unsigned int i = 0; i < 120; i++)
	{
		DrawLine(invvsp, i*2, 40, IntToRGB[fht_log_out[i+1]][0], IntToRGB[fht_log_out[i+1]][1], IntToRGB[fht_log_out[i+1]][2]);
		DrawLine(invvsp, (i*2)+1, 40, IntToRGB[fht_log_out[i+1]][0], IntToRGB[fht_log_out[i+1]][1], IntToRGB[fht_log_out[i+1]][2]);
		
	}
}
