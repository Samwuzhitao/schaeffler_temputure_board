/*
 * Version.c
 *
 *  Created on: 2016��4��26��
 *      Author: sam.wu
 */
#include "Version.h"

uint8 SN_ID[7]={
			bin2bcd(YEAR_H),
			bin2bcd(YEAR_L),
			bin2bcd(MONTH),
			bin2bcd(DAY),
			bin2bcd(HOUR),
			bin2bcd(MINUTE),
			bin2bcd(VERSION),
};
