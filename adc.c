/*
 * CFile1.c
 *
 * Created: 11.07.2019 07:11:52
 *  Author: Kolunio
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

void adc_init(void)
{
	ADCSRA = (1<<ADEN) |(1<<ADPS0) |(1<<ADPS1)|(1<<ADPS2);	// ustawienie preskalera na 128
	ADEN=1												//w??czenie przetwornika ADC)												
	ADMUX  =     (1<<REFS0);//|(1<<REFS1);					//kana? a0, napiecie 5v
																// REFS1:0: wybór napicia odniesienia ADC
																//na wewn?trzne zród?o 2,56V
																//z zewn?trznym kondensatorem na pinie AREF													
}

float adc_run_one(uint8_t ch)
{
	
	ADCSRA |= (1<<ADSC);									//ADSC: uruchomienie pojedynczej konwersji
	while(ADCSRA & (1<<ADSC));								//czeka na zakoñczenie konwersji  sprawdzanie flagi
	ADMUX = _BV(ch)										//przykl zm kan
	float result =(float)(ADCL | (ADCH <<8))/1024*5;
	return result;
}