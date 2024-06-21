
#include <TouchKey.h>
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "ADC.h"
#include "INT.h"
#include "TIMER.h"
#include "EPWM.h"
#include "LED.h"
#include "OLED.h"
#include "SPI.h"
#include "oledchar.h"
#include <DRV8301.h>
#include <PressKey.h>
//定义变量


void main(void){
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();

    InitSysCtrl();
    //DINT;
    float fx = 0;
    //struct struct_DRV8301 drv8301;
    //struct struct_DRV8301* pdrv8301 = &drv8301;
	InitPieCtrl();
	IER = 0x0000;   //disable all interrupt
	IFR = 0x0000;   //clear all interrupt flag
	InitPieVectTable();

	TIM0_Init(3-1, 1000-1);    //150MHz 3 divide ->50Mhz, count 1000  ->50kHz
	DELAY_US(100);
    LED_Init();
    DELAY_US(2000000);

    OLED_Init();
    OLED_Clear();

    OLED_ShowPicture(34, 2, 60, 60, BtPic, 1);
    OLED_Refresh();
    PressKey_Init();
    DRV8301_Init(pdrv8301);
    ctr.theta = 0;
    ctr.step = 1000;
	while(1){
	    //DRV8301_PWMSet(*pdrv8301);
	    //DELAY_US(50*1000);
	    //DELAY_US(1);

	    fx = Scan_PressKey();

	    if(fx == 1){
	        ctr.A += 0.05;
	    }

	    if(fx == 4){
	        ctr.A -= 0.05;
	    }

	    DRV8301_SenseGet(pdrv8301);
	    DRV8301_Display(drv8301);
	    OLED_Refresh();
	}
}

interrupt void TIM0_Int(void){
    EALLOW;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
    EDIS;
    LED3_TOGGLE;
    ctr.theta += PI/ctr.step;
    if(ctr.theta > 2*PI){
        ctr.theta=0;
    }
    DRV8301_SVPWM(pdrv8301);
    LED3_TOGGLE;// float format 11.4uS
}
