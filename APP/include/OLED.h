/*
 * OLED.h
 *
 *  Created on: 2024Äê3ÔÂ29ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_OLED_H_
#define APP_INCLUDE_OLED_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Global.h"


#define SCL GPIO21
#define SCLMUX GPAMUX2
#define SCLDIR GPADIR
#define SCLPUD GPAPUD

#define SDA GPIO20
#define SDAMUX GPAMUX2
#define SDADIR GPADIR
#define SDAPUD GPAPUD

#define SCL_HIGH    GpioDataRegs.GPASET.bit.SCL=1
#define SCL_LOW     GpioDataRegs.GPACLEAR.bit.SCL=1
#define SDA_HIGH    GpioDataRegs.GPASET.bit.SDA=1
#define SDA_LOW     GpioDataRegs.GPACLEAR.bit.SDA=1

#define OLED_CMD    0 // write command
#define OLED_DATA   1 //write data
#define IIC_Delay   DELAY_US(2)

void OLED_Init(void);
void OLED_WR_Byte(Uint16 dat,Uint16 mode);
void I2C_Start(void);
void I2C_Stop(void);
void Send_Byte(Uint16 dat);
void I2C_WaitAck(void);
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_Refresh_fix(Uint16 x1, Uint16 x2, Uint16 y);
void OLED_DrawPoint(Uint16 x,Uint16 y,Uint16 t);
void OLED_ShowChar(Uint16 x,Uint16 y,char chr,Uint16 mode);
void OLED_ShowString(Uint16 x,Uint16 y,char *chr,Uint16 mode);
void OLED_ShowInt(Uint16 x, Uint16 y, Uint32 num, Uint16 mode);
void OLED_ShowHex(Uint16 x, Uint16 y, Uint16 hex, Uint16 mode);
void OLED_ShowPicture(Uint16 x,Uint16 y,Uint16 sizex,Uint16 sizey,U8 BMP[],Uint16 mode);


#endif /* APP_INCLUDE_OLED_H_ */
