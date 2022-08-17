/*
 * uart.c
 *
 * Created: 02.07.2019 08:05:09
 *  Author: Kolunio
 */ 
#include "Parse_Type.h"
#include "Command.h"
#include <avr/sfr_defs.h>
#include "uC_UART.h"
//#include "zasilacz.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include <util/delay.h>

char TX_buf_scan[ROZMIAR];							//tablica dla bufora cykliczego

bufor_cykliczny buffer_TX={TX_buf_scan,0,0};		//bufor uzywany przy wysy?aniu raportu
/*
static int	Serial_Available(void)
{
	return(UART_STATUS_REG & (1 << UART_RECEIVE_COMPLETE));	// wait for data
}
*/
void Print_to_buf(bufor_cykliczny* q, char text[])
{
	unsigned int i;
	i = 0;
	while ( text[i] != 0 )
	{
	UART_TX_buf_writein(q, text[i]);
		i++;
	}


}

int UART_TX_buf_writein(bufor_cykliczny  *q, char data)
{
	uint8_t head_temp = q->poczatek + 1;			// Przypisujemy do zmiennej nast?pny indeks
	
													// Sprawdzamy czy jest miejsce w buforze.
													// Je?eli bufor jest pe?ny to wysy?amy
	if ( head_temp == q->koniec )
	{
								
		UART_TX_bufor_send(q);
		return 1;									//wys?ano zaw bufora
	}


	if ( head_temp == ROZMIAR )
	head_temp = 0;
	
												//jest miejsce w buf
	q->buffer_pointer[head_temp] = data;	// Wpisujemy wartoœæ do bufora
	q->poczatek = head_temp;				// Zapisujemy nowy indeks head o 1 wiekszy
	
	
	return 0;								//bufor nie zosta? jescze oprozniony
}

void UART_TX_bufor_send(bufor_cykliczny *q)
{
	while(q->poczatek != q->koniec)
	{
		q->koniec++;							 // Inkrementujemy indeks tail
		
												// Je?li by? to ostatni element tablicy
													//to ustawiamy wskaznik na jej pocz?tek
		if (q->koniec ==ROZMIAR)
		q->koniec = 0;
		
		
		rsSndByte(q->buffer_pointer[q->koniec]);		// Odczytujemy wartosc z bufora
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
	while(!(UCSR1A &(1<<UDRE1)));
	UDR1 = c;
}

void clear_RX(void)
{
	for(int i=0;i<8;i++)
	RX_buf[i] = 0;
}

void init_UART(void)
	{
		
		UBRR1H =(unsigned char) (baud_setting >> 8);        //wpisanie starszego bajtu
		UBRR1L = (unsigned char) baud_setting;             //wpisanie mlodszego bajtu
	
		UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1) ;   //aktywne przerwanie od odbioru oraz 
														//zmiana trybu dzia?ania pinów D0 i D1
		UCSR1C = (1 << UCSZ10) | (1 << UCSZ11); 
		/*
		//(3<<UCSZ10)  ;
		//synchro  oraz brak kontroli parzystoœci, 1 bit stopu, 8 bitów danych
		 W³¹czenie przerwania, gdy bufor nadawczy pusty */
		
	}

void rsPrintfloat(float ff)  
{

	rsPrint(ftoch(ff));
	rsPrint("\n\r");
} 
				
void	PrintDecInt(int theNumber, int digitCnt)
{
int	theChar;
int	myNumber;

	myNumber	=	theNumber;

	if ((myNumber > 100) || (digitCnt >= 3))
	{
		theChar		=	0x30 + myNumber / 100;
		rsSndByte(theChar );
	}

	if ((myNumber > 10) || (digitCnt >= 2))
	{
		theChar	=	0x30  + ((myNumber % 100) / 10 );
		rsSndByte(theChar );
	}
	theChar	=	0x30 + (myNumber % 10);
	rsSndByte(theChar );
}
	
void rsPrintReg(uint8_t reg, char name[])
{
	rsPrint(name);
	rsPrint("=\t");
	rsPrint(decToBin(reg));
	
	PrintNewLine();
}

void PrintAllReg(void)
	{
		/*rsPrintReg(MCUSR, "MCUSR");
		rsPrintReg(WDTCSR,"WDTCSR");
		rsPrintReg(MCUCR,"MCUCR");
		rsPrintReg(EIMSK,"EIMSK");
		rsPrintReg(TIMSK0,"TIMSK0");
		rsPrintReg(TIMSK1,"TIMSK1");
		rsPrintReg(TIMSK3,"TIMSK3");
		rsPrintReg(PCICR,"PCICR");*/
		rsPrintReg(UDIEN ,"UDIEN");
		/*rsPrintReg(PINC,"PINC");
		rsPrintReg(PIND,"PIND");
		rsPrintReg(PINE,"PINE");
		rsPrintReg(PINF,"PINF");*/
		//rsPrintReg(TIFR1,"TIFR1");
		//rsPrintReg(TIFR3,"TIFR3");
		//rsPrintReg(TIFR4,"TIFR4");
		//rsPrintReg(EIFR,"EIFR");
		//rsPrintReg(TCCR0A,"TCCR0A");
		//rsPrintReg(TCCR0B,"TCCR0B");
		//rsPrintReg(TCNT0,"TCNT0");
		//rsPrintReg(PCMSK0,"PCMSK0");
		rsPrintReg(USBINT,"USBINT");
		rsPrintReg(USBCON,"USBCON");
		rsPrintReg(USBSTA,"USBSTA");
		rsPrintReg(UHWCON,"UHWCON");
		
		
	}		
	
void	PrintNewLine(void)
{
	rsPrint("\n\r");
}

/*
void	PrintMESSAGES(const void *dataPtr, unsigned char offset)
{
char	theChar;

	dataPtr		+=	offset;

	do {
	
		theChar	=	pgm_read_byte_near((uint16_t)dataPtr++);
	
		if (theChar != 0)
		{
			sendchar(theChar);
		}
	} while (theChar != 0);
}
*/


/*	
void	PrintHexByte(unsigned char theByte)
{
	
char	theChar;

	theChar	=	0x30 + ((theByte >> 4) & 0x0f);
	if (theChar > 0x39)
	{
		theChar	+=	7;
	}
	sendchar(theChar );

	theChar	=	0x30 + (theByte & 0x0f);
	if (theChar > 0x39)
	{
		theChar	+=	7;
	}
	sendchar(theChar );
}

*/	
