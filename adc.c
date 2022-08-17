/*
 * CFile1.c
 *
 * Created: 11.07.2019 07:11:52
 *  Author: Kolunio
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "adc.h"
void adc_init(void)
{
	ADCSRA = (1<<ADEN) |(1<<ADPS0) |(1<<ADPS1)|(1<<ADPS2);// ustawienie preskalera na 128//ADEN=1 w��czenie przetwornika ADC)
	
	ADMUX  =    (1<<REFS1) | (1<<REFS0)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0); 
	    //kanal 7 pin PA7
	// REFS1:0: wyb�r napi�cia odniesienia ADC
	//na wewn�trzne �r�d�o 2,56V
	//z zewn�trznym kondensatorem na pinie AREF
	
	// wyb�r kana�u ADC5 na pinie PA5
}

float adc_run_one(void)
{
	
	
	ADCSRA |= (1<<ADSC); //ADSC: uruchomienie pojedynczej konwersji
	while(ADCSRA & (1<<ADSC)); //czeka na zako�czenie konwersji  sprawdzanie flagi
	
	float result =(float)ADC * 2.56/1024;
	
	return result;
	
}