/*
 * TFT.c
 *
 * Created: 24-02-2017 
 * Author : Henning Hargaard
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"
#include "colorCalc.h"

#define	SIZE 256

int main(void)
{
/**********CALCULATE VERSION *********/
//
	//char color[3] = {0};
	//float val = 0;
		//
	//DisplayInit();
	//
	//AllPixelsOff();
	//_delay_ms(1000);
	//AllPixelsOn();
	//_delay_ms(1000);
	//FillRectangle(0,0,320,240,31,63,31);
	//_delay_ms(1000);
	//DisplayOn();
	//while (1)
	//{
		//getColor565(val, color);
		//FillRectangle(0,0,320,240, color[0], color[1] , color[2]);
		//val = (val<1? val + 0.1 : 0);
				//
	//}
	//
	//
	//
/**********LOOK UP VERSION ***********/
  char index = 0;

  char RR[SIZE], GG[SIZE], BB[SIZE];
  
  DisplayInit();
  
  AllPixelsOff();
  _delay_ms(1000);
  AllPixelsOn();
  _delay_ms(1000);
  FillRectangle(0,0,320,240,31,63,31);
  _delay_ms(1000);
  DisplayOn();
  generateGradientRGB(SIZE,RR,GG,BB);
  while(1)
  {
	  
	  FillRectangle(0,0,320,240, RR[index], GG[index] ,BB[index]);
	  
	  if (index<(SIZE-1))
	  index++;
	  else
	  {
		  index = 0;
		  _delay_ms(500);
	  }
	  
  }
}
