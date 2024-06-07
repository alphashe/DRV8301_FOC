/*
 * PressKey.c
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#include <PressKey.h>

void Key_Init(void){
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    //enable GPIO input clock

    GpioCtrlRegs.GPBMUX1.bit.K1 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPBDIR.bit.K1 = GPIODIR_INPUT;
    GpioCtrlRegs.GPBPUD.bit.K1 = GPIOPUD_PULLUP;

    GpioCtrlRegs.GPCMUX1.bit.K2 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPCDIR.bit.K2 = GPIODIR_INPUT;
    GpioCtrlRegs.GPCPUD.bit.K2 = GPIOPUD_PULLUP;

    GpioCtrlRegs.GPCMUX2.bit.K3 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPCDIR.bit.K3 = GPIODIR_INPUT;
    GpioCtrlRegs.GPCPUD.bit.K3 = GPIOPUD_PULLUP;

    GpioCtrlRegs.GPAMUX2.bit.K4 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.K4 = GPIODIR_INPUT;
    GpioCtrlRegs.GPAPUD.bit.K4 = GPIOPUD_PULLUP;

    GpioCtrlRegs.GPBMUX1.bit.K5 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPBDIR.bit.K5 = GPIODIR_INPUT;
    GpioCtrlRegs.GPBPUD.bit.K5 = GPIOPUD_PULLUP;

    GpioCtrlRegs.GPAMUX2.bit.K6 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.K6 = GPIODIR_INPUT;
    GpioCtrlRegs.GPAPUD.bit.K6 = GPIOPUD_PULLUP;

    GpioCtrlRegs.GPAMUX2.bit.K7 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.K7 = GPIODIR_INPUT;
    GpioCtrlRegs.GPAPUD.bit.K7 = GPIOPUD_PULLUP;

    EDIS;

}

U8 Scan_PressKey(void){
    U8 prek=0;
    if(K1_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K1_I==0)
            prek=1;
        while(K1_I==0);
    }
    if(K2_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K2_I==0)
            prek=2;
        while(K2_I==0);
    }
    if(K3_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K3_I==0)
            prek=3;
        while(K3_I==0);
    }
    if(K4_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K4_I==0)
            prek=4;
        while(K4_I==0);
    }
    if(K5_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K5_I==0)
            prek=5;
        while(K5_I==0);
    }
    if(K6_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K6_I==0)
            prek=6;
        while(K6_I==0);
    }
    if(K7_I==0){
        DELAY_US(10000);    //delay 10mS in order to eliminate jitter
        if(K7_I==0)
            prek=7;
        while(K7_I==0);
    }
    return prek;
}
