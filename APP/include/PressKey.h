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

#define K1IO GPIO35
#define K2IO GPIO72
#define K3IO GPIO80
#define K4IO GPIO28
#define K5IO GPIO40
#define K6IO GPIO30
#define K7IO GPIO29

#define K1 GpioDataRegs.GPBDAT.bit.K1IO
#define K2 GpioDataRegs.GPCDAT.bit.K2IO
#define K3 GpioDataRegs.GPCDAT.bit.K3IO
#define K4 GpioDataRegs.GPADAT.bit.K4IO
#define K5 GpioDataRegs.GPBDAT.bit.K5IO
#define K6 GpioDataRegs.GPADAT.bit.K6IO
#define K7 GpioDataRegs.GPADAT.bit.K7IO


void PressKey_Init(void);
U8 Scan_PressKey(void);

#endif /* APP_INCLUDE_PRESSKEY_H_ */
