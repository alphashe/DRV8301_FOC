/*
 * DRV8301.c
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#include <DRV8301.h>

struct struct_DRV8301* DRV8301_Init(void){
    static struct struct_DRV8301 drv8301;
    drv8301.en_buck=0;
    drv8301.en_gate=0;
    drv8301.dc_cal=0;

    SPI_Init();
    DRV8301_Read(&drv8301);
    DRV8301_Display(drv8301);

    return &drv8301;
}

void DRV8301_menu(void){
    OLED_ShowString(43, 0, "DRV8301", 1);
    OLED_ShowString(0, 8, "Device ID:", 1);
    OLED_ShowString(0, 16, "EN_BUCK:", 1);
    OLED_ShowString(0, 24, "EN_GATE:", 1);
    OLED_ShowString(0, 32, "DC_CAL:", 1);
}

void DRV8301_Display(struct struct_DRV8301 distemp){
    OLED_Clear();
    DRV8301_menu();
    if(distemp.device_id == 0)
        OLED_ShowString(0, 8, "Disconnect!", 1);
    else
        OLED_ShowInt(60, 8, distemp.device_id, 1);

    if(distemp.en_buck==1)
        OLED_ShowString(48, 16, "On ", 1);
    else
        OLED_ShowString(48, 16, "off", 1);

    if(distemp.en_gate==1)
        OLED_ShowString(48, 24, "On ", 1);
    else
        OLED_ShowString(48, 24, "off", 1);

    if(distemp.dc_cal==1)
        OLED_ShowString(42, 32, "On ", 1);
    else
        OLED_ShowString(42, 32, "off", 1);

    OLED_Refresh();
}

void DRV8301_Read(struct struct_DRV8301* distemp){
    Uint16 tempdata[4]={0, 0, 0, 0};
    SPIA_SendReciveData(0x8000); //read register:0b00
    tempdata[0] = SPIA_SendReciveData(0x8800); //read register:0b01
    tempdata[1] = SPIA_SendReciveData(0x9000); //read register:0b10
    tempdata[2] = SPIA_SendReciveData(0x9800); //read register:0b11
    tempdata[3] = SPIA_SendReciveData(0x0000); //

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
}
