/*
 * EPWM.c
 *
 *  Created on: 2024年1月26日
 *      Author: alpha
 */

#include "EPWM.h"

////////////////////////////////////////////////////////
//EPWM1 init
/////////////////////
void EPWM1_Init(Uint16 tbprd){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; //ENABLE ePWM1 CLK
    EDIS;

    InitEPwm1Gpio();    //provided by TI

    EPwm1Regs.TBPRD = tbprd;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //SYNC OUTPUT when CTR=0
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;      //sync in
    EPwm1Regs.TBPHS.half.TBPHS = 0;
    EPwm1Regs.TBCTR = 0x0000;   //Clear counter

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // count up
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //Setup shadow register load on ZERO
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set Compare values
    EPwm1Regs.CMPA.half.CMPA = 0;       //Set Compare A value
    EPwm1Regs.CMPB = 0;                 //Set Compare B value

    // Set actions
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
    EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET; // Set PWM1B on Zero
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR; // Clear PWM1B on event B, up count

   // EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    //EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
   // EPwm1Regs.DBFED = 5; // FED = 5 TBCLKs
   // EPwm1Regs.DBRED = 5; // RED = 5 TBCLKs

    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1; // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1st event

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Start all the timers synced
    EDIS;
}

////////////////////////////////////////////////////////
//EPWM2 init
/////////////////////
void EPWM2_Init(Uint16 tbprd){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1; //ENABLE ePWM1 CLK
    EDIS;

    InitEPwm2Gpio();    //provided by TI

    EPwm2Regs.TBPRD = tbprd;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //SYNC OUTPUT when CTR=0
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;      //sync in
    EPwm2Regs.TBPHS.half.TBPHS = 0;
    EPwm2Regs.TBCTR = 0x0000;   //Clear counter

    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // count up
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //Setup shadow register load on ZERO
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set Compare values
    EPwm2Regs.CMPA.half.CMPA = 0;       //Set Compare A value
    EPwm2Regs.CMPB = 0;                 //Set Compare B value

    // Set actions
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
    EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET; // Set PWM1B on Zero
    EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR; // Clear PWM1B on event B, up count

   // EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    //EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
   // EPwm1Regs.DBFED = 5; // FED = 5 TBCLKs
   // EPwm1Regs.DBRED = 5; // RED = 5 TBCLKs

    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // Select INT on Zero event
    EPwm2Regs.ETSEL.bit.INTEN = 1; // Enable INT
    EPwm2Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1st event

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Start all the timers synced
    EDIS;
}

////////////////////////////////////////////////////////
//EPWM3 init
/////////////////////
void EPWM3_Init(Uint16 tbprd){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1; //ENABLE ePWM1 CLK
    EDIS;

    InitEPwm3Gpio();    //provided by TI

    EPwm3Regs.TBPRD = tbprd;
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; //SYNC OUTPUT when CTR=0
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;      //sync in
    EPwm3Regs.TBPHS.half.TBPHS = 0;
    EPwm3Regs.TBCTR = 0x0000;   //Clear counter

    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // count up
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //Setup shadow register load on ZERO
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set Compare values
    EPwm3Regs.CMPA.half.CMPA = 0;       //Set Compare A value
    EPwm3Regs.CMPB = 0;                 //Set Compare B value

    // Set actions
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
    EPwm3Regs.AQCTLB.bit.ZRO = AQ_SET; // Set PWM1B on Zero
    EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR; // Clear PWM1B on event B, up count

   // EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    //EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
   // EPwm1Regs.DBFED = 5; // FED = 5 TBCLKs
   // EPwm1Regs.DBRED = 5; // RED = 5 TBCLKs

    EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // Select INT on Zero event
    EPwm3Regs.ETSEL.bit.INTEN = 1; // Enable INT
    EPwm3Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1st event

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Start all the timers synced
    EDIS;
}

////////////////////////////////////////////////////////
//EPWM5 init
/////////////////////
void EPWM5_Init(Uint16 tbprd){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1; //ENABLE ePWM6 CLK
    EDIS;

    InitEPwm5Gpio();    //provided by TI

    GpioCtrlRegs.GPADIR.bit.GPIO10 = 0; //input
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 0; //input

    EPwm5Regs.TBPRD = tbprd;
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm5Regs.TBPHS.half.TBPHS = 0;
    EPwm5Regs.TBCTR = 0x0000;   //Clear counter

    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // count up
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //Setup shadow register load on ZERO
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set Compare values
    EPwm5Regs.CMPA.half.CMPA = 0;       //Set Compare A value
    EPwm5Regs.CMPB = 0;                 //Set Compare B value

    // Set actions
    EPwm5Regs.AQCTLA.bit.ZRO = AQ_SET; // Set PWM1A on Zero
    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
    EPwm5Regs.AQCTLB.bit.ZRO = AQ_SET; // Set PWM1B on Zero
    EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR; // Clear PWM1B on event B, up count

    //EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
    //EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active Hi complementary
    //EPwm5Regs.DBFED = 5; // FED = 5 TBCLKs
    //EPwm5Regs.DBRED = 5; // RED = 5 TBCLKs

    EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // Select INT on Zero event
    EPwm5Regs.ETSEL.bit.INTEN = 1; // Enable INT
    EPwm5Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1st event

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Start all the timers synced
    EDIS;
}

void EPWM6_Init(Uint16 tbprd){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1; //ENABLE ePWM6 CLK
    EDIS;

    InitEPwm6Gpio();    //provided by TI

    EPwm6Regs.TBPRD = tbprd;
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm6Regs.TBPHS.half.TBPHS = 0;
    EPwm6Regs.TBCTR = 0x0000;   //Clear counter

    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_DOWN; // count up
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //Setup shadow register load on ZERO
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set Compare values
    EPwm6Regs.CMPA.half.CMPA = 0;       //Set Compare A value
    EPwm6Regs.CMPB = 0;                 //Set Compare B value

    // Set actions
    EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm6Regs.AQCTLB.bit.CBD = AQ_SET;

    EPwm6Regs.AQCTLA.bit.ZRO = AQ_CLEAR; // Set PWM1A on Zero
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET; // Clear PWM1A on event A, up count
    EPwm6Regs.AQCTLB.bit.ZRO = AQ_CLEAR; // Set PWM1B on Zero
    EPwm6Regs.AQCTLB.bit.CBU = AQ_SET; // Clear PWM1B on event B, up count



    EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO; // Select INT on Zero event
    EPwm6Regs.ETSEL.bit.INTEN = 1; // Enable INT
    EPwm6Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1st event

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Start all the timers synced
    EDIS;

}

void EPwm1A_SetCompare(Uint16 val){
    EPwm1Regs.CMPA.half.CMPA = val;  //设置占空比
}
void EPwm1B_SetCompare(Uint16 val){
    EPwm1Regs.CMPB = val;  //设置占空比
}

void EPwm2A_SetCompare(Uint16 val){
    EPwm2Regs.CMPA.half.CMPA = val;  //设置占空比
}
void EPwm2B_SetCompare(Uint16 val){
    EPwm2Regs.CMPB = val;  //设置占空比
}
void EPwm3A_SetCompare(Uint16 val){
    EPwm3Regs.CMPA.half.CMPA = val;  //设置占空比
}
void EPwm3B_SetCompare(Uint16 val){
    EPwm3Regs.CMPB = val;  //设置占空比
}

void EPwm5A_SetCompare(Uint16 val)
{
    EPwm5Regs.CMPA.half.CMPA = val;  //设置占空比
}
void EPwm5B_SetCompare(Uint16 val)
{
    EPwm5Regs.CMPB = val;  //设置占空比
}


void EPwm6A_SetCompare(Uint16 val)
{
    EPwm6Regs.CMPA.half.CMPA = val;  //设置占空比
}
void EPwm6B_SetCompare(Uint16 val)
{
    EPwm6Regs.CMPB = val;  //设置占空比
}
