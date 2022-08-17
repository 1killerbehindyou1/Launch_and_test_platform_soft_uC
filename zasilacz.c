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
#include "Procedury_dla_main.h"
extern  buffer_TX;

Zasilacz CHANNEL[]={			//Tablica struktur dla 4 zasilaczy z domyślnymi wartościami
	{volt_zero,curr_zero, RES1, DivCurCH4, DivCurCH1, disable, voltage_source, curr_zero, volt_zero, Pow},
	{volt_zero,curr_zero, RES2, DivCurCH2, DivCurCH2, disable, voltage_source, curr_zero, volt_zero, Pow},
	{volt_zero,curr_zero, RES3, DivCurCH3 ,DivCurCH3, disable, voltage_source, curr_zero, volt_zero, Pow},
	{volt_zero,curr_zero, RES4, DivCurCH4, DivCurCH4, disable, voltage_source, curr_zero, volt_zero, Pow},
};

float SkanVol(uint8_t ch)					//mierzy wartość napieciowego wyj odpowiedniego dla "ch"
{																						//jest 8 kanałów po 2 dla kazdfego kanału
	return (adc_run_one(2*ch -1)*  CHANNEL[ch-1].DivVol);     //obliczanie rzeczwistego napiecia po uwzglednieniu dzielnika
}	
void SetVol(uint8_t ch, float f)			//ustawia napięcie wyjściowe w trybie zródła napiecia
{
		CHANNEL[ch-1].Voltage =f;
		
		if(SkanVol(ch)>CHANNEL[ch-1].Voltage)
		{
			while((SkanVol(ch)-CHANNEL[ch-1].Voltage)>0.1)
			{
				trig_pot(ch,down);
			}
		}
		else
		{
			while((SkanVol(ch)-CHANNEL[ch-1].Voltage)>0.1)
			{
				trig_pot(ch,up);
			}
		}
	
	
}

void SetOvCurr(uint8_t ch, float f)			//ustawia wartosc ograniczenia napieciowego w trybie źr pradowego
{
	CHANNEL[ch-1].OvCurr = f;
}

 void CheckOvCurr(uint8_t ch)				//sprawdza czy nie przekroczono ograniczenia pradowego i zmniejsza nap
 {
	 if(SkanCurr(ch) > CHANNEL[ch-1].OvCurr)
	 {
		 rsPrint("CH");
		 rsPrintfloat(ch);
		 rsPrint("\tOver Current flag!\t");
		 rsPrintfloat( CHANNEL[ch-1].OvCurr);
		 rsPrint("\tA\n\r");
		 
		 while(SkanCurr(ch)- CHANNEL[ch-1].OvCurr > 0.02)
		 {
			 trig_pot(down, ch);
		 }
	 }
 }
	
 void SetVoltageSource(uint8_t ch)			//ustawia tryb pracy przetwornicy na żródło pradowe z ogr napiecia
 	{
	 	
	 	CHANNEL[ch-1].MOD =voltage_source;
 	}
 	
 float SkanCurr(uint8_t ch)					//mierzye wartosci aktualnie płynacego pradu odpowiedniego dla "ch"
 {
	 float v1= adc_run_one(2*ch -1)* CHANNEL[ch-1].DivVol;
	 float v2= adc_run_one(2*ch )* CHANNEL[ch-1].DivCurr;
	
	 float f=(v1-v2)/ CHANNEL[ch-1].ResCurr;				//wyznaczanie wartosci prądu po														//uwzglednieniu wartosci rezystora bocznikujacego
															//przypisanie aktualnej wartpości do pola "Current" struktury
	 return f;
	 
 }

void SetCurr(uint8_t ch, float f)			//ustawia wartosc pradu roboczego napieciowego w trybie źr pradowego
		{
			CHANNEL[ch-1].Current = f;
		}
			
void CheckCurr(uint8_t ch)					//reguluje prąd wyjciowy w trybie zródła pradowego
 {
	
	
		CHANNEL[ch-1].Current ;
		
		if(SkanCurr(ch)>CHANNEL[ch-1].Current)
		{
			while((SkanCurr(ch)-CHANNEL[ch-1].Current)>0.1)
			{
				trig_pot(ch,down);
			}
		}
		else
		{
			while((SkanCurr(ch)-CHANNEL[ch-1].Current)>0.1)
			{
				trig_pot(ch,up);
			}
		}
	}

void SetOvVol(uint8_t ch, float f)			//ustawia wartosc ograniczenia napieciowego w trybie źr pradowego
 {
	 CHANNEL[ch-1].OvVol = f;
 }

void CheckOvVol(uint8_t ch)					//sprawdza czy nie przekroczono ograniczenia napięciowego i zmniejsza nap
 {
	 
	if(SkanVol(ch)> CHANNEL[ch-1].OvVol)
	{
		while((SkanVol(ch)-CHANNEL[ch-1].OvVol)>0.1)
		{
			trig_pot(ch,down);
		}
	}
 }   

 void SetCurrentSource(uint8_t ch)		//ustawia tryb pracy przetwornicy na żródło pradowe z ogr napiecia
 {
		CHANNEL[ch-1].MOD =current_source;
 }

void CheckOvPower(uint8_t nr)			//skanuje i ew koryguje przekroczenie dopuszalnej mocy na danym kanale
{
	
	
	if((SkanVol(nr)* skan_curr(nr)) > CHANNEL[nr-1].PowMax)
	{
		rsPrint("CH");
		rsPrintfloat(nr);
		rsPrint("\tOver Power flag!\n\r");
		
		while((SkanVol(nr)* skan_curr(nr)) > CHANNEL[nr-1].PowMax) 
		{
			trig_pot(nr,down);
		}
	}
	
	
} 

void Skan_all_supply (void)
{
	
	
	UART_raport("Power supply:/n/r");
	for(int i=0;i>4;i++)
	{
		
		if( CHANNEL[i].EN ==1 )
		{
			if( CHANNEL[i].MOD== voltage_source )
			{
				CheckOvPower(i);
				CheckOvCurr(i);
				
			}
			if( CHANNEL[i].MOD== current_source )
			{
				 CheckOvPower(i);
				 CheckOvVol(i);
				 CheckCurr(i);
				
			}
			UART_raport("CH");
			UART_raport(intoch(i));
			UART_raport(":\tCurr:\t");
			UART_raport (ftoch(SkanCurr(i)));
			UART_raport("A\tVol:\t");
			UART_raport (ftoch(SkanVol(i)));
			UART_raport("V\t");
			UART_raport("Power: \t");
			UART_raport(i);
			UART_raport(ftoch( SkanVol(i)* SkanCurr(i)));
			UART_raport("\tW\n\r");
		}
	}
	
	
	UART_TX_bufor_send(&buffer_TX);
	
}
