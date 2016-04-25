/**
  ******************************************************************************
  * @file    APP/SerialProtocol.c
  * @author  Samwu
  * @version V1.0.0
  * @date    23-2-2016
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#include "platform_config.h"
#include "stm32f10x.h"                         // STM32F10x Library Definitions  
#include "SerialProtocol.h"
#include "CanProtocol.h"
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
#define USARTRINGBUFFERLEN                   10

/* Private variables ---------------------------------------------------------*/
/* compile message variables */
const uint8_t compile_data[] = __DATE__;
const uint8_t compile_time[] = __TIME__;
const uint8_t version[]="1.0";
const uint8_t serial_number[]="0000";


/* usart peripheral variables */
USART_MeaasgeTypedef  USART_Meaasge;

extern __IO uint8_t CanSetSrcAddr;

uint8_t UsartRxCmdTopCounter = 0;
uint8_t UsartRxCmdButtomCounter = 0;
uint8_t UsartReceivecompleteFlg = 0;	

uint8_t UsartRxCmdRingBuffer[USARTRINGBUFFERLEN][20];

/******************************************************************************
  Function:GE_MessageGet
  Description:The Grid - Eye hook function,
              Used to Get PC message sent to Grid-Eye board
              The format of the message below
              -----------------------------------
             |   "***"   |   u8   |  Array       |
             |  pHeader  |  pLen  |  pCmdData[]  |
              -----------------------------------
  Input:
      pMeaasge:Grid-EYE message struct pointer
      TX_data :Character of the PC was sent
  Output:None
  Return:None
  Others:
******************************************************************************/
void USART_MessageGet( USART_MeaasgeTypedef *pMeaasge, uint8_t data )
{				
	int i;
	switch (pMeaasge->ReceiveState)
	{
	 	case USART_RECEIVE_IDLE:
		{
			if( data == '*' )
			{
				pMeaasge->ReceiveState =USART_RECEIVE_HEADER1;
			}	 
		}
		break;
				 
		case USART_RECEIVE_HEADER1:
		{
			if( data == '*' )
			{
				pMeaasge->ReceiveState = USART_RECEIVE_HEADER2;
			}
			else
			{
				pMeaasge->ReceiveState = USART_RECEIVE_IDLE;
			}
		}
		break;
				
		case USART_RECEIVE_HEADER2:
		{
			if( data == '*' )
			{
				pMeaasge->ReceiveState = USART_RECEIVE_CMDLEN;
			}
			else
			{
				pMeaasge->ReceiveState = USART_RECEIVE_IDLE;
			}
		}
		break;	
						 
		case USART_RECEIVE_CMDLEN:
		{
			if( data == '*' )
			{
				pMeaasge->ReceiveState = USART_RECEIVE_CMDLEN;
			}					
			else
			{
				pMeaasge->ReceiveDataLen = data;
				pMeaasge->ReceiveDataNum = 0;
				pMeaasge->ReceiveState = USART_RECEIVE_CMDDATA;
			}
		}
		break;
		
		case USART_RECEIVE_CMDDATA:
		{
			Serial_buffer_add( pMeaasge, data );
			/* Is the data receiving */
			if((pMeaasge->ReceiveDataNum == pMeaasge->ReceiveDataLen) 
				 && (pMeaasge->ReceiveDataNum != 0))
			{
				
				//printf("ReceiveDataBuf data is = %s \r\n",pMeaasge->ReceiveDataBuf+1);

				/* Complete CMD parse data reset, enter the idle state */
				for(i = 0; i < pMeaasge->ReceiveDataNum; i++)
				{
					UsartRxCmdRingBuffer[UsartRxCmdTopCounter][i] = pMeaasge->ReceivetmpData[i];
				}
				
				for(i = pMeaasge->ReceiveDataNum; i < 8; i++)
				{
					UsartRxCmdRingBuffer[UsartRxCmdTopCounter][i] = pMeaasge->ReceivetmpData[i];
				}
				
				Serial_buffer_clear( pMeaasge );
				pMeaasge->ReceiveDataLen = 0;
				pMeaasge->ReceiveDataNum = 0;
				
				UsartReceivecompleteFlg = 1;
					
				UsartRxCmdTopCounter++;
				if(UsartRxCmdTopCounter == USARTRINGBUFFERLEN)
				{
					UsartRxCmdTopCounter = 0;
				}
				
				pMeaasge->ReceiveState = USART_RECEIVE_IDLE;
			}
			else
			{
				pMeaasge->ReceiveState = USART_RECEIVE_CMDDATA;
			}
		}
		break;
				
		default:
			break;
	}	
	
}

/**
  * @brief  printf compile message.
  * @param  None
  * @retval None
  */
void Serial_buffer_add( USART_MeaasgeTypedef *pMeaasge, uint8_t data )
{
	pMeaasge->ReceivetmpData[(pMeaasge->ReceiveDataNum)++] = data;
}

/**
  * @brief  printf compile message.
  * @param  None
  * @retval None
  */
void Serial_buffer_clear( USART_MeaasgeTypedef *pMeaasge )
{
	unsigned int i;
	for(i=0; i< pMeaasge->ReceiveDataNum; i++)
	{
		pMeaasge->ReceivetmpData[i] = 0;
	}
}

/******************************************************************************
  Function:Serial_Process
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_Process( void )
{
	if(UsartReceivecompleteFlg == 1)
	{
		if(UsartRxCmdTopCounter  !=  UsartRxCmdButtomCounter)
		{	
			
			Serial_cmd_parse();

			UsartRxCmdButtomCounter++;
			
			if(UsartRxCmdButtomCounter == USARTRINGBUFFERLEN)
			{
				UsartRxCmdButtomCounter = 0;
			}
			
			if(UsartRxCmdTopCounter == UsartRxCmdButtomCounter)
			{
				 UsartReceivecompleteFlg = 0;
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
void Serial_cmd_parse( void )
{
	switch(((USART_FrameTypedef *)\
		      (UsartRxCmdRingBuffer[UsartRxCmdButtomCounter]))->Funcode)
	{
		case USART_CMD_UART_MENU_HELP: 
			{
				Serial_Serial_cmd_menu();  
			}
			break;
		
		case USART_CMD_CAN_MENU_HELP:
			{
				Serial_to_can_test();      
			}
			break;
		
		case USART_CMD_CAN_AUTO_TEST:
			{
				Serial_to_can_auto_test(
							UsartRxCmdRingBuffer[UsartRxCmdButtomCounter][1],
							*(uint16_t *)(&(UsartRxCmdRingBuffer[UsartRxCmdButtomCounter][2])));
			} 
			break;
			
		case USART_CMD_COMPILE_MESSAGE:
			{
				Serial_show_compile_msg(); 
			}
			break;
		
		case USART_CMD_SET_CAN_SCR_ADDR:
			{
				Serial_set_srcaddr();      
			}
			break;	
		
		default:
			{
				Serial_send_cmd_to_can();  
			}
			break;
	}
	
}

/******************************************************************************
  Function:Serial_SetCanID
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_SetCanID(CanTxMsg *pMessage, uint8_t CanCmd, 
	                   uint8_t Idtype, uint8_t CanDstID )
{
	
	CAN_STDEXTIDAddrTypedef CanSrcAddr,CanDstAddr;
	CAN_EXTSTDIDTypedef CAN_ID;
	
	CanSrcAddr.IDAddr = CanSetSrcAddr;
	CanDstAddr.IDAddr = CanDstID;

	CAN_ID.CanProtocolId.FunCode = CanCmd;
	CAN_ID.CanProtocolId.SrcAddr = CanSrcAddr.CanIDAddr.StdIDAddr;
	CAN_ID.CanProtocolId.DstAddr = CanDstAddr.CanIDAddr.StdIDAddr;
	
	
	/* Transmit */
	if( Idtype == CAN_ID_STD )
	{
		CAN_ID.CanProtocolId.ExtDstAddr = 0x00;
		CAN_ID.CanProtocolId.ExtSrcAddr = 0x00;
		pMessage->StdId = CAN_ID.Id;
	}
	else
	{
		CAN_ID.CanProtocolId.ExtDstAddr = CanDstAddr.CanIDAddr.ExtIDAddr;
		CAN_ID.CanProtocolId.ExtSrcAddr = CanSrcAddr.CanIDAddr.ExtIDAddr;
		pMessage->ExtId = CAN_ID.Id;
	}
	
	pMessage->RTR = CAN_RTR_DATA;
	
	pMessage->IDE = Idtype;
	
	pMessage->DLC = 8;
}

/******************************************************************************
  Function:Serial_show_compile_msg
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_show_compile_msg( void )
{
	printf("compile_data:%s\r\n",compile_data);
	printf("compile_data:%s\r\n",compile_time);
	printf("software_version:%s\r\n",version);
}

void Serial_set_srcaddr(void)
{
	CanSetSrcAddr = UsartRxCmdRingBuffer[UsartRxCmdButtomCounter][1];
	printf("Set current board address is :%x \r\n",CanSetSrcAddr);
}

void Set_srcaddr( uint8_t SrcAddr )
{
	CanSetSrcAddr = SrcAddr;
	printf("Set current board address is :%x \r\n",CanSetSrcAddr);
}


/******************************************************************************
  Function:Serial_send_cmd_to_can
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_send_cmd_to_can( void )
{
	uint8_t i;
	CanTxMsg UartToCanTxMessage;
	
	Serial_SetCanID(&UartToCanTxMessage,
	((USART_FrameTypedef *)(UsartRxCmdRingBuffer[UsartRxCmdButtomCounter]))->Funcode,
	((USART_FrameTypedef *)(UsartRxCmdRingBuffer[UsartRxCmdButtomCounter]))->CanIDType,
	((USART_FrameTypedef *)(UsartRxCmdRingBuffer[UsartRxCmdButtomCounter]))->CanDstID);
	
	for( i=0; i<8; i++ )
	{
		UartToCanTxMessage.Data[i] = ((USART_FrameTypedef *)\
		      (UsartRxCmdRingBuffer[UsartRxCmdButtomCounter]))->Data[i];
	}
	
	CAN_Transmit(CAN1, &UartToCanTxMessage);
}


/******************************************************************************
  Function:Serial_to_can_test
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_Serial_cmd_menu( void )
{
	printf("------------------------ Uart Cmd menu ----------------------\r\n"
	"USART_CMD_COMPILE_MESSAGE:\r\n"
	"\t Test Cmd : 2A 2A 2A 01 30 \r\n"
	"USART_CMD_SET_CAN_SCR_ADDR:\r\n"
	"\t Test Cmd : 2A 2A 2A 02 31 02(addr)  \r\n"
	"USART_CMD_UART_MENU_HELP:\r\n"
	"\t Test Cmd : 2A 2A 2A 01 32 \r\n"
	"USART_CMD_CAN_MENU_HELP:\r\n"
	"\t Test Cmd : 2A 2A 2A 01 33 \r\n"
	"USART_CMD_CAN_AUTO_TEST:\r\n"
	"\t Test Cmd : 2A 2A 2A 01 34  \r\n"
	);

}

/******************************************************************************
  Function:Serial_to_can_test
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_to_can_test( void )
{
	printf("------------------------ Can Test menu -----------------------\r\n"
	"USART_CMD_READ_AD_TEST:\r\n"
	"\t Test Cmd : 2A 2A 2A 04 01 00 01 00(channel) \r\n"
	"USART_CMD_READ_RW_EEPROM_TEST:\r\n"
	"\t Read Cmd  : 2A 2A 2A 07 02 00 01 01 00 00 04 \r\n"
	"\t Write Cmd : 2A 2A 2A 0B 02 00 01 00 00 00 04 11 22 33 44 \r\n"
	"USART_CMD_READ_CHECK_BOARD_TEST:\r\n"
	"\t TEDS   Cmd : 2A 2A 2A 04 03 00 01 01 \r\n"
	"\t PT1000 Cmd : 2A 2A 2A 04 03 00 01 02 \r\n"
	"USART_CMD_READ_RW_SN_TEST:\r\n"
	"\t Read SN Cmd  : 2A 2A 2A 04 04 00 01 01 \r\n"
	"\t Write SN Cmd : 2A 2A 2A 0b 04 00 01 00 11 22 33 44 55 66 77 \r\n"
	"USART_CMD_CALIBRATION_TEMP_TEST:\r\n"
	"\t Read Caltmp Cmd  : 2A 2A 2A 05 05 00 01 01 00  \r\n"
	"\t Write Caltmp Cmd : 2A 2A 2A 09 05 00 01 00 00 16 63 34 77 \r\n"
	);
}

/******************************************************************************
  Function:Serial_to_can_test
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void Serial_to_can_auto_test( uint8_t cmd, uint16_t Cnt )
{
	//uint8_t *Str;
	
	SetCanAutoTest(1);
	
	/*
	switch( cmd )
	{
		case USART_CMD_READ_AD_TEST: 
		{			 
			Str = "USART_CMD_READ_AD_TEST";
			//Can_auto_test_Adc();
		}
		break;
		
		case USART_CMD_READ_RW_EEPROM_TEST:   
		{
			Can_auto_test_eeprom( Cnt );      
			Str = "USART_CMD_READ_RW_EEPROM_TEST";
		}
		break;
		
		case USART_CMD_CHECK_BOARD_TEST:  
		{			
			//Can_auto_test_check_borad();
			Str = "USART_CMD_CHECK_BOARD_TEST";
		}
		break;
		
		case USART_CMD_READ_RW_SN_TEST:  
		{			
			//Can_auto_test_RW_SN_ID(); 
			Str =	"USART_CMD_READ_RW_SN_TEST";	
		}
		break;
		
		case USART_CMD_CALIBRATION_TEMP_TEST:
		{			
			//Can_auto_test_CalTemp(); 
			Str =	"USART_CMD_CALIBRATION_TEMP_TEST";
		}
		break;
		
		default: break;
		
	}
	
	printf(" Can auto test Cmd : %s Test times : %d \r\n", Str, Cnt );
	*/
	
}





/***************************** END OF FILE ************************************/
