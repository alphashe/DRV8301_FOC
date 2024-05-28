/*
 * LED.h
 *
 *  Created on: 2024Äê3ÔÂ25ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_LED_H_
#define APP_INCLUDE_LED_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Global.h"

#define LED3 GPIO75
#define LED4 GPIO74

#define LED3_HIGH    GpioDataRegs.GPCSET.bit.LED3=1
#define LED3_LOW     GpioDataRegs.GPCCLEAR.bit.LED3=1
#define LED3_TOGGLE GpioDataRegs.GPCTOGGLE.bit.LED3=1

#define LED4_HIGH    GpioDataRegs.GPCSET.bit.LED4=1
#define LED4_LOW     GpioDataRegs.GPCCLEAR.bit.LED4=1
#define LED4_TOGGLE GpioDataRegs.GPCTOGGLE.bit.LED4=1




void LED_Init(void);







#endif /* APP_INCLUDE_LED_H_ */
