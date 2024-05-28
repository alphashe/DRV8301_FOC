/*
 * EPWM.h
 *
 *  Created on: 2024Äê1ÔÂ26ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_EPWM_H_
#define APP_INCLUDE_EPWM_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

void EPWM6_Init(Uint16 tbprd);
void EPwm6A_SetCompare(Uint16 val);
void EPwm6B_SetCompare(Uint16 val);

void EPWM5_Init(Uint16 tbprd);
void EPwm5A_SetCompare(Uint16 val);
void EPwm5B_SetCompare(Uint16 val);

void EPWM1_Init(Uint16 tbprd);
void EPwm1A_SetCompare(Uint16 val);
void EPwm1B_SetCompare(Uint16 val);

void EPWM2_Init(Uint16 tbprd);
void EPwm2A_SetCompare(Uint16 val);
void EPwm2B_SetCompare(Uint16 val);

void EPWM3_Init(Uint16 tbprd);
void EPwm3A_SetCompare(Uint16 val);
void EPwm3B_SetCompare(Uint16 val);



#endif /* APP_INCLUDE_EPWM_H_ */
