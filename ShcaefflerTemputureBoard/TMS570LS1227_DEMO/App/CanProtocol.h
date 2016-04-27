/*
 * CanProtocol.h
 *
 *  Created on: 2016Äê3ÔÂ29ÈÕ
 *      Author: Sam
 */

#ifndef CANPROTOCOL_H_
#define CANPROTOCOL_H_

#define USE_CAN_TEST
//#define USE_CAN_NORMAL

#define CAN_SRC_ADDR                1     /* This Board addr         */
#define CANRINGBUFFERLEN            10

/* Can commond code */
#define CAN_CMD_READ_AD             1     /* Read adc value cmd code */
#define CAN_CMD_RW_EEPROM           2     /* Read and write cmd code */
#define CAN_CMD_CHECK_BOARD         3     /* check themself cmd code */
#define CAN_CMD_RW_SN_ID            4     /* Read SN id cmd code     */
#define CAN_CMD_CALIBRATION_TEMP    5     /* Calculate and set calibration temperature parameter */


/* Can ID type */
#define CAN_ID_STD                  0     /* Std ID type             */
#define CAN_ID_EXT                  4     /* Ext ID type             */
#define CAN_RTR_DATA                0     /* Data frame              */


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that
                        will be received. This parameter can be a value of
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to
                        0xFF. */

  uint8_t FMI;     /*!< Specifies the index of the filter the message stored in
                        the mailbox passes through. This parameter can be a
                        value between 0 to 0xFF */
} CanMsg;

/* Can receiving character relevant variables */
typedef union
{
	struct
	{
		uint8_t ExtIDAddr:5;          /*!< bit:  3..7   extid                  */
		uint8_t StdIDAddr:3;          /*!< bit:  0..2   stdid                  */
	}CanIDAddr;
	uint8_t IDAddr;
} CAN_STDEXTIDAddrTypedef;

typedef union
{
	struct
	{
		uint32_t RESERVED0:11;      /*!< bit: 21..31 Reserved                  */
		uint32_t ExtDstAddr:5;      /*!< bit: 16..20 destination address       */
		uint32_t ExtSrcAddr:5;      /*!< bit: 11..15 destination address       */
		uint32_t DstAddr:3;         /*!< bit:  8..10 destination address       */
		uint32_t SrcAddr:3;         /*!< bit:  5..7  source address            */
		uint32_t FunCode:5;         /*!< bit:  0..4  CMD Code                  */
	}CanExtId;
	struct
	{
		uint32_t RESERVED1:3;       /*!< bit: 29..31 Reserved1                 */
		uint32_t DstAddr:3;         /*!< bit: 25..28 destination address       */
		uint32_t SrcAddr:3;         /*!< bit: 22..24 source address            */
		uint32_t FunCode:5;         /*!< bit: 17..21 CMD Code                  */
		uint32_t RESERVED0:18;      /*!< bit:  0..17 Reserved0                 */
	}CanStdId;
	uint32_t Id;
} CAN_EXTSTDIDTypedef;

typedef struct
{
	uint8_t SrcAddr;             /*!< source address               */
	uint8_t DstAddr;             /*!< destination address          */
	uint8_t CanCmd;
	uint8_t IdType;              /*!< CAN ID type                  */
	uint8_t data[8];
} CAN_MessageTypedef;

typedef enum
{
	NoErr = 0,
	ReadAdcErr,
	EepromErr,
	CanTransmitErr
} ErrTypedef;

/* Private function prototypes -----------------------------------------------*/
uint8 canGetIDType(canBASE_t *node, uint32 messageBox);
void cansetIDType(canBASE_t *node, uint32 messageBox, uint8 IdType);
void canSetID(canBASE_t *node, uint32 messageBox, uint32 msgBoxArbitVal);

void CAN_MessageGet( CanMsg *pMessage );
uint32 Can_cmd_parse( void );
void Can_Process( void );
void Can_change_return_id( CanMsg *pMessage, uint8_t cmd , ErrTypedef ErrFlg );

void Can_return_sn_msg( CanMsg *CanToCanTxMessage );
void Can_return_ad_msg( CanMsg *CanToCanTxMessage );
void Can_return_eeprom_msg( CanMsg *CanToCanTxMessage );
void Can_return_board_msg( CanMsg *CanToCanTxMessage );
void Can_return_Calibration_temp_msg( CanMsg *CanToCanTxMessage );

#endif /* CANPROTOCOL_H_ */
