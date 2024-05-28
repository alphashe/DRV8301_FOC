/*
 * INT.c
 *
 *  Created on: 2024Äê1ÔÂ25ÈÕ
 *      Author: alpha
 */
#include "INT.h"

void EXTI1_Init(void){
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;
    EDIS;

    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = GPIOMUX_GPIO;    //normal IO
    GpioCtrlRegs.GPADIR.bit.GPIO12 = GPIODIR_INPUT;     //input
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = GPIOPUD_PULLUP;    //pullup
    GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;   //XINT1 clk synic with SYSCLKOUT

    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = GPIOMUX_GPIO;
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = GPIODIR_INPUT;     //OUTPUT
    GpioCtrlRegs.GPBPUD.bit.GPIO48 = GPIOPUD_PULLUP;

    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 12;  //XINT1 select 12;

    PieVectTable.XINT1 = &EXTI1_KEY;

    EDIS;

    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;

    XIntruptRegs.XINT1CR.bit.POLARITY = 0;  //falling edge triggered interrupt
    XIntruptRegs.XINT1CR.bit.ENABLE= 1; // enable XINT1
    IER |= M_INT1;  //enable CPU interrupt1

    EINT;   //turn on global interrupt
    ERTM;
}

interrupt void EXTI1_KEY(void){
    Uint32 i;
    for(i=0; i<70000; i++);
    while(!(GpioDataRegs.GPADAT.bit.GPIO12));
    //LED2_TOGGLE;
    PieCtrlRegs.PIEACK.bit.ACK1=1;
}
