/*
 * procedury_obsługi_sprzetu.h
 *
 * Created: 02.07.2019 08:20:46
 *  Author: Kolunio
 */ 


#ifndef PROCEDURY_OBSLUGI_SPRZETU_H_
#define PROCEDURY_OBSLUGI_SPRZETU_H_

//dzielniki napiec
#define divVol1 4.06

#define divCurr1 4.38

#define vol 1
#define curr 0

float set_vol(uint8_t);
void check_ovc(uint8_t );
typedef struct  {
	float current;
	float voltage;
	//bool EN;
}Zasilacz;

 Zasilacz CH1, CH2, CH3, CH4;


void Skan_zas(uint8_t );




#endif /* PROCEDURY_OBSŁUGI_SPRZETU_H_ */