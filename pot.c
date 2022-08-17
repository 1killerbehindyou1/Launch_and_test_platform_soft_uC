/*
 * pot.c
 *
 * Created: 25.07.2019 15:33:34
 *  Author: Kolunio
 */ 
#include "pot.h"
#include <util/delay.h>
#include "I2C_dev.h"
#include "uart.h"
#include <avr/io.h>
#include "adc.h"
#define sbi(PORT,PIN) ((PORT)|=1<<(PIN))
#define cbi(PORT,PIN) ((PORT)&=~(1<<(PIN)))

void pot_init(void)
{
	DDRC |= (1<< PORTC7)|(1<< PORTC6);
	PORTC |= (1<< PORTC7)|(1<<  PORTC6);
	//pcf8574_setoutput( ZAS, 255);
}



void trig_pot(uint8_t k)
{
	if(k == down)
	{
		sbi(PORTC,U_D);
		_delay_ms(10);
	}
	
	if(k == up)
	{
		cbi(PORTC,U_D);
		_delay_ms(10);
	}
	cbi(PORTC,INC);
	_delay_ms(1);
	sbi(PORTC,INC);
	_delay_ms(1);
}