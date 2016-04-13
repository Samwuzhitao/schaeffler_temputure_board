/*
 * ADS1247.c
 *
 *  Created on: 2016Äê4ÔÂ13ÈÕ
 *      Author: sam.wu
 */
#include "ADS1247.h"
#include "spi.h"

#define ADS1247_WRITE_REG_DELAYMS 10

spiDAT1_t dataconfig1_t;

void spiSetCs(spiBASE_t *spi, uint8 nCs)
{
	spi->PC3 |=    (uint32)((uint32)1U << nCs) ; /* SCS[0] */
}

void spiClearCs(spiBASE_t *spi, uint8 nCs)
{
	spi->PC3 &=   ~(uint32)((uint32)1U << nCs) ; /* SCS[0] */
}

/******************************************************************************
  Function:ADS1247_ReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint8_t ADS1247_ReadWriteData( uint8_t data )
{
	uint16 Writedata,ReadData;

	Writedata = data;

	spiTransmitAndReceiveData(spiREG1, &dataconfig1_t, 1, &Writedata, &ReadData);

	return ReadData;
}

/******************************************************************************
  Function:ADS1247_Delay
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void ADS1247_Delay(unsigned int nCount)
{
	volatile unsigned int i,j;
	for(i = nCount; i > 0 ;i--)
		for(j = 10000; j > 0; j--);
}
/******************************************************************************
  Function:ADS1247_ReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
  Output:
  Return:
  Others:None
******************************************************************************/
void ADS1247_ReadRegisters( uint8_t addr, uint8_t num, uint8_t data[] )
{
	uint8_t i;

	spiClearCs(spiREG1, 0);

	ADS1247_ReadWriteData( addr | ADS1247_CMD_RREG );

	ADS1247_ReadWriteData( num );

	for(i=0; i<num+1; i++)
	{
		data[i] = ADS1247_ReadWriteData(ADS1247_CMD_NOP);
	}

	spiSetCs(spiREG1, 0);
}

/******************************************************************************
  Function:ADS1247_ReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
  Output:
  Return:
  Others:None
******************************************************************************/
uint8_t ADS1247_ReadRegister( uint8_t addr )
{
	uint8_t data;

	spiClearCs(spiREG1, 0);

	ADS1247_ReadWriteData( addr | ADS1247_CMD_RREG );

	ADS1247_ReadWriteData( 0x00 );

	data = ADS1247_ReadWriteData( ADS1247_CMD_NOP );


	spiSetCs(spiREG1, 0);

	return data;
}
/******************************************************************************
  Function:ADS1247_ReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
  Output:
  Return:
  Others:None
******************************************************************************/
void ADS1247_WriteRegisters( uint8_t addr, uint8_t num, uint8_t data[] )
{
	uint8_t i;

	spiClearCs(spiREG1, 0);

	ADS1247_ReadWriteData( addr | ADS1247_CMD_WREG );

	ADS1247_ReadWriteData( num );

	for(i=0; i<num+1; i++)
	{
		 ADS1247_ReadWriteData(data[i]);
	}

	spiSetCs(spiREG1, 0);
}

/******************************************************************************
  Function:ADS1247_ReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		data : data
  Output:
  Return:
  Others:None
******************************************************************************/
void ADS1247_WriteRegister( uint8_t addr, int8_t data )
{
	spiClearCs(spiREG1, 0);

	ADS1247_ReadWriteData( addr | ADS1247_CMD_WREG );

	ADS1247_ReadWriteData( 0x00 );

	ADS1247_ReadWriteData( data );

	spiSetCs(spiREG1, 0);
}
/******************************************************************************
  Function:ADS1247_ReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void ADS1247_Init( void )
{
	// Reset ADC
	spiClearCs(spiREG1, 0);
	ADS1247_ReadWriteData( ADS1247_CMD_RESET );
	spiSetCs(spiREG1, 0);
	ADS1247_Delay(60);

	// Positive = IN1 Negitive = IN2
	ADS1247_WriteRegister( ADS1247_REG_MUX0, 0x0A );
	ADS1247_Delay(ADS1247_WRITE_REG_DELAYMS);

	// not Enable Bias voltage
	ADS1247_WriteRegister( ADS1247_REG_VBIAS, 0x00 );
	ADS1247_Delay(ADS1247_WRITE_REG_DELAYMS);

	// Set Internal OSC, (IDAC)Internal reference, (ADC)OnBoard reference
	ADS1247_WriteRegister( ADS1247_REG_MUX1, 0x30 );
	ADS1247_Delay(ADS1247_WRITE_REG_DELAYMS);

	// Set PGA = 1   DOR = 320SPS
	ADS1247_WriteRegister( ADS1247_REG_SYS0, 0x06 );
	ADS1247_Delay(ADS1247_WRITE_REG_DELAYMS);

	// Set DOUT/(/D/R/D/Y) pin functons only as Data Out
  // I = 200uA
	ADS1247_WriteRegister( ADS1247_REG_IDAC0, 0x02 );
	ADS1247_Delay(ADS1247_WRITE_REG_DELAYMS);

	// Connected the AIN0 and AIN3 pin for the  source DAC
	ADS1247_WriteRegister( ADS1247_REG_IDAC1, 0x30 );
	ADS1247_Delay(ADS1247_WRITE_REG_DELAYMS);
}

/******************************************************************************
  Function:ADS1247_ReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint32_t ADS1247_ReadData( void )
{
	uint8_t i,Data[3];

	spiClearCs(spiREG1, 0);

	ADS1247_ReadWriteData( ADS1247_CMD_RDATA );

	for(i=0; i<3; i++)
	{
		Data[i] = ADS1247_ReadWriteData(ADS1247_CMD_NOP);
	}

	spiSetCs(spiREG1, 0);

	return (Data[0]<<16 | Data[1]<<8 | Data[2]);
}
