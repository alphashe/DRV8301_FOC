/*
 * KEY.c
 *
 *  Created on: 2024Äê3ÔÂ26ÈÕ
 *      Author: alpha
 */
#include "KEY.h"
volatile Uint32 keytime=0;   //every time to read
int32 newkeytime[4]={0,0,0,0};    //the new station of key
int32 initkeytime[4]={0,0,0,0};
int32 keystat[2][4]={0,0,0,0,0,0,0,0};  //1:press down, 0:didn't press
volatile Uint16 keyreadover=0;
void K2_Init(void){

    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    //enable GPIO input clock

    GpioCtrlRegs.K2MUX.bit.K2IO = GPIOMUX_GPIO;
    GpioCtrlRegs.K2DIR.bit.K2IO = GPIODIR_OUTPUT;
    GpioCtrlRegs.K2PUD.bit.K2IO = GPIOPUD_PULLUP;
    K2_LOW;
    GpioCtrlRegs.K2QSEL.bit.K2IO = 0;   //XINT1 clk synic with SYSCLKOUT

    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = IONUM;  //XINT2 select IONUM;
    PieVectTable.XINT2 = &EXTI2_KEY2;
    EDIS;

    PieCtrlRegs.PIEIER1.bit.INTx5 = 1; //search in Peripheral interrupt Vector table (INT1,INTx5) ->XINT2
    XIntruptRegs.XINT2CR.bit.POLARITY = POLARITY_RISING;  //falling edge triggered interrupt
    XIntruptRegs.XINT2CR.bit.ENABLE= 1; // enable XINT1

    IER |= M_INT1;  //enable CPU interrupt1     IER:CPU interrupt enable register
    EINT;   //turn on global interrupt
    ERTM;

}


void Init_Key_Time(void){
    Uint16 i=0;
    Uint16 j=0;
    for(j=0; j<4; j++){
        initkeytime[j]=0;
    }
    for(i=0; i<3; i++)
        initkeytime[1] +=measurekey2();
    initkeytime[1] = initkeytime[1] / 3;

}
//when pressed, the time becomes longer
Uint16 Scan_Key(void){
    Uint16 i=0;
    Uint16 scankey=0;
    newkeytime[1]=0;
    keystat[1][1]=0;
    for(i=0; i<10; i++){
        newkeytime[1] =measurekey2();

        DELAY_US(1000);
        if((initkeytime[1]-newkeytime[1])>KEYHYS)
            keystat[1][1]+=1;
        else
            keystat[1][1] +=10;
    }
    if( keystat[1][1]==3 & keystat[0][1]==0){
        scankey = 1<<1;
        keystat[0][1]=1;
    }
    OLED_ShowInt(0, 0, newkeytime[1], 1);
    OLED_Refresh();
    return scankey;
}

Uint32 measurekey2(void){
    EALLOW;
    GpioCtrlRegs.K2DIR.bit.K2IO = GPIODIR_OUTPUT;
    GpioCtrlRegs.K2PUD.bit.K2IO = GPIOPUD_PULLUP;
    K2_LOW;
    DELAY_US(2);
    CpuTimer0Regs.TCR.bit.TRB = TRB_RELOAD;
   // DELAY_US(10);
    GpioCtrlRegs.K2DIR.bit.K2IO = GPIODIR_INPUT;
    GpioCtrlRegs.K2PUD.bit.K2IO = GPIOPUD_FLOAT;
    EDIS;
    //keytime=0;
    while(!keyreadover);
    keyreadover=0;
    return keytime;
}

interrupt void EXTI2_KEY2(void){

    keytime = CpuTimer0Regs.TIM.all;
    keyreadover=1;
    EALLOW;
    PieCtrlRegs.PIEACK.bit.ACK1=1;
    EDIS;
}

