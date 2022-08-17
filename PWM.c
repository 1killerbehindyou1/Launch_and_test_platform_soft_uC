/*
 * CFile1.c
 *
 * Created: 11.07.2019 10:15:20
 *  Author: Kolunio
 */ 
#define F_CPU 16000000UL
#include "PWM.h"
#include <avr/io.h>
#include "Procedury_dla_main.h"
#include "adc.h"
#include <util/delay.h>
#include "uart.h"
//#include "zasilacz.h"

void PWM0_init(void)
{
//TCCR4D = 0;
TCCR4B |= (1<<PWM4X);				//dla wgm40=0 i wgm41 =0 tryb pwm ( w rej tccr4d)
	TCCR4C |= (1<<COM4D0);			//w³aczenie piunów oc4d i /oc4d jako wyjscia pwm
	TCCR4C |= (1<<PWM4D);			//PWM timer4 based on comparator D dla pinów PD6 i PD7
									//Preksaler = 64,  fpwm = 976,5Hz (16 000 000Hz / 64 / 256).
	
	DDRD = (1<<PIND7 );				//wyjscie pwm 
	//  TCCR4D|= (1<<WGM40);
	 OCR4D= 127;
	
}

/*
void timer2_init(void)
{
									//praca w przerwaniu od przepelnienia, preskaler 256, wysylanie danych co 3,5ms
	TCCR2 |= (1<<CS02);
	TIMSK |= (1<<TOIE2);
}*/
/*
 void PWM0_set_V(float f,uint8_t vol)
 {
	 OCR4D =(uint8_t)f;
	 
	 rsPrintfloat(adc_run_one(vol));
	 while(((adc_run_one(vol)-f)>0.10)||((adc_run_one(vol)-f)<-0.10))
	 {
		 OCR4D ++;
		
	rsPrintfloat(adc_run_one(vol));
	 }
	 

 }*/
