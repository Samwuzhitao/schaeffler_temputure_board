/**
  ******************************************************************************
  * @file    APP/SerialProtocol.h
  * @author  Samwu
  * @version V1.0.0
  * @date    23-2-2016
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  */ 
#ifndef __SERIALPROTOCOL_H
#define __SERIALPROTOCOL_H

/* Define to prevent recursive inclusion -------------------------------------*/
#include "platform_config.h"
#include "stm32f10x.h"                         // STM32F10x Library Definitions  

/* Private typedef -----------------------------------------------------------*/
/* A serial port receiving character relevant variables */
typedef struct 
{
	uint32_t ReceiveDataNum;       /* the conuter for the databuf   */
	uint8_t  ReceiveState;         /* receive state                 */
	uint8_t  ReceiveDataLen;       /* The length of the data        */
	uint8_t  ReceivetmpData[11];   /* The length of the data        */
} USART_MeaasgeTypedef;

typedef struct 
{ 
	uint8_t  Funcode;              /* receive state                 */	
	uint8_t  CanIDType;            /* receive state                 */
	uint8_t  CanDstID;             /* receive state                 */
	uint8_t  Data[8];             /* The length of the data        */
} USART_FrameTypedef;

/* Data statement */
#define USART_RECEIVE_IDLE         0     /* no received data      */
#define USART_RECEIVE_HEADER1      1     /* start receive Header  */
#define USART_RECEIVE_HEADER2      2     /* start receive Header  */
#define USART_RECEIVE_CMDLEN       3     /* start receive Header  */
#define USART_RECEIVE_CMDDATA      4     /* start receive CmdData */


/* Data statement */
#define USART_CMD_COMPILE_MESSAGE            '0'   /*  */
#define USART_CMD_SET_CAN_SCR_ADDR           '1'   /*  */
#define USART_CMD_UART_MENU_HELP             '2'   /*  */
#define USART_CMD_CAN_MENU_HELP              '3'
#define USART_CMD_CAN_AUTO_TEST              '4'

#define USART_CMD_READ_AD_TEST               'A'
#define USART_CMD_READ_RW_EEPROM_TEST        'B'
#define USART_CMD_CHECK_BOARD_TEST           'C'
#define USART_CMD_READ_RW_SN_TEST            'D'
#define USART_CMD_CALIBRATION_TEMP_TEST      'E'

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Serial_show_compile_msg( void );
void USART_MessageGet( USART_MeaasgeTypedef *pMeaasge, uint8_t TX_data );

void Serial_buffer_clear( USART_MeaasgeTypedef *pMeaasge );
void Serial_buffer_add( USART_MeaasgeTypedef *pMeaasge, uint8_t data );
void Serial_Process( void );
void Serial_cmd_parse( void );
void Serial_SetCanID(CanTxMsg *pMessage, uint8_t CanCmd, uint8_t Idtype, uint8_t CanDstID );
void Serial_set_srcaddr(void);
void Serial_send_cmd_to_can( void );

void Serial_Serial_cmd_menu( void );
void Serial_to_can_test( void );
void Serial_to_can_auto_test( uint8_t cmd, uint16_t Cnt );
void Can_send_cmd_to_can( uint8_t CmdDta[] );
void Can_auto_test_eeprom( uint16_t Cnt );

uint8_t IsCanAutoTest( void );
void CanAutoTestIndexAdd( void );
uint8_t *GetCanCurrentTestCmd( void );

#endif
/***************************** END OF FILE ************************************/
