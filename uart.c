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

bufor_cykliczny buffer_TX={TX_buf_scan,0,0};//bufor uzywany przy wysy�aniu raportu


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
		UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//|(1<<UDRIE);                 //aktywne przerwanie od odbioru oraz zmiana trybu dzia�ania pin�w D0 i D1
		UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);     //praca synchroniczna, brak kontroli parzysto�ci, 1 bit stopu, 8 bit�w danych
		

		/* W��czenie przerwania, gdy bufor nadawczy pusty */
		
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
	uint8_t head_temp = q->poczatek + 1;	// Przypisujemy do zmiennej nast�pny indeks
	
	// Sprawdzamy czy jest miejsce w buforze.
	// Je�li bufor jest pe�ny to wysy�amy
	if ( head_temp == q->koniec )
	{
		//_delay_ms(500); //dodatkowe opznienie
			UART_TX_bufor_wyslij(q);
			return 1; //wys�ano zaw bufora
	}


			if ( head_temp == ROZMIAR )
			head_temp = 0;	
			
			//jest miejsce w buf
			q->buffer_pointer[head_temp] = data;	// Wpisujemy warto�� do bufora
			q->poczatek = head_temp;			// Zapisujemy nowy indeks head o 1 wiekszy
			
			
			return 0; //bufor nie zosta� jescze oprozniony
	}
	
void UART_TX_bufor_wyslij(bufor_cykliczny *q)
	{
		while(q->poczatek != q->koniec)
		{
			q->koniec++; // Inkrementujemy indeks tail
			
			// Je�li by� to ostatni element tablicy to ustawiamy wska�nik na jej pocz�tek
			if (q->koniec ==ROZMIAR)
			q->koniec = 0;
			
			
			rsSndByte(q->buffer_pointer[q->koniec]);		// Odczytujemy warto�� z bufora
		}	
		
			}
	
	