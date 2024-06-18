
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
    float step =1000.0;
    struct struct_DRV8301 drv8301;
    struct struct_DRV8301* pdrv8301 = &drv8301;
	InitPieCtrl();
	IER = 0x0000;   //disable all interrupt
	IFR = 0x0000;   //clear all interrupt flag
	InitPieVectTable();

	TIM0_Init(150-1, 700000-1);    //150MHz 150 divide ->1uS, count 700*1000  ->700mS
	DELAY_US(100);
    LED_Init();
    DELAY_US(2000000);

    OLED_Init();
    OLED_Clear();

    OLED_ShowPicture(34, 2, 60, 60, BtPic, 1);
    OLED_Refresh();

    DRV8301_Init(pdrv8301);
    theta = 0;
	while(1){
	    //DRV8301_PWMSet(*pdrv8301);
	    //DELAY_US(50*1000);
	    //DELAY_US(1);
	    DRV8301_SVPWM(pdrv8301);
	    theta += PI/step;
	    if(theta > 2*PI)
	        theta=0;
	    fx = Scan_PressKey();

	    if(fx == 1){
	        step += 200;
	    }

	    if(fx == 4){
	        step -= 200;
	    }
	    OLED_ShowString(0, 2*8, "step=",1);
	    OLED_ShowInt(40, 2*8, step, 1);
	    OLED_Refresh_fix(40, 127, 2);
	}
}

