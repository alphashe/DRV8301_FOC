/*
 * KEY.h
 *
 *  Created on: 2024Äê3ÔÂ26ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_TOUCHKEY_H_
#define APP_INCLUDE_TOUCHKEY_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Global.h"
#include "OLED.h"

#define K2MUX GPAMUX2
#define K2DIR GPADIR
#define K2PUD GPAPUD
#define K2QSEL GPAQSEL2
#define K2IO GPIO29
#define IONUM 29

#define K2_HIGH    GpioDataRegs.GPASET.bit.K2IO=1
#define K2_LOW     GpioDataRegs.GPACLEAR.bit.K2IO=1
#define KEYHYS 3000

void K2_Init(void);
interrupt void EXTI2_KEY2(void);
Uint32 measurekey2(void);
void Init_Key_Time(void);
Uint16 Scan_Key(void);
#endif /* APP_INCLUDE_TOUCHKEY_H_ */
