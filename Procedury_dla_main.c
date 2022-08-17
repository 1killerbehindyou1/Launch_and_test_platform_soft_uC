/*
 * CFile1.c
 *
 * Created: 02.07.2019 01:47:25
 *  Author: Kolunio
 */ 
#include "pot.h"
#include "uart.h"
#include "Procedury_dla_main.h"
#include "I2C_dev.h"
#include "adc.h"
#include "PWM.h"
#include "zasilacz.h"

#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

 uint8_t flag_pol =0;
 uint8_t  flag_val =0; 
extern bufor_cykliczny buffer_TX;
extern char flstr[];

/*
Funkacja skanuj bedzie dziala w petli glownej programu, skanowane beda wszystkie urzadzenia 
i zczytywane parametry ich pracy, dane beda wysylane do bufora ktory po ukonczeniu skanowania lub po 
przepelnieniu bedzie wysylany po UART
*/
void Skanuj(void)								
{
	Skan_zas(3);					
//UART_raport();	//raport ze skanowania adc na razie tylko jednen rodzaj pomiaru
	//Print_to_buf(&buffer_TX,ftoch(adc_run_one(vol)));
	//Print_to_buf(&buffer_TX," V\n\r");
	/*Print_to_buf(&buffer_TX,ftoch(adc_run_one(curr)));
	Print_to_buf(&buffer_TX," A\n\r");
	*/
}

void Diagnostyka(void)  //selftest wszystkich urzadzen i raport po UART
{
	
UART_raport("Diagnostic devices on PCB..\n\r");
rsPrint("\n\rtest magistrali I2C....");
Scan_I2C();
UART_raport("\n\rAll devices work properly!\n\n\n\r");
UART_TX_bufor_wyslij(&buffer_TX);

}


void inicjalizacja_mikrokontrolera(void)
{
	config_ext_int();
	
	init_UART();
	UART_raport("UART initialize...\n\r");
	
	/*timer2_init();*/
	//PWM0_init();
	UART_raport("PWM initialize...\n\r");

	 adc_init();
	UART_raport("ADC initialize...\n\r");
	
	init_i2C_dev();
	
	pot_init();
	UART_raport("Microkontroller initialized correctly!\n\r");

	UART_TX_bufor_wyslij(&buffer_TX);
	
}

void inicjalizacja_urzadzenPCB(void)
{
	pot_init();
}
void config_ext_int(void)
{
	UHWCON =0;
	USBCON =0;
	UDIEN =0;
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
	flag_pol =1;
	 for(int i =1;((RX_buf[i] !='/')&&(i<5));i++)
	 {
		 polecenie.arg[i-1] = RX_buf[i]; //przypisanie wartoœci do zmiennych arg
		 
		 polecenie.arg[i] = '\0';
	 }
	  
	
	break;
	}
	
	
	 case ('#'):	//odebrano wartosc liczbowa
	 {
		flag_val= 1;
		polecenie.value = chartofl(RX_buf);
		
		
		 for(int i =0;RX_buf[i] !='\0';i++)
		 {
			 RX_buf[i] = '\0'; //przypisanie wartoœci do zmiennych arg
		 }
		
		 break;	 
	 } 
	}
	
	if((flag_val==1) && (flag_pol==1))
		{
		flag_val =0;
		flag_pol =0;
		
			
	Dekoduj_rozkaz();
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
	
	 
	
	
	if(polecenie.arg[0] == USTAW) //ustawianie param
	{
		
		switch(polecenie.arg[1])
		{
			
			case 'z': //zasilacze
			{
				
				if (polecenie.arg[2] == 'c')
				{  //ustawienie ovc na kanale 1
				
				switch (polecenie.arg[3])
				{
				case '1': CH1.current = polecenie.value;
				break;
				case '2': CH2.current = polecenie.value;
				break;
				case '3': CH3.current = polecenie.value;
				break;
				
				case '4': CH4.current = polecenie.value;
				break;
				 
				}
				}
				
				if(polecenie.arg[2] =='v' )
				{
					switch (polecenie.arg[3])
				{
				case '1': CH1.voltage = polecenie.value;
				break;
				case '2': CH2.voltage = polecenie.value;
				break;
				case '3': CH3.voltage = polecenie.value;
					break;
				
				case '4': CH4.voltage = polecenie.value;
				break;
				
				
				}
				
				
				}
				break;
				
			set_vol(polecenie.arg[3]);
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
	
	else if (polecenie.arg[1] == WYSWIETL)
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
	