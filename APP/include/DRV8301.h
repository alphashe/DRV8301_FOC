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

//extern HAL_Obj hal;

//extern struct struct_DRV8301 drv8301;
//extern struct struct_DRV8301* pdrv8301;



//////////////////////////////DRV8301 Register//////////////////////////////
//R2 bit [1,0]
enum GATE_CURRENT{  
    gate_current_1p7A,
    gate_current_0p7A,
    gate_current_0p25A
};
//R2 bit[2]
enum PWM_MODE{
    pwm_mode_sixpwm,
    pwm_mode_threepwm
};
//R2 bit[5,4]
enum OCP_MODE{
    ocp_mode_current_limit,
    ocp_mode_oc_latch,
    ocp_mode_report,
    ocp_mode_disable
};
//R2 bit[10...6]
enum OC_ADJ_SET {
    oc_adj_set_0p060, oc_adj_set_0p068, oc_adj_set_0p076, oc_adj_set_0p086, oc_adj_set_0p097, oc_adj_set_0p109, oc_adj_set_0p123, oc_adj_set_1p138,
    oc_adj_set_0p155, oc_adj_set_0p175, oc_adj_set_0p197, oc_adj_set_0p222, oc_adj_set_0p250, oc_adj_set_0p282, oc_adj_set_0p317, oc_adj_set_1p358,
    oc_adj_set_0p403, oc_adj_set_0p454, oc_adj_set_0p511, oc_adj_set_0p576, oc_adj_set_0p648, oc_adj_set_0p730, oc_adj_set_0p822, oc_adj_set_1p926,
    oc_adj_set_1p043, oc_adj_set_2p175, oc_adj_set_1p324, oc_adj_set_1p491, oc_adj_set_1p679, oc_adj_set_1p892, oc_adj_set_2p131, oc_adj_set_2p4
};
//R3 bit[1,0]
enum OCTW_MODE {
    octw_mode_otoc,
    octw_mode_ot,
    octw_mode_oc
};
//R3 bit[3,2]
enum GAIN {
    gain_10,
    gain_20,
    gain_40,
    gain_80
};
//R3 bit[4] [5]
enum DC_CAL_CH12 {
    dc_cal_connect,
    dc_cal_short
};
//R3 bit[6]
enum OC_TOFF {
    oc_toff_CBC,
    oc_toff_offtime
};
typedef struct _DRV8301_HAL{
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
    U8 oc_adj_set;

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

}DRV8301_Obj;
typedef struct _DRV8301_HAL* DRV8301_Handle;

typedef struct _EPWM_HAL {
    //CTL pwm
    Uint16 Period;
    Uint16 PWMA;
    Uint16 PWMB;
    Uint16 PWMC;
}EPWM_Obj;
typedef struct _EPWM_HAL* EPWM_Handle;

typedef struct _SENCE_HAL {
    //Sense
    float hella;
    float hellb;
    float hellc;
    Uint16 hell;
    float Ia;
    float Ib;
    float Ic;
}SENCE_Obj;
typedef struct _SENCE_HAL* SENCE_Handle;

typedef struct _CONTRL_HAL{
    float Ialpha;
    float Ibeta;
    float Id;
    float Iq;

    float A;
    float theta;
}CONTRL_Obj;
typedef struct _CONTRL_HAL* CONTRL_Handle;

typedef struct _HAL_Obj {
    DRV8301_Handle drv8301handle;
    EPWM_Handle    epwmhandle;
    SENCE_Handle   sencehandle;
    CONTRL_Handle  contrlhandle;
}HAL_Obj;
typedef struct _HAL_Obj* HAL_handle;

/////////////////////////handle funciton/////////////////////////////////////////
HAL_handle Hal_Init(HAL_Obj hal);
//DRV8301_Handle HAL_DRV8301_Init(void);
EPWM_Handle EPWM_Init(void);
SENCE_Handle SENCE_Init(void);
CONTRL_Handle CONTRL_Init(void);
void SetParam(HAL_handle handle);
void DRV8301_Config(HAL_handle handle);









/// 
/// function
/// 
void DRV8301_Init(HAL_handle handle);
void DRV8301GPIO_Init(void);
void DRV8301_Display(HAL_handle handle);
void DRV8301_Read(HAL_handle handle);
void DRV8301_PWMSet(HAL_handle handle);
void DRV8301_PWMOff(HAL_handle handle);

void DRV8301_Enable(HAL_handle handle);
void DRV8301_Disable(HAL_handle handle);
void DRV8301_ENSense(HAL_handle handle);
void DRV8301_DISSense(HAL_handle handle);
void DRV8301_SenseGet(HAL_handle handle);

void DRV8301_SixStep(HAL_handle handle);
void DRV8301_SPWM(HAL_handle handle);
void DRV8301_SVPWM(HAL_handle handle);
void DRV8301_Clark(HAL_handle handle);
void DRV8301_Park(HAL_handle handle);
void DRV8301_Ipark(HAL_handle handle);
//if error, return 1. else return 0
U8 DRV8301_Check(HAL_handle handle);


#endif /* APP_INCLUDE_DRV8301_H_ */
