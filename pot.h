/*
 * pot.h
 *
 * Created: 25.07.2019 15:33:02
 *  Author: Kolunio
 */
#include <avr/io.h> 
#define sbi(PORT,PIN) ((PORT)|=1<<(PIN))
#define cbi(PORT,PIN) ((PORT)&=~(1<<(PIN)))

#ifndef POT_H_
#define POT_H_
#define INC PORTC6 
#define U_D PORTC7 
#define PORTPOT PORTC

#define up 1
#define down 0
void pot_init(void);
void trig_pot(uint8_t);

#endif /* POT_H_ */