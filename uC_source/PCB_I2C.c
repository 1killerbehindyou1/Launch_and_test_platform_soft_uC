/*
 * Init_I2C_Devices.c
 *
 * Created: 28.06.2019 03:45:56
 *  Author: Kolunio
 */ 
#include "uC_UART.h"
#include "PCB_I2C.h"


//DEFINICJE ADRESOW SLAWOW
#include <util/twi.h>
#include <util/delay.h> 
#include <avr/io.h>


#define sbi(PORT,PIN) ((PORT)|=1<<(PIN))
#define cbi(PORT,PIN) ((PORT)&=~(1<<(PIN)))

 #include <inttypes.h>
 
void init_i2C_dev()
{
	
	 //PORTC|=((1<<PC0)|(1<<PC1)); // wlaczenie podciagania dla i2c
	 sbi(TWCR,TWEA); //w??czenie generacji potwierdze? ACK
	 sbi(TWCR,TWINT); //znacznik przerwania dla modu?u TWI
	 sbi(TWCR,TWEN); //w??czenie modu?u TWI
	 sbi(TWCR,TWIE); //uaktywnienie przerwañ dla modu?u TWI
	 TWSR = 0;                         /* no prescaler */
	 TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */
	//reset the pin status

}


 

/*
 * initialize
 *//*
void pcf8574_init() {
	//init i2c
	i2c_init();
	_delay_us(10);

	//reset the pin status
	uint8_t i = 0;
	for(i=0; i<PCF8574_MAXDEVICES; i++)
		pcf8574_pinstatus[i] = 0;

}*/

/*
 * get output status
 */
int8_t  PCA9555_getoutput(uint8_t deviceid) {
	int8_t data = -1;
	if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES)) {
		data = pcf8574_pinstatus[deviceid];
	}
	return data;
}

/*
 * get output pin status
 */
int8_t  PCA9555_getoutputpin(uint8_t deviceid, uint8_t pin) {
	int8_t data = -1;
	if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pin >= 0 && pin < PCF8574_MAXPINS)) {
		data = pcf8574_pinstatus[deviceid];
		data = (data >> pin) & 0b00000001;
	}
	return data;
}

/*
 * set output pins
 */
int8_t PCA9555_setoutput(uint8_t adres,uint8_t data) {
	//if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES)) {
		//pcf8574_pinstatus[deviceid] = data;
		//i2c_start(((PCF8574_ADDRBASE+deviceid)<<1) | I2C_WRITE);
		i2c_start(adres);
		i2c_write(data);
		i2c_stop();
		return 0;
	//}
	//return -1;
}

/*
 * set output pins, replace actual status of a device from pinstart for pinlength with data
 */
int8_t PCA9555_setoutputpins(uint8_t deviceid, uint8_t pinstart, uint8_t pinlength, int8_t data) {
	//example:
	//actual data is         0b01101110
	//want to change              ---
	//pinstart                    4
	//data                        101   (pinlength 3)
	//result                 0b01110110
	if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pinstart - pinlength + 1 >= 0 && pinstart - pinlength + 1 >= 0 && pinstart < PCF8574_MAXPINS && pinstart > 0 && pinlength > 0)) {
	    uint8_t b = 0;
	    b = pcf8574_pinstatus[deviceid];
	    uint8_t mask = ((1 << pinlength) - 1) << (pinstart - pinlength + 1);
		data <<= (pinstart - pinlength + 1);
		data &= mask;
		b &= ~(mask);
		b |= data;
	    pcf8574_pinstatus[deviceid] = b;
	    //update device
		i2c_start(((PCF8574_ADDRBASE+deviceid)<<1) | I2C_WRITE);
		i2c_write(b);
		i2c_stop();
		return 0;
	}
	return -1;
}

/*
 * set output pin
 */
int8_t PCA9555_setoutputpin(uint8_t deviceid, uint8_t pin, uint8_t data) {
	if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pin >= 0 && pin < PCF8574_MAXPINS)) {
	    uint8_t b = 0;
	    b = pcf8574_pinstatus[deviceid];
	    b = (data != 0) ? (b | (1 << pin)) : (b & ~(1 << pin));
	    pcf8574_pinstatus[deviceid] = b;
	    //update device
		i2c_start(((PCF8574_ADDRBASE+deviceid)<<1) | I2C_WRITE);
		i2c_write(b);
		i2c_stop();
		return 0;
	}
	return -1;
}

/*
 * set output pin high
 */
int8_t pcf8574_setoutputpinhigh(uint8_t deviceid, uint8_t pin) {
	return pcf8574_setoutputpin(deviceid, pin, 1);
}

/*
 * set output pin low
 */
int8_t pcf8574_setoutputpinlow(uint8_t deviceid, uint8_t pin) {
	return pcf8574_setoutputpin(deviceid, pin, 0);
}


/*
 * get input data
 */
int8_t PCA9555_getinput(uint8_t deviceid) {
	int8_t data = -1;
	if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES)) {
		i2c_start(((PCF8574_ADDRBASE+deviceid)<<1) | I2C_READ);
		data = ~i2c_readNak();
		i2c_stop();
	}
	return data;
}

/*
 * get input pin (up or low)
 */
int8_t PCA9555_getinputpin(uint8_t deviceid, uint8_t pin) {
	int8_t data = -1;
	if((deviceid >= 0 && deviceid < PCF8574_MAXDEVICES) && (pin >= 0 && pin < PCF8574_MAXPINS)) {
		data = pcf8574_getinput(deviceid);
		if(data != -1) {
			data = (data >> pin) & 0b00000001;
		}
	}
	return data;
}

void Scan_I2C(void)
{
	for(int i=1;i<128;i++)
	{
			i2c_start(i);
	
	if(TWSR == 0x18)
	{
		rsPrint("\n\rOdnaleziono urzadzenie pod Adresem =");
		PrintDecInt(i,3);
		rsPrint("\n\r");
	}
	}
	
	
}

void print_TWSR(void)
{
switch (TWSR)
{
	case 0x08: rsPrint("A START condition has been  transmitted\n\r");
	break;
	case 0x10: rsPrint("A repeated START conditionhas been transmitted\n\r");
	break;
	case 0x18: rsPrint("SLA+W has been transmitted ACK has been received\n\r");
	break;
	case 0x20:  rsPrint("SLA+W has been transmitted NOT ACK has been received\n\r");
	break;
	case 0x28:rsPrint("Data byte has been transmitted;ACK has been received\n\r");
	break;
	case 0x30:  rsPrint("Data byte has been transmitted NOT ACK has been received\n\r");
	break;
	case 0x38: rsPrint("Arbitration lost in SLA+W ordata bytes\n\r");
	break;
	
	
}	
}

/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return 0 = device accessible, 1= failed to access device
*************************************************************************/
unsigned char i2c_start(unsigned char address)
{
    uint8_t   twst;									
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);    // send START condition										
	while(!(TWCR & (1<<TWINT)));						// wait until transmission completed
														// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;										
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);							// send device address
										
	while(!(TWCR & (1<<TWINT)));							// wail until transmission completed and
															// ACK/NACK has been received

	
	twst = TW_STATUS & 0xF8;								// check value of TWI Status Register. Mask prescaler bits.
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;

}


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 
 Input:   address and transfer direction of I2C device
*************************************************************************/
void i2c_start_wait(unsigned char address)
{
    uint8_t   twst;


    while ( 1 )
    {
	    // send START condition
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
    	// wait until transmission completed
    	while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
    
    	// send device address
    	TWDR = address;
    	TWCR = (1<<TWINT) | (1<<TWEN);
    
    	// wail until transmission completed
    	while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
    	{    	    
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	        
	        // wait until stop condition is executed and bus released
	        while(TWCR & (1<<TWSTO));
	        
    	    continue;
    	}
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }

}/* i2c_start_wait */


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction 

 Input:   address and transfer direction of I2C device
 
 Return:  0 device accessible
          1 failed to access device
*************************************************************************/
unsigned char i2c_rep_start(unsigned char address)
{
    return i2c_start( address );

}/* i2c_rep_start */


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
void i2c_stop(void)
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}/* i2c_stop */


/*************************************************************************
  Send one byte to I2C device
  
  Input:    byte to be transfered
  Return:   0 write successful 
            1 write failed
*************************************************************************/
unsigned char i2c_write( uint8_t data )
{	
    uint8_t   twst;
    
	// send data to the previously addressed device
	TWDR = data;

	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}/* i2c_write */


/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;

}/* i2c_readAck */


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;

}/* i2c_readNak */
