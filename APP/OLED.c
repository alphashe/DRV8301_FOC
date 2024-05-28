/*
 * OLED.c
 *
 *  Created on: 2024年3月29日
 *      Author: alpha
 */
#include "OLED.h"
#include "oledchar.h"
Uint16 OLED_GRAM[128][8];

void OLED_Init(void){
    EALLOW;     //all operations on registers need to be Add
    GpioCtrlRegs.SCLMUX.bit.SCL = GPIOMUX_GPIO;
    GpioCtrlRegs.SCLDIR.bit.SCL = GPIODIR_OUTPUT;   //OUTPUT
    GpioCtrlRegs.SCLPUD.bit.SCL = GPIOPUD_PULLUP;   //PULLUP

    GpioCtrlRegs.SDAMUX.bit.SDA = GPIOMUX_GPIO;
    GpioCtrlRegs.SDADIR.bit.SDA = GPIODIR_OUTPUT;
    GpioCtrlRegs.SDAPUD.bit.SDA = GPIOPUD_PULLUP;

    SCL_LOW;
    SDA_LOW;
    EDIS;

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x30,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//  Page Addressing Mode
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD);
   /*
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
    OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
    OLED_WR_Byte(0x81,OLED_CMD); // contract control
    OLED_WR_Byte(0xFF,OLED_CMD);//--128
    OLED_WR_Byte(0xA0,OLED_CMD);//！！！！设定x方向，0xA0选择正常方向（0xA1为反方向）
    OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
    OLED_WR_Byte(0xC0,OLED_CMD);//！！！！设定Y方向，0xC8正常反向，0xC0为正常方向
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
    OLED_WR_Byte(0x00,OLED_CMD);//

    OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
    OLED_WR_Byte(0x80,OLED_CMD);//

    OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05,OLED_CMD);//

    OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    OLED_WR_Byte(0xF1,OLED_CMD);//

    OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
    OLED_WR_Byte(0x12,OLED_CMD);//

    OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
    OLED_WR_Byte(0x30,OLED_CMD);//

    OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
    OLED_WR_Byte(0x14,OLED_CMD);//

    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
*/

}

//send one byte, mode:0:command, 1:data
void OLED_WR_Byte(Uint16 dat,Uint16 mode){
    I2C_Start();
    Send_Byte(0x78);    //OLED address
    I2C_WaitAck();
    if(mode)
        Send_Byte(0x40);    //will write a data
    else
        Send_Byte(0x00);    //will write a command
    I2C_WaitAck();
    Send_Byte(dat);
    I2C_WaitAck();
    I2C_Stop();
}

void I2C_Start(void){
    SDA_HIGH;
    IIC_Delay;
    SCL_HIGH;
    IIC_Delay;
    SDA_LOW;
    IIC_Delay;
    SCL_LOW;
    IIC_Delay;
}

void I2C_Stop(void){
    SDA_LOW;
    IIC_Delay;
    SCL_HIGH;
    IIC_Delay;
    SDA_HIGH;
    IIC_Delay;
    IIC_Delay;
}

void Send_Byte(Uint16 dat){
    Uint16 i;
    for(i=0;i<8;i++){
        if(dat&0x80)//write High bit first
            SDA_HIGH;
        else
            SDA_LOW;
        IIC_Delay;
        SCL_HIGH;
        IIC_Delay;
        SCL_LOW;// one clock
        dat<<=1;
        IIC_Delay;
        //SDA_LOW;
       // IIC_Delay;
    }
}

void I2C_WaitAck(void){
    SDA_HIGH;
    IIC_Delay;
    SCL_HIGH;
    IIC_Delay;
    SCL_LOW;
    IIC_Delay;
}

void OLED_Clear(void){
    Uint16 i,n;
    for(i=0;i<8;i++){
        for(n=0;n<128;n++)
            OLED_GRAM[n][i]=0;//clear all data
    }
    OLED_Refresh();//
}

void OLED_Refresh(void){
    Uint16 i,n;
    for(i=0;i<8;i++){
        OLED_WR_Byte(0xb0+i,OLED_CMD); //set line starting address
        OLED_WR_Byte(0x00,OLED_CMD);   //set low column starting address
        OLED_WR_Byte(0x10,OLED_CMD);   //set high column starting address
        I2C_Start();
        Send_Byte(0x78);
        I2C_WaitAck();
        Send_Byte(0x40);    //will send data later
        I2C_WaitAck();
        for(n=0;n<128;n++){
            Send_Byte(OLED_GRAM[n][i]);
            I2C_WaitAck();
        }
        I2C_Stop();
    }
}

// x:0~128 y:0~8
void OLED_Refresh_fix(Uint16 x1, Uint16 x2, Uint16 y){
    Uint16 n=x1;
    OLED_WR_Byte(0xb0+y,OLED_CMD); //set line starting address  8 line 128 column  0xb2
    OLED_WR_Byte(0x00|(n&0x0f),OLED_CMD);   //starting address low 4bit   0x00~0x0f  0x00
    n=n>>4;
    OLED_WR_Byte(0x10|(n&0x0f),OLED_CMD);   //starting address high 4bit  0x10~0x17   eg: 0x0a, 0x1b -> start column: 0xab

    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    Send_Byte(0x40);    //will send data later
    I2C_WaitAck();
    for(n=x1;n<x2;n++){
        Send_Byte(OLED_GRAM[n][y]);
        I2C_WaitAck();
    }
    I2C_Stop();
}

void OLED_DrawPoint(Uint16 x,Uint16 y,Uint16 t)
{
    Uint16 i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    if(t){OLED_GRAM[x][i]|=n;}
    else
    {
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];
        OLED_GRAM[x][i]|=n;
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    }
}

void OLED_ShowChar(Uint16 x,Uint16 y,char chr, Uint16 mode)
{
    Uint16 i,m,temp,size2,chr1;
    Uint16 y0=y;//x0=x,
    //if(size1==8)size2=6;
    size2 = 6;
    chr1 = chr-' ';  //calculate the offset value
    for(i=0; i<size2; i++){
        temp = asc2_0806[chr1][i]; //call 0806 font
        for(m=0;m<8;m++){
            if(temp&0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp>>=1;
            y++;
        }
        x++;
        //if((size1!=8)&&((x-x0)==size1/2))
        //{x=x0;y0=y0+8;}
        y=y0;
  }
}

void OLED_ShowString(Uint16 x,Uint16 y,char *chr,Uint16 mode)
{
    while((*chr>=' ')&&(*chr<='~'))//
    {
        OLED_ShowChar(x,y,*chr,mode);
        x += 6;
        chr++;
  }
}

void OLED_ShowInt(Uint16 x, Uint16 y, Uint32 num, Uint16 mode){
    Uint16 bitnum=0;
    Uint32 temp = num;
    Uint16 i=0;
    while(temp){
        bitnum++;
        temp=temp/10;
    }
    for(i=0;i<bitnum;i++){
        OLED_ShowChar(x+6*bitnum-(6*i)-6, y, num%10 +'0',  1);
        num=num/10;
    }

}

void OLED_ShowHex(Uint16 x, Uint16 y, Uint16 hex, Uint16 mode){
    Uint16 temp=hex, i=0;
    for(i=0;i<4;i++){
        if(temp%16<10)
            OLED_ShowChar(x+18-(6*i), y, temp%16+'0', mode);
        else
            OLED_ShowChar(x+18-(6*i), y, temp%16-10+'A', mode);

        temp=temp/16;
    }
}

void OLED_ShowPicture(Uint16 x,Uint16 y,Uint16 sizex,Uint16 sizey,U8 BMP[],Uint16 mode)
{
    Uint16 j=0;
    Uint16 i,n,temp,m;
    Uint16 x0=x,y0=y;
    sizey=sizey/8+((sizey%8)?1:0);
    for(n=0;n<sizey;n++){
        for(i=0;i<sizex;i++){
            temp=BMP[j];
            j++;
            for(m=0;m<8;m++){
                if(temp&0x01)
                    OLED_DrawPoint(x,y,mode);
                else
                    OLED_DrawPoint(x,y,!mode);
                temp>>=1;
                y++;
            }
            x++;
            if((x-x0)==sizex){
                x=x0;
                y0=y0+8;
            }
            y=y0;
        }
     }
}
