/*
 * DRV8301.c
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#include <DRV8301.h>
//HAL_Obj hal;


//struct struct_DRV8301 drv8301;
//struct struct_DRV8301* pdrv8301 = &drv8301;

HAL_handle Hal_Init(HAL_Obj hal) {
    static HAL_handle   obj;
    static DRV8301_Obj  drv8301obj;
    static EPWM_Obj     epwmobj;
    static SENCE_Obj    senceobj;
    static CONTRL_Obj   contrlobj;
    obj = &hal;
    obj->drv8301handle  = &drv8301obj;
    obj->epwmhandle     = &epwmobj;
    obj->sencehandle    = &senceobj;
    obj->contrlhandle   = &contrlobj;

    return &obj;
}



void SetParam(HAL_handle handle) {
    HAL_handle obj = handle;

    //DRV8301 param init
    // set Register address
    // and all param set to default value 
    obj->drv8301handle->Reg0 = 0 << 11;
    obj->drv8301handle->Reg1 = 1 << 11;
    obj->drv8301handle->Reg2 = 2 << 11;
    obj->drv8301handle->Reg3 = 3 << 11;
    //R2
    obj->drv8301handle->gate_current = gate_current_1p7A;
    obj->drv8301handle->pwm_mode = pwm_mode_sixpwm;
    obj->drv8301handle->ocp_mode = ocp_mode_current_limit;
    obj->drv8301handle->oc_adj_set = oc_adj_set_0p403;
    //R3
    obj->drv8301handle->octw_mode = octw_mode_otoc;
    obj->drv8301handle->gain = gain_10;
    obj->drv8301handle->dc_cal_ch1 = dc_cal_connect;
    obj->drv8301handle->dc_cal_ch2 = dc_cal_connect;
    obj->drv8301handle->oc_toff = oc_toff_CBC;

    //EPWM param init
    obj->epwmhandle->Period = 3000;      //up&dowm mode, 25kHz -> period=150 000/25*2 = 3 000
    obj->epwmhandle->PWMA = 0;          //lower mos open
    obj->epwmhandle->PWMB = 0;
    obj->epwmhandle->PWMC = 0;
}

void DRV8301_Init(HAL_handle handle){
    HAL_handle obj = handle;

    SPI_Init();
    ADC_Init();
    EPWM1_Init(obj->epwmhandle->Period);   //150MHz 150 *50 =20khz
    EPWM2_Init(obj->epwmhandle->Period);
    EPWM3_Init(obj->epwmhandle->Period);
    DRV8301GPIO_Init();

    DRV8301_Enable(obj);
    DELAY_US(10000);    //wait 5~10mS
    DRV8301_Config(obj);
    DRV8301_Read(obj);
    DRV8301_Display(obj);
}

void DRV8301_Config(HAL_handle handle) {
    HAL_handle obj = handle;
    Uint16 senddata = 0;//Bit15: 1:Read  0:Write
    //DRV8301 will recive data in next cycle
    //R2
    senddata |= obj->drv8301handle->Reg2;
    senddata |= obj->drv8301handle->gate_current;
    senddata |= obj->drv8301handle->pwm_mode << 2;
    senddata |= obj->drv8301handle->ocp_mode << 4;
    senddata |= obj->drv8301handle->oc_adj_set << 6;
    SPIA_SendReciveData(senddata);
    //R3
    senddata = 0;
    senddata |= obj->drv8301handle->Reg3;
    senddata |= obj->drv8301handle->octw_mode;
    senddata |= obj->drv8301handle->gain << 2;
    senddata |= obj->drv8301handle->dc_cal_ch1 << 4;
    senddata |= obj->drv8301handle->dc_cal_ch2 << 5;
    senddata |= obj->drv8301handle->oc_toff << 6;
    SPIA_SendReciveData(senddata);
    SPIA_SendReciveData(senddata);
}

void DRV8301_Display(HAL_handle handle){
    HAL_handle obj = handle;
    OLED_Clear();
    OLED_ShowString(43, 0, "DRV8301", 1);
    OLED_ShowString(0, 1 * 8, "Device ID:", 1);
    OLED_ShowString(0, 2 * 8, "EN_GATE:", 1); //8*6
    OLED_ShowString(68, 2 * 8, "DC_CAL:", 1); //7*6

    OLED_ShowString(0, 3 * 8, "PWMA:", 1);   //6*6
    OLED_ShowString(64, 3 * 8, "PWMB:", 1);   //6*6
    OLED_ShowString(0, 4 * 8, "PWMC:", 1);   //6*6
    OLED_ShowString(0, 5 * 8, "SO1:", 1);     //4*6
    OLED_ShowString(64, 5 * 8, "SO2:", 1);     //4*6

    if(obj->drv8301handle->device_id == 0 || obj->drv8301handle->device_id == 0xf)
        OLED_ShowString(0, 8, "Disconnect!", 1);
    else
        OLED_ShowInt(60, 8, obj->drv8301handle->device_id, 1);

    if(obj->drv8301handle->en_gate==1)
        OLED_ShowString(8*6, 2*8, "On ", 1);
    else
        OLED_ShowString(8*6, 2*8, "off", 1);

    if(obj->drv8301handle->dc_cal==1)
        OLED_ShowString(7*6+68, 2*8, "On ", 1);
    else
        OLED_ShowString(7*6+68, 2*8, "off", 1);

    OLED_Refresh();
}
void DRV8301_Read(HAL_handle handle){
    HAL_handle obj = handle;
    Uint16 senddata = 0x8000;   //Bit15: 1:Read  0:Write 
    Uint16 recivdata[4] = { 0 };

    //DRV8301 will send data in next cycle
    //send R0 address
    senddata |= obj->drv8301handle->Reg0;
    SPIA_SendReciveData(senddata);

    //send R1 address, recive R0 data to recivdata[0]
    senddata = 0x8000;
    senddata |= obj->drv8301handle->Reg1;
    recivdata[0] = SPIA_SendReciveData(senddata); 

    //send R2 address, recive R1 data to recivdata[1]
    senddata = 0x8000;
    senddata |= obj->drv8301handle->Reg2;
    recivdata[1] = SPIA_SendReciveData(senddata); 

    //send R3 address, recive R2 data to recivdata[2]
    senddata = 0x8000;
    senddata |= obj->drv8301handle->Reg3;
    recivdata[2] = SPIA_SendReciveData(senddata);

    //send R3 address, recive R3 data to recivdata[3]
    recivdata[3] = SPIA_SendReciveData(senddata); 


    //Register 0
    obj->drv8301handle->fetlc_oc      = (recivdata[0] & 0b1);   //D0
    obj->drv8301handle->fethc_oc      = (recivdata[0] & 0b10) >> 1;   //D1
    obj->drv8301handle->fetlb_oc      = (recivdata[0] & 0b100) >> 2;   //D2
    obj->drv8301handle->fethb_oc      = (recivdata[0] & 0b1000) >> 3;   //D3
    obj->drv8301handle->fetlc_oc      = (recivdata[0] & 0b10000) >> 4;   //D4
    obj->drv8301handle->fethc_oc      = (recivdata[0] & 0b100000) >> 5;   //D5
    obj->drv8301handle->otw           = (recivdata[0] & 0b1000000) >> 6;   //D6
    obj->drv8301handle->otsd          = (recivdata[0] & 0b10000000) >> 7;   //D7
    obj->drv8301handle->pvdd_uv       = (recivdata[0] & 0b100000000) >> 8;   //D8
    obj->drv8301handle->gvdd_uv       = (recivdata[0] & 0b1000000000) >> 9;   //D9
    obj->drv8301handle->fault         = (recivdata[0] & 0b10000000000) >> 10;   //D10

    //Register 1
    obj->drv8301handle->device_id     = (recivdata[1] & 0b1111);   //D0 D1 D2 D3
    obj->drv8301handle->gvdd_ov       = (recivdata[1] & 0b10000000) >> 7;  //D7

    //Register 2
    obj->drv8301handle->gate_current  = (recivdata[2] & 0b11);  //D0 D1
    obj->drv8301handle->pwm_mode      = (recivdata[2] & 0b1000) >> 3;   //D3
    obj->drv8301handle->ocp_mode      = (recivdata[2] & 0b110000) >> 4;   //D4 D5
    obj->drv8301handle->oc_adj_set    = (recivdata[2] & 0b11111000000) >> 6;    //D6 D7 D8 D9 D10

    //Register 3
    obj->drv8301handle->octw_mode     = (recivdata[3] & 0b11);  //D0 D1
    obj->drv8301handle->gain          = (recivdata[3] & 0b1100) >> 2;   //D2 D3
    obj->drv8301handle->dc_cal_ch1    = (recivdata[3] & 0b10000) >> 4;   //D4
    obj->drv8301handle->dc_cal_ch2    = (recivdata[3] & 0b100000) >> 5;   //D5
    obj->drv8301handle->oc_toff       = (recivdata[3] & 0b1000000) >> 6;    //D6

}
void DRV8301GPIO_Init(void){
    EALLOW;     //all operations on registers need to be Add

    //init OUTPUT
    GpioCtrlRegs.GPAMUX1.bit.EN_GATE = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.EN_GATE = GPIODIR_OUTPUT;   //OUTPUT
    GpioCtrlRegs.GPAPUD.bit.EN_GATE = GPIOPUD_PULLUP;   //PULLUP
    GpioCtrlRegs.GPAMUX1.bit.DC_CAL = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.DC_CAL = GPIODIR_OUTPUT;   //OUTPUT
    GpioCtrlRegs.GPAPUD.bit.DC_CAL = GPIOPUD_PULLUP;   //PULLUP

    //init INPUT
    GpioCtrlRegs.GPAMUX2.bit.PWRGD = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.PWRGD = GPIODIR_INPUT;   //input
    GpioCtrlRegs.GPAPUD.bit.PWRGD = GPIOPUD_FLOAT;   //float
    GpioCtrlRegs.GPAMUX1.bit.NOCTW = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.NOCTW = GPIODIR_INPUT;   //input
    GpioCtrlRegs.GPAPUD.bit.NOCTW = GPIOPUD_FLOAT;   //float
    GpioCtrlRegs.GPAMUX1.bit.NFAULT = GPIOMUX_GPIO;
    GpioCtrlRegs.GPADIR.bit.NFAULT = GPIODIR_INPUT;   //input
    GpioCtrlRegs.GPAPUD.bit.NFAULT = GPIOPUD_FLOAT;   //float

    EDIS;

    EN_GATE_L;
    DC_CAL_H;

}
void DRV8301_PWMSet(HAL_handle handle){
    HAL_handle obj = handle;
    //limit max Duty max 95%.
    if (obj->epwmhandle->PWMA > (obj->epwmhandle->Period * 0.95))
        obj->epwmhandle->PWMA = obj->epwmhandle->Period * 0.95;
    if (obj->epwmhandle->PWMB > (obj->epwmhandle->Period * 0.95))
        obj->epwmhandle->PWMB = obj->epwmhandle->Period * 0.95;
    if (obj->epwmhandle->PWMC > (obj->epwmhandle->Period * 0.95))
        obj->epwmhandle->PWMC = obj->epwmhandle->Period * 0.95;

    PWM_HA(obj->epwmhandle->PWMA);
    PWM_LA(obj->epwmhandle->PWMA);
    PWM_HB(obj->epwmhandle->PWMB);
    PWM_LB(obj->epwmhandle->PWMB);
    PWM_HC(obj->epwmhandle->PWMC);
    PWM_LC(obj->epwmhandle->PWMC);

//--------------------------------------------------------------------------
    //limit max Duty from 5% to 95%.
    //if duty =0, all pwm shut down
    //PWMA
    /*
    if(temp.PWMA>(75*95))      //7500*0.95,  7500*0.05 avoid to calculate float
        temp.PWMA = 75*95;
    else if(temp.PWMA ==0)
        temp.PWMA = 0;
    else if(temp.PWMA <(75*5))
        temp.PWMA = 75*5;

    if(temp.PWMA ==0){
        PWM_HA(temp.PWMA);
        PWM_LA(7500-temp.PWMA);
    }
    else{
        PWM_HA(temp.PWMA);
        PWM_LA(temp.PWMA);
    }

    ///PWMB
    if(temp.PWMB>(75*95))
        temp.PWMB = 75*95;
    else if(temp.PWMB ==0)
        temp.PWMB = 0;
    else if(temp.PWMB <(75*5))
        temp.PWMB = 75*5;
    if(temp.PWMB ==0){
        PWM_HB(temp.PWMB);
        PWM_LB(7500-temp.PWMB);
    }
    else{
        PWM_HB(temp.PWMB);
        PWM_LB(temp.PWMB);
    }

    //PWMC
    if(temp.PWMC>(75*95))
        temp.PWMC = 75*95;
    else if(temp.PWMC ==0)
        temp.PWMC = 0;
    else if(temp.PWMC <(75*5))
        temp.PWMC = 75*5;
    if(temp.PWMC ==0){
        PWM_HC(temp.PWMC);
        PWM_LC(7500-temp.PWMC);
    }
    else{
        PWM_HC(temp.PWMC);
        PWM_LC(temp.PWMC);
    }
    */

}
void DRV8301_PWMOff(HAL_handle handle){
    PWM_HA(0);
    PWM_LA(7500);
    PWM_HB(0);
    PWM_LB(7500);
    PWM_HC(0);
    PWM_LC(7500);

    //temp->PWMA = temp->PWMB = temp->PWMC =0;
}
void DRV8301_Enable(HAL_handle handle){
    HAL_handle obj = handle;
    EN_GATE_H;
    obj->drv8301handle->en_gate = 1;
}
void DRV8301_Disable(HAL_handle handle){
    HAL_handle obj = handle;
    EN_GATE_L;
    obj->drv8301handle->en_gate = 0;
}
void DRV8301_ENSense(HAL_handle handle){
    HAL_handle obj = handle;
    DC_CAL_L;
    obj->drv8301handle->dc_cal = 0;
}
void DRV8301_DISSense(HAL_handle handle){
    HAL_handle obj = handle;
    DC_CAL_H;
    obj->drv8301handle->dc_cal = 1;
}
//get Ia,Ib,Ic, and hella,b,c. translate hell to code "abc"
void DRV8301_SenseGet(HAL_handle handle){
    HAL_handle obj = handle;
    Uint16 so1=0, so2=0;
    Uint16 a=0, b=0, c=0;
    so1 = Read_ADCValueResult0();
    so2 = Read_ADCValueResult1();
    c = Read_ADCValueResult2();
    b = Read_ADCValueResult3();
    a = Read_ADCValueResult4();

    obj->sencehandle->hell = 0;
    obj->sencehandle->Ib = (so1/4096.0 *3) / 0.08;
    obj->sencehandle->Ic = (so2/4096.0 *3) / 0.08;
    obj->sencehandle->Ia = -obj->sencehandle->Ib - obj->sencehandle->Ic;
    obj->sencehandle->hella = a/4096.0 * 3;
    obj->sencehandle->hellb = b/4096.0 * 3;
    obj->sencehandle->hellc = c/4096.0 * 3;
    if(obj->sencehandle->hella>1)
        obj->sencehandle->hell = obj->sencehandle->hell | 0b100;
    if(obj->sencehandle->hellb>1)
        obj->sencehandle->hell = obj->sencehandle->hell | 0b010;
    if(obj->sencehandle->hellc>1)
        obj->sencehandle->hell = obj->sencehandle->hell | 0b001;
}

//will return 1, if something wrong. return 0, when normal operation
U8 DRV8301_Check(HAL_handle handle){
    //temp->pwrgd = PWRGD_I;
    //temp->nfault = NFAULT_I;
    //temp->noctw = NOCTW_I;

    //if(temp->pwrgd==0 || temp->nfault==0 || temp->noctw==0)
    //    return 1;
    return 0;
}

void DRV8301_SixStep(HAL_handle handle){
    //DRV8301_SenseGet(temp);
    ////hell:abc
    //if(temp->hell == 0b001){
    //    temp->PWMA = 500;
    //    temp->PWMB = 500;
    //    temp->PWMC = 3000;
    //}
    //if(temp->hell == 0b010){
    //    temp->PWMA = 500;
    //    temp->PWMB = 3000;
    //    temp->PWMC = 500;
    //}
    //if(temp->hell == 0b011){
    //    temp->PWMA = 500;
    //    temp->PWMB = 3000;
    //    temp->PWMC = 500;
    //}
    //if(temp->hell == 0b100){
    //    temp->PWMA = 500;
    //    temp->PWMB = 500;
    //    temp->PWMC = 3000;
    //}
    //if(temp->hell == 0b101){
    //    temp->PWMA = 500;
    //    temp->PWMB = 500;
    //    temp->PWMC = 3000;
    //}
    //if(temp->hell == 0b110){
    //    temp->PWMA = 3000;
    //    temp->PWMB = 500;
    //    temp->PWMC = 500;
    //}

    //DRV8301_PWMSet(*temp);
}

void DRV8301_SPWM(HAL_handle handle){
	//theta
    //struct Contrl svpwm;
    //svpwm.Ialpha = cos(theta) * 0.2;
    //svpwm.Ibeta = sin(theta) * 0.2;
    //temp->PWMA = (svpwm.Ialpha) * 3750 + 3750;
    //temp->PWMB = (-0.5*svpwm.Ialpha + 0.866*svpwm.Ibeta) * 3750 + 3750;
    //temp->PWMC = (-0.5*svpwm.Ialpha - 0.866*svpwm.Ibeta) * 3750 + 3750;

    //DRV8301_PWMSet(*temp);
}

void DRV8301_SVPWM(HAL_handle handle){
    HAL_handle obj = handle;
    float U1, U2, U3;
    float T[7] = {0};
    //ctr.Ialpha = cos(theta) * ctr.A;
    //ctr.Ibeta = sin(theta) * ctr.A;
    //U1 = 0.866 * (ctr.Ibeta);                                 //0~1
    //U2 = 0.866 * (0.5 * ctr.Ibeta - 0.866 * ctr.Ialpha);      //0~1
    //U3 = 0.866 * (0.5 * ctr.Ibeta + 0.866 * ctr.Ialpha);      //0~1  max value is sqr(3)/2

    obj->contrlhandle->Ialpha = cos(obj->contrlhandle->theta) * obj->contrlhandle->A;
    obj->contrlhandle->Ibeta = sin(obj->contrlhandle->theta) * obj->contrlhandle->A;
    U1 = 0.866 * (obj->contrlhandle->Ibeta);
    U2 = 0.866 * (0.5 * obj->contrlhandle->Ibeta - 0.866 * obj->contrlhandle->Ialpha);
    U2 = 0.866 * (0.5 * obj->contrlhandle->Ibeta + 0.866 * obj->contrlhandle->Ialpha);

    //sector 1
    if (obj->contrlhandle->theta >= 0 && obj->contrlhandle->theta < PI / 3) {
        T[4] = -U2;                     //100
        T[6] = U1;                      //110
        T[0] = 0.5 * (1 - T[4] - T[6]);   //000 & 111
        obj->epwmhandle->PWMA = (Uint16)((1 - 0.5 * (T[4] + T[6] + T[0])) * obj->epwmhandle->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        obj->epwmhandle->PWMB = (Uint16)((1 - 0.5 * (T[6] + T[0])) * obj->epwmhandle->Period);
        obj->epwmhandle->PWMC = (Uint16)((1 - 0.5 * (T[0])) * obj->epwmhandle->Period);
    }
    //sector 2
    if (obj->contrlhandle->theta >= 0 && obj->contrlhandle->theta < PI / 3) {
        T[6] = U3;                     //110
        T[2] = U2;                      //010
        T[0] = 0.5 * (1 - T[6] - T[2]);   //000 & 111
        obj->epwmhandle->PWMA = (Uint16)((1 - 0.5 * (T[6] + T[0])) * obj->epwmhandle->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        obj->epwmhandle->PWMB = (Uint16)((1 - 0.5 * (T[6] + T[2] + T[0])) * obj->epwmhandle->Period);
        obj->epwmhandle->PWMC = (Uint16)((1 - 0.5 * (T[0])) * obj->epwmhandle->Period);
    }
    //sector 3
    if (obj->contrlhandle->theta >= 0 && obj->contrlhandle->theta < PI / 3) {
        T[2] = U1;                     //010
        T[3] = -U3;                      //011
        T[0] = 0.5 * (1 - T[2] - T[3]);   //000 & 111
        obj->epwmhandle->PWMA = (Uint16)((1 - 0.5 * (T[0])) * obj->epwmhandle->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        obj->epwmhandle->PWMB = (Uint16)((1 - 0.5 * (T[2] + T[3] + T[0])) * obj->epwmhandle->Period);
        obj->epwmhandle->PWMC = (Uint16)((1 - 0.5 * (T[3] + T[0])) * obj->epwmhandle->Period);
    }
    //sector 4
    if (obj->contrlhandle->theta >= 0 && obj->contrlhandle->theta < PI / 3) {
        T[3] = U2;                     //011
        T[1] = -U1;                      //001
        T[0] = 0.5 * (1 - T[3] - T[1]);   //000 & 111
        obj->epwmhandle->PWMA = (Uint16)((1 - 0.5 * (T[0])) * obj->epwmhandle->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        obj->epwmhandle->PWMB = (Uint16)((1 - 0.5 * (T[3] + T[0])) * obj->epwmhandle->Period);
        obj->epwmhandle->PWMC = (Uint16)((1 - 0.5 * (T[3] + T[1] + T[0])) * obj->epwmhandle->Period);
    }
    //sector 5
    if (obj->contrlhandle->theta >= 0 && obj->contrlhandle->theta < PI / 3) {
        T[1] = -U3;                     //001
        T[5] = -U2;                      //101
        T[0] = 0.5 * (1 - T[1] - T[5]);   //000 & 111
        obj->epwmhandle->PWMA = (Uint16)((1 - 0.5 * (T[5] + T[0])) * obj->epwmhandle->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        obj->epwmhandle->PWMB = (Uint16)((1 - 0.5 * (T[0])) * obj->epwmhandle->Period);
        obj->epwmhandle->PWMC = (Uint16)((1 - 0.5 * (T[1] + T[5] + T[0])) * obj->epwmhandle->Period);
    }
    //sector 6
    if (obj->contrlhandle->theta >= 0 && obj->contrlhandle->theta < PI / 3) {
        T[5] = -U1;                     //101
        T[4] = U3;                      //100
        T[0] = 0.5 * (1 - T[5] - T[4]);   //000 & 111
        obj->epwmhandle->PWMA = (Uint16)((1 - 0.5 * (T[5] + T[4] + T[0])) * obj->epwmhandle->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        obj->epwmhandle->PWMB = (Uint16)((1 - 0.5 * (T[0])) * obj->epwmhandle->Period);
        obj->epwmhandle->PWMC = (Uint16)((1 - 0.5 * (T[5] + T[0])) * obj->epwmhandle->Period);
    }

    ////    float
    ////sector 1
    //if(theta >=0 && theta < PI/3){
    //    T[4] = -U2;     //100
    //    T[6] = U1;      //110
    //    T[0] = (1-T[6] - T[4])/2;   //000 & 111
    //    //temp->PWMA = (Uint16)((T[6]+T[4]+T[0]) * 7500.0);
    //    //temp->PWMB = (Uint16)((T[6]+T[0]) * 7500.0);
    //    //temp->PWMC = (Uint16)((T[0]) * 7500.0);
    //    temp->PWMA = (Uint16)((1 - 0.5 * (T[6] + T[4] + T[0])) * temp->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
    //    temp->PWMB = (Uint16)((1 - 0.5 * (T[6] + T[0])) * temp->Period);
    //    temp->PWMC = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
    //}

    ////sector 2
    //if(theta >=PI/3 && theta < 2*PI/3){
    //    T[2] = U2;     //010
    //    T[6] = U3;     //110
    //    T[0] = (1-T[2] - T[6])/2;   //000 & 111
    //    temp->PWMA = (Uint16)((1 - 0.5 * (T[6] + T[0])) * (float)temp->Period);
    //    temp->PWMB = (Uint16)((1 - 0.5 * (T[2] + T[6] + T[0])) * (float)temp->Period);
    //    temp->PWMC = (Uint16)((1 - 0.5 * (T[0])) * (float)temp->Period);
    //}

    ////sector 3
    //if(theta >=2*PI/3 && theta < PI){
    //    T[3] = -U3;     //011
    //    T[2] = U1;      //010
    //    T[0] = (1-T[3] - T[2])/2;   //000 & 111
    //    temp->PWMA = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
    //    temp->PWMB = (Uint16)((1 - 0.5 * (T[3] + T[2] + T[0])) * temp->Period);
    //    temp->PWMC = (Uint16)((1 - 0.5 * (T[3] + T[0])) * temp->Period);
    //}

    ////sector 4
    //if(theta >=PI && theta < 4*PI/3){
    //    T[1] = -U1;      //001
    //    T[3] = U2;       //011
    //    T[0] = (1-T[1] - T[3])/2;   //000 & 111
    //    //temp->PWMA = (Uint16)((T[0]) * 7500.0);
    //    //temp->PWMB = (Uint16)((T[3]+T[0]) * 7500.0);
    //    //temp->PWMC = (Uint16)((T[1]+T[3]+T[0]) * 7500.0);
    //    temp->PWMA = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
    //    temp->PWMB = (Uint16)((1 - 0.5 * (T[3] + T[0])) * temp->Period);
    //    temp->PWMC = (Uint16)((1 - 0.5 * (T[1] + T[3] + T[0])) * temp->Period);
    //}

    ////sector 5
    //if(theta >=4*PI/3 && theta < 5*PI/3){
    //    T[5] = -U2;      //101
    //    T[1] = -U3;       //001
    //    T[0] = (1-T[5] - T[1])/2;   //000 & 111
    //    //temp->PWMA = (Uint16)((T[5]+T[0]) * 7500.0);
    //    //temp->PWMB = (Uint16)((T[0]) * 7500.0);
    //    //temp->PWMC = (Uint16)((T[5]+T[1]+T[0]) * 7500.0);
    //    temp->PWMA = (Uint16)((1 - 0.5 * (T[5] + T[0])) * temp->Period);
    //    temp->PWMB = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
    //    temp->PWMC = (Uint16)((1 - 0.5 * (T[5] + T[1] + T[0])) * temp->Period);
    //}


    ////sector 6
    //if(theta >=5*PI/3 && theta < 2*PI){
    //    T[4] = U3;      //100
    //    T[5] = -U1;       //101
    //    T[0] = (1-T[4] - T[5])/2;   //000 & 111
    //    //temp->PWMA = (Uint16)((T[4]+T[5]+T[0]) * 7500.0);
    //    //temp->PWMB = (Uint16)((T[0]) * 7500.0);
    //    //temp->PWMC = (Uint16)((T[5]+T[0]) * 7500.0);
    //    temp->PWMA = (Uint16)((1 - 0.5 * (T[4] + T[5] + T[0])) * temp->Period);
    //    temp->PWMB = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
    //    temp->PWMC = (Uint16)((1 - 0.5 * (T[5] + T[0])) * temp->Period);
    //}

    DRV8301_PWMSet(obj);
}
void DRV8301_Clark(HAL_handle handle){
	//ctr->Ialpha = temp.Ia - 0.5*temp.Ib - 0.5*temp.Ic;
	//ctr->Ibeta = 0.866*temp.Ib - 0.866*temp.Ic;
    HAL_handle obj = handle;
    obj->contrlhandle->Ialpha = obj->sencehandle->Ia - 0.5 * obj->sencehandle->Ib - 0.5 * obj->sencehandle->Ic;
    obj->contrlhandle->Ibeta = 0.866 * obj->sencehandle->Ib - 0.866 * obj->sencehandle->Ic;

}


void DRV8301_Park(HAL_handle handle){
	//ctr->Id = cos(theta)*ctr->Ialpha + sin(theta)*ctr->Ibeta;
	//ctr->Iq = -sin(theta)*ctr->Ialpha + cos(theta)*ctr->Ibeta;
    HAL_handle obj = handle;
    obj->contrlhandle->Id = cos(obj->contrlhandle->theta) * obj->contrlhandle->Ialpha + sin(obj->contrlhandle->theta) * obj->contrlhandle->Ibeta;
    obj->contrlhandle->Iq = -sin(obj->contrlhandle->theta) * obj->contrlhandle->Ialpha + cos(obj->contrlhandle->theta) * obj->contrlhandle->Ibeta;
}

void DRV8301_Ipark(HAL_handle handle){
    //ctr->Ialpha = ctr->Id * cos(thata) - ctr->Iq * sin(theta);
    //ctr->Ialpha = ctr->Id * sin(thata) + ctr->Iq * cos(theta);
    HAL_handle obj = handle;
    obj->contrlhandle->Ialpha = obj->contrlhandle->Id * cos(obj->contrlhandle->theta) - obj->contrlhandle->Iq * sin(obj->contrlhandle->theta);
    obj->contrlhandle->Ibeta = obj->contrlhandle->Id * sin(obj->contrlhandle->theta) - obj->contrlhandle->Iq * cos(obj->contrlhandle->theta);

}

