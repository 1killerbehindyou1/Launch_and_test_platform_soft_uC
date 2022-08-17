/*
 * uart.h
 *
 * Created: 02.07.2019 08:05:25
 *  Author: Kolunio
 */ 
#include <avr/io.h>

#ifndef UART_H_
#define UART_H_

#define BAUD 9600
#define F_CPU 16000000
#define baud_setting  ((F_CPU / (16UL * BAUD)) - 1)
#define ROZMIAR 256  //wielkosc bufora cyklicznego

 
typedef struct  {
	 char *buffer_pointer;
	uint8_t poczatek;
	uint8_t koniec;
}bufor_cykliczny;   //struktura dla typu bufor_cykliczny

 
char RX_buf[8];  //bufor na polecenia, odbiorczy


int UART_TX_bufor_dopisz( bufor_cykliczny* ,char ); //dopisuje dana do bufora 
void UART_TX_bufor_wyslij(bufor_cykliczny* );  //wysy³a zaw bufora po UART
void Print_to_buf( bufor_cykliczny* , char *); //drukuje do wskazanego bufora


void clear_RX(void);
void rsSndByte(char );  //elementarna funkcja funkcja drukuje znak
void rsPrint(char *);		//drukuje string bez buforowania
void rsPrintfloat(float );   //drukuje vartosc float bez buforowania

void init_UART(void);
char * ftoch(float );  //konwersja z float na string

#endif /* UART_H_ */