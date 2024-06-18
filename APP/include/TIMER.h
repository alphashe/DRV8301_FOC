/*
 * TIME.h
 *
 *  Created on: 2024Äê1ÔÂ26ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_TIMER_H_
#define APP_INCLUDE_TIMER_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "LED.h"
#include "Global.h"


void TIM0_Init(Uint32 divide, Uint32 count);

interrupt void TIM0_Int(void);

#endif /* APP_INCLUDE_TIMER_H_ */
