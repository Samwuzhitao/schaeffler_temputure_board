
#include "OneWire.h"
#include "stdio.h"

#include "het.h"
volatile static unsigned char globalVar_Channel = 0;			//num 0~1 for channel 0~1

//TEDS Enable pin functions
/*============================================================================*/
static void PinEnMode()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_EN_MODE; break;
		case 1 : TEDS1_EN_MODE; break;
		default: break;
	}
}

static void PinEnHIGH()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_EN_HIGH; break;
		case 1 : TEDS1_EN_HIGH; break;
		default: break;
	}
}

static void PinEnLOW()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_EN_LOW; break;
		case 1 : TEDS1_EN_LOW; break;
		default: break;
	}
}
/*============================================================================*/


//TEDS Write pin functions
/*============================================================================*/
static void PinOutMode()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_WRITE_MODE; break;
		case 1 : TEDS1_WRITE_MODE; break;
		default: break;
	}
}

static void PinOutHIGH()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_WRITE_HIGH; break;
		case 1 : TEDS1_WRITE_HIGH; break;
		default: break;
	}
}

static void PinOutLOW()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_WRITE_LOW; break;
		case 1 : TEDS1_WRITE_LOW; break;
		default: break;
	}
}
/*============================================================================*/


//TEDS Read pin functions
/*============================================================================*/
static void PinInMode()
{
	switch( globalVar_Channel )
	{
		case 0 : TEDS0_READ_MODE; break;
		case 1 : TEDS0_READ_MODE; break;
		default: break;
	}
}


static unsigned char PinInput( void )
{
	switch( globalVar_Channel )
	{
		case 0 :
		{
			if( IS_TEDS0_READ_HIGH ){
				return HIGH;
			}else{
				return LOW;
			}
		}
		case 1 :
		{
			if( IS_TEDS1_READ_HIGH ){
				return HIGH;
			}else{
				return LOW;
			}
		}
		default: return ERROR;
	}
}
/*============================================================================*/


//TEDS 1-Wire Write byte functions
/*============================================================================*/
static void OneWire_WriteOne()
{
	PinOutLOW();		//Pull down the bus line
	delay_us( 8 );
	PinOutHIGH();		//Release the bus line
	delay_us( 110 );
}

static void OneWire_WriteZero()
{
	PinOutLOW();		//Pull down the bus line
	delay_us( 80 );
	PinOutHIGH();		//Release the bus line
	delay_us( 40 );
}

void OneWire_TxByte( unsigned char Byte )
{
	unsigned char i = 0;
	PinOutMode();
	for( i = 0; i < 8; i++ ){
		if( Byte & 0x01 ){
			OneWire_WriteOne();
		}else{
			OneWire_WriteZero();
		}
		Byte >>= 1;
	}
}
/*============================================================================*/


//TEDS 1-Wire Read byte functions
/*============================================================================*/
static unsigned char OneWire_ReadBit()
{
	unsigned char Bit = 0;

	PinOutMode();
	PinOutLOW();		//Pull down the bus line
	delay_us( 4 );
	PinOutHIGH();		//Release the line bus 
	delay_us( 4 );
	if( HIGH == PinInput() ){
		 Bit = 1;
	}else{
		Bit = 0;
	}
	delay_us( 100 );
	return Bit;
}


unsigned char OneWire_RxByte( )
{
	unsigned char i = 0;
	unsigned char Byte = 0;

	for( i = 0; i < 8; i++ ){
		Byte >>= 1;
		if( OneWire_ReadBit() ){
			Byte |= 0x80;
		}
	}

	return Byte;
}
/*============================================================================*/


unsigned char OneWireDevice_Selection( unsigned char DeviceNo )
{
	if( (DeviceNo >= 0) || (DeviceNo < NUMBER_OF_DEVICE ) ){
		globalVar_Channel = DeviceNo;
		return DeviceNo;
	}else{
		globalVar_Channel = 0xFF;
		return 0xFF;
	}
}


void OneWire_Init( )
{
	{//Initialize Pin
		PinEnHIGH();
		PinEnMode();

		PinOutHIGH();
		PinOutMode();

		PinInMode();
		delay_us( 10000 );
	}

	{//reset pulse
		PinOutMode();
		PinOutLOW();		//Pull down the bus line
		delay_us( 500 );
		PinOutHIGH();		//Release the bus line
		delay_us( 40 );
	}

	{//presence pulse checking
		unsigned char i = 0;
		unsigned char DevicePresence = 0;
		PinInMode();
		for( i = 0; i < 20; i++ ){
			if( LOW == PinInput() ){
				DevicePresence = 1;
				break;
			}
			delay_us( 10 );
		}
		if( !DevicePresence ){
			printf("No slave\n\r");
		}else{
			printf("discover slave\n\r");
		}
		printf("Wait for releasing the bus:");
		delay_us( 200 );
		if( HIGH == PinInput() ){
			printf("Succ\n\r");
		}else{
			printf("Fail\n\r");
		}
	}
	PinOutMode();
}
