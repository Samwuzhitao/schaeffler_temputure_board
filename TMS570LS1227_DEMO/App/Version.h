/*
 * Version.h
 *
 *  Created on: 2016Äê4ÔÂ26ÈÕ
 *      Author: sam.wu
 */

#ifndef TMS570LS1227_DEMO_APP_VERSION_H_
#define TMS570LS1227_DEMO_APP_VERSION_H_
#include "stdio.h"
#include "sys_common.h"


#define YEAR_H   (20) // year    = 2016
#define YEAR_L   (16)
#define MONTH    (04) // month   = 04
#define DAY      (26) // day     = 26
#define HOUR     (10) // time    = 10:42
#define MINUTE   (42)
#define VERSION  (10) // Version = 1.0

#define bin2bcd(num) ((num/10)<<4 | (num%10))

#define SetBCD(a,b)  ((uint8)((( a & 0x0f ) << 4) | ( b & 0x0f )))


#endif /* TMS570LS1227_DEMO_APP_VERSION_H_ */
