/*
 * LED.c
 *
 *  Created on: 2024Äê3ÔÂ25ÈÕ
 *      Author: alpha
 */

#include "LED.h"

void LED_Init(void){
    EALLOW;     //all operations on registers need to be Add


    GpioCtrlRegs.GPCMUX1.bit.LED3 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPCDIR.bit.LED3 = GPIODIR_OUTPUT;   //OUTPUT
    GpioCtrlRegs.GPCPUD.bit.LED3 = GPIOPUD_PULLUP;   //PULLUP

    GpioCtrlRegs.GPCMUX1.bit.LED4 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPCDIR.bit.LED4 = GPIODIR_OUTPUT;
    GpioCtrlRegs.GPCPUD.bit.LED4 = GPIOPUD_PULLUP;

    LED3_LOW;
    LED4_LOW;
    EDIS;
}



