/*
 * CFile1.c
 *
 * Created: 11.07.2019 10:15:20
 *  Author: Kolunio
 */ 
#define F_CPU 16000000
#include "PWM.h"
#include <avr/io.h>
#include "Procedury_dla_main.h"
#include "adc.h"
#include <util/delay.h>
#include "uart.h"

void PWM0_init(void)
{
	TCCR0 |= (1<<WGM00)|(1<<WGM01);//Tryb: Fast PWM 8bit
	
	TCCR0 |= (1<<COM00)|(1<<COM01);//Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
	TCCR0 = (1<<CS00);//Preksaler = 64,  fpwm = 976,5Hz (16 000 000Hz / 64 / 256).
	
	DDRB = (1<<PINB3);    //wyjœcie pwm 
	
	OCR0= 0;
	
}

void timer2_init(void)
{
	//praca w przerwaniu od przepelnienia, preskaler 256, wysylanie danych co 3,5ms
	TCCR2 |= (1<<CS02);
	TIMSK |= (1<<TOIE2);
}

 void PWM0_set_V(float f)
 {
	 
	 while(((adc_run_one()-f)>0.10)||((adc_run_one()-f)<-0.10))
	 {
		 OCR0 ++;
		
	
	 }
	 
	 
	 
 }
