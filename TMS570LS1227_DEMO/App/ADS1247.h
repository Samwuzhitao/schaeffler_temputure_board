/*
 * ADS1247.h
 *
 *  Created on: 2016Äê4ÔÂ13ÈÕ
 *      Author: sam.wu
 */

#ifndef TMS570LS1227_DEMO_APP_ADS1247_H_
#define TMS570LS1227_DEMO_APP_ADS1247_H_

#include "sys_common.h"
#include "spi.h"

 /* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* REGISTER ADDRESS */
#define ADS1247_REG_MUX0                 0x00 ///< Multiplexer Control Register 0
#define ADS1247_REG_VBIAS                0x01 ///< Bias Voltage Registe
#define ADS1247_REG_MUX1                 0x02 ///< Multiplexer Control Register 1
#define ADS1247_REG_SYS0                 0x03 ///< System Control Register 0
#define ADS1247_REG_OFC0                 0x04 ///< Offset Calibration Coefficient Register 0
#define ADS1247_REG_OFC1                 0x05 ///< Offset Calibration Coefficient Register 1
#define ADS1247_REG_OFC2                 0x06 ///< Offset Calibration Coefficient Register 2
#define ADS1247_REG_FSC0                 0x07 ///< Full-Scale Calibration Coefficient Register 0
#define ADS1247_REG_FSC1                 0x08 ///< Full-Scale Calibration Coefficient Register 1
#define ADS1247_REG_FSC2                 0x09 ///< Full-Scale Calibration Coefficient Register 2
#define ADS1247_REG_IDAC0                0x0A ///< IDAC Control Register 0
#define ADS1247_REG_IDAC1                0x0B ///< IDAC Control Register 1
#define ADS1247_REG_GPIOCFG              0x0C ///< GPIO Configuration Register
#define ADS1247_REG_GPIODIR              0x0D ///< GPIO Direction Register
#define ADS1247_REG_GPIODAT              0x0E ///< GPIO Data Register

/* COMMAND */
/* System control command */
#define ADS1247_CMD_WAKEUP               0x00 ///< Exit sleep mode
#define ADS1247_CMD_SLEEP                0x02 ///< Enter sleep mode
#define ADS1247_CMD_SYNC                 0x04 ///< Synchronize the A/D conversion
#define ADS1247_CMD_RESET                0x06 ///< Reset to power-up values
#define ADS1247_CMD_NOP                  0xFF ///< No operation

/* Read data command */
#define ADS1247_CMD_RDATA                0x12 ///< Read data once
#define ADS1247_CMD_RDATAC               0x14 ///< Read data continuously
#define ADS1247_CMD_SDATAC               0x16 ///< Stop reading data continuously

/* Read write register command */
#define ADS1247_CMD_RREG                 0x20 ///< Read from register
#define ADS1247_CMD_WREG                 0x40 ///< Read from register

/* Calibration command */
#define ADS1247_CMD_SYSOCAL              0x60 ///< System offset calibration
#define ADS1247_CMD_SYSGCAL              0x61 ///< System gain calibration
#define ADS1247_CMD_SYSFOCAL             0x61 ///< Self offset calibration





/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t ADS1247_ReadWriteData(uint8_t data);
uint8_t ADS1247_ReadRegister( uint8_t addr );
void ADS1247_ReadRegisters( uint8_t addr, uint8_t num, uint8_t data[] );
void ADS1247_WriteRegister( uint8_t addr, int8_t data );
void ADS1247_WriteRegisters( uint8_t addr, uint8_t num, uint8_t data[] );
uint32_t ADS1247_ReadData( void );
void ADS1247_Init( void );
void ADS1247_Delay(unsigned int nCount);

#endif /* TMS570LS1227_DEMO_APP_ADS1247_H_ */
