/*
 * CanProtocol.c
 *
 *  Created on: 2016Äê3ÔÂ29ÈÕ
 *      Author: Sam
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#include "stdio.h"
#include "sys_common.h"
#include "can.h"
#include "CanProtocol.h"
#include "ADS1247.h"
#include "LED.h"
#include "DS2431.h"


uint8 SN_ID[7] = { 0x12,0x23,0x56,0x78,0xab,0xcd,0xef };

/* Private variables ---------------------------------------------------------*/
/* can peripheral variables */
uint8 CanSetSrcAddr = CAN_SRC_ADDR;
uint8 CanRxCmdTopCounter = 0;
uint8 CanRxCmdButtomCounter = 0;
uint8 CanReceivecompleteFlg = 0;

CAN_MessageTypedef CanRxCmdRingBuffer[CANRINGBUFFERLEN];

/******************************************************************************
  Function:
		cansetIDType
  Description:
  	    Set certain messagebox Id type
  Input:
	    node : Can port
	    messageBox : Used message box
	    IdType : The type of ID
  Output:None
  Others:None
******************************************************************************/
void cansetIDType(canBASE_t *node, uint32 messageBox, uint8 IdType)
{
    /** - Wait until IF2 is ready for use */
    while ((node->IF2STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

	/** - Configure IF2 for
	*     - Message direction - Read
	*     - Data Read
	*     - Clears NewDat bit in the message object.
	*/
	node->IF2CMD = 0xA0U;
	/* Copy passed value into the arbitration register. */
	node->IF2MSK |= 0x80000000;

	 if(IdType == CAN_ID_EXT)
	 {
		 node->IF2ARB |= 0x40000000;
	 }
	 else
	 {
	     node->IF2ARB &= ~0x40000000;
	 }

    /** - Update message box number. */
    /*SAFETYMCUSW 93 S MR: 6.1,6.2,10.1,10.2,10.3,10.4 <APPROVED> "LDRA Tool issue" */
    node->IF2NO = (uint8) messageBox;

    /** - Wait until data are copied into IF2 */
    while ((node->IF2STAT & 0x80U) ==0x80U)
    {
    } /* Wait */
}

/******************************************************************************
  Function:
		canGetIDType
  Description:
  	    Get certain messagebox Id type
  Input:
	    node : Can port
	    messageBox : Used messagebox
  Output:
  	    IdType : The type of ID
  Others:None
******************************************************************************/
uint8 canGetIDType(canBASE_t *node, uint32 messageBox)
{
    uint32 IdType;

    /** - Wait until IF2 is ready for use */
    while ((node->IF2STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

	/** - Configure IF2 for
	*     - Message direction - Read
	*     - Data Read
	*     - Clears NewDat bit in the message object.
	*/
	node->IF2CMD = 0xA0U;
	/* Copy passed value into the arbitration register. */
	IdType = (node->IF2ARB)& 0x40000000;
    /** - Update message box number. */
    /*SAFETYMCUSW 93 S MR: 6.1,6.2,10.1,10.2,10.3,10.4 <APPROVED> "LDRA Tool issue" */
    node->IF2NO = (uint8) messageBox;

    /** - Wait until data are copied into IF2 */
    while ((node->IF2STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

    if(IdType)
    	return CAN_ID_EXT;
    else
    	return CAN_ID_STD;
}

/******************************************************************************
  Function:
		canSetID
  Description:
  	    Set certain messagebox Id type
  Input:
	    node : Can port
	    messageBox : Used message box
	    msgBoxArbitVal : The ID
  Output:None
  Others:None
******************************************************************************/
void canSetID(canBASE_t *node, uint32 messageBox, uint32 msgBoxArbitVal)
{

    /** - Wait until IF2 is ready for use */
    while ((node->IF2STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

	/** - Configure IF2 for
	*     - Message direction - Read
	*     - Data Read
	*     - Clears NewDat bit in the message object.
	*/
	node->IF2CMD = 0xA0U;
	/* Copy passed value into the arbitration register. */
	//node->IF2ARB &= 0x80000000U;
	//node->IF2ARB |= (msgBoxArbitVal & 0x7FFFFFFFU);
	node->IF2ARB  = (uint32)0x80000000U | (uint32)0x00000000U | (uint32)0x20000000U | msgBoxArbitVal ;

    /** - Update message box number. */
    /*SAFETYMCUSW 93 S MR: 6.1,6.2,10.1,10.2,10.3,10.4 <APPROVED> "LDRA Tool issue" */
    node->IF2NO = (uint8) messageBox;

    /** - Wait until data are copied into IF2 */
    while ((node->IF2STAT & 0x80U) ==0x80U)
    {
    } /* Wait */

}

/******************************************************************************
  Function:
		CAN_Transmit
  Description:
  	    Sent data via certain messagebox
  Input:
	    node : Can port
	    messageBox : Used message box
	    msgBoxArbitVal : The ID
  Output: The function will return:
           - 0: When the setup of the TX message box wasn't successful
           - 1: When the setup of the TX message box was successful
  Others:None
******************************************************************************/
static uint32 CAN_Transmit(canBASE_t *node, uint32 messageBox, CanMsg *pMessage )
{
	CAN_EXTSTDIDTypedef CanId;
	uint8 Err,Status;

	/* Set ID step
	 * [1] set id
	 * [2] set id type
	 * */
	/* get can id*/
	if(pMessage->IDE == CAN_ID_STD)
		CanId.Id = pMessage->StdId;
	else
		CanId.Id = pMessage->ExtId;
	/* set id */
	canSetID(canREG3, 1, CanId.Id);

	/* set can id type */
	if(pMessage->IDE == CAN_ID_STD)
		cansetIDType(canREG3, 1, CAN_ID_STD);
	else
		cansetIDType(canREG3, 1, CAN_ID_EXT);

	Status = canTransmit( node, messageBox, pMessage->Data );

	if(Status == 1)
	{
		//printf("Massage box%2d id = %8x idtype = %1d canTransmit return status = %4d \r\n", 1, CanId.Id, pMessage->IDE, Status);
		Err = 0;
	}
	else
	{
		canInit();
	}

	return Err;

}

/******************************************************************************
  Function:
		CAN_MessageGet
  Description:
  	    Get the data from the current message
  Input:
	    pMessage: current message
  Output:None
  Others:None
******************************************************************************/
void CAN_MessageGet( CanMsg *pMessage )
{
	int i;
	uint8 cmd;
	CAN_STDEXTIDAddrTypedef CanSrcAddr,CanDstAddr;
	CAN_EXTSTDIDTypedef CanId;

	if( pMessage->IDE  == CAN_ID_STD )
	{
		CanId.Id = pMessage->StdId;
		CanDstAddr.CanIDAddr.StdIDAddr = CanId.CanStdId.DstAddr;
		CanSrcAddr.CanIDAddr.StdIDAddr = CanId.CanStdId.SrcAddr;
		cmd = CanId.CanStdId.FunCode;
	}
	else
	{
		CanId.Id = pMessage->ExtId;
		CanDstAddr.CanIDAddr.StdIDAddr = CanId.CanExtId.DstAddr;
		CanSrcAddr.CanIDAddr.StdIDAddr = CanId.CanExtId.SrcAddr;
		CanDstAddr.CanIDAddr.ExtIDAddr = CanId.CanExtId.ExtDstAddr;
		CanSrcAddr.CanIDAddr.ExtIDAddr = CanId.CanExtId.ExtSrcAddr;
		cmd = CanId.CanExtId.FunCode;
	}

	//printf("CanId.id.FunCode: %x\r\n",(cmd) & 0x1f);
	//printf("CanId.id.SrcAddr: %x\r\n",CanSrcAddr.IDAddr );
	//printf("CanId.id.DstAddr: %x\r\n",CanDstAddr.IDAddr);

	if( CanDstAddr.IDAddr == CanSetSrcAddr  )
	{
		CanRxCmdRingBuffer[CanRxCmdTopCounter].SrcAddr = CanSrcAddr.IDAddr;
		CanRxCmdRingBuffer[CanRxCmdTopCounter].DstAddr = CanDstAddr.IDAddr;
		CanRxCmdRingBuffer[CanRxCmdTopCounter].CanCmd = ( cmd ) & 0x1f;
		CanRxCmdRingBuffer[CanRxCmdTopCounter].IdType = pMessage->IDE;

		for( i=0; i<8; i++)
		{
			CanRxCmdRingBuffer[CanRxCmdTopCounter].data[i] = pMessage->Data[i];
		}

		CanReceivecompleteFlg = 1;

		CanRxCmdTopCounter++;

		if(CanRxCmdTopCounter == CANRINGBUFFERLEN)
		{
			CanRxCmdTopCounter = 0;
		}
	}
}

/******************************************************************************
  Function:
		Can_change_return_id
  Description:
  	    Change return id
  Input:
	    pMessage: current message
	    cmd: The current can command
	    ErrFlg: The current error types
  Output:None
  Others:None
******************************************************************************/
void Can_change_return_id( CanMsg *pMessage, uint8_t cmd , ErrTypedef ErrFlg )
{

	CAN_EXTSTDIDTypedef MyCAN_ID;
	CAN_STDEXTIDAddrTypedef MySrcIDAddr,MyDstIDAddr;
	uint8 ReturnStatusFlg = 0;

	if( ErrFlg )
		ReturnStatusFlg = 0;
	else
		ReturnStatusFlg = 0x10;

	MyDstIDAddr.IDAddr = CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr;
	MySrcIDAddr.IDAddr = CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr;

	if(  CanRxCmdRingBuffer[CanRxCmdButtomCounter].IdType == CAN_ID_STD )
	{
		MyCAN_ID.CanStdId.DstAddr = MyDstIDAddr.CanIDAddr.StdIDAddr ;
		MyCAN_ID.CanStdId.SrcAddr = MySrcIDAddr.CanIDAddr.StdIDAddr ;
		MyCAN_ID.CanStdId.FunCode = cmd | ReturnStatusFlg;
		/*
		printf("CanId.id.FunCode: %x\r\n",MyCAN_ID.CanStdId.FunCode);
		printf("CanId.id.SrcAddr: %x\r\n",MyCAN_ID.CanStdId.SrcAddr);
		printf("CanId.id.DstAddr: %x\r\n",MyCAN_ID.CanStdId.DstAddr);
		*/
	}
	else
	{
		MyCAN_ID.CanExtId.DstAddr = MyDstIDAddr.CanIDAddr.StdIDAddr ;
		MyCAN_ID.CanExtId.SrcAddr = MySrcIDAddr.CanIDAddr.StdIDAddr ;
		MyCAN_ID.CanExtId.ExtDstAddr = MyDstIDAddr.CanIDAddr.ExtIDAddr;
		MyCAN_ID.CanExtId.ExtSrcAddr = MySrcIDAddr.CanIDAddr.ExtIDAddr;
		MyCAN_ID.CanExtId.FunCode = cmd | ReturnStatusFlg;
		/*
		printf("CanId.id.FunCode: %x\r\n",MyCAN_ID.CanExtId.FunCode);
		printf("CanId.id.SrcAddr: %x\r\n",MyCAN_ID.CanExtId.SrcAddr);
		printf("CanId.id.DstAddr: %x\r\n",MyCAN_ID.CanExtId.DstAddr);
		*/
	}

	/* Transmit */
	if(  CanRxCmdRingBuffer[CanRxCmdButtomCounter].IdType == CAN_ID_STD )
	{
		pMessage->StdId = MyCAN_ID.Id;
	}
	else
	{
		pMessage->ExtId = MyCAN_ID.Id;
	}

	pMessage->RTR = CAN_RTR_DATA;

	pMessage->IDE = CanRxCmdRingBuffer[CanRxCmdButtomCounter].IdType;

	pMessage->DLC = 8;
}


/******************************************************************************
  Function:
		Can_Process
  Description:
  	    Can dealing message process
  Input:None
  Output:None
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
  Function:
		Can_cmd_parse
  Description:
  	    Can Process
  Input:None
  Output:
  	    status: The results of processing
  Others:None
******************************************************************************/
uint32 Can_cmd_parse( void )
{
	uint32 status;
	CanMsg CanReturnMessage;

	switch(CanRxCmdRingBuffer[CanRxCmdButtomCounter].CanCmd & 0x0f)
	{
		case CAN_CMD_READ_AD:
			Can_return_ad_msg(&CanReturnMessage);
			break;
		case CAN_CMD_CHECK_BOARD:
			Can_return_board_msg(&CanReturnMessage);
			break;
		case CAN_CMD_RW_EEPROM:
			Can_return_eeprom_msg(&CanReturnMessage);
			break;
		case CAN_CMD_RW_SN_ID:
			Can_return_sn_msg(&CanReturnMessage);
			break;
		case CAN_CMD_CALIBRATION_TEMP:
			Can_return_Calibration_temp_msg(&CanReturnMessage);
			break;
		default:
			break;
	}

	status = CAN_Transmit(canREG3, 1, &CanReturnMessage);
	return status;
}

/******************************************************************************
  Function:
		Can_return_sn_msg
  Description:
  	    Can Serial number operation function
  Input:
  	  	CanToCanTxMessage: return message
  Output:None
  Others:None
******************************************************************************/
void Can_return_sn_msg( CanMsg *CanToCanTxMessage )
{
	ErrTypedef Err;
	uint8 CmdFlg = 0;
	uint8 i;

	CmdFlg = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];

#ifdef USE_CAN_TEST

	CanToCanTxMessage->Data[0] = CmdFlg;

	if( CmdFlg == 0x01 )
	{
		for(i=0;i<7;i++)
			CanToCanTxMessage->Data[i+1] = SN_ID[i];
	}

	if( CmdFlg == 0x00 )
	{
		for(i=0;i<7;i++)
		{
			SN_ID[i] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[i+1];
			CanToCanTxMessage->Data[i+1] = SN_ID[i];
		}

	}

	Err = NoErr;

#endif

	Can_change_return_id( CanToCanTxMessage, CAN_CMD_RW_SN_ID, Err );

}


/******************************************************************************
  Function:
		Can_return_ad_msg
  Description:
  	    Can ad operation function
  Input:
  	  	CanToCanTxMessage: return message
  Output:None
  Others:None
******************************************************************************/
void Can_return_ad_msg( CanMsg *CanToCanTxMessage )
{
	uint32_t AdcValue = 0;
	uint32 Temperature = 0;
	float  AdcToPT1000Value = 0;
	ErrTypedef Err;
	uint8 Ch;

	Ch = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];

#ifdef USE_CAN_NORMAL
	//AdcValue = AdcReadData( Ch );
	AdcValue = AdcFilterReadData( Ch );
#endif

#ifdef USE_CAN_TEST
	//AdcValue = 0x123456;
	//AdcValue = AdcReadData( Ch );
	AdcValue = AdcFilterReadData( Ch );
#endif

	if( AdcValue > 0x7FFFFF )
		Err = ReadAdcErr;
	else
		Err = NoErr;

	AdcToPT1000Value = (float)AdcValue * 1640/0x7FFFFF ;

	Temperature = ResistanceToTemperature( AdcToPT1000Value, Ch );;

	Can_change_return_id( CanToCanTxMessage, CAN_CMD_READ_AD, Err );

	CanToCanTxMessage->Data[0] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];
	CanToCanTxMessage->Data[1] = *((uint8_t *)(&AdcValue)+3);
	CanToCanTxMessage->Data[2] = *((uint8_t *)(&AdcValue)+2);
	CanToCanTxMessage->Data[3] = *((uint8_t *)(&AdcValue)+1);
	CanToCanTxMessage->Data[4] = *((uint8_t *)(&Temperature)+3);
	CanToCanTxMessage->Data[5] = *((uint8_t *)(&Temperature)+2);
	CanToCanTxMessage->Data[6] = *((uint8_t *)(&Temperature)+1);
	CanToCanTxMessage->Data[7] = *((uint8_t *)(&Temperature)+0);
}

/******************************************************************************
  Function:
		Can_return_eeprom_msg
  Description:
  	    Can eeprom operation function
  Input:
  	  	CanToCanTxMessage: return message
  Output:None
  Others:None
******************************************************************************/
void Can_return_eeprom_msg( CanMsg *CanToCanTxMessage )
{
	uint8_t i;
	uint8_t EepromWritedata[8],EepromReaddata[8];;
	uint8_t EepromCh = 0;
	uint8_t EepromDataLen = 0;
	uint8_t EepromReadWriteCmd = 0;
	uint8_t EepromStartAddr = 0;
	uint8_t EepromBackState = 0;
	ErrTypedef Err;

	EepromReadWriteCmd = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];
	EepromCh = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1] ;
	EepromStartAddr = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[2];
	EepromDataLen = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[3];

	if( EepromReadWriteCmd == 0x00) // Write Cmd
	{
		EepromWritedata[0] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[4];
		EepromWritedata[1] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[5];
		EepromWritedata[2] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[6];
		EepromWritedata[3] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[7];

#ifdef USE_CAN_NORMAL
		EepromBackState = Eeprom_RW_value(Eepromdata,
								EepromCh,EepromReadWriteCmd,EepromStartAddr);
#endif

		DS2341_Write_8Byte( EepromCh, EepromStartAddr, EepromWritedata);
		DS2431_ReadData( EepromCh, EepromStartAddr, EepromReaddata, EepromDataLen );

#ifdef USE_CAN_TEST
		EepromBackState = 0;
#endif
	}

	if( EepromReadWriteCmd == 0x01) // Read Cmd
	{
#ifdef USE_CAN_NORMAL
		EepromBackState = Eeprom_RW_value(Eepromdata,
								EepromCh,EepromReadWriteCmd,EepromStartAddr);
#endif

#ifdef USE_CAN_TEST

		EepromBackState = 0;

		DS2431_ReadData( EepromCh, EepromStartAddr, EepromReaddata, EepromDataLen );
#endif
	}

	switch(EepromBackState)
	{
		case 0: Err = NoErr;          break;
		case 1: Err = ReadEepromErr;  break;
		case 2: Err = WriteEepromErr; break;
		default: break;
	}

	Can_change_return_id( CanToCanTxMessage, CAN_CMD_RW_EEPROM ,Err);

	CanToCanTxMessage->Data[0] = EepromReadWriteCmd;
	CanToCanTxMessage->Data[1] = EepromCh;
	CanToCanTxMessage->Data[2] = EepromStartAddr;
	CanToCanTxMessage->Data[3] = EepromDataLen;

	for( i=0; i< EepromDataLen; i++ )
	{
		CanToCanTxMessage->Data[4+i] = EepromReaddata[i];
	}

	for( i=0; i<4-EepromDataLen; i++ )
	{
		CanToCanTxMessage->Data[4+EepromDataLen+i] = 0x00;
	}
}

/******************************************************************************
  Function:
		Can_return_board_msg
  Description:
  	    Can noard check operation function
  Input:
  	  	CanToCanTxMessage: return message
  Output:None
  Others:None
******************************************************************************/
void Can_return_board_msg( CanMsg *CanToCanTxMessage )
{
	uint8_t i;
	uint8_t DeviceType = 0;
	uint16_t DevieCheckState = 0;
	ErrTypedef Err;

	DeviceType = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];

#ifdef USE_CAN_NORMAL
	DevieCheckState = Board_Check(DeviceType);
#endif

#ifdef USE_CAN_TEST

	DevieCheckState = 0xaa55;
	Err = NoErr;

#endif

	Can_change_return_id( CanToCanTxMessage, CAN_CMD_CHECK_BOARD, Err );

	CanToCanTxMessage->Data[0] = DeviceType;
	CanToCanTxMessage->Data[1] = *((uint8_t *)(&DevieCheckState)+1);
	CanToCanTxMessage->Data[2] = *(uint8_t *)(&DevieCheckState);

	for(i=0; i<5; i++)
	{
		CanToCanTxMessage->Data[3+i] = 0x00;
	}
}


/******************************************************************************
  Function:
		Can_return_Calibration_temp_msg
  Description:
  	    Can Calibration temperature parameter operation function
  Input:
  	  	CanToCanTxMessage: return message
  Output:None
  Others:None
******************************************************************************/
void Can_return_Calibration_temp_msg( CanMsg *CanToCanTxMessage )
{
	uint8_t i;
	uint8 CmdFlg = 0;
	uint8 Ch = 0;
	uint16 k = 0, b = 0;
	AdcCalibrationTypedef AdcCal;
	ErrTypedef Err;

	CmdFlg = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];
	Ch = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[1];

#ifdef USE_CAN_NORMAL

#endif

#ifdef USE_CAN_TEST
	Err = NoErr;
#endif

	if( CmdFlg == 0x00) /* Write  Calibration parameter */
	{
		AdcCal.k = *(uint16 *)&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[2]);
		AdcCal.b = *(uint16 *)&(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[4]);

		SetTempCalibrationParameter( Ch , AdcCal);
	}

	if( CmdFlg == 0x01 ) /* Read  Calibration parameter */
	{
		// Do Nothing
	}

	AdcCal = GetTempCalibrationParameter( Ch );

	k = AdcCal.k;
	b = AdcCal.b;

	CanToCanTxMessage->Data[2] = *((uint8_t *)(&k)+1);
	CanToCanTxMessage->Data[3] = *(uint8_t *)(&k);
	CanToCanTxMessage->Data[4] = *((uint8_t *)(&b)+1);
	CanToCanTxMessage->Data[5] = *(uint8_t *)(&b);

	Can_change_return_id( CanToCanTxMessage, CAN_CMD_CALIBRATION_TEMP, Err );

	CanToCanTxMessage->Data[0] = CmdFlg;
	CanToCanTxMessage->Data[1] = Ch;

	for(i=0; i<2; i++)
	{
		CanToCanTxMessage->Data[6+i] = 0x00;
	}
}
