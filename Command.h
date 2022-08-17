/*
 * IncFile1.h
 *
 * Created: 02.07.2019 01:43:34
 *  Author: Kolunio
 */ 

//definicje kodó rezultatu diagnostyki
int rezultat_diagnostyki;


//definicje kodó rezultatuskanowania
int rezultat_skanowania; 



//kody komend
//****************************************************/
/*
Budowa komendy:
arg1 - typ operacji
arg2 - peryferia ktorego dotyczy
arg3 - dodatkowy arg
arg4 - dodatkowy arg
ka¿dy rozkaz sk³ada siê z 4 argumentów + znacznik pocz¹tku i koñca
po rozkazie nastepuje przes³anie wartoœci do zapisu (w przypadku USTAW lub nie w przypadku WYSLIJ
*//*********************************************************/
/*
Sk³adnia komendy , przyk³ad:
arg[0] = '1' - USTAW
arg[1] = 'z' - zasiacz
arg[2] = '3' - 3 kana³
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

#include "uC_UART.h"
#include "uC_ADC.h"
#include "PCB_I2C.h"

#include "PCB_PowSupply.h"
#include <util/delay.h>
#include "uC_PWM.h"
#include "PCB_DigitalPot.h"


typedef struct {
	float value;
	char arg[4];
	
}KOM;
  
KOM polecenie;
//********************definicje znaków dla komendy*********************//
#define USTAW 0
#define WYSWIETL 1
#define ZMIEN_TRYB 2

#define ZASILACZ_ 'z'	
#define GENERATOR 'g'
#define WYJSCIE_ANALOGOWE 'a'
#define KOMPARATOR 'c'
#define PRZEKAZNIK 'r'

	
	#define PRZEBIEG 'p'
	#define CZESTOTLIWOSC 'f'
	#define OGRANICZENIE 'c'
	#define NAPIECIE 'v'
	

#define operation 0
#define device 1
#define parameter 2
#define channel 3

/*
polecenie.arg[0] == polecenie.arg[operation]
polecenie.arg[1]== polecenie.arg[device]
polecenie.arg[2]== polecenie.arg[parameter]
polecenie.arg[3]==	polecenie.arg[channel]
*/

#define mark_arg '*'
#define mark_val '#'
#define mark_end '/'

#define END_OF_TAB '\0'
#define SET 1
#define CLR 0
//*****************************************//


void config_ext_int(void);
void init_dev_onPCB(void);
void CPU_init(void); 
void Diagnostics(void);
void Skan(void);    //skanuje co okreœlony czas wszytkie parametry, oraz wysy³a je wszystkie po UART 
void UART_raport(char *result); 
void UART_Get_command(void);          //pobiera rozkaz i zwraca tablice
void Decode_command(void);

