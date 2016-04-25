

#ifndef XTMS570_1227_DS2431_ONEWIRE_ONEWIRE_H_
#define XTMS570_1227_DS2431_ONEWIRE_ONEWIRE_H_

//#include "sys_common.h"

#define NUMBER_OF_DEVICE	8
#define DEVICE_0		0
#define DEVICE_1		1
#define DEVICE_2		2
#define DEVICE_3		3
#define DEVICE_4		4
#define DEVICE_5		5
#define DEVICE_6		6
#define DEVICE_7		7
#define OK				0
#define ERROR			0xFF
#define ERROR_NO_DEVICE				0xFE


#define delay_us( num )		__delay_cycles( 45 * num )

unsigned char OneWireDevice_Selection( unsigned char Device );
unsigned char OneWire_Init( );
void OneWire_TxByte( unsigned char Byte );
unsigned char  OneWire_RxByte( );

#define TEDS0
#define TEDS1
#define TEDS2
#define TEDS3
#define TEDS4
#define TEDS5
#define TEDS6
#define TEDS7
            
#ifdef TEDS0_WRITE_PIN_DS2431

	#define		PIN_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 4U)
	#define		PIN_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 4U)
	#define		PIN_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 4U)

	#define		PIN_OUT_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 0U)
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

#ifdef TEDS2

	#define		TEDS2_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 25U)
	#define		TEDS2_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 25U)
	#define		TEDS2_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 25U)

	#define		TEDS2_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 1U)
	#define		TEDS2_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 1U)
	#define		TEDS2_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 1U)

	#define		TEDS2_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 2U) )
	#define		IS_TEDS2_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 2U)
	#define		IS_TEDS2_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 2U))

#endif

#ifdef TEDS3

	#define		TEDS3_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 19U)
	#define		TEDS3_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 19U)
	#define		TEDS3_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 19U)

	#define		TEDS3_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 22U)
	#define		TEDS3_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 22U)
	#define		TEDS3_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 22U)

	#define		TEDS3_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 15U) )
	#define		IS_TEDS3_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 15U)
	#define		IS_TEDS3_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 15U))

#endif

#ifdef TEDS4

	#define		TEDS4_EN_MODE			gioPORTA->DIR|= (uint32)((uint32)1U << 5U)
	#define		TEDS4_EN_HIGH			gioPORTA->DSET = (uint32)((uint32)1U << 5U)
	#define		TEDS4_EN_LOW			gioPORTA->DCLR = (uint32)((uint32)1U << 5U)

	#define		TEDS4_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 18U)
	#define		TEDS4_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 18U)
	#define		TEDS4_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 18U)

	#define		TEDS4_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 31U) )
	#define		IS_TEDS4_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 31U)
	#define		IS_TEDS4_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 31U))

#endif

#ifdef TEDS5

	#define		TEDS5_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 30U)
	#define		TEDS5_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 30U)
	#define		TEDS5_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 30U)

	#define		TEDS5_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 12U)
	#define		TEDS5_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 12U)
	#define		TEDS5_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 12U)

	#define		TEDS5_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 14U) )
	#define		IS_TEDS5_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 14U)
	#define		IS_TEDS5_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 14U))

#endif

#ifdef TEDS6

	#define		TEDS6_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 28U)
	#define		TEDS6_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 28U)
	#define		TEDS6_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 28U)

	#define		TEDS6_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 8U)
	#define		TEDS6_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 8U)
	#define		TEDS6_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 8U)

	#define		TEDS6_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 10U) )
	#define		IS_TEDS6_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 10U)
	#define		IS_TEDS6_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 10U))

#endif

#ifdef TEDS7

	#define		TEDS7_EN_MODE			hetREG1->DIR |= (uint32)((uint32)1U << 23U)
	#define		TEDS7_EN_HIGH			hetREG1->DSET = (uint32)((uint32)1U << 23U)
	#define		TEDS7_EN_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 23U)

	#define		TEDS7_WRITE_MODE		hetREG1->DIR |= (uint32)((uint32)1U << 24U)
	#define		TEDS7_WRITE_HIGH		hetREG1->DSET = (uint32)((uint32)1U << 24U)
	#define		TEDS7_WRITE_LOW			hetREG1->DCLR = (uint32)((uint32)1U << 24U)

	#define		TEDS7_READ_MODE			hetREG1->DIR &= ~( (uint32)((uint32)1U << 26U) )
	#define		IS_TEDS7_READ_HIGH		hetREG1->DIN & (uint32)((uint32)1U << 26U)
	#define		IS_TEDS7_READ_LOW		!(hetREG1->DIN & (uint32)((uint32)1U << 26U))

#endif

#define		HIGH		1
#define		LOW		0


#endif /* XTMS570_1227_DS2431_ONEWIRE_ONEWIRE_H_ */
