/*
 * Mic_test.c
 *
 * Created: 05-Apr-18 10:58:46
 * Author : Mads H. Lund
 */ 
#define FHT_N 256
#define LOG_OUT 1
#define OCTAVE 1
#define F_CPU 16000000


#include "FHT.h"

void init_ADC();
void init_Timer1();


#include <avr/io.h>
#include <avr/interrupt.h>


//volatile short fht_input[FHT_N];
volatile short FHT_index = 0; 
volatile short full_index = 0;

void init_ADC()
{
	// ADC0 (PF0) is input
	DDRF &= 0b11111110;
	// Internal 5 volt reference, ADLAR = 0, Input = ADC0 single ended
	ADMUX = 0b01100000;
	// ADC enable
	// ADC interrupt enabled
	// Automatic start enabled
	// ADC prescaler = 32 (=> ADC clock = 16 MHz / 32 = 500 kHZ)
	ADCSRA = 0b10101101;
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

ISR(TIMER1_COMPB_vect)
{
	
}

ISR(ADC_vect)
{
	cli();
	int k = ADCH<<2;
	k -= 0x0200;
	k <<= 6;
	fht_input[FHT_index] = k;
	//fht_input[FHT_index] = ADCH<<2;
	
	FHT_index++;
	if(FHT_index == 256)
	{
		FHT_index = 0;
		full_index = 1;
		ADCSRA &= 0b01111111;
	}
	sei();
		
}
 
int main()
{
  init_ADC();
  init_Timer1();
  sei();  	
  while(1)
  {
	PORTB = ~PORTB;
	if (full_index == 1)
	{
		//full_index = 1;
		//ADCSRA |= 0b10000000;
		fht_window();
		fht_reorder();
		fht_run();
		fht_mag_log();
		full_index = 2; 
	}	 
	while(full_index == 2)
	{
		PORTB = 0xFF;

	}
  }
}