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

char arg[4];
float value;

#define USTAW '1'
	#define Zasilacz "z"
		
	
	
#define generator "g"
#define wyjscie_analogowe "a"
#define komparator "c"
#define przekaznik "r"

#define WYSWIETL '2'


#define  INNE  '3'

//*****************************************//




void inicjalizacja_mikrokontrolera(void); 
void Diagnostyka(void);
void Skanuj(void);    //skanuje co okre�lony czas wszytkie parametry, oraz wysy�a je wszystkie po UART 
void UART_raport(char *result); 
void UART_Pobierz_rozkaz(void);          //pobiera rozkaz i zwraca tablice
void Dekoduj_rozkaz(void);


float chartofl(char * );
char *intoch(int);