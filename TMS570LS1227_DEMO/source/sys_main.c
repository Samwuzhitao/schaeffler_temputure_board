/** @file sys_main.c 
*   @brief Application main file
*   @date 02-Mar-2016
*   @version 04.05.02
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
#include "stdio.h"
#include "sci.h"
#include "sys_core.h"
#include "can.h"
#include "sys_vim.h"
#include "CanProtocol.h"
#include "spi.h"
#include "ADS1247.h"
#include "gio.h"
#include "het.h"

/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#define DELAY_VALUE           10000000
#define CAN_DATA_BUFFER_LEN   64

extern spiDAT1_t dataconfig1_t;

uint8 can_sent_buffer_data[CAN_DATA_BUFFER_LEN];
uint8 can_receive_buffer_data[CAN_DATA_BUFFER_LEN*8];

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void main(void){
/* USER CODE BEGIN (3) */
    volatile unsigned long int i,j;

//  uint32 status,tmpid,tmpIdType;
//	CAN_EXTSTDIDTypedef CanId;

    gioInit();
    hetInit();
	sciInit();
	canInit();
	spiInit();
	canEnableErrorNotification(canREG3);
//	canEnableloopback(canREG3,Internal_Lbk);

	cansetIDType(canREG3, 2, CAN_ID_STD);
/*
	CanId.CanStdId.FunCode = 0x07;
	CanId.CanStdId.SrcAddr = 0x01;
	CanId.CanStdId.DstAddr = 0x02;
	canSetID(canREG3, 1, CanId.Id);
	cansetIDType(canREG3, 1, CAN_ID_STD);
*/
	vimInit();
	_enable_interrupt_();
/*
	for (j = 0; j<CAN_DATA_BUFFER_LEN; j++)
	{
		can_sent_buffer_data[j] =  j;
		can_receive_buffer_data[j] = 0;
	}
*/
	//spiEnableLoopback(spiREG1, Analog_Lbk);
	//spiEnableLoopback(spiREG3, Analog_Lbk);

	AdcInit();

	while(1)
	{
		Can_Process();

		/* Initiate SPI2 Transmit and Receive through Interrupt Mode */
		//spiTransmitData(spiREG1, &dataconfig1_t, 1, &TX_Data);

		/* Initiate SPI1 Transmit and Receive through Polling Mode*/
		//spiTransmitData(spiREG3, &dataconfig1_t, 1, &TX_Data);

		/*
		printf("Massage box%2d id = %8x idtype = %1d \r\n", 1, canGetID(canREG3, 1) , canGetIDType(canREG3, 1));
		printf("Massage box%2d id = %8x idtype = %1d \r\n", 2, canGetID(canREG3, 2) , canGetIDType(canREG3, 2));
		status = canTransmit(canREG3, 1, &(can_sent_buffer_data[0]));
		printf("Massage box%2d id = %8x idtype = %1d canTransmit return status = %4d \r\n", 1, tmpid>>18, tmpIdType, status);
		*/
		//spiTransmitAndReceiveData(spiREG1, &dataconfig1_t, 1, &data, &ReadData);
		//spiTransmitAndReceiveData(spiREG3, &dataconfig1_t, 1, &data, &ReadData);
		//printf("spiTransmitAndReceiveData Loopback Value is : %2x \r\n", ReadData);
		//ReadData = ADS1247_ReadWriteData(data);
		//ADS1247_WriteRegister(i,0x02);


		//ADS1247_Init();
		/*
		for(i=0;i<0x0F;i++)
		{
			spiClearCs(spiREG3, 0);
			ADS1247_ReadWriteData( ADS1247_CMD_RESET );
			spiSetCs(spiREG3, 0);
			ADS1247_Delay(60);

			ADS1247_WriteRegister( ADS1247_REG_MUX0, 0x0A );
			ADS1247_Delay(20);
			x = ADS1247_ReadRegister(ADS1247_REG_MUX0);
			printf("ADS1247_REG_MUX0 Register Value is : %2x \r\n", x);
			ADS1247_Delay(20);
		}
		*/
		/*
		while(1)
		{
			int i;
			for (i=0; i<16; i++)
			{
				AdcReadData(i);
				printf("The ADS1247 Read Value is : %x \r\n",x);
			}
		}
		*/

		//printf("The ADS1247 Read Value is : %f \r\n",( float )x*3300*5/0xffffff);

		//for (i = 0; i < DELAY_VALUE; i++);

	}
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
