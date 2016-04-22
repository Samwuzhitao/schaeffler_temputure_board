/*
 * LED.h
 *
 *  Created on: 2016��4��21��
 *      Author: sam.wu
 */

#ifndef TMS570LS1227_DEMO_APP_LED_H_
#define TMS570LS1227_DEMO_APP_LED_H_
#include "can.h"

typedef enum
{
	LED4 = 0,
	LED5,
	LED6,
	LED7,
} LEDTypedef;

void LEDInit( void );
void LEDFlash( void );
void LEDOn( LEDTypedef Led );
void LEDOff( LEDTypedef Led );

#endif /* TMS570LS1227_DEMO_APP_LED_H_ */
