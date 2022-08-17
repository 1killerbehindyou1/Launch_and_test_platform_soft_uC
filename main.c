/*
 * WLASCIWY PROGRAM.c
 *
 * Created: 02.07.2019 01:43:09
 * Author : Kolunio
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h> 

#include "Command.h"
#include "uC_UART.h"
#include "uC_ADC.h"
#include "PCB_I2C.h"
#include "PCB_PowSupply.h"
#include "uC_PWM.h"
#include "PCB_DigitalPot.h"
#include "Parse_Type.h"
#define INC PORTC6
#define U_D PORTC7

uint8_t RecievCharCount =0;

extern bufor_cykliczny buffer_TX;
extern KOM polecenie;
extern uint8_t flag_val;
extern uint8_t flag_pol;
extern Zasilacz CHANNEL[];

int main(void)
{
	DDRD |= (1<<PIND5);
	cbi(PORTD, PIND5); 
CPU_init(); 
init_dev_onPCB();
Diagnostics();

sei();
	
	
    while (1) //W pêtki g³ónej bedzie odbywa³o sie skanowanie i wysy³anie raportu po uart 
				//zawierajace wszystkie odczytane parametry urzadzen
	{	
	
	
		Skan();

	}
	
return 0;

}
/*
Format polecenia:

*.../ - komenda sterujaca
#.../ - komenda liczbowa

zczytywanie komendy do bufora konczy sie gdy napotkany zostaniew znak '/'


*/
ISR(USART1_RX_vect)								//Przerwanie od przychodz¹cej komendy wyw gdy dana w UDR
{
sbi(PORTD,PIND5);	 
	
	if(RecievCharCount < 8)									// odbieranie komendy do bufora polecen
	{	
		RX_buf[RecievCharCount]= UDR1;
		RecievCharCount++;	
	}
	
	 if ((RecievCharCount >= 8)||(RX_buf[RecievCharCount-1]== mark_end))			//rozpoznano znak konca '/' 
	{
		
		UART_Get_command();
		RecievCharCount=0;
		
	}
	

}


/*
ISR (USART_UDRE_vect)							//przerwanie od pustego bufora
{
	
	}*//*ISR(TIMER2_OVF_vect) {		}*/