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
extern Zasilacz CHANNEL[];
/*
Funkacja skanuj bedzie dziala w petli glownej programu, skanowane beda wszystkie urzadzenia 
i zczytywane parametry ich pracy, dane beda wysylane do bufora ktory po ukonczeniu skanowania lub po 
przepelnieniu bedzie wysylany po UART
*/
void Skan(void)											// Skanowanie parametrów w czasie rzeczywistym		
{
	Skan_all_supply();					

}

void Diagnostics(void)									//selftest wszystkich urzadzen i raport po UART
{
	
UART_raport("Diagnostic devices on PCB..\n\r");
rsPrint("\n\rtest magistrali I2C....");
Scan_I2C();										//skanowanie i wykrywanie adresó urz?dze? I2C
UART_raport("\n\rAll devices work properly!\n\n\n\r");
UART_TX_bufor_send(&buffer_TX);

}

void CPU_init(void)										// Inicjalizacja urz?dze? wewn?trznych mikrokontrolera
{
	config_ext_int();                             //wy??czenie wst?pnie za??czonych przerwa? od USB 
													//przez bootloader  
	
	init_UART();									//konfiguracja rejestrów odpowiedzialnych za prac? UART
	UART_raport("UART initialize...\n\r");
	
	//timer2_init();
	//PWM0_init();
	//UART_raport("PWM initialize...\n\r");

	 adc_init();									//inicjalizacja przetwornika ADC
	UART_raport("ADC initialize...\n\r");
	
	init_i2C_dev();								 //konfiguracja rejestrów odpowiedzialnych za prac? I2C
	
	//pot_init();
	UART_raport("Microkontroler initialized correctly!\n\r");

	UART_TX_bufor_send(&buffer_TX);
	
}																

void init_dev_onPCB(void)								// Inicjalizacja urz?dze? wewn?trznych mikrokontrolera
{
	//pot_init();
}

void config_ext_int(void)
{
	UHWCON =0;
	USBCON =0;
	UDIEN =0;
}
						
void UART_raport(char *result)							//utworzenie raportu z poj wyniku pomiaru 
{														//do wys?ania i wys?anie do bufora

	 Print_to_buf(&buffer_TX, result);					//buforowanie wynikow pomiaru 

}
void UART_Get_command(void)								//pobranie komendy z UART
{
	switch (RX_buf[0])
	{
	case ('*'):												//odebrano rozkaz
		{
		flag_pol =1;
			 for(int i =1;((RX_buf[i] !='/')&&(i<5));i++)
			 {
				 polecenie.arg[i-1] = RX_buf[i];			//przypisanie wartoœci do zmiennych arg
				 polecenie.arg[i] = '\0';
			 }
		break;
		}
	 case ('#'):											//odebrano wartosc liczbowa
		 {
			flag_val= 1;
			polecenie.value = chartofl(RX_buf);
			
			 for(int i =0;RX_buf[i] !='\0';i++)
			 {
				 RX_buf[i] = '\0';							//przypisanie warto?ci do zmiennych arg
			 }
			 break;	 
		 } 
	}
	
	if((flag_val==1) && (flag_pol==1))						//komenda bedzie przetworzona jedunie w przypadku 
															//odebrania kompletu rozkaz + warto??
		{
		flag_val =0;
		flag_pol =0;	
		Decode_command();									//wywo?anie funkcji dekoduj?cej
		}
	
}


	
char* intoch(int t)										//konwersja typu int na typ char
{
	
	for(int i=0;i<=2;i++)
	{
		flstr[i]= 48 + t/pow(10,2-i);
		t -= (flstr[i] - 48)*pow(10,2-i);
	}
		flstr[3]= 0;
		return flstr;
	}						
		
char* decToBin(uint8_t x)								//konwersja typu int na posta? binarn?
	{
		
		int i, k;
		char bin[8];

		for (i = 7; i >= 0; i--)
		{
			k = x >> i;

			if (k & 1)
			bin[7-i] = '1';
			else
			bin[7-i] = '0';
		}
		bin[8] = '\0';
		return bin;
		
	}							
		
char* ftoch(float f)									//konwersja typu float na typ char
	{
		
		int in =(int) 100*f;						//rzutowanie na typ int

		int e= 0,w=100;
		while((in/w )>1)							//ustalenie ilosci cyfr w zmiennej pomn o 1
		w *=10;

		

		while(in >0)
		{
			if(w==10)
			{

				flstr[e]= '.';
				e++;
			}
			flstr[e]=(int)in/w + 48;
			in -=w*(flstr[e]-48);

			e++;
			w /=10;

		}
		flstr[e]= 0;
		
		return flstr;
	}
	
float chartofl(char * RX_buf)							//konwersja   typu char na typ float
	{
		float a=0;
		int o=0;

		while(RX_buf[o+1] !='.' )						//okreslenie ilosci miejsc ca?kowiotych
		{
			o++;
			
		}

		for(int i=1;i <=o+5;i++)						//0 elem tabl to #
		{


			o--;
			if(RX_buf[i] =='.' )
			i++;

			a += (RX_buf[i]-48)*pow(10,o) ;
		}

		return a;
	}



void Decode_command(void)								
{
	if(polecenie.arg[0] == USTAW)						//komenda USTAW
	{
		switch(polecenie.arg[1])
		{
			case Zasila:					//sterowanie kana?ami przetfornic
			{
				if(CHANNEL[(uint8_t)chartofl(polecenie.arg[3])].MOD== voltage_source)   //Jezeli tryb pracy = voltage_source
				{
					if (polecenie.arg[2] == ograniczenie)			// ustawienie ograniczenia pradowego
					{										
						CHANNEL[(uint8_t)chartofl(polecenie.arg[3])].OvCurr= polecenie.value;
						SetOvCurr(((uint8_t)chartofl(polecenie.arg[3])), polecenie.value);
						 UART_raport("Ustawiono ograniczenie pr?dowe na CH");
						 UART_raport((polecenie.arg[3]));
						 UART_raport("/t");
						 UART_raport (ftoch(polecenie.value));
						 UART_raport("A/t");
						UART_TX_bufor_send(&buffer_TX);
						polecenie.value =0;
						}
					if (polecenie.arg[2] == napiecie)					 //ustawienie napiecia wysciowego
					{													
						CHANNEL[(uint8_t)chartofl(polecenie.arg[3])].Voltage= polecenie.value;
						SetVol((uint8_t)chartofl(polecenie.arg[3]), polecenie.value);
						 UART_raport("Tryb pracy dla CH");
						 UART_raport(polecenie.arg[3]);
						 UART_raport("\tZrodlo napiecia\n\r");
						 UART_raport("napi?cie na CH");
						 UART_raport(polecenie.arg[3]);
						 UART_raport("\t");
						 UART_raport (ftoch(SkanVol((uint8_t)chartofl(polecenie.arg[3]))));
						 UART_raport("V");
						 UART_raport("\tOgraniczenie pradowe:");
						 UART_raport (ftoch(CHANNEL[(uint8_t)chartofl(polecenie.arg[3])].OvCurr));
						 UART_raport("A");
						 UART_TX_bufor_send(&buffer_TX);
						
						polecenie.value =0;
						
					}	
				}
				if(CHANNEL[(uint8_t)chartofl(polecenie.arg[3])].MOD== current_source)  //Jezeli tryb pracy = ?ród?o pr?dowe
				{
					if(polecenie.arg[2] == napiecie )					//ustawianie maksymalnego napi?cia
					{
						CHANNEL[(uint8_t)chartofl(polecenie.arg[3])].OvVol = polecenie.value;
					
						polecenie.value =0;
					}
				
					if(polecenie.arg[2] == ograniczenie)				//usatawianie pradu zród?a pradowego
					{
					    
					}
				}
				
				break;
			}
			case przekaznik:									//sterowanie przeka?nikami
			{
				break;
			}	
			case wyjscie_analogowe:									//ustawianie wyjscia analogowego
			{
				break;
			}
			case komparator:									//ustawianie komparatorów
			{
				break;
			}
			case generator:									//ustawianie generatora
			{
				break;
			}
			}
	}
	
	if (polecenie.arg[0] == WYSWIETL)				//komenda WYSWIETL zadany parametr
	{	
	}
	if (polecenie.arg[0] == ZMIEN_TRYB)				//komenda steruj?ca urzadzeniemr
	{	
	}
}


	
		