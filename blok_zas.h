/*
 * blok_zas.h
 *
 * Created: 02.07.2019 15:05:51
 *  Author: Kolunio
 */ 
#include <stdio.h>
#include <string.h>

#ifndef BLOK_ZAS_H_
#define BLOK_ZAS_H_



//*********zmienne
  struct zas{
	float c, v;
	
	};
	
struct	zas zas_ch[4];

//*****Procedury obs³ugi*******
 void zas_ustaw(char,char);

 void zas_spr_ovc(void);
 void zas_diagn(void);


 void zas_EN( void );
 void zas_DIS( void );

#endif /* BLOK_ZAS_H_ */