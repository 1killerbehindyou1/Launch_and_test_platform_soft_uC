/*
 * WLASCIWY PROGRAM.c
 *
 * Created: 02.07.2019 01:43:09
 * Author : Kolunio
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
 #include "Procedury_dla_main.h"
#include "uart.h"
//#include <avr/sfr_defs.h>
#include "adc.h"
#include "I2C_dev.h"
//#define F_CPU 16000000
#include "zasilacz.h"
#include <util/delay.h> 
#include "PWM.h"
#include "pot.h"
#include "zasilacz.h"
#include "pot.h"
#define INC PORTC6
#define U_D PORTC7
int b =0;
extern bufor_cykliczny buffer_TX;
extern KOM polecenie;
 extern uint8_t flag_val;
 extern uint8_t flag_pol;


int main(void)
{
	DDRD |= (1<<PIND5);
	cbi(PORTD, PIND5); 
inicjalizacja_mikrokontrolera(); 
inicjalizacja_urzadzenPCB();
Diagnostyka();

sei();
	
	
    while (1) //W pêtki g³ónej bedzie odbywa³o sie skanowanie i wysy³anie raportu po uart 
				//zawierajace wszystkie odczytane parametry urzadzen
	{	
	
	
		Skanuj();

	}
	
return 0;

}
/*
Format polecenia:

*.../ - komenda sterujaca
#.../ - komenda liczbowa

zczytywanie komendy do bufora konczy sie gdy napotkany zostaniew znak '/'


*/
ISR(USART1_RX_vect) //Przerwanie od przychodz¹cej komendy wyw gdy dana w UDR
{
sbi(PORTD,PIND5);	 
	
	if(b < 8)  // odbieranie komendy do bufora polecen
	{	
		RX_buf[b]= UDR1;
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
	
	}*//*ISR(TIMER2_OVF_vect) {		}*/