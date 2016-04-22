

#ifndef XTMS570_1227_DS2431_ONEWIRE_ONEWIRE_H_
#define XTMS570_1227_DS2431_ONEWIRE_ONEWIRE_H_

//#include "sys_common.h"

#define NUMBER_OF_DEVICE	8
#define DEVICE_0		0
#define DEVICE_l		1
#define DEVICE_2		2
#define DEVICE_3		3
#define DEVICE_4		4
#define DEVICE_4		4
#define DEVICE_5		5
#define DEVICE_6		6
#define DEVICE_7		7
#define NO_DEVICE		0xFF
#define ERROR			0xFF

#define delay_us( num )		__delay_cycles( 45 * num )

unsigned char OneWireDevice_Selection( unsigned char Device );
void OneWire_Init( );
void OneWire_TxByte( unsigned char Byte );
unsigned char  OneWire_RxByte( );

#define TEDS0
#define TEDS1

#ifdef TEDS0_WRITE_PIN_DS2431

	#define		PIN_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 4U)
	#define		PIN_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 4U)
	#define		PIN_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 4U)

	#define		PIN_OUT_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 0U)
	#define		PIN_IN_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 0U) )

	#define		PIN_OUT_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 0U)
	#define		PIN_OUT_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 0U)

	#define		IS_PIN_IN_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 0U)
	#define		IS_PIN_IN_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 0U))

#endif

#ifdef TEDS0

	#define		TEDS0_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 4U)
	#define		TEDS0_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 4U)
	#define		TEDS0_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 4U)

	#define		TEDS0_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 0U)
	#define		TEDS0_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 0U)
	#define		TEDS0_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 0U)

	#define		TEDS0_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 9U) )
	#define		IS_TEDS0_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 9U)
	#define		IS_TEDS0_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 9U))

#endif

#ifdef TEDS1_WRITE_PIN_DS2431

	#define		PIN_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 7U)
	#define		PIN_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 7U)
	#define		PIN_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 7U);

	#define		PIN_OUT_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 3U)
	#define		PIN_IN_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 3U) )

	#define		PIN_OUT_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 3U)
	#define		PIN_OUT_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 3U)

	#define		IS_PIN_IN_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 3U)
	#define		IS_PIN_IN_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 3U))

#endif

#ifdef TEDS1

	#define		TEDS1_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 7U)
	#define		TEDS1_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 7U)
	#define		TEDS1_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 7U)

	#define		TEDS1_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 3U)
	#define		TEDS1_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 3U)
	#define		TEDS1_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 3U)

	#define		TEDS1_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 5U) )
	#define		IS_TEDS1_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 5U)
	#define		IS_TEDS1_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 5U))

#endif

#define		HIGH		1
#define		LOW		0


#endif /* XTMS570_1227_DS2431_ONEWIRE_ONEWIRE_H_ */
