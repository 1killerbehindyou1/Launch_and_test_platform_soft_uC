/*
 * procedury_obsługi_sprzetu.c
 *
 * Created: 02.07.2019 08:21:07
 *  Author: Kolunio
 */ 
#include <avr/sfr_defs.h>
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <util/delay.h>
#include "I2C_dev.h"
#include "adc.h"
#include "pot.h"
#include "zasilacz.h"
/*
**************GPIO******************
void ustaw_przek(int a)
{
	
}
*/
//*************ZASILACZ************
/*
int ustaw_ch (float val, uint8_t kan, uint8_t dev )
{


	
}*/
/*
int check_ovc(int ch )
{
}
*/
void Skan_zas(uint8_t ch)
{
	rsPrint("CH3:");
    check_ovc(ch);
 
	rsPrintfloat(adc_run_one(vol)* divVol1);

	rsPrint("V\n\r");
	
}

float set_vol(uint8_t ch)
{
	//pcf8574_setoutput( dev, 1);
	//_delay_ms(200);
	float f;
	switch (ch)
	{
		case 1:f= CH1.voltage;
		break;
		case 2: f= CH2.voltage;
		break;
		case 3: f= CH3.voltage;
		break;
		case 4:f= CH4.voltage;
		break;
	}
	
	
	if((adc_run_one(vol)* divVol1)>f)
	{
		while(((adc_run_one(vol)-f)* divVol1)>0.1)
		{
			rsPrintfloat(adc_run_one(vol));
			rsPrint("\n\r");
			trig_pot(down);
		}
	}
	else
	{
		while(((adc_run_one(vol)-f)* divVol1)>0.1)
		{
			rsPrintfloat(adc_run_one(vol));
			rsPrint("\n\r");
			trig_pot(up);
		}
	}
	//pcf8574_setoutput( dev, 0);
	return (adc_run_one(vol)-f)* divVol1;
}

 
 void check_ovc(uint8_t ch)
 {
	 	float f;
	 	switch (ch)
	 	{
		 	case 1:f= CH1.current;
		 	break;
		 	case 2: f= CH2.current;
		 	break;
		 	case 3: f= CH3.current;
		 	break;
		 	case 4:f= CH4.current;
		 	break;
	 	}
		if((adc_run_one(curr)* divCurr1)>f)
		{
			while(((adc_run_one(curr)-f)* divCurr1)>0.01)
			{
				rsPrintfloat(adc_run_one(curr));
				rsPrint("\n\r");
				trig_pot(down);
			}
		}
		else
		{
			while(((adc_run_one(curr)-f)* divCurr1)>0.01)
			{
				rsPrintfloat(adc_run_one(curr));
				rsPrint("\n\r");
				trig_pot(up);
			}
		} 
		rsPrintfloat(adc_run_one(curr));
		rsPrint("A\t");
 }
 
 
 
 
