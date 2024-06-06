/*
 * DRV8301.c
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#include <DRV8301.h>

struct struct_DRV8301* DRV8301_Init(void){
    static struct struct_DRV8301 drv8301;

    SPI_Init();
    DRV8301GPIO_Init();

    DRV8301_Enable(&drv8301);
    DELAY_US(10000);

    DRV8301_Read(&drv8301);
    DRV8301_Display(drv8301);

    EPWM1_Init(7500);   //150MHz 150 *50 =20khz
    EPWM2_Init(7500);
    EPWM3_Init(7500);

    EPwm1A_SetCompare(0);
    EPwm1B_SetCompare(0);
    EPwm2A_SetCompare(0);
    EPwm2B_SetCompare(0);
    EPwm3A_SetCompare(0);
    EPwm3B_SetCompare(0);

    return &drv8301;
}



void DRV8301_menu(void){
    OLED_ShowString(43, 0, "DRV8301", 1);
    OLED_ShowString(0, 8, "Device ID:", 1);
    OLED_ShowString(0, 16, "EN_BUCK:", 1);
    OLED_ShowString(0, 24, "EN_GATE:", 1);
    OLED_ShowString(0, 32, "DC_CAL:", 1);
}

void DRV8301_Display(struct struct_DRV8301 temp){
    OLED_Clear();
    DRV8301_menu();

    if(temp.device_id == 0)
        OLED_ShowString(0, 8, "Disconnect!", 1);
    else
        OLED_ShowInt(60, 8, temp.device_id, 1);

    if(temp.en_gate==1)
        OLED_ShowString(48, 24, "On ", 1);
    else
        OLED_ShowString(48, 24, "off", 1);

    if(temp.dc_cal==1)
        OLED_ShowString(42, 32, "On ", 1);
    else
        OLED_ShowString(42, 32, "off", 1);

    OLED_ShowHexfix(20, 40, temp.Reg0, 4, 1);
    OLED_ShowHexfix(84, 40, temp.Reg1, 4, 1);
    OLED_ShowHexfix(20, 48, temp.Reg2, 4, 1);
    OLED_ShowHexfix(84, 48, temp.Reg3, 4, 1);
    OLED_Refresh();
}

void DRV8301_Read(struct struct_DRV8301* temp){
    SPIA_SendReciveData(0x8000);
    temp->Reg0 = SPIA_SendReciveData(0x8800); //read register:0b00
    temp->Reg1 = SPIA_SendReciveData(0x9000); //read register:0b01
    temp->Reg2 = SPIA_SendReciveData(0x9800); //read register:0b10
    temp->Reg3 = SPIA_SendReciveData(0x9800); //read register:0b11
    //temp->Reg0 = SPIA_SendReciveData(0x0000); //

    /*
    //Register 2
    distemp->gate_current=tempdata[2]&0x3;  //D0 D1
    tempdata[2] = tempdata[2] >>3;
    distemp->pwm_mode = tempdata[2]&0x01;   //D3
    tempdata[2] = tempdata[2] >>1;
    distemp->ocp_mode = tempdata[2]&0x03;   //D4 D5
    tempdata[2] = tempdata[2] >>2;
    distemp->adj_set = tempdata[2]&0x1f;    //D6 D7 D8 D9 D10

    //Register 3
    distemp->octw_mode=tempdata[3]&0x3;  //D0 D1
    tempdata[3] = tempdata[3] >>2;
    distemp->gain = tempdata[3]&0x03;   //D2 D3
    tempdata[3] = tempdata[3] >>2;
    distemp->dc_cal_ch1 = tempdata[3]&0x01;   //D4
    tempdata[3] = tempdata[3] >>1;
    distemp->dc_cal_ch2 = tempdata[3]&0x01;   //D5
    tempdata[3] = tempdata[3] >>1;
    distemp->oc_toff = tempdata[3]&0x01;    //D6
    */
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

void DRV8301_Contr(struct struct_DRV8301 temp){
    EPwm1B_SetCompare(3000);
    EPwm1A_SetCompare(3000);
    EPwm2B_SetCompare(5000);
    EPwm2A_SetCompare(5000);
    EPwm3B_SetCompare(2000);
    EPwm3A_SetCompare(2000);
}

void DRV8301_Enable(struct struct_DRV8301* temp){
    EN_GATE_H;
    temp->en_gate=1;
    DRV8301_Display(*temp);
}
void DRV8301_Disable(struct struct_DRV8301* temp){
    EN_GATE_L;
    temp->en_gate=0;
    DRV8301_Display(*temp);
}

void DRV8301_SenseOp(struct struct_DRV8301* temp){
    DC_CAL_L;
    temp->dc_cal=0;
}
void DRV8301_SenseCal(struct struct_DRV8301* temp){
    DC_CAL_H;
    temp->dc_cal=1;
}

void DRV8301_Check(struct struct_DRV8301* temp){
    temp->pwrgd = PWRGD_I;
    temp->nfault = NFAULT_I;
    temp->noctw = NOCTW_I;
}
