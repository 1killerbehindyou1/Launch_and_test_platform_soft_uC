/*
 * CFile1.c
 *
 * Created: 02.07.2019 01:47:25
 *  Author: Kolunio
 */ 

#include "Command.h"
#include "uC_UART.h"
#include "uC_ADC.h"
#include "PCB_I2C.h"
# include "Parse_Type.h"
#include "PCB_PowSupply.h"

#include "uC_PWM.h"
#include "PCB_DigitalPot.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

 uint8_t flag_arg =0;
 uint8_t  flag_val =0; 
extern bufor_cykliczny buffer_TX;
extern char floatString[];
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
	case (mark_arg):												//odebrano rozkaz
		{
		flag_arg =SET;
			 for(int i =1;((RX_buf[i] !=mark_end )&&(i<5));i++)
			 {
				 polecenie.arg[i-1] = RX_buf[i];			//przypisanie wartoœci do zmiennych arg
				 polecenie.arg[i] = END_OF_TAB;
			 }
		break;
		}
	 case (mark_val):											//odebrano wartosc liczbowa
		 {
			flag_arg= SET;
			polecenie.value = CharToFloat(RX_buf);
			
			 for(int i =0;RX_buf[i] !=END_OF_TAB;i++)
			 {
				 RX_buf[i] = END_OF_TAB;							//przypisanie warto?ci do zmiennych arg
			 }
			 break;	 
		 } 
	}
	
	if((flag_val==SET) && (flag_arg==SET))						//komenda bedzie przetworzona jedunie w przypadku 
															//odebrania kompletu rozkaz + warto??
		{
		flag_val =CLR;
		flag_arg =CLR;	
		Decode_command();									//wywo?anie funkcji dekoduj?cej
		}
	
}

void Decode_command(void)								
{
	if(polecenie.arg[operation] == USTAW)						//komenda USTAW
	{
		switch(polecenie.arg[device])
		{
			case ZASILACZ_:					//sterowanie kana?ami przetfornic
			{
				if(CHANNEL[(uint8_t)CharToFloat(polecenie.arg[channel])].MOD== voltage_source)   //Jezeli tryb pracy = voltage_source
				{
					if (polecenie.arg[parameter] == OGRANICZENIE)			// ustawienie ograniczenia pradowego
					{										
						CHANNEL[(uint8_t)CharToFloat(polecenie.arg[channel])].OvCurr= polecenie.value;
						SetOvCurr(((uint8_t)CharToFloat(polecenie.arg[channel])), polecenie.value);
						 UART_raport("Ustawiono ograniczenie pradowe na CH");
						 UART_raport((polecenie.arg[channel]));
						 UART_raport("/t");
						 UART_raport (FloatToChar(polecenie.value));
						 UART_raport("A/t");
						UART_TX_bufor_send(&buffer_TX);
						polecenie.value =0;
						}
					if (polecenie.arg[parameter] == NAPIECIE)					 //ustawienie napiecia wysciowego
					{													
						CHANNEL[(uint8_t)CharToFloat(polecenie.arg[channel])].Voltage= polecenie.value;
						SetVol((uint8_t )CharToFloat(polecenie.arg[channel]), polecenie.value);
						 UART_raport("Tryb pracy dla CH");
						 UART_raport(polecenie.arg[channel]);
						 UART_raport("\tZrodlo napiecia\n\r");
						 UART_raport("napiecie na CH");
						 UART_raport(polecenie.arg[channel]);
						 UART_raport("\t");
						 UART_raport (FloatToChar(SkanVol((uint8_t)CharToFloat(polecenie.arg[channel]))));
						 UART_raport("V");
						 UART_raport("\tOgraniczenie pradowe:");
						 UART_raport (FloatToChar(CHANNEL[(uint8_t)CharToFloat(polecenie.arg[channel])].OvCurr));
						 UART_raport("A");
						 UART_TX_bufor_send(&buffer_TX);
						
						polecenie.value =0;
						
					}	
				}
				if(CHANNEL[(uint8_t)CharToFloat(polecenie.arg[channel])].MOD== current_source)  //Jezeli tryb pracy = ?ród?o pr?dowe
				{
					if(polecenie.arg[parameter] == NAPIECIE )					//ustawianie maksymalnego napi?cia
					{
						CHANNEL[(uint8_t)CharToFloat(polecenie.arg[channel])].OvVol = polecenie.value;
					
						polecenie.value =0;
					}
				
					if(polecenie.arg[parameter] == OGRANICZENIE)				//usatawianie pradu zród?a pradowego
					{
					    
					}
				}
				
				break;
			}
			case PRZEKAZNIK:									//sterowanie przeka?nikami
			{
				break;
			}	
			case WYJSCIE_ANALOGOWE:									//ustawianie wyjscia analogowego
			{
				break;
			}
			case KOMPARATOR:									//ustawianie komparatorów
			{
				break;
			}
			case GENERATOR:									//ustawianie generatora
			{
				break;
			}
			}
	}
	
	if (polecenie.arg[operation] == WYSWIETL)				//komenda WYSWIETL zadany parametr
	{	
	}
	if (polecenie.arg[operation] == ZMIEN_TRYB)				//komenda steruj?ca urzadzeniemr
	{	
	}
}


	
		