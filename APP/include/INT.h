/*
 * INT.h
 *
 *  Created on: 2024��1��25��
 *      Author: alpha
 *  �ⲿ�ж�IOֻ��0-63
 *  only the GPIO0-63 could be used as external interrupt input
 */

#ifndef APP_INCLUDE_INT_H_
#define APP_INCLUDE_INT_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Global.h"

void EXTI1_Init(void);
interrupt void EXTI1_KEY(void);



#endif /* APP_INCLUDE_INT_H_ */
