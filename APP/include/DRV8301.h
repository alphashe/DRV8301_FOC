/*
 * DRV8301.h
 *
 *  Created on: 2024Äê5ÔÂ28ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_DRV8301_H_
#define APP_INCLUDE_DRV8301_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <OLED.h>
#include <SPI.h>

struct struct_DRV8301{
    /*
    //R0
    U8 gvdd_uv;
    U8 pvdd_uv;
    U8 otsd;
    U8 otw;
    U8 fetha_oc;
    U8 fetla_oc;
    U8 fethb_oc;
    U8 fetlb_oc;
    U8 fethc_oc;
    U8 fetlc_oc;
    */

    //R1
    U8 gvdd_ov;
    U8 device_id;

    //R2
    U8 gate_current;
    U8 pwm_mode;
    U8 ocp_mode;
    U8 adj_set;

    //R3
    U8 octw_mode;
    U8 gain;
    U8 dc_cal_ch1;
    U8 dc_cal_ch2;
    U8 oc_toff;


    U8 en_buck;
    U8 en_gate;
    U8 dc_cal;
};


struct struct_DRV8301* DRV8301_Init(void);
void DRV8301_menu(void);
void DRV8301_Display(struct struct_DRV8301 distemp);
void DRV8301_Read(struct struct_DRV8301* distemp);



#endif /* APP_INCLUDE_DRV8301_H_ */
