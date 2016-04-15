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

/******************************************************************************
  Function:
  	  SpiSetCs
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void SpiSetCs( spiBASE_t *spi, uint8 nCs )
{
	spi->PC3 |=    (uint32)((uint32)1U << nCs) ; /* SCS[0] */
}

/******************************************************************************
  Function:
  	  SpiClearCs
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void SpiClearCs( spiBASE_t *spi, uint8 nCs )
{
	spi->PC3 &=   ~(uint32)((uint32)1U << nCs) ; /* SCS[0] */
}

/******************************************************************************
  Function:
  	  SpiReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint8_t SpiReadWriteData( spiBASE_t *spi, uint8_t data )
{
	uint16 Writedata,ReadData;

	Writedata = data;

	spiTransmitAndReceiveData( spi, &dataconfig1_t, 1, &Writedata, &ReadData );

	return ReadData;
}

/******************************************************************************
  Function:
  	  SpiDelay
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Ads1247Delay( uint32 nCount )
{
	volatile uint32 i,j;
	for(i = nCount; i > 0 ;i--)
		for(j = 10000; j > 0; j--);
}
/******************************************************************************
  Function:
  	  Ads1247ReadRegisters
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
  Output:
  Return:
  Others:None
******************************************************************************/
void Ads1247ReadRegisters( spiBASE_t *spi, uint8_t addr, uint8_t num, uint8_t data[] )
{
	uint8_t i;

	SpiClearCs( spi, 0 );

	SpiReadWriteData( spi, addr | ADS1247_CMD_RREG );

	SpiReadWriteData( spi, num );

	for(i=0; i<num+1; i++)
	{
		data[i] = SpiReadWriteData( spi, ADS1247_CMD_NOP );
	}

	SpiSetCs( spi, 0 );
}

/******************************************************************************
  Function:
  	  spiReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
  Output:
  Return:
  Others:None
******************************************************************************/
uint8_t Ads1247ReadRegister( spiBASE_t *spi, uint8_t addr )
{
	uint8_t data;

	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, addr | ADS1247_CMD_RREG );

	SpiReadWriteData( spi, 0x00 );

	data = SpiReadWriteData( spi, ADS1247_CMD_NOP );


	SpiSetCs( spi, 0);

	return data;
}
/******************************************************************************
  Function:Ads1247ReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
  Output:
  Return:
  Others:None
******************************************************************************/
void Ads1247WriteRegisters( spiBASE_t *spi, uint8_t addr, uint8_t num, uint8_t data[] )
{
	uint8_t i;

	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, addr | ADS1247_CMD_WREG );

	SpiReadWriteData( spi, num );

	for(i=0; i<num+1; i++)
	{
		 SpiReadWriteData( spi, data[i]);
	}

	SpiSetCs( spi, 0);
}

/******************************************************************************
  Function:Ads1247ReadRegister
  Description:
  Input:
		addr : Start register adddress (0 to 15)
		data : data
  Output:
  Return:
  Others:None
******************************************************************************/
void Ads1247WriteRegister( spiBASE_t *spi, uint8_t addr, int8_t data )
{
	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, addr | ADS1247_CMD_WREG );

	SpiReadWriteData( spi, 0x00 );

	SpiReadWriteData( spi, data );

	SpiSetCs( spi, 0);
}
/******************************************************************************
  Function:SpiReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Ads1247Init( spiBASE_t *spi )
{
	// Reset ADC
	SpiClearCs( spi, 0);
	SpiReadWriteData( spi, ADS1247_CMD_RESET );
	SpiSetCs( spi, 0);
	Ads1247Delay(60);

	// Positive = IN1 Negitive = IN2
	Ads1247WriteRegister( spi, ADS1247_REG_MUX0, 0x0A );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);

	// not Enable Bias voltage
	Ads1247WriteRegister( spi, ADS1247_REG_VBIAS, 0x00 );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);

	// Set Internal OSC, (IDAC)Internal reference, (ADC)OnBoard reference
	Ads1247WriteRegister( spi, ADS1247_REG_MUX1, 0x30 );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);

	// Set PGA = 1   DOR = 320SPS
	Ads1247WriteRegister( spi, ADS1247_REG_SYS0, 0x06 );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);

	// Set DOUT/(/D/R/D/Y) pin functons only as Data Out
    // I = 200uA
	Ads1247WriteRegister( spi, ADS1247_REG_IDAC0, 0x02 );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);

	// Connected the AIN0 and AIN3 pin for the  source DAC
	Ads1247WriteRegister( spi, ADS1247_REG_IDAC1, 0x30 );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
}

/******************************************************************************
  Function:SpiReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint32_t Ads1247ReadData( spiBASE_t *spi )
{
	uint8_t i,Data[3];

	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, ADS1247_CMD_RDATA );

	for(i=0; i<3; i++)
	{
		Data[i] = SpiReadWriteData( spi, ADS1247_CMD_NOP);
	}

	SpiSetCs( spi, 0);

	return (Data[0]<<16 | Data[1]<<8 | Data[2]);
}

/******************************************************************************
  Function:
  	  AdcInit
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void AdcInit( void )
{
	Ads1247Init( spiREG1 );
	Ads1247Init( spiREG3 );
}

/******************************************************************************
  Function:
  	  AdcInit
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
spiBASE_t *AdcChannelSet( uint8 Ch )
{
	spiBASE_t *spi;

	if ( Ch < 8 )
	{
		spi = spiREG1;
		LV138CH1_EN_0;
		LV138CH2_EN_1;
		switch ( Ch )
		{
			case 0: LV138CH1_A0_0; LV138CH1_A1_0; LV138CH1_A2_0; break;
			case 1: LV138CH1_A0_1; LV138CH1_A1_0; LV138CH1_A2_0; break;
			case 2: LV138CH1_A0_0; LV138CH1_A1_1; LV138CH1_A2_0; break;
			case 3: LV138CH1_A0_1; LV138CH1_A1_1; LV138CH1_A2_0; break;
			case 4: LV138CH1_A0_0; LV138CH1_A1_0; LV138CH1_A2_1; break;
			case 5: LV138CH1_A0_1; LV138CH1_A1_0; LV138CH1_A2_1; break;
			case 6: LV138CH1_A0_0; LV138CH1_A1_1; LV138CH1_A2_1; break;
			case 7: LV138CH1_A0_1; LV138CH1_A1_1; LV138CH1_A2_1; break;
			default : break;
		}
	}
	else
	{
		spi = spiREG3;
		LV138CH1_EN_1;
		LV138CH2_EN_0;
		switch ( Ch - 8 )
		{
			case 0: LV138CH2_A0_0; LV138CH2_A1_0; LV138CH2_A2_0; break;
			case 1: LV138CH2_A0_1; LV138CH2_A1_0; LV138CH2_A2_0; break;
			case 2: LV138CH2_A0_0; LV138CH2_A1_1; LV138CH2_A2_0; break;
			case 3: LV138CH2_A0_1; LV138CH2_A1_1; LV138CH2_A2_0; break;
			case 4: LV138CH2_A0_0; LV138CH2_A1_0; LV138CH2_A2_1; break;
			case 5: LV138CH2_A0_1; LV138CH2_A1_0; LV138CH2_A2_1; break;
			case 6: LV138CH2_A0_0; LV138CH2_A1_1; LV138CH2_A2_1; break;
			case 7: LV138CH2_A0_1; LV138CH2_A1_1; LV138CH2_A2_1; break;
			default : break;
		}
	}

	return spi;
}

/******************************************************************************
  Function:SpiReadWriteData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint32_t AdcReadData( uint8 Ch )
{
	uint8_t i,Data[3];
	spiBASE_t *spi;

	spi = AdcChannelSet( Ch );

	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, ADS1247_CMD_RDATA );

	for(i=0; i<3; i++)
		Data[i] = SpiReadWriteData( spi, ADS1247_CMD_NOP);

	SpiSetCs( spi, 0);

	return (Data[0]<<16 | Data[1]<<8 | Data[2]);
}

