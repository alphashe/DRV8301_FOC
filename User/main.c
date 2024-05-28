
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
//定义变量


void main(void){
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();

    InitSysCtrl();
    //DINT;
    Uint16 fx = 0, data_recive=0, data_trans=0;
    Uint16 i=0;
    Uint32 realtime=0;
    Uint32 shownum=0;
    struct struct_DRV8301 *p8301;
	InitPieCtrl();
	IER = 0x0000;   //disable all interrupt
	IFR = 0x0000;   //clear all interrupt flag
	InitPieVectTable();

	TIM0_Init(150-1, 700000-1);    //150MHz 150 divide ->1uS, count 700*1000  ->700mS
	DELAY_US(100);
    LED_Init();
    //K2_Init();
    DELAY_US(1000000);

    OLED_Init();
    OLED_Clear();

    OLED_ShowPicture(34, 2, 60, 60, BtPic, 1);
    OLED_Refresh();

    DELAY_US(1000000);
    p8301 = DRV8301_Init();
   // ADC_Init();
   // Key_Init();
   // EXTI1_Init();

    //EPWM1_Init(5000);
   // EPWM2_Init(5000);
   // EPWM3_Init(5000);

  //  EPwm1A_SetCompare(1000);
   // EPwm1B_SetCompare(1000);
  //  EPwm2A_SetCompare(1000);
  //  EPwm2B_SetCompare(1000);
   // EPwm3A_SetCompare(1000);
  //  EPwm3B_SetCompare(1000);

    //Init_Key_Time();
	while(1){

	    DELAY_US(15*1000);
	    OLED_ShowInt(121, 48, fx, 1);
	    OLED_Refresh_fix(121, 127, 6);
	    realtime=CpuTimer0Regs.TIM.all;
	    OLED_ShowInt(0, 40, realtime, 1);
	    OLED_Refresh_fix(0, 127, 5);
	   fx++;
	   // if(fx==500000){
	   //     LED4_TOGGLE;
	   // }
	    if(fx==9){

	        fx=0;
	    }
	}
 }

