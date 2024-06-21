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
#include <ADC.h>
#include <math.h>
#include "IQmathLib.h"

#define EN_GATE     GPIO11
#define DC_CAL      GPIO10
#define PWRGD       GPIO17
#define NOCTW       GPIO12
#define NFAULT      GPIO13

#define EN_GATE_H   GpioDataRegs.GPASET.bit.EN_GATE=1
#define EN_GATE_L   GpioDataRegs.GPACLEAR.bit.EN_GATE=1
#define DC_CAL_H    GpioDataRegs.GPASET.bit.DC_CAL=1
#define DC_CAL_L    GpioDataRegs.GPACLEAR.bit.DC_CAL=1

#define PWRGD_I       GpioDataRegs.GPADAT.bit.PWRGD
#define NOCTW_I       GpioDataRegs.GPADAT.bit.NOCTW
#define NFAULT_I      GpioDataRegs.GPADAT.bit.NFAULT

#define PWM_HA(D)   EPwm3A_SetCompare(D);
#define PWM_LA(D)   EPwm3B_SetCompare(D);
#define PWM_HB(D)   EPwm2A_SetCompare(D);
#define PWM_LB(D)   EPwm2B_SetCompare(D);
#define PWM_HC(D)   EPwm1A_SetCompare(D);
#define PWM_LC(D)   EPwm1B_SetCompare(D);


extern struct struct_DRV8301 drv8301;
extern struct struct_DRV8301* pdrv8301;
extern struct Contrl ctr;
extern struct Contrl* pctr;
struct struct_DRV8301{
    //R0
    Uint16 Reg0;
    U8 fault;
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

    //CTL command
    U8 en_gate;
    U8 dc_cal;
    U8 pwrgd;
    U8 noctw;
    U8 nfault;

    //CTL pwm
    Uint16 Period;
    Uint16 PWMA;
    Uint16 PWMB;
    Uint16 PWMC;

    //Sense
    float hella;
    float hellb;
    float hellc;
    Uint16 hell;
    float Ia;
    float Ib;
    float Ic;

};

struct Contrl{
    float Ialpha;
    float Ibeta;
    float Id;
    float Iq;

    float A;
    float theta;
    float step;
};

/////////////////////////funciton/////////////////////////////////////////
void DRV8301_Init(struct struct_DRV8301* temp);
void DRV8301GPIO_Init(void);
void DRV8301_menu(void);
void DRV8301_Display(struct struct_DRV8301 temp);
void DRV8301_Read(struct struct_DRV8301* temp);
void DRV8301_PWMSet(struct struct_DRV8301 temp);
void DRV8301_PWMOff(struct struct_DRV8301* temp);
void DRV8301_Enable(struct struct_DRV8301* temp);
void DRV8301_Disable(struct struct_DRV8301* temp);
void DRV8301_ENSense(struct struct_DRV8301* temp);
void DRV8301_DISSense(struct struct_DRV8301* temp);
void DRV8301_SenseGet(struct struct_DRV8301* temp);

void DRV8301_SixStep(struct struct_DRV8301* temp);
void DRV8301_SPWM(struct struct_DRV8301* temp);
void DRV8301_SVPWM(struct struct_DRV8301* temp);
void DRV8301_Clark(struct struct_DRV8301 temp, struct Contrl* ctr);
void DRV8301_Park(struct struct_DRV8301 temp, struct Contrl* ctr);
void DRV8301_Ipark(struct struct_DRV8301 temp, struct Contrl* ctr);
//if error, return 1. else return 0
U8 DRV8301_Check(struct struct_DRV8301* temp);


#endif /* APP_INCLUDE_DRV8301_H_ */
