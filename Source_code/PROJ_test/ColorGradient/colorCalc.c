/*
 * color.c
 *
 * Created: 24-May-18 11:36:57
 *  Author: madslund
 */ 
#include "colorCalc.h"

char colorB[3]={0xBB, 0xBB, 0x11};
char colorA[3]={0x33, 0x33, 0x88};

void getColor565(float val, char * rgbPtr)
{
	rgbPtr[0] = (colorA[0] + val *(colorB[0] - colorA[0]));
	rgbPtr[1] = (colorA[1] + val *(colorB[1] - colorA[1]));
	rgbPtr[2] = (colorA[2] + val *(colorB[2] - colorA[2]));
	rgbPtr[0] = rgbPtr[0]>>3;
	rgbPtr[1] = rgbPtr[1]>>2;
	rgbPtr[2] = rgbPtr[2]>>3;
}

void generateGradientRGB(int size, char* RR, char* GG, char* BB)
{
	int i;
	float factor = 0;
	char temp_color[3] = {0};
		
	for (i = 0; i < size; i++)
	{
		factor = (float)i/(float)size;
		getColor565(factor, temp_color);
		
		RR[i] = temp_color[0];
		GG[i] = temp_color[1];
		BB[i] = temp_color[2];
	}
}