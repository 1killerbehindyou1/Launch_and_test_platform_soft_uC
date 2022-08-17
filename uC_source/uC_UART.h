/*
 * uart.h
 *
 * Created: 02.07.2019 08:05:25
 *  Author: Kolunio
 */ 

#include <avr/sfr_defs.h>
#include <avr/io.h>

#ifndef UART_H_
#define UART_H_
#define F_CPU 16000000UL
	#define BAUD 38400										//domyaslna dla mod bluetooth
#define baud_setting  ((F_CPU / (16UL * BAUD)) - 1)
#define ROZMIAR 256										//wielkosc bufora cyklicznego
#define	UART_BAUD_RATE_LOW			UBRR1L
	#define	UART_STATUS_REG				UCSR1A
	#define	UART_CONTROL_REG			UCSR1B
	#define	UART_ENABLE_TRANSMITTER		TXEN1
	#define	UART_ENABLE_RECEIVER		RXEN1
	#define	UART_TRANSMIT_COMPLETE		TXC1
	#define	UART_RECEIVE_COMPLETE		RXC1
	#define	UART_DATA_REG				UDR1
	#define	UART_DOUBLE_SPEED			U2X1
	
/* DEFINICJE KOMUNIKATOW - PRZYKLADY *********************************************
const char	gTextMsg_Explorer[]			PROGMEM	=	"Arduino explorer stk500V2 by MLS";
	const char	gTextMsg_Prompt[]			PROGMEM	=	"Bootloader>";
	const char	gTextMsg_HUH[]				PROGMEM	=	"Huh?";
	const char	gTextMsg_COMPILED_ON[]		PROGMEM	=	"Compiled on = ";
	const char	gTextMsg_CPU_Type[]			PROGMEM	=	"CPU Type    = ";
	const char	gTextMsg_AVR_ARCH[]			PROGMEM
		=	"__AVR_ARCH__= ";
	const char	gTextMsg_AVR_LIBC[]			PROGMEM	=	"AVR LibC Ver= ";
	const char	gTextMsg_GCC_VERSION[]		PROGMEM	=	"GCC Version = ";
	const char	gTextMsg_CPU_SIGNATURE[]	PROGMEM	=	"CPU ID      = ";
	const char	gTextMsg_FUSE_BYTE_LOW[]	PROGMEM	=	"Low fuse    = ";
	const char	gTextMsg_FUSE_BYTE_HIGH[]	PROGMEM	=	"High fuse   = ";
	const char	gTextMsg_FUSE_BYTE_EXT[]	PROGMEM	=	"Ext fuse    = ";
	const char	gTextMsg_FUSE_BYTE_LOCK[]	PROGMEM	=	"Lock fuse   = ";
	const char	gTextMsg_GCC_DATE_STR[]		PROGMEM	=	__DATE__;
	const char	gTextMsg_AVR_LIBC_VER_STR[]	PROGMEM	=	__AVR_LIBC_VERSION_STRING__;
	const char	gTextMsg_GCC_VERSION_STR[]	PROGMEM	=	__VERSION__;
	const char	gTextMsg_VECTOR_HEADER[]	PROGMEM	=	"V#   ADDR   op code     instruction addr   Interrupt";
	const char	gTextMsg_noVector[]			PROGMEM	=	"no vector";
	const char	gTextMsg_rjmp[]				PROGMEM	=	"rjmp  ";
	const char	gTextMsg_jmp[]				PROGMEM	=	"jmp ";
	const char	gTextMsg_WHAT_PORT[]		PROGMEM	=	"What port:";
	const char	gTextMsg_PortNotSupported[]	PROGMEM	=	"Port not supported";
	const char	gTextMsg_MustBeLetter[]		PROGMEM	=	"Must be a letter";
	const char	gTextMsg_SPACE[]			PROGMEM	=	" ";
	const char	gTextMsg_WriteToEEprom[]	PROGMEM	=	"Writting EE";
	const char	gTextMsg_ReadingEEprom[]	PROGMEM	=	"Reading EE";
	const char	gTextMsg_EEPROMerrorCnt[]	PROGMEM	=	"EE err cnt=";
	const char	gTextMsg_PORT[]				PROGMEM	=	"PORT";

*/
/************************************************************************/
/*	Help messages
	const char	gTextMsg_HELP_MSG_0[]		PROGMEM	=	"0=Zero addr";
	const char	gTextMsg_HELP_MSG_QM[]		PROGMEM	=	"?=CPU stats";
	const char	gTextMsg_HELP_MSG_AT[]		PROGMEM	=	"@=EEPROM test";
	const char	gTextMsg_HELP_MSG_B[]		PROGMEM	=	"B=Blink LED";
	const char	gTextMsg_HELP_MSG_E[]		PROGMEM	=	"E=Dump EEPROM";
	const char	gTextMsg_HELP_MSG_F[]		PROGMEM	=	"F=Dump FLASH";
	const char	gTextMsg_HELP_MSG_H[]		PROGMEM	=	"H=Help";
	const char	gTextMsg_HELP_MSG_L[]		PROGMEM	=	"L=List I/O Ports";
//	const char	gTextMsg_HELP_MSG_Q[]		PROGMEM	=	"Q=Quit & jump to user pgm";
	const char	gTextMsg_HELP_MSG_Q[]		PROGMEM	=	"Q=Quit";
	const char	gTextMsg_HELP_MSG_R[]		PROGMEM	=	"R=Dump RAM";
	const char	gTextMsg_HELP_MSG_V[]		PROGMEM	=	"V=show interrupt Vectors";
	const char	gTextMsg_HELP_MSG_Y[]		PROGMEM	=	"Y=Port blink";

	const char	gTextMsg_END[]				PROGMEM	=	"*";

 */
typedef struct  {
	 char *buffer_pointer;
	uint8_t poczatek;
	uint8_t koniec;
}bufor_cykliczny;									//struktura dla typu bufor_cykliczny
 
char RX_buf[8];										//bufor na polecenia, odbiorczy
/*
//static unsigned char recchar_timeout(void);   //timeout na odbieranie danych po UART
//static int	Serial_Available(void); //swprawdza dostepnosc UART
*/
int UART_TX_buf_writein( bufor_cykliczny* ,char ); //dopisuje dana do bufora 
void UART_TX_bufor_send(bufor_cykliczny* );		//wys?a zaw. bufora po UART
void Print_to_buf( bufor_cykliczny* , char *);		//drukuje do wskazanego bufora
void PrintDecInt(int , int );void PrintHexByte(unsigned char );
void PrintNewLine(void);
void PrintMESSAGES(const void *, unsigned char );
void clear_RX(void);
void rsSndByte(char );							//elementarna funkcja funkcja drukuje znak
void rsPrint(char *);							//drukuje string bez buforowania
void rsPrintfloat(float );						//drukuje vartosc float bez buforowania
void init_UART(void);					
void rsPrintReg(uint8_t,char*);
void PrintAllReg(void);

#endif /* UART_H_ */