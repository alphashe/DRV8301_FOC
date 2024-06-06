/*
 * SPI.c
 *
 *  Created on: 2024Äê4ÔÂ16ÈÕ
 *      Author: alpha
 */

#include "SPI.h"


void SPI_Init(void){
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;
    EDIS;

    InitSpiaGpio();

    SpiaRegs.SPIFFTX.all = 0xE040;  //enable FIFO clear transmit interrupt
    SpiaRegs.SPIFFRX.all = 0x204F;  //enable FIFO received 16 depth
    SpiaRegs.SPIFFCT.all = 0x0;     //clear FIFO count

    SpiaRegs.SPICCR.all = 0x000F;   //reset SPI, transmit at up edge, receive at down edge, 16 bit data
    SpiaRegs.SPICTL.all = 0x0006;   //no phase delay, master mode
    SpiaRegs.SPIBRR = 0x007F;       //determine SPICLK
    SpiaRegs.SPICCR.all = 0x008F;   // exit reset status
    SpiaRegs.SPIPRI.bit.FREE = 1;   //free run
}

Uint16 SPIA_SendReciveData(Uint16 dat){
    SpiaRegs.SPITXBUF = dat;

    while(SpiaRegs.SPIFFRX.bit.RXFFST !=1);
    return SpiaRegs.SPIRXBUF;
}
