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

AdcCalibrationTypedef Ads1247PositiveCalibration[16] =
{
	{1000 , 0},   // Ads1247 channel 0 Calibration parameter
	{1000 , 0},   // Ads1247 channel 1 Calibration parameter
	{1000 , 0},   // Ads1247 channel 2 Calibration parameter
	{1000 , 0},   // Ads1247 channel 3 Calibration parameter
	{1000 , 0},   // Ads1247 channel 4 Calibration parameter
	{1000 , 0},   // Ads1247 channel 5 Calibration parameter
	{1000 , 0},   // Ads1247 channel 6 Calibration parameter
	{1000 , 0},   // Ads1247 channel 7 Calibration parameter
	{1000 , 0},   // Ads1247 channel 8 Calibration parameter
	{1000 , 0},   // Ads1247 channel 9 Calibration parameter
	{1000 , 0},   // Ads1247 channel 10 Calibration parameter
	{1000 , 0},   // Ads1247 channel 11 Calibration parameter
	{1000 , 0},   // Ads1247 channel 12 Calibration parameter
	{1000 , 0},   // Ads1247 channel 13 Calibration parameter
	{1000 , 0},   // Ads1247 channel 14 Calibration parameter
	{1000 , 0},   // Ads1247 channel 15 Calibration parameter
};

AdcCalibrationTypedef Ads1247NegativeCalibration[16] =
{
	{1000, 0},   // Ads1247 channel 0 Calibration parameter
	{1000, 0},   // Ads1247 channel 1 Calibration parameter
	{1000, 0},   // Ads1247 channel 2 Calibration parameter
	{1000, 0},   // Ads1247 channel 3 Calibration parameter
	{1000, 0},   // Ads1247 channel 4 Calibration parameter
	{1000, 0},   // Ads1247 channel 5 Calibration parameter
	{1000, 0},   // Ads1247 channel 6 Calibration parameter
	{1000, 0},   // Ads1247 channel 7 Calibration parameter
	{1000, 0},   // Ads1247 channel 8 Calibration parameter
	{1000, 0},   // Ads1247 channel 9 Calibration parameter
	{1000, 0},   // Ads1247 channel 10 Calibration parameter
	{1000, 0},   // Ads1247 channel 11 Calibration parameter
	{1000, 0},   // Ads1247 channel 12 Calibration parameter
	{1000, 0},   // Ads1247 channel 13 Calibration parameter
	{1000, 0},   // Ads1247 channel 14 Calibration parameter
	{1000, 0},   // Ads1247 channel 15 Calibration parameter
};

spiDAT1_t dataconfig1_t;

/******************************************************************************
  Function:
  	    SpiSetCs
  Description:
    	Set spi cs high
  Input:
  	    spi  : Spi port
  	    nCs : Used nCs in spi
  Output:None
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
  	    Set spi cs low
  Input:
		spi  : Spi port
  	    nCs : Used nCs in spi
  Output:None
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
  	  	Spi read and write data function.
  Input:
		spi  : Spi port
		data : Data sent via spi
  Output:
  	  	ReadData : Data read via spi
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
		delay function
  Input:
		nCount: the number of milliseconds
  Output:None
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
  	  	spi  : Spi port
		addr : Start register adddress (0 to 15)
		num  : number of registers to be read (number of bytes - 1)
		data : Data sent via spi
  Output:None
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
  	  	spi  : Spi port
		addr : Start register adddress (0 to 15)
		data : Data read via spi
  Output:None
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
  	  	spi  : Spi port
		addr : Start register adddress (0 to 15)
		num  : number of registers to be write (number of bytes - 1)
		data : Data sent via spi
  Output:None
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
  	  	spi  : Spi port
		addr : Start register adddress (0 to 15)
		data : Data sent via spi
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
  Function:
		Ads1247Init
  Description:
  	    Ads1247 initialize function
  Input:
		spi : Spi port
  Output:
  	    Err : initialize register result of error
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
	//printf("ADS1247_REG_MUX0 write Register = %d", WriteRegister);
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_MUX0 );
	//printf("ADS1247_REG_MUX0 Read Register = %d", ReadRegister);
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_MUX0;

	// not Enable Bias voltage
	WriteRegister = 0x00;
	Ads1247WriteRegister( spi, ADS1247_REG_VBIAS, WriteRegister );
	//printf("ADS1247_REG_VBIAS write Register = %d", WriteRegister);
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_VBIAS );
	//printf("ADS1247_REG_VBIAS Read Register = %d", ReadRegister);
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_VBIAS;

	// Set Internal OSC, (IDAC)REF0 reference, (ADC)OnBoard reference
	WriteRegister = 0x20;
	Ads1247WriteRegister( spi, ADS1247_REG_MUX1, WriteRegister );
	//printf("ADS1247_REG_MUX1 write Register = %d", WriteRegister);
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_MUX1 );
	//printf("ADS1247_REG_MUX1 Read Register = %d", ReadRegister);
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_MUX1;

	// Set PGA = 1   DOR = 320SPS
	WriteRegister = 0x06;
	Ads1247WriteRegister( spi, ADS1247_REG_SYS0, WriteRegister );
	//printf("ADS1247_REG_SYS0 write Register = %d", WriteRegister);
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_SYS0 );
	//printf("ADS1247_REG_SYS0 Read Register = %d", ReadRegister);
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_SYS0;

	// Set DOUT/(/D/R/D/Y) pin functons only as Data Out
    // I = 1mA
	WriteRegister = 0x06;
	Ads1247WriteRegister( spi, ADS1247_REG_IDAC0, WriteRegister );
	//printf("ADS1247_REG_IDAC0 write Register = %d", WriteRegister);
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	// ADS1247_REG_IDAC0:bit4-bit7 read only
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_IDAC0 ) & 0x0F;
	//printf("ADS1247_REG_IDAC0 Read Register = %d", ReadRegister);
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_IDAC0;


	// Connected the AIN0 and AIN3 pin for the  source DAC
	WriteRegister = 0x30;
	Ads1247WriteRegister( spi, ADS1247_REG_IDAC1, WriteRegister );
	//printf("ADS1247_REG_IDAC1 write Register = %d", WriteRegister);
	Ads1247Delay(ADS1247_WRITE_REG_DELAYMS);
	ReadRegister = Ads1247ReadRegister( spi, ADS1247_REG_IDAC1 );
	//printf("ADS1247_REG_IDAC1 Read Register = %d", ReadRegister);
	if( ReadRegister != WriteRegister )
		Err = 1 << ADS1247_REG_IDAC1;

	return Err;
}

/******************************************************************************
  Function:
  	    Ads1247ReadData
  Description:
        Read the Ad value
  Input:
		spi : Spi port
  Output:
  	    AdcData : Current Ad value
  Others:None
******************************************************************************/
uint32_t Ads1247ReadData( spiBASE_t *spi )
{
	uint8_t i,Data[3];
	uint32 AdcData = 0;

	SpiClearCs( spi, 0);

	SpiReadWriteData( spi, ADS1247_CMD_RDATA );

	for(i=0; i<3; i++)
	{
		Data[i] = SpiReadWriteData( spi, ADS1247_CMD_NOP);
	}

	SpiSetCs( spi, 0);

	AdcData = Data[0]<<16 | Data[1]<<8 | Data[2];

	return AdcData;
}

/******************************************************************************
  Function:
  	    AdcSpiInit
  Description:
  	    Initialize Spi for ADS1247
  Input:
		spi : Spi port
  Output:
  	    Err: the error code for initialize.
  Others:None
******************************************************************************/
uint16  AdcSpiInit( spiBASE_t *spi )
{
	uint16 Err = 1;
	uint8 times = 0;

	while( Err )
	{
		times++;

		Err = Ads1247Init( spi );

		if( times > ADS1247_ALLOW_FAILURE_NUMBER)
		{
			printf("ADS1247 initialize fail, please check you board! and reset your board. \r\n");
			return Err;
		}
	}

	return Err;
}

/******************************************************************************
  Function:
  	    AdcInit
  Description:
  	    Initialize 2 ADS1247
  Input:None
  Output:
  	    Err: the error code for initialize.
  Others:None
******************************************************************************/
uint16 AdcInit( void )
{
	uint16 Err = 0;

	Err |= AdcSpiInit( spiREG1 );
	Err |= AdcSpiInit( spiREG3 );

	return Err;
}

/******************************************************************************
  Function:
		AdcChannelSet
  Description:
  	    Set sampling channel and get the operational spi port
  Input:
		Ch : Sampling channel
  Output:
		spi : This channel corresponding to spi port
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
  	    Read ad value of certain channel
  Input:
		Ch : Sampling channel
  Output:
  	    AdcValue : Current Ad value of certain channel
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
  	    Read a certain channel adc values and filtering
  Input:
		Ch : Sampling channel
  Output:
  	    AdcValue : Current Ad value of certain channel
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
	AdcMaxTimes = 0;
	for(i=1; i<10; i++)
	{
		CurrentAdcValue = AdcValue[i];
		if( CurrentAdcValue > AdcMaxValue)
		{
			AdcMaxValue = CurrentAdcValue;
			AdcMaxTimes = i;
		}
	}

	AdcMinValue = AdcValue[9];
	AdcMinTimes = 9;
	for(i=9; i>0; i--)
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
  	  The resistance value is converted into temperature
  Input:
	  Resistance:The resistance value
	  Ch : Sampling channel
  Output:
	Temperature:temperature * 1000
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

/******************************************************************************
  Function:
		SetTempCalibrationParameter
  Description:
  	    Set temperature calibration parameter
  Input:
	    Ch : Sampling channel
	    AdcCal :The temperature calibration parameter need to be set
  Output:None
  Others:None
******************************************************************************/
void SetTempCalibrationParameter(uint8 Ch , AdcCalibrationTypedef AdcCal)
{
	Ads1247PositiveCalibration[Ch] = AdcCal;
}

/******************************************************************************
  Function:
		GetTempCalibrationParameter
  Description:
  	    Get certain channel temperature calibration parameter
  Input:
	    Ch : Sampling channel
  Output:
  	    AdcCal :The temperature calibration parameter need to be set
  Others:None
******************************************************************************/
AdcCalibrationTypedef GetTempCalibrationParameter( uint8 Ch )
{
	AdcCalibrationTypedef AdcCal;

	AdcCal = Ads1247PositiveCalibration[Ch];

	return AdcCal;
}
