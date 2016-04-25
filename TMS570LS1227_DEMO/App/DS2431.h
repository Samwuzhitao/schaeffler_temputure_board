/*
 * DS2431.h
 *
 *  Created on: 2016Äê3ÔÂ5ÈÕ
 *      Author: Tim
 */

#ifndef DS2431_H_
#define DS2431_H_


#define READ_ROM		0x33
//#define MACTH_ROM		0x33
//#define SEARCH_ROM		0x33
#define SKIP_ROM		0xCC
#define RESUME_ROM		0xA5
//#define OVER_DRIVE_SKIP_ROM	0x33
//#define OVER_MATCH_ROM		0x33

#define WRITE_SCRATCHPAD	0x0F
#define READ_SCRATCHPAD		0xAA
#define COPY_SCRATCHPAD		0x55
#define READ_MEMORY			0xF0

#define OK							0
#define ERROR						0xFF
#define ERROR_NO_DEVICE				0xFE
#define ERROR_CRC8					0xFD
#define ERROR_CRC16					0xFC
#define ERROR_DEVICE_NUM			0xFB
#define ERROR_ADDRESS_BOUNDARY		0xFA

unsigned char DS2341_Write_8Byte( unsigned char DeviceNo, unsigned char Address, unsigned char Data[] );
unsigned char DS2431_ReadData( unsigned char DeviceNo, unsigned short Address, unsigned char Data[], unsigned char NumofByte );


#endif /* DS2431_H_ */
