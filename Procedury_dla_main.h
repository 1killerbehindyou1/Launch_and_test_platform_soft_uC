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
void Skanuj(void);    //skanuje co okreœlony czas wszytkie parametry, oraz wysy³a je wszystkie po UART 
void UART_raport(char *result); 
void UART_Pobierz_rozkaz(void);          //pobiera rozkaz i zwraca tablice
void Dekoduj_rozkaz(void);


float chartofl(char * );
char *intoch(int);