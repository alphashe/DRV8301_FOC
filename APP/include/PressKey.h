/*
 * PressKey.h
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_PRESSKEY_H_
#define APP_INCLUDE_PRESSKEY_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#define K1 GPIO35
#define K2 GPIO72
#define K3 GPIO80
#define K4 GPIO28
#define K5 GPIO40
#define K6 GPIO30
#define K7 GPIO29

#define K1_I GpioDataRegs.GPBDAT.bit.K1
#define K2_I GpioDataRegs.GPCDAT.bit.K2
#define K3_I GpioDataRegs.GPCDAT.bit.K3
#define K4_I GpioDataRegs.GPADAT.bit.K4
#define K5_I GpioDataRegs.GPBDAT.bit.K5
#define K6_I GpioDataRegs.GPADAT.bit.K6
#define K7_I GpioDataRegs.GPADAT.bit.K7


void PressKey_Init(void);
U8 Scan_PressKey(void);

#endif /* APP_INCLUDE_PRESSKEY_H_ */
