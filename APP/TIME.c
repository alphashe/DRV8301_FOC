/*
 * TIME.c
 *
 *  Created on: 2024Äê1ÔÂ26ÈÕ
 *      Author: alpha
 *
 *
 *
 *
 */

#include "TIME.h"

void TIM0_Init(Uint32 divide, Uint32 count){
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // enable CPU Timer0
    //PieVectTable.TINT0 = &TIM0_Int;   //no interrupt required
    EDIS;

    CpuTimer0.RegsAddr = &CpuTimer0Regs; //
    CpuTimer0.InterruptCount = 0;   // Reset interrupt counter:
    CpuTimer0.CPUFreqInMHz = (float)mainCLK;
    CpuTimer0.PeriodInUSec = (float)count;

    CpuTimer0Regs.TCR.bit.TSS = TSS_STOP; //makr sure timer0 in stop state

    CpuTimer0Regs.TPR.all = divide && 0xFF;      //all TPR 32bit 4.29 billion
    CpuTimer0Regs.TPRH.all = divide >> 8;
    CpuTimer0Regs.PRD.all = count;
    CpuTimer0Regs.TCR.bit.TRB = TRB_RELOAD; //ereload TIM&PSC(in TPR)

    // Initialize timer control register:
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.FREE = 0;     // FREE:SOFT    0,0 :stop timer after timer decrements by 1    0,1 :stop timer when it become 0   1,x:free run
    CpuTimer0Regs.TCR.bit.TIE = 0;      // 0 = Disable/ 1 = Enable Timer Interrupt

    CpuTimer0Regs.TCR.bit.TSS = TSS_START; //enable Timer
    //IER |= M_INT1;
    //PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    //EINT;
    //ERTM;//enable global interrupt
}

interrupt void TIM0_Int(void){
    EALLOW;
    //LED3_TOGGLE;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
    EDIS;
}
