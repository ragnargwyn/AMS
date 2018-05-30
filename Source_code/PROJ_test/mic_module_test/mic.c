/*
 * mic.c
 *
 * Created: 24-May-18 15:51:14
 *  Author: madslund
 */ 

#include "mic.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "FHT.h"

//volatile short fht_input[FHT_N];
volatile short FHT_index = 0;
volatile short full_index = 0;

void init_ADC()
{
	// ADC0 (PF0) is input
	DDRF &= 0b11111110;
	// Internal 5 volt reference, ADLAR = 0, Input = ADC0 single ended
	ADMUX = 0b01100000;
	// ADC disabled... for now!
	// ADC interrupt enabled
	// Automatic start enabled
	// ADC prescaler = 32 (=> ADC clock = 16 MHz / 32 = 500 kHZ)
	ADCSRA = 0b00101101;
	// Auto trigger source = Timer 1 output comp B
	ADCSRB = 0b00000101;
}

void init_Timer1()
{
	DDRB = 0xFF;
	PORTB = 0;
	// timer1 in CTC mode
	// Output cmp B
	// Prescaler  = 1
	TCCR1A = 0b00010000;
	TCCR1B = 0b00001001;
	TIMSK1 = 0b00000100;
	// Calculated in MATLAB
	OCR1A = 399;
}

void initRecorder()
{
	init_ADC();
	init_Timer1();
	sei();
}

void record()
{
	full_index = 0;			// reset recording index
	ADCSRA |= 0b10000000;	// enable ADC
	while (!full_index)		// wait for recording
	{
	}
}

void analyze()
{
	fht_window();
	fht_reorder();
	fht_run();				// do the Fast Hartley Transform
	fht_mag_log();			// logarithmic output
	full_index = 2;			// raise fht_log_out is full
}


float getTopFrequency()
{
	int max = fht_log_out[1], pos = 1;
	
	for (int i = 2; i < FHT_N/2 ; i++ )
	{
		if (fht_log_out[i]>max)
		{
			max = fht_log_out[i];
			pos = i;
		}
	}
	return ((float)pos*DELTA_F);	
}

float getSoundPressureLevel()
{
	
	int sum = 0;
	float mean = 0;
	
	for (int i = 0; i < FHT_N/2; i++)
	{
		sum = sum + fht_log_out[i];
	}
	mean = (float)sum/FHT_N/2;
	return SPL_REF * mean;
}

float getOffsetLevel()
{
	
	int sum = 0;
	float mean = 0;
	
	for (int i = 0; i < FHT_N; i++)
	{
		sum = sum + fht_input[i];
	}
	mean = (float)sum/FHT_N;
	return mean;
}


ISR(TIMER1_COMPB_vect)
{
	// Empty ISR
	// Is necessary to make timer work
}

ISR(ADC_vect)
{
	cli();				// Do not disturb
	int k = ADCH<<2;	// Convert to 10 bits
	k -= 0x0200;		// Convert to signed
	k <<= 6;			// Convert to 16 bits
	fht_input[FHT_index] = k;
	
	FHT_index++;
	if(FHT_index == 256)
	{
		FHT_index = 0;
		ADCSRA &= 0b01111111;
		full_index = 1;
	}
	sei();				// Done
	
}