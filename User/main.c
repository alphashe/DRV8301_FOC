
#include <TouchKey.h>
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "ADC.h"
#include "INT.h"
#include "TIME.h"
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
    Uint16 fx = 0;

    struct struct_DRV8301 *pdrv8301;
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

    DELAY_US(1000000);
    pdrv8301 = DRV8301_Init();
   // ADC_Init();
   // Key_Init();
   // EXTI1_Init();

	while(1){
	    DRV8301_PWMSet(*pdrv8301);
	    //DELAY_US(50*1000);

	   // fx = Scan_PressKey();
	    if(fx == 1){
	        pdrv8301->PWMA = 6000;
	        pdrv8301->PWMB = 500;
	        pdrv8301->PWMC = 500;
	        DRV8301_PWMSet(*pdrv8301);
	    }

	    if(fx == 4){
	        pdrv8301->PWMA = 500;
	        pdrv8301->PWMB = 6000;
	        pdrv8301->PWMC = 500;
	        DRV8301_PWMSet(*pdrv8301);
	    }

	    DRV8301_SenseGet(pdrv8301);
	    DRV8301_Display(*pdrv8301);
	    OLED_ShowInt(121, 6*8, fx, 1);
	    OLED_Refresh_fix(121, 127, 6);
	}
}

