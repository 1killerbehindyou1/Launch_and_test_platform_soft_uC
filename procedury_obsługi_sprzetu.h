/*
 * procedury_obsługi_sprzetu.h
 *
 * Created: 02.07.2019 08:20:46
 *  Author: Kolunio
 */ 


#ifndef PROCEDURY_OBSŁUGI_SPRZETU_H_
#define PROCEDURY_OBSŁUGI_SPRZETU_H_

//*********GPIO*********
void ustaw_przek(int);



//*************ZASILACZ************
int ustaw_ch (int kan, int v);
int ovc1, ovc2, ovc3;

#endif /* PROCEDURY_OBSŁUGI_SPRZETU_H_ */