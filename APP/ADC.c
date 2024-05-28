/*
 * ADC.c
 *
 *  Created on: 2024Äê1ÔÂ25ÈÕ
 *      Author: alpha
 */

#include "ADC.h"

void ADC_Init(void){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // Enable ADC clock
    SysCtrlRegs.HISPCP.all = 3; // ADC: 25MHZ

    InitAdc();  // provide by TI

    AdcRegs.ADCTRL1.bit.ACQ_PS = 0x0F; // sequential sampling
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 1; // 25Mhz no frequency division
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1; // 1:  Cascaded sequencer mode    0:double sort mode
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;// 1:Synchronous sampling     0:sequential sampling
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // A0 as the sample channel  save data to results0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // A1 as the sample channel  save data to results1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2; // A0 as the sample channel  save data to results0
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3; // A1 as the sample channel  save data to results1
    //AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x03; // B0 as the sample channel save data to results5
    //AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x02; // A2 as the sample channel
    AdcRegs.ADCTRL1.bit.CONT_RUN = 1; // continuous sampling
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x4;  // max sample channel, CONV05 have used , so it should be 5. but actully, only two channel working. Cascaded mode max 15, double sort max 7;
    //AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = 0xf;  // max sample channel, case only A0 is used , so 0x0;
    AdcRegs.ADCTRL2.all = 0x2000;   //software trigger

    EDIS;
}

Uint16 Read_ADCValueResult0(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag
    return AdcRegs.ADCRESULT0>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult1(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT1>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult2(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT2>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult3(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT3>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult4(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT4>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult5(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT5>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult6(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT6>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult7(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT7>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult8(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT8>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult9(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT9>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult10(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT10>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult11(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT11>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult12(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT12>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult13(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT13>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult14(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT14>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

Uint16 Read_ADCValueResult15(void){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    return AdcRegs.ADCRESULT15>>4;   //return translate results, high 12bits valid, low 4bits invalid
}

void Read_ADCValueResultAll(Uint16 *result){
    while(AdcRegs.ADCST.bit.INT_SEQ1 == 0);     // wait the end of the translation
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     //clear interrupt flag

    *result++ = AdcRegs.ADCRESULT0>>4;
    *result++ = AdcRegs.ADCRESULT1>>4;
    *result++ = AdcRegs.ADCRESULT2>>4;
    *result++ = AdcRegs.ADCRESULT3>>4;
    *result++ = AdcRegs.ADCRESULT4>>4;
    *result++ = AdcRegs.ADCRESULT5>>4;
    *result++ = AdcRegs.ADCRESULT6>>4;
    *result++ = AdcRegs.ADCRESULT7>>4;
    *result++ = AdcRegs.ADCRESULT8>>4;
    *result++ = AdcRegs.ADCRESULT9>>4;
    *result++ = AdcRegs.ADCRESULT10>>4;
    *result++ = AdcRegs.ADCRESULT11>>4;
    *result++ = AdcRegs.ADCRESULT12>>4;
    *result++ = AdcRegs.ADCRESULT13>>4;
    *result++ = AdcRegs.ADCRESULT14>>4;
    *result = AdcRegs.ADCRESULT15>>4;

}

