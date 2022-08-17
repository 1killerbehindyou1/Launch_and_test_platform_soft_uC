/*
 * procedury_obsługi_sprzetu.h
 *
 * Created: 02.07.2019 08:20:46
 *  Author: Kolunio
 */ 


#ifndef PROCEDURY_OBSLUGI_SPRZETU_H_
#define PROCEDURY_OBSLUGI_SPRZETU_H_

//dzielniki napiec
#define Pow 10 

#define DivVolCH1 4.06
#define DivCurCH1 4.38
#define RES1 0.1

#define DivVolCH2 4.06
#define DivCurCH2 4.38
#define RES2 0.1

#define DivVolCH3 4.06
#define DivCurCH3 4.38
#define RES3 0.1

#define DivVolCH4 4.06
#define DivCurCH4 4.38
#define RES4 0.1

#define curr_zero 0.02
#define volt_zero 1

#define current_source 1
#define voltage_source 0
#define disable 0
#define enable 1


float SkanVol(uint8_t );
void SetVol(uint8_t , float);
void SetOvCurr(uint8_t , float);	
void CheckOvCurr(uint8_t);
void SetVoltageSource(uint8_t );

float SkanCurr(uint8_t );	
void SetCurr(uint8_t, float);	
void SetOvVol(uint8_t, float);
void CheckOvVol(uint8_t);
void SetCurrentSource(uint8_t);			

void CheckOvPower(uint8_t);
void Skan_all_supply(void );

typedef struct {
	
	float OvCurr;				//ograniczenie pradowe
	float OvVol;				// ograniczenie napięciowe w trybie zródla pradowego
	float ResCurr;				//dokładna wartosc rezystorza szeregoowego
	float DivVol;				//podział dzielnika pomiaru napiecioa
	float DivCurr;				//podział dzielnika pomiaru pradu
	uint8_t EN;					//1 vwłaczony, 0 wył
	uint8_t MOD;				// 0- tryb const voltage 1- trub zródła pradowego
	float Current;				//wartosc radu w trybie złodła pradu
	float Voltage;				// wrtosc napiecia w trybie zródła napiecia
	float PowMax;				// Maksymalna Moc dostarczana przez kanał
	
	
}Zasilacz;


#endif /* PROCEDURY_OBSŁUGI_SPRZETU_H_ */