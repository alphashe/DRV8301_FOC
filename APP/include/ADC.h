/*
 * ADC.h
 *
 *  Created on: 2024Äê1ÔÂ25ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_ADC_H_
#define APP_INCLUDE_ADC_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

void ADC_Init(void);

Uint16 Read_ADCValueResult0(void);
Uint16 Read_ADCValueResult1(void);
Uint16 Read_ADCValueResult2(void);
Uint16 Read_ADCValueResult3(void);
Uint16 Read_ADCValueResult4(void);
Uint16 Read_ADCValueResult5(void);
Uint16 Read_ADCValueResult6(void);
Uint16 Read_ADCValueResult7(void);
Uint16 Read_ADCValueResult8(void);
Uint16 Read_ADCValueResult9(void);
Uint16 Read_ADCValueResult10(void);
Uint16 Read_ADCValueResult11(void);
Uint16 Read_ADCValueResult12(void);
Uint16 Read_ADCValueResult13(void);
Uint16 Read_ADCValueResult14(void);
Uint16 Read_ADCValueResult15(void);

void Read_ADCValueResultAll(Uint16 *result);

#endif /* APP_INCLUDE_ADC_H_ */
