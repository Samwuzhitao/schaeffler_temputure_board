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
#include "gio.h"
#include "het.h"

#ifdef __cplusplus
extern "C" {
#endif

 /* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint16 k;
	uint16 b;
} AdcCalibrationTypedef;

/* Private macro -------------------------------------------------------------*/
#define LV138CH1_EN_1  hetREG1->DOUT  |= (uint32)((uint32)1U << 20U)   /* PIN141 */
#define LV138CH1_EN_0  hetREG1->DOUT  &=~(uint32)((uint32)1U << 20U)
#define LV138CH1_A0_1  gioPORTA->DOUT |= (uint32)((uint32)1U << 7U)    /* PIN22 */
#define LV138CH1_A0_0  gioPORTA->DOUT &=~(uint32)((uint32)1U << 7U)
#define LV138CH1_A1_1  gioPORTA->DOUT |= (uint32)((uint32)1U << 6U)    /* PIN16 */
#define LV138CH1_A1_0  gioPORTA->DOUT &=~(uint32)((uint32)1U << 6U)
#define LV138CH1_A2_1  gioPORTA->DOUT |= (uint32)((uint32)1U << 2U)    /* PIN9 */
#define LV138CH1_A2_0  gioPORTA->DOUT &=~(uint32)((uint32)1U << 2U)

#define LV138CH2_EN_1  spiREG5->PC3   |= (uint32)((uint32)1U << 8U)    /* PIN97  */
#define LV138CH2_EN_0  spiREG5->PC3   &=~(uint32)((uint32)1U << 8U)
#define LV138CH2_A0_1  spiREG5->PC3   |= (uint32)((uint32)1U << 9U)    /* PIN100 */
#define LV138CH2_A0_0  spiREG5->PC3   &=~(uint32)((uint32)1U << 9U)
#define LV138CH2_A1_1  spiREG5->PC3   |= (uint32)((uint32)1U << 10U)   /* PIN99  */
#define LV138CH2_A1_0  spiREG5->PC3   &=~(uint32)((uint32)1U << 10U)
#define LV138CH2_A2_1  spiREG5->PC3   |= (uint32)((uint32)1U << 11U)   /* PIN98  */
#define LV138CH2_A2_0  spiREG5->PC3   &=~(uint32)((uint32)1U << 11U)

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

/* Reg  */
#define MUX0_SN_AIN0                     (uint8)((uint8)(0x00 << 0))
#define MUX0_SN_AIN1                     (uint8)((uint8)(0x01 << 0))
#define MUX0_SN_AIN2                     (uint8)((uint8)(0x02 << 0))
#define MUX0_SN_AIN3                     (uint8)((uint8)(0x03 << 0))

#define MUX0_SP_AIN0                     (uint8)((uint8)(0x00 << 3))
#define MUX0_SP_AIN1                     (uint8)((uint8)(0x01 << 3))
#define MUX0_SP_AIN2                     (uint8)((uint8)(0x02 << 3))
#define MUX0_SP_AIN3                     (uint8)((uint8)(0x03 << 3))

#define MUX0_BCS_OFF                     (uint8)((uint8)(0x00 << 6))
#define MUX0_BCS_0_5UA                   (uint8)((uint8)(0x01 << 6))
#define MUX0_BCS_2_0UA                   (uint8)((uint8)(0x02 << 6))
#define MUX0_BCS_10_0UA                  (uint8)((uint8)(0x03 << 6))


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SpiSetCs( spiBASE_t *spi, uint8 nCs );
void SpiClearCs( spiBASE_t *spi, uint8 nCs );
uint8_t SpiReadWriteData( spiBASE_t *spi, uint8_t data );

void Ads1247Delay( uint32 nCount );
uint8_t Ads1247ReadRegister( spiBASE_t *spi, uint8_t addr );
void Ads1247WriteRegister( spiBASE_t *spi, uint8_t addr, int8_t data );
void Ads1247ReadRegisters( spiBASE_t *spi, uint8_t addr, uint8_t num, uint8_t data[] );
void Ads1247WriteRegisters( spiBASE_t *spi, uint8_t addr, uint8_t num, uint8_t data[] );

uint16 Ads1247Init( spiBASE_t *spi );
uint32_t Ads1247ReadData( spiBASE_t *spi );

uint16 AdcInit( void );
spiBASE_t *AdcChannelSet( uint8 Ch );
uint32_t AdcReadData( uint8 Ch );
uint32_t AdcFilterReadData( uint8 Ch );
uint32_t ResistanceToTemperature( float Resistance , uint8 Ch);
void SetTempCalibrationParameter(uint8 Ch , AdcCalibrationTypedef AdcCal);
AdcCalibrationTypedef GetTempCalibrationParameter( uint8 Ch );

#ifdef __cplusplus
}
#endif

#endif /* TMS570LS1227_DEMO_APP_ADS1247_H_ */
