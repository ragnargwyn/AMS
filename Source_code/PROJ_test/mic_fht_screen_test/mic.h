/*
 * mic.h
 *
 * Created: 24-May-18 15:51:47
 *  Author: madslund
 */ 


#ifndef MIC_H_
#define MIC_H_

#define FHT_N 256
#define LOG_OUT 1
#define OCTAVE 0

#define SAMPLE_RATE 20000
#define DELTA_F SAMPLE_RATE/FHT_N

#define SPL_REF 6

void initRecorder();
void init_ADC();
void init_Timer1();

void record();
void analyze();

char * getFHTptr();
float getTopFrequency();
float getSoundPressureLevel();



#endif /* MIC_H_ */