/*
 * CFile1.c
 *
 * Created: 02.07.2019 01:47:25
 *  Author: Kolunio
 */ 
#include <math.h>
#include "uart.h"
#include "Procedury_dla_main.h"
#include <math.h>
#include <avr/io.h>
#include "blok_zas.h"
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "adc.h"
#include "PWM.h"
#include <util/delay.h>
extern bufor_cykliczny buffer_TX;
extern char flstr[];
/*
Funkacja skanuj bedzie dziala w petli glownej programu, skanowane beda wszystkie urzadzenia 
i zczytywane parametry ich pracy, dane beda wysylane do bufora ktory po ukonczeniu skanowania lub po 
przepelnieniu bedzie wysylany po UART
*/
void Skanuj(void)								
{
						
//UART_raport();	//raport ze skanowania adc na razie tylko jednen rodzaj pomiaru
	Print_to_buf(&buffer_TX,ftoch(adc_run_one()));
	Print_to_buf(&buffer_TX,"\n\r");
	
}

void Diagnostyka(void)  //selftest wszystkich urzadzen i raport po UART
{
	
UART_raport("Diagnostic devices on PCB..\n\r");
UART_raport("All devices work properly!\n\n\n\r");
UART_TX_bufor_wyslij(&buffer_TX);
sei();
}


void inicjalizacja_mikrokontrolera(void)
{
	
	init_UART();
	UART_raport("UART initialize...\n\r");
	
	/*timer2_init();
	PWM0_init();
	UART_raport("PWM initialize...\n\r");*/

	 adc_init();
	UART_raport("ADC initialize...\n\r");

	
	
	UART_raport("Microkontroller initialized correctly!\n\r");

	UART_TX_bufor_wyslij(&buffer_TX);
	
}

void UART_raport(char *result)  //utworzenie raportu z poj wyniku pomiaru do wys³ania i wys³anie do bufora
{
	 Print_to_buf(&buffer_TX, result);  //buforowanie wynikow pomiaru 

}


void UART_Pobierz_rozkaz(void)
{
	
	
	switch (RX_buf[0])
	{
	case ('*'):  //odebrano rozkaz
	{
	
	 for(int i =1;((RX_buf[i] !='/')&&(i<5));i++)
	 {
		 arg[i] = RX_buf[i]; //przypisanie wartoœci do zmiennych arg
	 }
	  break;
	}
	 case ('#'):	//odebrano wartosc liczbowa
	 {
		 
		value= chartofl(RX_buf);
		PWM0_set_V(value);
		
		 break;	 
	 } 
	}
	
	
}


float chartofl(char * RX_buf)    //konwersja   char na float
{
	float a=0;
	int o=0;

	while(RX_buf[o+1] !='.' )  //okreslenie ilosci miejsa ca³kowiotych
	{
		o++;
		
	}

	for(int i=1;i <=o+5;i++)  //0 elem tabl to #
	{


		o--;
		if(RX_buf[i] =='.' )
		i++;

		a += (RX_buf[i]-48)*pow(10,o) ;	
	}

	return a;
}


void Dekoduj_rozkaz(void) //niezaimplenatowane
{
	
	if(arg[0] == USTAW) //ustawianie param
	{
		
		switch(arg[1])
		{
			
			case 'z': //zasilacze
			{
				rsPrint("case_z");
				
				if (arg[2] == 'c')
				{  //ustawienie ovc na kanale 1
				zas_ustaw(arg[2],arg[3]);
				 rsPrint("ustawiono zas.ch.c/n/r v=");  //*0z3v/n
				 
				}
				
				if(arg[2] =='v' )
				{
					zas_ustaw(arg[2],arg[3]);
				 rsPrint("utawiono zas.ch.v/n/r");
				
				break;
				}
			}
			
			case 'p':  //
			{
				
				break;
			}
			case 'a':
			{
				break;
			}
			case 'c':
			{
				break;
			}
			case 'r':
			{
			
				break;
			}
		}
		
		
		
	}
	
	else if (arg[1] == WYSWIETL)
	{
	}
	
	
}

	
char* intoch(int t)
{
	
	for(int i=0;i<=2;i++)
	{
		flstr[i]= 48 + t/pow(10,2-i);
		t -= (flstr[i] - 48)*pow(10,2-i);
	}
		flstr[3]= 0;
		return flstr;
	}
	