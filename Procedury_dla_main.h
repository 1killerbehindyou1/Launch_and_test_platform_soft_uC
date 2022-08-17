/*
 * IncFile1.h
 *
 * Created: 02.07.2019 01:43:34
 *  Author: Kolunio
 */ 

//definicje kod� rezultatu diagnostyki
int rezultat_diagnostyki;


//definicje kod� rezultatuskanowania
int rezultat_skanowania; 



//kody komend
//****************************************************/
/*
Budowa komendy:
arg1 - typ operacji
arg2 - peryferia ktorego dotyczy
arg3 - dodatkowy arg
arg4 - dodatkowy arg
ka�dy rozkaz sk�ada si� z 4 argument�w + znacznik pocz�tku i ko�ca
po rozkazie nastepuje przes�anie warto�ci do zapisu (w przypadku USTAW lub nie w przypadku WYSLIJ
*//*********************************************************/
/*
Sk�adnia komendy , przyk�ad:
arg[0] = '1' - USTAW
arg[1] = 'z' - zasiacz
arg[2] = '3' - 3 kana�
arg[3]  = 'v' - napiecie
arg[4]
*/



/*
typedef struct Reg{
	//uint8_t device_REG[][];
	//int* device_Reg_indeks[];
	uint8_t I2C[][];
	
};
*/

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

typedef struct {
	float value;
	char arg[4];
	
}KOM;
 
 
KOM polecenie;



#define USTAW '1'
	/*#define Zasilacz "z"
		
	
	
#define generator "g"
#define wyjscie_analogowe "a"
#define komparator "c"
#define przekaznik "r"
*/
#define WYSWIETL '2'


#define  INNE  '3'

//*****************************************//


void config_ext_int(void);
void inicjalizacja_urzadzenPCB(void);
void inicjalizacja_mikrokontrolera(void); 
void Diagnostyka(void);
void Skanuj(void);    //skanuje co okre�lony czas wszytkie parametry, oraz wysy�a je wszystkie po UART 
void UART_raport(char *result); 
void UART_Pobierz_rozkaz(void);          //pobiera rozkaz i zwraca tablice
void Dekoduj_rozkaz(void);


float chartofl(char * );
char *intoch(int);