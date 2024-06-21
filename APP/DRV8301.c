/*
 * DRV8301.c
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#include <DRV8301.h>
float theta=0;
struct struct_DRV8301 drv8301;
struct struct_DRV8301* pdrv8301 = &drv8301;

void DRV8301_Init(struct struct_DRV8301* temp){
    //static struct struct_DRV8301 drv8301;
    temp->Period = 3000;    //150MHz 50kHz/2 = 25kHz
    temp->PWMA = 0;
    temp->PWMB = 0;
    temp->PWMC = 0;
    SPI_Init();
    ADC_Init();
    EPWM1_Init(temp->Period);   //150MHz 150 *50 =20khz
    EPWM2_Init(temp->Period);
    EPWM3_Init(temp->Period);
    DRV8301GPIO_Init();

    DRV8301_PWMOff(temp);
    DRV8301_Enable(temp);
    DELAY_US(10000);    //wait 5~10mS
    DRV8301_Read(temp);
    DRV8301_Display(*temp);

}


void DRV8301_menu(void){
    OLED_ShowString(43, 0, "DRV8301", 1);
    OLED_ShowString(0, 1*8, "Device ID:", 1);
    OLED_ShowString(0, 2*8, "EN_GATE:", 1); //8*6
    OLED_ShowString(68, 2*8, "DC_CAL:", 1); //7*6

    //OLED_ShowString(0, 3*8, "HELLA:", 1);   //6*6
    //OLED_ShowString(64, 3*8, "HELLB:", 1);   //6*6
    //OLED_ShowString(0, 4*8, "HELLC:", 1);   //6*6
    OLED_ShowString(0, 3*8, "PWMA:", 1);   //6*6
    OLED_ShowString(64, 3*8, "PWMB:", 1);   //6*6
    OLED_ShowString(0, 4*8, "PWMC:", 1);   //6*6
    OLED_ShowString(0, 5*8, "SO1:", 1);     //4*6
    OLED_ShowString(64, 5*8, "SO2:", 1);     //4*6
}

void DRV8301_Display(struct struct_DRV8301 temp){
    OLED_Clear();
    DRV8301_menu();

    if(temp.device_id == 0 || temp.device_id == 0xf)
        OLED_ShowString(0, 8, "Disconnect!", 1);
    else
        OLED_ShowInt(60, 8, temp.device_id, 1);

    if(temp.en_gate==1)
        OLED_ShowString(8*6, 2*8, "On ", 1);
    else
        OLED_ShowString(8*6, 2*8, "off", 1);

    if(temp.dc_cal==1)
        OLED_ShowString(7*6+68, 2*8, "On ", 1);
    else
        OLED_ShowString(7*6+68, 2*8, "off", 1);

    OLED_ShowInt(6*6, 3*8, temp.hella, 1);
    OLED_ShowInt(6*6+64, 3*8, temp.hellb, 1);
    OLED_ShowInt(6*6, 4*8, temp.hellc, 1);
/*
    OLED_ShowInt(6*6, 3*8, temp.PWMA, 1);
    OLED_ShowInt(6*6+64, 3*8, temp.PWMB, 1);
    OLED_ShowInt(6*6, 4*8, temp.PWMC, 1);
*/

    OLED_Refresh();
}

void DRV8301_Read(struct struct_DRV8301* temp){
    SPIA_SendReciveData(0x8000);
    temp->Reg0 = SPIA_SendReciveData(0x8800); //read register:0b00
    temp->Reg1 = SPIA_SendReciveData(0x9000); //read register:0b01
    temp->Reg2 = SPIA_SendReciveData(0x9800); //read register:0b10
    temp->Reg3 = SPIA_SendReciveData(0x9800); //read register:0b11
    //temp->Reg0 = SPIA_SendReciveData(0x0000); //


    //Register 0
    temp->fetlc_oc      = (temp->Reg0 & 0b1);   //D0
    temp->fethc_oc      = (temp->Reg0 & 0b10) >> 1;   //D1
    temp->fetlb_oc      = (temp->Reg0 & 0b100) >> 2;   //D2
    temp->fethb_oc      = (temp->Reg0 & 0b1000) >> 3;   //D3
    temp->fetlc_oc      = (temp->Reg0 & 0b10000) >> 4;   //D4
    temp->fethc_oc      = (temp->Reg0 & 0b100000) >> 5;   //D5
    temp->otw           = (temp->Reg0 & 0b1000000) >> 6;   //D6
    temp->otsd          = (temp->Reg0 & 0b10000000) >> 7;   //D7
    temp->pvdd_uv       = (temp->Reg0 & 0b100000000) >> 8;   //D8
    temp->gvdd_uv       = (temp->Reg0 & 0b1000000000) >> 9;   //D9
    temp->fault         = (temp->Reg0 & 0b10000000000) >> 10;   //D10

    //Register 1
    temp->device_id     = (temp->Reg1 & 0b1111);   //D0 D1 D2 D3
    temp->gvdd_ov       = (temp->Reg1 & 0b10000000) >> 7;  //D7

    //Register 2
    temp->gate_current  = (temp->Reg2 & 0b11);  //D0 D1
    temp->pwm_mode      = (temp->Reg2 & 0b1000) >> 3;   //D3
    temp->ocp_mode      = (temp->Reg2 & 0b110000) >> 4;   //D4 D5
    temp->adj_set       = (temp->Reg2 & 0b11111000000) >> 6;    //D6 D7 D8 D9 D10

    //Register 3
    temp->octw_mode     = (temp->Reg3 & 0b11);  //D0 D1
    temp->gain          = (temp->Reg3 & 0b1100) >> 2;   //D2 D3
    temp->dc_cal_ch1    = (temp->Reg3 & 0b10000) >> 4;   //D4
    temp->dc_cal_ch2    = (temp->Reg3 & 0b100000) >> 5;   //D5
    temp->oc_toff       = (temp->Reg3 & 0b1000000) >> 6;    //D6

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

void DRV8301_PWMSet(struct struct_DRV8301 temp){

    //limit max Duty max 95%.
    if(temp.PWMA>(temp.Period*0.95))
        temp.PWMA = temp.Period*0.95;
    if(temp.PWMB>(temp.Period*0.95))
        temp.PWMB = temp.Period*0.95;
    if(temp.PWMC>(temp.Period*0.95))
        temp.PWMC = temp.Period*0.95;

    PWM_HA(temp.PWMA);
    PWM_LA(temp.PWMA);
    PWM_HB(temp.PWMB);
    PWM_LB(temp.PWMB);
    PWM_HC(temp.PWMC);
    PWM_LC(temp.PWMC);

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

void DRV8301_PWMOff(struct struct_DRV8301* temp){
    PWM_HA(0);
    PWM_LA(7500);
    PWM_HB(0);
    PWM_LB(7500);
    PWM_HC(0);
    PWM_LC(7500);

    temp->PWMA = temp->PWMB = temp->PWMC =0;
}

void DRV8301_Enable(struct struct_DRV8301* temp){
    EN_GATE_H;
    temp->en_gate=1;
}
void DRV8301_Disable(struct struct_DRV8301* temp){
    EN_GATE_L;
    temp->en_gate=0;
}

void DRV8301_ENSense(struct struct_DRV8301* temp){
    DC_CAL_L;
    temp->dc_cal=0;
}
void DRV8301_DISSense(struct struct_DRV8301* temp){
    DC_CAL_H;
    temp->dc_cal=1;
}

//get Ia,Ib,Ic, and hella,b,c. translate hell to code "abc"
void DRV8301_SenseGet(struct struct_DRV8301* temp){
    Uint16 so1=0, so2=0;
    Uint16 a=0, b=0, c=0;
    so1 = Read_ADCValueResult0();
    so2 = Read_ADCValueResult1();
    c = Read_ADCValueResult2();
    b = Read_ADCValueResult3();
    a = Read_ADCValueResult4();



    temp->hell = 0;
    temp->Ib = (so1/4096.0 *3) / 0.08;
    temp->Ic = (so2/4096.0 *3) / 0.08;
    temp->Ia = -temp->Ib - temp->Ic;
    temp->hella = a/4096.0 * 3;
    temp->hellb = b/4096.0 * 3;
    temp->hellc = c/4096.0 * 3;
    if(temp->hella>1)
        temp->hell = temp->hell | 0b100;
    if(temp->hellb>1)
        temp->hell = temp->hell | 0b010;
    if(temp->hellc>1)
        temp->hell = temp->hell | 0b001;
}

//will return 1, if something wrong. return 0, when normal operation
U8 DRV8301_Check(struct struct_DRV8301* temp){
    temp->pwrgd = PWRGD_I;
    temp->nfault = NFAULT_I;
    temp->noctw = NOCTW_I;

    if(temp->pwrgd==0 || temp->nfault==0 || temp->noctw==0)
        return 1;
    return 0;
}

void DRV8301_SixStep(struct struct_DRV8301* temp){
    //DRV8301_SenseGet(temp);
    //hell:abc
    if(temp->hell == 0b001){
        temp->PWMA = 500;
        temp->PWMB = 500;
        temp->PWMC = 3000;
    }
    if(temp->hell == 0b010){
        temp->PWMA = 500;
        temp->PWMB = 3000;
        temp->PWMC = 500;
    }
    if(temp->hell == 0b011){
        temp->PWMA = 500;
        temp->PWMB = 3000;
        temp->PWMC = 500;
    }
    if(temp->hell == 0b100){
        temp->PWMA = 500;
        temp->PWMB = 500;
        temp->PWMC = 3000;
    }
    if(temp->hell == 0b101){
        temp->PWMA = 500;
        temp->PWMB = 500;
        temp->PWMC = 3000;
    }
    if(temp->hell == 0b110){
        temp->PWMA = 3000;
        temp->PWMB = 500;
        temp->PWMC = 500;
    }

    DRV8301_PWMSet(*temp);
}

void DRV8301_SPWM(struct struct_DRV8301* temp){
	//theta
    struct Contrl svpwm;
    svpwm.Ialpha = cos(theta) * 0.2;
    svpwm.Ibeta = sin(theta) * 0.2;
    temp->PWMA = (svpwm.Ialpha) * 3750 + 3750;
    temp->PWMB = (-0.5*svpwm.Ialpha + 0.866*svpwm.Ibeta) * 3750 + 3750;
    temp->PWMC = (-0.5*svpwm.Ialpha - 0.866*svpwm.Ibeta) * 3750 + 3750;

    DRV8301_PWMSet(*temp);
}

void DRV8301_SVPWM(struct struct_DRV8301* temp){
    struct Contrl ctr;

    float U1, U2, U3;
    float T[7] = {0};
    ctr.A = (0.4);
    ctr.Ialpha = cos(theta) * ctr.A;
    ctr.Ibeta = sin(theta) * ctr.A;
    U1 = 0.866 * (ctr.Ibeta);                                 //0~1
    U2 = 0.866 * (0.5 * ctr.Ibeta - 0.866 * ctr.Ialpha);      //0~1
    U3 = 0.866 * (0.5 * ctr.Ibeta + 0.866 * ctr.Ialpha);      //0~1  max value is sqr(3)/2


    //    float
    //sector 1
    if(theta >=0 && theta < PI/3){
        T[4] = -U2;     //100
        T[6] = U1;      //110
        T[0] = (1-T[6] - T[4])/2;   //000 & 111
        //temp->PWMA = (Uint16)((T[6]+T[4]+T[0]) * 7500.0);
        //temp->PWMB = (Uint16)((T[6]+T[0]) * 7500.0);
        //temp->PWMC = (Uint16)((T[0]) * 7500.0);
        temp->PWMA = (Uint16)((1 - 0.5 * (T[6] + T[4] + T[0])) * temp->Period);     //PWM mode is up&down ,Duty = Period *(1-T/2)
        temp->PWMB = (Uint16)((1 - 0.5 * (T[6] + T[0])) * temp->Period);
        temp->PWMC = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
    }

    //sector 2
    if(theta >=PI/3 && theta < 2*PI/3){
        T[2] = U2;     //010
        T[6] = U3;     //110
        T[0] = (1-T[2] - T[6])/2;   //000 & 111
        temp->PWMA = (Uint16)((1 - 0.5 * (T[6] + T[0])) * (float)temp->Period);
        temp->PWMB = (Uint16)((1 - 0.5 * (T[2] + T[6] + T[0])) * (float)temp->Period);
        temp->PWMC = (Uint16)((1 - 0.5 * (T[0])) * (float)temp->Period);
    }

    //sector 3
    if(theta >=2*PI/3 && theta < PI){
        T[3] = -U3;     //011
        T[2] = U1;      //010
        T[0] = (1-T[3] - T[2])/2;   //000 & 111
        temp->PWMA = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
        temp->PWMB = (Uint16)((1 - 0.5 * (T[3] + T[2] + T[0])) * temp->Period);
        temp->PWMC = (Uint16)((1 - 0.5 * (T[3] + T[0])) * temp->Period);
    }

    //sector 4
    if(theta >=PI && theta < 4*PI/3){
        T[1] = -U1;      //001
        T[3] = U2;       //011
        T[0] = (1-T[1] - T[3])/2;   //000 & 111
        //temp->PWMA = (Uint16)((T[0]) * 7500.0);
        //temp->PWMB = (Uint16)((T[3]+T[0]) * 7500.0);
        //temp->PWMC = (Uint16)((T[1]+T[3]+T[0]) * 7500.0);
        temp->PWMA = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
        temp->PWMB = (Uint16)((1 - 0.5 * (T[3] + T[0])) * temp->Period);
        temp->PWMC = (Uint16)((1 - 0.5 * (T[1] + T[3] + T[0])) * temp->Period);
    }

    //sector 5
    if(theta >=4*PI/3 && theta < 5*PI/3){
        T[5] = -U2;      //101
        T[1] = -U3;       //001
        T[0] = (1-T[5] - T[1])/2;   //000 & 111
        //temp->PWMA = (Uint16)((T[5]+T[0]) * 7500.0);
        //temp->PWMB = (Uint16)((T[0]) * 7500.0);
        //temp->PWMC = (Uint16)((T[5]+T[1]+T[0]) * 7500.0);
        temp->PWMA = (Uint16)((1 - 0.5 * (T[5] + T[0])) * temp->Period);
        temp->PWMB = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
        temp->PWMC = (Uint16)((1 - 0.5 * (T[5] + T[1] + T[0])) * temp->Period);
    }


    //sector 6
    if(theta >=5*PI/3 && theta < 2*PI){
        T[4] = U3;      //100
        T[5] = -U1;       //101
        T[0] = (1-T[4] - T[5])/2;   //000 & 111
        //temp->PWMA = (Uint16)((T[4]+T[5]+T[0]) * 7500.0);
        //temp->PWMB = (Uint16)((T[0]) * 7500.0);
        //temp->PWMC = (Uint16)((T[5]+T[0]) * 7500.0);
        temp->PWMA = (Uint16)((1 - 0.5 * (T[4] + T[5] + T[0])) * temp->Period);
        temp->PWMB = (Uint16)((1 - 0.5 * (T[0])) * temp->Period);
        temp->PWMC = (Uint16)((1 - 0.5 * (T[5] + T[0])) * temp->Period);
    }

    DRV8301_PWMSet(*temp);
}
void DRV8301_Clark(struct struct_DRV8301 temp, struct Contrl* ctr){
	ctr->Ialpha = temp.Ia - 0.5*temp.Ib - 0.5*temp.Ic;
	ctr->Ibeta = 0.866*temp.Ib - 0.866*temp.Ic;
}


void DRV8301_Park(struct struct_DRV8301 temp, struct Contrl* ctr){
	ctr->Id = cos(theta)*ctr->Ialpha + sin(theta)*ctr->Ibeta;
	ctr->Iq = -sin(theta)*ctr->Ialpha + cos(theta)*ctr->Ibeta;
}

void DRV8301_Ipark(struct struct_DRV8301 temp, struct Contrl* ctr){

}

