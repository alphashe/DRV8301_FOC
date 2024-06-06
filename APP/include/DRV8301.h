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
#include <EPWM.h>

#define EN_GATE     GPIO11
#define DC_CAL      GPIO10

#define EN_GATE_H   GpioDataRegs.GPASET.bit.EN_GATE=1
#define EN_GATE_L   GpioDataRegs.GPACLEAR.bit.EN_GATE=1
#define DC_CAL_H    GpioDataRegs.GPASET.bit.DC_CAL=1
#define DC_CAL_L    GpioDataRegs.GPACLEAR.bit.DC_CAL=1

struct struct_DRV8301{

    //R0
    Uint16 Reg0;
    /*U8 gvdd_uv;
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
    Uint16 Reg1;
    U8 gvdd_ov;
    U8 device_id;

    //R2
    Uint16 Reg2;
    U8 gate_current;
    U8 pwm_mode;
    U8 ocp_mode;
    U8 adj_set;

    //R3
    Uint16 Reg3;
    U8 octw_mode;
    U8 gain;
    U8 dc_cal_ch1;
    U8 dc_cal_ch2;
    U8 oc_toff;



    //CTL
    U8 en_buck;
    U8 en_gate;
    U8 dc_cal;

};

/////funciton
struct struct_DRV8301* DRV8301_Init(void);
void DRV8301GPIO_Init(void);
void DRV8301_menu(void);
void DRV8301_Display(struct struct_DRV8301 temp);
void DRV8301_Read(struct struct_DRV8301* temp);
void DRV8301_Contr(struct struct_DRV8301 temp);
void DRV8301_Enable(struct struct_DRV8301* temp);
void DRV8301_Disable(struct struct_DRV8301* temp);



#endif /* APP_INCLUDE_DRV8301_H_ */
