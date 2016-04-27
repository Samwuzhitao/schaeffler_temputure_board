
#include "stdio.h"
#include "DS2431.h"
#include "OneWire.h"


static unsigned char OneWire_8BIT_CRC( unsigned char Byte_a[], unsigned char NumofByte  )
{
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char ShiftReg = 0;

	for( j = 0; j < NumofByte; j++ ){
		unsigned char tmp = Byte_a[j];
		for( i = 0; i < 8; i++ ){
			if ( ( ( tmp & 0x01 ) && ( ShiftReg & 0x01 ) )			\
			||   ( !( tmp & 0x01 ) && !( ShiftReg & 0x01) ) ) {
				ShiftReg >>=1;
			}else{
				ShiftReg ^= 0x18;
				ShiftReg >>= 1;
				ShiftReg |= 0x80;
			}
			tmp >>= 1;
		}
	}

	return ShiftReg;
}


static unsigned short  OneWire_16BIT_CRC( unsigned short crc, unsigned char Byte_a[], unsigned char NumofByte  )
{
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned short ShiftReg = crc;

	for( j = 0; j < NumofByte; j++ ){
		unsigned char tmp = Byte_a[j];
		for( i = 0; i < 8; i++ ){
			if ( ( ( tmp & 0x01 ) && ( ShiftReg & 0x01 ) )			\
			||   ( !( tmp & 0x01 ) && !( ShiftReg & 0x01) ) ) {
				ShiftReg >>=1;
			}else{
				ShiftReg  ^= 0x4002;
				ShiftReg  >>= 1;
				ShiftReg |= 0x8000;
			}
			tmp >>= 1;
		}
	}

	return ShiftReg;
}


static unsigned char Read_ROM( unsigned char Data[], unsigned char NumofByte )
{	
	unsigned char i = 0;

	if( ERROR_NO_DEVICE == OneWire_Init() ){
		return ERROR_NO_DEVICE;
	}
	OneWire_TxByte( READ_ROM );
	for( i = 0; i < 8; i++ ){
		Data[i] = OneWire_RxByte();
	}
	if( !OneWire_8BIT_CRC( Data, NumofByte ) ){
		//printf("ROM code CRC-8 Correct\n\r");
		return OK;
	}else{
		return ERROR_CRC8;
	}
}


static unsigned char DS2431_WriteScratchpad( unsigned short Address, unsigned char Data[], unsigned char NumofByte  )
{
	unsigned short crc_read = 0;

	if( ERROR_NO_DEVICE == OneWire_Init() ){
		return ERROR_NO_DEVICE;
	}
	{//Write data to scratchpad, LSB first.
		unsigned char i = 0;
		OneWire_TxByte( SKIP_ROM );		//ROM function command
		OneWire_TxByte( WRITE_SCRATCHPAD );	//Memory function command
		OneWire_TxByte( Address );			
		OneWire_TxByte( Address >> 8 );		
		for( i = 0; i < NumofByte; i++ ){	//Write data
			OneWire_TxByte( Data[i] );
		}
	}
	{//Read CRC generated by the DS24321, LSB first.
		unsigned char tmp = 0;

		tmp = OneWire_RxByte();
		crc_read = OneWire_RxByte();
		crc_read <<= 8;
		crc_read |= tmp;
		crc_read = ~crc_read;		//The 16-bit CRC generated by DS2431 comes in the inverted form.
	}
	{//calculates the CRC-16 value of the entire data stream,
		//starting at the command code and ending at the last data byte.
		unsigned char DataTmp_a[3] = { WRITE_SCRATCHPAD, (unsigned char)(Address), (unsigned char)(Address>>8) };
		unsigned short crc = 0;

		crc = OneWire_16BIT_CRC( 0, DataTmp_a, sizeof( DataTmp_a ) );
		crc = OneWire_16BIT_CRC( crc, Data, NumofByte);
		if( crc == crc_read ){		//compares the CRC value read from device to the one it calculates.
			//printf("Data CRC-16 Correct, writing sucessful\n\r");
			return OK; 
		}else{
			//printf("Data CRC-16 Error, writing failed\n\r");
			return ERROR_CRC16;
		}
	}
}


static unsigned char DS2431_ReadScratchpad( unsigned char AuthorizationData[], unsigned char Data[], unsigned char NumofByte  )
{
	unsigned short crc_read = 0;
	unsigned short Address;
	unsigned char Reg_ES;

	if( ERROR_NO_DEVICE == OneWire_Init() ){
		return ERROR_NO_DEVICE;
	}
	{//Read data from scratchpad, LSB first.
		unsigned char i = 0;
		unsigned char tmp = 0;

		OneWire_TxByte( SKIP_ROM );		//ROM function command
		OneWire_TxByte( READ_SCRATCHPAD );	//Memory function command
		//read Address	
		tmp = OneWire_RxByte();
		Address = OneWire_RxByte();
		Address <<= 8;
		Address |= tmp;
		//read register ES	
		Reg_ES= OneWire_RxByte();
		//read data
		for( i = 0; i < 8; i++ ){		
			Data[i] = OneWire_RxByte();
		}
	}
	{//Read CRC generated by the DS24321, LSB first.
		unsigned char tmp = 0;

		tmp = OneWire_RxByte();
		crc_read = OneWire_RxByte();
		crc_read <<= 8;
		crc_read |= tmp;
		crc_read = ~crc_read;		//The 16-bit CRC generated by DS2431 comes in the inverted form.
	}
	{//calculates the CRC-16 value of the entire data stream,
		//starting at the command code and ending at the last data byte.
		unsigned char DataTmp_a[4] = { READ_SCRATCHPAD, (unsigned char)(Address), (unsigned char)(Address>>8), Reg_ES };
		unsigned short crc = 0;

		crc = OneWire_16BIT_CRC( 0, DataTmp_a, sizeof( DataTmp_a ) );
		crc = OneWire_16BIT_CRC( crc, Data, NumofByte);
		if( crc == crc_read ){		//compares the CRC value read from device to the one it calculates.
			//printf("Data CRC-16 Correct, reading sucessful\n\r");

		}else{
			//printf("Data CRC-16 Error, reading failed\n\r");
			return ERROR_CRC16;
		}
	}
	{//obtain 3-byte authorzation pattern for subsequent CopyScrathpad command
		AuthorizationData[0] = (unsigned char)(Address);
		AuthorizationData[1] = (unsigned char)(Address>>8);
		AuthorizationData[2] = Reg_ES;
	}
	return OK;
}


static unsigned char DS2431_CopyScratchpad( unsigned char AuthorizationData[] )
{
	unsigned char i = 0;

	if( ERROR_NO_DEVICE == OneWire_Init() ){
		return ERROR_NO_DEVICE;
	}
	OneWire_TxByte( SKIP_ROM );		//ROM function command
	OneWire_TxByte( COPY_SCRATCHPAD );	//Memory function command
	//Write Authorization data
	OneWire_TxByte( AuthorizationData[0] );
	OneWire_TxByte( AuthorizationData[1] );
	OneWire_TxByte( AuthorizationData[2] );
	//wait for 10ms the copy function to complete
	for( i = 0; i < 10; i++ ){
		delay_us( 1000 );
	}
	{//
		unsigned char tmp = 0;
		tmp = OneWire_RxByte();
		if( (tmp==0xAA) || (tmp==0x55) ){
			//printf("copying sucessful\n\r");
			return OK;
		}else{
			//printf("copying failed\n\r");
			return ERROR;
		}
	}
}


unsigned char DS2341_Write_8Byte( unsigned char DeviceNo, unsigned char Address, unsigned char Data[] )
{
	unsigned char DataTmp[8] = { 0 };
	unsigned char AuthorizationData[3] = { 0 };
	unsigned char tmp;

	if( ERROR == OneWireDevice_Selection( DeviceNo ) ){
		return ERROR_DEVICE_NUM;
	}
	if( Address%8 != 0  ){
		return ERROR_ADDRESS_BOUNDARY;
	}
	if( (tmp = DS2431_WriteScratchpad( Address, Data, 8 )) != OK ){
		return tmp; 
	}
	if( (tmp = DS2431_ReadScratchpad( AuthorizationData, DataTmp, 8 )) != OK ){
		return tmp; 
	}
	if( (tmp = DS2431_CopyScratchpad( AuthorizationData )) != OK ){
		return tmp; 
	}
	return OK;
}

unsigned char DS2431_ReadData( unsigned char DeviceNo, unsigned short Address, unsigned char Data[], unsigned char NumofByte )
{

	if( ERROR == OneWireDevice_Selection( DeviceNo ) ){
		return ERROR_DEVICE_NUM;
	}

	if( ERROR_NO_DEVICE == OneWire_Init() ){
		return ERROR_NO_DEVICE;
	}
	{//Read data from memory, LSB first.
		unsigned char i = 0;
		OneWire_TxByte( SKIP_ROM );		//ROM function command
		OneWire_TxByte( READ_MEMORY );		//Memory function command
		OneWire_TxByte( Address );
		OneWire_TxByte( Address >> 8 );
		//read data
		for( i = 0; i < NumofByte; i++ ){
			Data[i] = OneWire_RxByte();
		}
	}
	return OK;
}