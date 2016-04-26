/**
  ******************************************************************************
  * @file    APP/CanProtocol.c
  * @author  Samwu
  * @version V1.0.0
  * @date    24-2-2016
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#include "platform_config.h"
#include "stm32f10x.h"                         // STM32F10x Library Definitions  
#include "CanProtocol.h"
#include "SerialProtocol.h"
#include "stdio.h"



#define CANRINGBUFFERLEN                   10

/* Private variables ---------------------------------------------------------*/
/* can peripheral variables */
__IO uint8_t CanSetSrcAddr = 0;

uint8_t CanRxCmdTopCounter = 0;
uint8_t CanRxCmdButtomCounter = 0;
uint8_t CanReceivecompleteFlg = 0;	

uint8_t CanEepromTestReturnData[8];

uint8_t  CanAutoTestFlg = 0;		
uint16_t UsartCanAutoTestCnt = 0;
uint16_t CanAutoTestIndex = 0;
uint16_t CanAutoTestOkCnt = 0;
uint16_t CanAutoTestErrCnt = 0;
uint8_t  UsartCanAutoTestTimeoutFlg = 0;

CAN_MessageTypedef CanRxCmdRingBuffer[CANRINGBUFFERLEN];

/******************************************************************************
  Function:CAN_MessageGet
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void CAN_MessageGet( CanRxMsg *pMessage )
{	
	int i;
	uint8_t CanAutoFlg = 0;
	CAN_STDEXTIDAddrTypedef CanSrcAddr,CanDstAddr;
	CAN_EXTSTDIDTypedef CAN_ID;
	
	if( pMessage->IDE  == CAN_ID_STD )
	{
		CAN_ID.Id = pMessage->StdId;
		CanDstAddr.CanIDAddr.ExtIDAddr = 0x00;
		CanSrcAddr.CanIDAddr.ExtIDAddr = 0x00;
	}
	else
	{
		CAN_ID.Id = pMessage->ExtId;
		CanDstAddr.CanIDAddr.ExtIDAddr = CAN_ID.CanProtocolId.ExtDstAddr;
		CanSrcAddr.CanIDAddr.ExtIDAddr = CAN_ID.CanProtocolId.ExtSrcAddr;
	}
	
	CanDstAddr.CanIDAddr.StdIDAddr = CAN_ID.CanProtocolId.DstAddr;
	CanSrcAddr.CanIDAddr.StdIDAddr = CAN_ID.CanProtocolId.SrcAddr;
	
	//printf("CAN_ID.id.FunCode: %x\r\n",(CAN_ID.CanProtocolId.FunCode) & 0x1f);
	//printf("CAN_ID.id.SrcAddr: %x\r\n",CanSrcAddr.IDAddr );
	//printf("CAN_ID.id.DstAddr: %x\r\n",CanDstAddr.IDAddr);
	
	if( CanDstAddr.IDAddr == CanSetSrcAddr  )
	{
		CanAutoFlg = IsCanAutoTest();
		
		if( CanAutoFlg == 0 )
		{
			CanRxCmdRingBuffer[CanRxCmdTopCounter].SrcAddr = CanSrcAddr.IDAddr;
			CanRxCmdRingBuffer[CanRxCmdTopCounter].DstAddr = CanDstAddr.IDAddr;
			CanRxCmdRingBuffer[CanRxCmdTopCounter].CanCmd = (CAN_ID.CanProtocolId.FunCode) & 0x1f;
			CanRxCmdRingBuffer[CanRxCmdTopCounter].IdType = pMessage->IDE;
		}

		for( i=0; i<8; i++)
		{
			if( CanAutoFlg == 0 )
			{
				CanRxCmdRingBuffer[CanRxCmdTopCounter].data[i] = pMessage->Data[i];
			}
			else
			{
				/* Auto Test*/ 
				CanEepromTestReturnData[i] = pMessage->Data[i];
			}
		}

		CanReceivecompleteFlg = 1;
		
		if( CanAutoFlg == 0 )
		{
			CanRxCmdTopCounter++;
			
			if(CanRxCmdTopCounter == CANRINGBUFFERLEN)
			{
				CanRxCmdTopCounter = 0;
			}
		}
	}
}

/******************************************************************************
  Function:Can_Process
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_Process( void )
{
	if(CanReceivecompleteFlg == 1)
	{
		if(CanRxCmdTopCounter != CanRxCmdButtomCounter)
		{
			
			Can_cmd_parse();
			
			CanRxCmdButtomCounter++;
			
			if(CanRxCmdButtomCounter == CANRINGBUFFERLEN)
			{
				CanRxCmdButtomCounter = 0;
			}
			
			if(CanRxCmdTopCounter == CanRxCmdButtomCounter)
			{
				 CanReceivecompleteFlg = 0;
			}
		}
	}
}

/******************************************************************************
  Function:Can_cmd_parse
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_cmd_parse( void )
{
	if((CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd & 0x10) == 0x10)
	{
		switch(CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd & 0x0f)	
		{
			case CAN_CMD_READ_AD:          Can_show_ad_msg();               break;
			case CAN_CMD_CHECK_BOARD:      Can_show_board_msg();            break;
			case CAN_CMD_RW_EEPROM:        Can_show_eeprom_msg();  					break;
			case CAN_CMD_RW_SN_ID:         Can_show_sn_msg();               break;
			case CAN_CMD_CALIBRATION_TEMP: Can_show_Calibration_temp_msg(); break;
			default:                                                        break;
		}
	}
	else
	{
		printf(" Command execution error, Please check the parameters or hardware! ");
	}
}

/******************************************************************************
  Function:Can_show_sn_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_show_sn_msg( void )
{
	uint8_t *Str;
	
	switch(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0])
	{
		case 0x00: Str = "Write";  break;
		case 0x01: Str = "Read";break;
		default: break;
	}
	
	printf("SRC:%2x DST:%2x CMD:%2x %s SN is: %2x %2x %2x %2x %2x %2x %2x %2x \r\n",
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
				Str,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[2],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[3],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[4],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[5],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[6],
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[7]);


}

/******************************************************************************
  Function:Can_read_ad_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_show_ad_msg( void )
{
	float x,y;
	uint32_t z;
	
	x = ( float )(*((uint32_t *)(&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1]))) & 0xFFFFFF )*1640/0x7fffff;
	y = ( float )(*((uint32_t *)(&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[4]))) & 0xFFFFFF )/1000;
	z = (*((uint32_t *)(&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1]))) & 0xFFFFFF);
	
	printf("SRC:%2x DST:%2x CMD:%2x ADC Channel %2d Value is: %f \r\n",
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0],
					x);
	
	printf("SRC:%2x DST:%2x CMD:%2x ADC Channel %2d Temperature is: %f \r\n",
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0],
					y);
	
		printf("SRC:%2x DST:%2x CMD:%2x ADC Channel %2d ADValue is: %x \r\n",
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0],
					z);
					
}

/******************************************************************************
  Function:Can_show_eeprom_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_show_eeprom_msg( void )
{
	uint8_t *Str;
	
	if(CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd & 0x10)
	{
				switch(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0])
		{
			case 0x00: Str = "Write";  break;
			case 0x01: Str = "Read";   break;
			default: break;
		}
		
		printf("SRC:%2x DST:%2x CMD:%2x %s %2xch EEPROM %2x byte %2x addr data is:  %2x %2x %2x %2x \r\n",
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
					Str,
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1],
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[3],
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[2],
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[4],
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[5],
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[6],
					CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[7]);

	}
	else
	{
		if((CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0] & 0x01))
		{
			printf(" Write fail \r\n");
		}
		else
		{
			printf(" Read fail \r\n");
		}
	}
}

/******************************************************************************
  Function:Can_show_board_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_show_board_msg( void )
{
	uint8_t * Str;

	switch(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0])
	{
		case 0x01: Str = "TEDS State is: ";  break;
		case 0x02: Str = "PT1000 State is: ";break;
		default: break;
	}
	
	printf("SRC:%2x DST:%2x CMD:%2x Check Type %2x %s %x \r\n",
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0],
				Str,
				*(uint16_t *)(&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1])));
	
}

/******************************************************************************
  Function:Can_show_Calibration_temp_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_show_Calibration_temp_msg( void )
{
	uint8_t * Str;
	
	switch(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0])
	{
		case 0x00: Str = "Write";  break;
		case 0x01: Str = "Read";break;
		default: break;
	}
	
	printf("SRC:%2x DST:%2x CMD:%2x Ch %2x %s Calibration parameter k = %x b = %x \r\n",
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd,
				CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1],
				Str,
				*(uint16_t *)(&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[2])),
				*(uint16_t *)(&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[4])));
	
}



/******************************************************************************
  Function:Serial_send_cmd_to_can
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_send_cmd_to_can( uint8_t CmdDta[] )
{
	uint8_t i;
	CanTxMsg CanToCanTxMessage;
	USART_FrameTypedef CanCmdMessage;
	
	CanCmdMessage.Funcode = CmdDta[0];
	CanCmdMessage.CanIDType = 0x00;
	CanCmdMessage.CanDstID = 0x01;
	
	Serial_SetCanID(&CanToCanTxMessage,CanCmdMessage.Funcode,
									 CanCmdMessage.CanIDType,CanCmdMessage.CanDstID);
	
	for( i=0; i<8; i++ )
	{
		CanToCanTxMessage.Data[i] = CmdDta[i+1];
	}
	
	CAN_Transmit(CAN1, &CanToCanTxMessage);
}

/******************************************************************************
  Function:Serial_send_cmd_to_can
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Testdelay(unsigned int nCount)	
{
  for(; nCount != 0; nCount--);
}



/******************************************************************************
  Function:CanAutoTestErrAdd
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void CanAutoTestErrAdd( void )
{
	CanAutoTestIndex++;
	CanAutoTestErrCnt++;
}

/******************************************************************************
  Function:CanAutoTestErrAdd
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void CanAutoTestOkAdd( void )
{
	CanAutoTestIndex++;
	CanAutoTestOkCnt++;
}

/******************************************************************************
  Function:Can_test_eeprom_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_test_eeprom_msg( uint8_t CanEepromTestCmd[] )
{
	uint8_t i,Status = 0 ;
	uint8_t *TestCmd ;
	uint16_t TimeoutCnt = 0;
	
	TestCmd = CanEepromTestCmd;
	
	while( !CanReceivecompleteFlg )
	{
		Testdelay(45000);
		TimeoutCnt++;

		
		if(TimeoutCnt > 100)
		{
			UsartCanAutoTestTimeoutFlg = 1;
			CanReceivecompleteFlg = 1;
		}
	}
	
	if( UsartCanAutoTestTimeoutFlg )
	{
		/* Time out Prpcess */
		CanAutoTestErrAdd();
	}
	else
	{
		for(i=0; i<4;i++)
		{
			printf("Write data[%x] = %x ", i, CanEepromTestReturnData[4+i]);
			printf("Read  data[%x] = %x \r\n", i, TestCmd[5+i]);
			if(CanEepromTestReturnData[4+i] == TestCmd[5+i])
			{
				Status |= 1 << i;		
			}
		}
		
		if(Status == 0x0f)
		{
			CanAutoTestOkAdd();
		}
		else
		{
			CanAutoTestErrAdd();
		}	
	}
		
	if(UsartCanAutoTestCnt == CanAutoTestIndex)
	{
		printf(" Auto Test %d  OK! , Ok = %d Err = %d        \r\n", 
						CanAutoTestIndex, CanAutoTestOkCnt, CanAutoTestErrCnt);
		
		CanAutoTestIndex = 0;
		CanAutoTestOkCnt = 0;
		CanAutoTestErrCnt = 0;
	}
	
	CanReceivecompleteFlg = 0;
	
}

/******************************************************************************
  Function:Can_test_eeprom_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_test_adc_msg( uint8_t CanAdcTestCmd[] )
{
	
	uint8_t Status = 0 ;
	uint8_t *TestCmd ;
	uint16_t TimeoutCnt = 0;
	float x;
	uint32_t z;
	
	TestCmd = CanAdcTestCmd;
	
	while( !CanReceivecompleteFlg )
	{
		Testdelay(45000);
		TimeoutCnt++;
		
		if(TimeoutCnt > 30)
		{
			UsartCanAutoTestTimeoutFlg = 1;
			CanReceivecompleteFlg = 1;
		}
	}
	
	if( UsartCanAutoTestTimeoutFlg )
	{
		/* Time out Prpcess */
		CanAutoTestErrAdd();
	}
	else
	{
		x = ( float )(*((uint32_t *)(&(CanEepromTestReturnData[1]))) & 0xFFFFFF )*1640/0x7fffff;
		z = (*((uint32_t *)(&(CanEepromTestReturnData[1]))) & 0x7FFFFF);
		printf("R = %f AdcValue = %x \r\n",x,z);
		
		if(z == 0x7FFFFF || z < 3000)
			Status = 1;
		else
			Status = 0;
		
		if(Status == 0)
		{
			CanAutoTestOkAdd();
		}
		else
		{
			CanAutoTestErrAdd();
		}	
	}
		
	if(UsartCanAutoTestCnt == CanAutoTestIndex)
	{
		printf(" Auto Ch = %d Test %d  OK! , Ok = %d Err = %d \r\n", TestCmd[1],
						CanAutoTestIndex, CanAutoTestOkCnt, CanAutoTestErrCnt);
		
		CanAutoTestIndex = 0;
		CanAutoTestOkCnt = 0;
		CanAutoTestErrCnt = 0;
	}
	
	CanReceivecompleteFlg = 0;
	
}

/******************************************************************************
  Function:Serial_send_cmd_to_can
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_eeprom_auto_test( uint16_t Cnt )
{
	uint16_t i;
	uint8_t CanEepromTestWriteCmd[9] = { CAN_CMD_RW_EEPROM, 0x00, 0x00, 0x00, 0x04, 0x11, 0x22, 0x33, 0x44 };
	
	UsartCanAutoTestCnt = Cnt;
	
	for(i=0; i<Cnt; i++)
	{
		Can_send_cmd_to_can( CanEepromTestWriteCmd );
		
		Can_test_eeprom_msg( CanEepromTestWriteCmd );
	}
	
}


/******************************************************************************
  Function:Can_Adc_auto_test
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_Adc_auto_test( uint16_t Cnt )
{
	uint16_t i;
	uint8_t CanAdcTestCmd[9] = { CAN_CMD_READ_AD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	UsartCanAutoTestCnt = Cnt;
	
	for(i=0; i<Cnt; i++)
	{
			
			Can_send_cmd_to_can( CanAdcTestCmd );
			
			Can_test_adc_msg( CanAdcTestCmd );
	}
	
}
/******************************************************************************
  Function:IsCanAutoTest
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
uint8_t IsCanAutoTest( void )
{
	return CanAutoTestFlg;
}
/******************************************************************************
  Function:SetCanAutoTest
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void SetCanAutoTest( uint8_t Flg )
{
	CanAutoTestFlg = Flg;
}
