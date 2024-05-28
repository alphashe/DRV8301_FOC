/*
 * SPI.h
 *
 *  Created on: 2024Äê4ÔÂ16ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_SPI_H_
#define APP_INCLUDE_SPI_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Global.h"

void SPI_Init(void);
Uint16 SPIA_SendReciveData(Uint16 dat);

#endif /* APP_INCLUDE_SPI_H_ */
