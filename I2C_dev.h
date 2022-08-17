/*
 * Init_I2C_dev.h
 *
 * Created: 28.06.2019 05:17:10
 *  Author: Kolunio
 */ 

#include <inttypes.h>
#ifndef INIT_I2C_DEV_H_
#define INIT_I2C_DEV_H_


//ADRESY URZADZEñ 
#define adr_lewy 64
#define adr_dolny 78
#define adr_prawy  70

#define Reg_Ind_lewy 1
#define Reg_Ind_dolny 2
#define Reg_Ind_prawy  3

#define I2C_MAXDEVICES 128
#define SCL_CLOCK  10000UL
#define F_CPU 16000000UL

#define I2C_READ 1
#define I2C_WRITE 0

#define PCF8574_ADDRBASE (0x20) //device base address


#define PCF8574_MAXDEVICES 8 //max devices, depends on address (3 bit)
#define PCF8574_MAXPINS 8 //max pin per device

//settings

//pin status
volatile uint8_t pcf8574_pinstatus[PCF8574_MAXDEVICES];


//functions

 int8_t pcf8574_getoutput(uint8_t );
 int8_t pcf8574_getoutputpin(uint8_t , uint8_t );
 int8_t pcf8574_setoutput(uint8_t,uint8_t );
int8_t pcf8574_setoutputpins(uint8_t , uint8_t , uint8_t , int8_t );
 int8_t pcf8574_setoutputpin(uint8_t , uint8_t, uint8_t );
 int8_t pcf8574_setoutputpinhigh(uint8_t , uint8_t );
 int8_t pcf8574_setoutputpinlow(uint8_t , uint8_t );
 int8_t pcf8574_getinput(uint8_t );
 int8_t pcf8574_getinputpin(uint8_t , uint8_t );


/*
#define Prawy_IndI2C Reg.device_Reg_indeks[1]
#define Lewy_IndI2C Reg.device_Reg_indeks[2]
#define Dolny_IndI2C Reg.device_Reg_indeks[3]

Reg I2C_Reg[Reg_Ind_lewy][]= adr_lewy;
Reg I2C_Reg[Reg_Ind_dolny][]= adr_dolny;
Reg I2C_Reg[Reg_Ind_prawy][]= adr_prawy;
*/
/*
Reg I2C_Reg.device_Reg_indeks[1] = &(I2C_Reg.device_REG[Rlewy][]);
Reg I2C_Reg.device_Reg_indeks[2] = &(I2C_Reg.device_REG[Rprawy][]);
Reg I2C_Reg.device_Reg_indeks[3] = &(I2C_Reg.device_REG[Rdolny][]);
*/
void init_i2C_dev(void);
unsigned char i2c_readAck(void);
unsigned char i2c_readNak(void);

  unsigned char i2c_write( uint8_t data );

void Scan_I2C(void);
unsigned char i2c_start(unsigned char);
 void i2c_stop(void);
void print_TWSR(void);

#define ZAS 0x70
#define CHAN3	3


#endif /* INIT_I2C_DEV_H_ */
//#define ADC_CONW 0b1001001x
//#define 0b10001100  //komenda [bit 8 1 - kanal pojedynczy 0 roznicowy, bity 4-6 wybor kanalu i
               //bity 2, 3 - 11 - wewnetrze nap odsn, start konwersj 1, 0 nieuzyw
			   
			   
//#define EXP_GPIO 0x21


//#define EXP_GEN 0x03


//#define EEPROM 0x04
 //uint8_t conf[2];
//void init_i2C_dev(void);