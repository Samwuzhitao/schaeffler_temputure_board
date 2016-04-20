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

#define CANRINGBUFFERLEN                   10

/* Private variables ---------------------------------------------------------*/
/* can peripheral variables */
uint8 CanSetSrcAddr = CAN_SRC_ADDR;
uint8 CanRxCmdTopCounter = 0;
uint8 CanRxCmdButtomCounter = 0;
uint8 CanReceivecompleteFlg = 0;

CAN_MessageTypedef CanRxCmdRingBuffer[CANRINGBUFFERLEN];


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
  Function: CAN_Transmit
  Description:
  Input :
  	  node       :node Pointer to CAN node
 	  messageBox :messageBox Message box number of CAN node
 	  pMessage   :Can need to transmit message
  Output:
  Return: The function will return:
           - 0: When the setup of the TX message box wasn't successful
           - 1: When the setup of the TX message box was successful
  Others:None
******************************************************************************/
static uint32 CAN_Transmit(canBASE_t *node, uint32 messageBox, CanMsg *pMessage )
{
	CAN_EXTSTDIDTypedef CanId;
	uint32 status;

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

	status = canTransmit( node, messageBox, pMessage->Data );

	printf("Massage box%2d id = %8x idtype = %1d canTransmit return status = %4d \r\n", 1, CanId.Id, pMessage->IDE, status);

	return status;

}

/******************************************************************************
  Function:CAN_MessageGet
  Description:
  Input:None
  Output:
  Return:
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
  Function:Can_to_can_change_id
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_change_return_id( CanMsg *pMessage, uint8_t cmd )
{

	CAN_EXTSTDIDTypedef MyCAN_ID;
	CAN_STDEXTIDAddrTypedef MySrcIDAddr,MyDstIDAddr;

	MyDstIDAddr.IDAddr = CanRxCmdRingBuffer[CanRxCmdButtomCounter].SrcAddr;
	MySrcIDAddr.IDAddr = CanRxCmdRingBuffer[CanRxCmdButtomCounter].DstAddr;

	if(  CanRxCmdRingBuffer[CanRxCmdButtomCounter].IdType == CAN_ID_STD )
	{
		MyCAN_ID.CanStdId.DstAddr = MyDstIDAddr.CanIDAddr.StdIDAddr ;
		MyCAN_ID.CanStdId.SrcAddr = MySrcIDAddr.CanIDAddr.StdIDAddr ;
		MyCAN_ID.CanStdId.FunCode = cmd | 0x10;
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
		MyCAN_ID.CanExtId.FunCode = cmd | 0x10;
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
			//Can_return_board_msg();
			break;
		case CAN_CMD_RW_EEPROM:
			//Can_return_eeprom_msg();
			break;
		case CAN_CMD_READ_SN_ID:
			Can_return_sn_msg(&CanReturnMessage);
			break;
		default:
			break;
	}

	status = CAN_Transmit(canREG3, 1, &CanReturnMessage);
	return status;
}

/******************************************************************************
  Function:Can_return_sn_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_return_sn_msg( CanMsg *CanToCanTxMessage )
{
	Can_change_return_id( CanToCanTxMessage, CAN_CMD_READ_SN_ID );

#ifdef USE_CAN_TEST
	CanToCanTxMessage->Data[0] = 0x11;
	CanToCanTxMessage->Data[1] = 0x22;
	CanToCanTxMessage->Data[2] = 0x33;
	CanToCanTxMessage->Data[3] = 0x44;
	CanToCanTxMessage->Data[4] = 0x55;
	CanToCanTxMessage->Data[5] = 0x66;
	CanToCanTxMessage->Data[6] = 0x77;
	CanToCanTxMessage->Data[7] = 0x88;
#endif

}


/******************************************************************************
  Function:Can_read_ad_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Can_return_ad_msg( CanMsg *CanToCanTxMessage )
{
	uint32_t AdcValue = 0;
	uint32 Temperature = 0;
	float  AdcToPT1000Value = 0;
	uint8 Ch;

	Can_change_return_id( CanToCanTxMessage, CAN_CMD_READ_AD );

#ifdef USE_CAN_NORMAL
	AdcValue = Adc_get_value(CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0]);
#endif

#ifdef USE_CAN_TEST
	//AdcValue = 0x123456;
	Ch = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];

	//AdcValue = AdcReadData( Ch );
	AdcValue = AdcFilterReadData( Ch );

	AdcToPT1000Value = (float)AdcValue * 1640/0x7FFFFF ;

	Temperature = ResistanceToTemperature( AdcToPT1000Value, Ch );;

#endif

	CanToCanTxMessage->Data[0] = CanRxCmdRingBuffer[CanRxCmdButtomCounter].data[0];
	CanToCanTxMessage->Data[1] = *((uint8_t *)(&AdcValue)+3);
	CanToCanTxMessage->Data[2] = *((uint8_t *)(&AdcValue)+2);
	CanToCanTxMessage->Data[3] = *((uint8_t *)(&AdcValue)+1);
	CanToCanTxMessage->Data[4] = *((uint8_t *)(&Temperature)+3);
	CanToCanTxMessage->Data[5] = *((uint8_t *)(&Temperature)+2);
	CanToCanTxMessage->Data[6] = *((uint8_t *)(&Temperature)+1);
	CanToCanTxMessage->Data[7] = *((uint8_t *)(&Temperature)+0);
}
