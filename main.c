/*
 * WLASCIWY PROGRAM.c
 *
 * Created: 02.07.2019 01:43:09
 * Author : Kolunio
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
 #include "Procedury_dla_main.h"
#include "blok_zas.h"
#include "uart.h"
#include <avr/sfr_defs.h>
#include "adc.h"
#define BAUD 9600
#define F_CPU 16000000
#define baud_setting  ((F_CPU / (16UL * BAUD)) - 1)
#include <util/delay.h> 

#include "PWM.h"
int b =0;
extern bufor_cykliczny buffer_TX;

int main(void)
{
inicjalizacja_mikrokontrolera(); 

Diagnostyka();
 
    while (1) //W pêtki g³ónej bedzie odbywa³o sie skanowanie i wysy³anie raportu po uart 
				//zawierajace wszystkie odczytane parametry urzadzen
	{	
		
	 Skanuj();//sprawdza napiecie z wyj PWM
	 
    }
	
return 0;

}
/*
Format polecenia:

*.../ - komenda sterujaca
#.../ - komenda liczbowa

zczytywanie komendy do bufora konczy sie gdy napotkany zostaniew znak '/'


*/
ISR(USART_RXC_vect) //Przerwanie od przychodz¹cej komendy wyw gdy dana w UDR
{
	
	
	if(b < 8)  // odbieranie komendy do bufora polecen
	{	
		RX_buf[b]= UDR;
		b++;	
	}
	
	 if ((b >= 8)||(RX_buf[b-1]== '/'))  //rozpoznano znak konca '/' 
	{
		
		UART_Pobierz_rozkaz();
		b=0;
		
	}
	

}

/*
ISR (USART_UDRE_vect) //przerwanie od pustego bufora
{
	
	}*//*ISR(TIMER2_OVF_vect) {		 UART_TX_bufor_wyslij(&buffer_TX);}*/