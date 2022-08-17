/*
 * CFile1.c
 *
 * Created: 11.07.2019 07:11:52
 *  Author: Kolunio
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/sfr_defs.h>
#include "adc.h"
void adc_init(void)
{
	ADCSRA = (1<<ADEN) |(1<<ADPS0) |(1<<ADPS1)|(1<<ADPS2);// ustawienie preskalera na 128//ADEN=1 w³¹czenie przetwornika ADC)
	
	ADMUX  =     (1<<REFS0);//|(1<<REFS1); //kana³ a0, napiecie 5v
	    //kanal 7 pin PA7
	// REFS1:0: wybór napiêcia odniesienia ADC
	//na wewnêtrzne Ÿród³o 2,56V
	//z zewnêtrznym kondensatorem na pinie AREF
	
	// wybór kana³u ADC5 na pinie PA5
}

float adc_run_one()
{
	
	//ADMUX |= 0;
	ADCSRA |= (1<<ADSC); //ADSC: uruchomienie pojedynczej konwersji
	while(ADCSRA & (1<<ADSC)); //czeka na zakoñczenie konwersji  sprawdzanie flagi
	//ADMUX = _BV(MUX0) //przykl zm kan
	float result =(float)(ADCL | (ADCH <<8))/1024*5;

	return result;
	
}