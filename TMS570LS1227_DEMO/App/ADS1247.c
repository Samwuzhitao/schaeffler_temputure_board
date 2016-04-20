/*
 * ADS1247.c
 *
 *  Created on: 2016Äê4ÔÂ13ÈÕ
 *      Author: sam.wu
 */
#include "ADS1247.h"
#include "spi.h"
#include "stdio.h"

#define ADS1247_WRITE_REG_DELAYMS                   10
#define ADS1247_ALLOW_FAILURE_NUMBER                10
#define ADS1247_PT1000_RRESISTANCE_VALUE            840

const AdcCalibrationTypedef Ads1247PositiveCalibration[16] =
{
	{1663 , 3477},   // Ads1247 channel 0 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 1 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 2 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 3 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 4 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 5 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 6 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 7 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 8 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 9 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 10 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 11 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 12 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 13 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 14 Calibration parameter
	{1663 , 3477},   // Ads1247 channel 15 Calibration parameter
};

const AdcCalibrationTypedef Ads1247NegativeCalibration[16] =
{
	{1 , 0},   // Ads1247 channel 0 Calibration parameter
	{1 , 0},   // Ads1247 channel 1 Calibration parameter
	{1 , 0},   // Ads1247 channel 2 Calibration parameter
	{1 , 0},   // Ads1247 channel 3 Calibration parameter
	{1 , 0},   // Ads1247 channel 4 Calibration parameter
	{1 , 0},   // Ads1247 channel 5 Calibration parameter
	{1 , 0},   // Ads1247 channel 6 Calibration parameter
	{1 , 0},   // Ads1247 channel 7 Calibration parameter
	{1 , 0},   // Ads1247 channel 8 Calibration parameter
	{1 , 0},   // Ads1247 channel 9 Calibration parameter
	{1 , 0},   // Ads1247 channel 10 Calibration parameter
	{1 , 0},   // Ads1247 channel 11 Calibration parameter
	{1 , 0},   // Ads1247 channel 12 Calibration parameter
	{1 , 0},   // Ads1247 channel 13 Calibration parameter
	{1 , 0},   // Ads1247 channel 14 Calibration parameter
	{1 , 0},   // Ads1247 channel 15 Calibration parameter
};

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
uint16 Ads1247Init( spiBASE_t *spi )
{
	uint16 Err = 0;
	uint8 WriteRegister,ReadRegister;

	// Reset ADC
	SpiClearCs( spi, 0);
	SpiReadWriteData( spi, ADS1247_CMD_RESET );
	SpiSetCs( spi, 0);
	Ads1247Delay(60);

	// Positive = IN1 Negitive = IN2
	WriteRegister =  MUX0_BCS_OFF | MUX0_SP_AIN1 | MUX0_SN_AIN2;
	Ads1247WriteRegister( spi, ADS1247_REG_MUX0, WriteRegister );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_MUX0 );
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_MUX0;

	// not Enable Bias voltage
	WriteRegister = 0x00;
	Ads1247WriteRegister( spi, ADS1247_REG_VBIAS, WriteRegister );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_VBIAS );

	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_VBIAS;

	// Set Internal OSC, (IDAC)Internal reference, (ADC)OnBoard reference
	WriteRegister = 0x30;
	Ads1247WriteRegister( spi, ADS1247_REG_MUX1, WriteRegister );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_MUX1 );
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_MUX1;

	// Set PGA = 1   DOR = 320SPS
	WriteRegister = 0x06;
	Ads1247WriteRegister( spi, ADS1247_REG_SYS0, WriteRegister );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_SYS0 );
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_SYS0;

	// Set DOUT/(/D/R/D/Y) pin functons only as Data Out
    // I = 1mA
	WriteRegister = 0x06;
	Ads1247WriteRegister( spi, ADS1247_REG_IDAC0, WriteRegister );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	// ADS1247_REG_IDAC0:bit4-bit7 read only
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_IDAC0 ) & 0x0F;
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_IDAC0;


	// Connected the AIN0 and AIN3 pin for the  source DAC
	WriteRegister = 0x30;
	Ads1247WriteRegister( spi, ADS1247_REG_IDAC1, WriteRegister );
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_IDAC1 );
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_IDAC1;

	return Err;
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
uint16 AdcInit( void )
{
	uint16 Err = 0;
	uint8 times = 0;
	//Err = Ads1247Init( spiREG1 );

	while( Err )
	{
		times++;

		Err = Ads1247Init( spiREG3 );

		if( times > ADS1247_ALLOW_FAILURE_NUMBER)
		{
			printf("ADS1247 initialize fail, please check you board! and reset your board. \r\n");
			while(1);
		}
	}

	return Err;
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
		spi = spiREG3;
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
		spi = spiREG1;
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
  Function:
  	  AdcReadData
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
	uint32 AdcValue;

	spi = AdcChannelSet( Ch );

	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, ADS1247_CMD_RDATA );

	for(i=0; i<3; i++)
		Data[i] = SpiReadWriteData( spi, ADS1247_CMD_NOP);

	SpiSetCs( spi, 0);

	AdcValue = (Data[0]<<16 | Data[1]<<8 | Data[2]);

	return AdcValue;
}

/******************************************************************************
  Function:
  	  AdcFilterReadData
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint32_t AdcFilterReadData( uint8 Ch )
{
	uint32_t AdcValue[10];
	uint32_t AdcMaxValue = 0,AdcMinValue = 0,
			 SumAdcValue = 0,CurrentAdcValue = 0,
			 FilterAdcValue = 0;
	uint32_t AdcMinTimes = 0,AdcMaxTimes = 0;
	uint8 i;

	for(i=0; i<10; i++)
	{
		AdcValue[i] = AdcReadData( Ch );
		Ads1247Delay(4);
	}

	AdcMaxValue = AdcValue[0];
	for(i=1; i<10; i++)
	{
		CurrentAdcValue = AdcValue[i];
		if( CurrentAdcValue > AdcMaxValue)
		{
			AdcMaxValue = CurrentAdcValue;
			AdcMaxTimes = i;
		}
	}

	AdcMinValue = AdcValue[0];
	for(i=1; i<10; i++)
	{
		CurrentAdcValue = AdcValue[i];
		if( CurrentAdcValue < AdcMinValue)
		{
			AdcMinValue = CurrentAdcValue;
			AdcMinTimes = i;
		}
	}

	for(i=0; i<10; i++)
	{
		if((i!=AdcMaxTimes) && (i!=AdcMinTimes))
			SumAdcValue += AdcValue[i];
	}

	FilterAdcValue = SumAdcValue/8;
	return FilterAdcValue;
}

/******************************************************************************
  Function:
  	  ResistanceToTemperature
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint32_t ResistanceToTemperature( float Resistance , uint8 Ch)
{
	uint32 Temperature;
	float PT1000Resistance = 0;

	PT1000Resistance = ( float )(Resistance)*Ads1247PositiveCalibration[Ch].k/1000 - Ads1247PositiveCalibration[Ch].b/10;

	Temperature = (uint32)(PT1000Resistance - 1000)*1000/3.91;

	return Temperature;
}
