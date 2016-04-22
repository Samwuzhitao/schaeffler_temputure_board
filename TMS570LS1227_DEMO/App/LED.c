/*
 * LED.c
 *
 *  Created on: 2016Äê4ÔÂ21ÈÕ
 *      Author: sam.wu
 */
#include "sys_common.h"
#include "LED.h"

uint8 LEdStatus = 0x0F;
/******************************************************************************
  Function:
  	  LEDInit
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void LEDInit( void )
{
	LEdStatus = 0x00;
	LEDFlash();
}

/******************************************************************************
  Function:
  	  LEDFlash
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void LEDFlash( void )
{
	uint8 Status[4] = { 1, 1, 1, 1 };
	uint8 i;

	for(i=0;i<4;i++)
	{
		Status[i] = (LEdStatus >> 1) & 0x01;
	}

	canIoSetPort(canREG1, Status[0], Status[1]);
	canIoSetPort(canREG2, Status[2], Status[3]);
}

/******************************************************************************
  Function:
  	  LEDOn
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void LEDOn( LEDTypedef Led )
{
	LEdStatus &= (uint8)(~(1 << Led));
	LEDFlash();
}

/******************************************************************************
  Function:
  	  LEDOff
  Description:
  Input:None
  Output:
  Return:
  Others:None
******************************************************************************/
void LEDOff( LEDTypedef Led )
{
	LEdStatus |= (uint8)(1 << Led);
	LEDFlash();
}

