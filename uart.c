/*
 * uart.c
 *
 * Created: 02.07.2019 08:05:09
 *  Author: Kolunio
 */ 
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <util/delay.h>
char flstr [8];
char TX_buf_scan[ROZMIAR];  //tablica dla bufora cykliczego

bufor_cykliczny buffer_TX={TX_buf_scan,0,0};//bufor uzywany przy wysy³aniu raportu


void Print_to_buf(bufor_cykliczny* q, char text[])
{
	unsigned int i;
	i = 0;
	while ( text[i] != 0 )
	{
	UART_TX_bufor_dopisz(q, text[i]);
		i++;
	}


}

void rsPrint(char text[]) 
{

	unsigned int i;
	i = 0;
	while ( text[i] != 0 )
	{
		 rsSndByte( text[i]);
		i++;
	}

}

void rsSndByte( char c) 
{
	while(!(UCSRA &(1<<UDRE)));
	UDR = c;
}

void clear_RX(void)
{
	for(int i=0;i<8;i++)
	RX_buf[i] = 0;
}
void init_UART(void)
	{
		PORTD = 3;
		UBRRH = (baud_setting >> 8);        //wpisanie starszego bajtu
		UBRRL = baud_setting;             //wpisanie mlodszego bajtu
		
		//UCSRA bez zmian - 0x00
		UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//|(1<<UDRIE);                 //aktywne przerwanie od odbioru oraz zmiana trybu dzia³ania pinów D0 i D1
		UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);     //praca synchroniczna, brak kontroli parzystoœci, 1 bit stopu, 8 bitów danych
		

		/* W³¹czenie przerwania, gdy bufor nadawczy pusty */
		
	}

void rsPrintfloat(float ff)  
{

	rsPrint(ftoch(ff));
	rsPrint("\n\r");
} 

char* ftoch(float f)  
 {
				
				 int in =(int) 100*f;  //rzutowanie na typ int

		int e= 0,w=100;
		 while((in/w )>1)  //ustalenie ilosci cyfr w zmiennej pomn o 1
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

	
int UART_TX_bufor_dopisz(bufor_cykliczny  *q, char data)
{
	uint8_t head_temp = q->poczatek + 1;	// Przypisujemy do zmiennej nastêpny indeks
	
	// Sprawdzamy czy jest miejsce w buforze.
	// Jeœli bufor jest pe³ny to wysy³amy
	if ( head_temp == q->koniec )
	{
		//_delay_ms(500); //dodatkowe opznienie
			UART_TX_bufor_wyslij(q);
			return 1; //wys³ano zaw bufora
	}


			if ( head_temp == ROZMIAR )
			head_temp = 0;	
			
			//jest miejsce w buf
			q->buffer_pointer[head_temp] = data;	// Wpisujemy wartoœæ do bufora
			q->poczatek = head_temp;			// Zapisujemy nowy indeks head o 1 wiekszy
			
			
			return 0; //bufor nie zosta³ jescze oprozniony
	}
	
void UART_TX_bufor_wyslij(bufor_cykliczny *q)
	{
		while(q->poczatek != q->koniec)
		{
			q->koniec++; // Inkrementujemy indeks tail
			
			// Jeœli by³ to ostatni element tablicy to ustawiamy wskaŸnik na jej pocz¹tek
			if (q->koniec ==ROZMIAR)
			q->koniec = 0;
			
			
			rsSndByte(q->buffer_pointer[q->koniec]);		// Odczytujemy wartoœæ z bufora
		}	
		
			}
	
	