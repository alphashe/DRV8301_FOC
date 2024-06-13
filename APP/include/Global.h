/*
 * Global.h
 *
 *  Created on: 2024Äê3ÔÂ26ÈÕ
 *      Author: alpha
 */

#ifndef APP_INCLUDE_GLOBAL_H_
#define APP_INCLUDE_GLOBAL_H_

#define mainCLK 150
#define PI 3.14159


////////////////GPIO/////////////
enum GPIOMUX{
    GPIOMUX_GPIO,
};
enum GPIODIR{
    GPIODIR_INPUT,
    GPIODIR_OUTPUT
};
enum GPIOPUD{
    GPIOPUD_PULLUP,
    GPIOPUD_FLOAT
};

enum POLARITY{
    POLARITY_FALLING,
    POLARITY_RISING,
    POLARITY_BOTH=3
};


/////////////////////TIMER///////////////////
enum TSS{
    TSS_START,
    TSS_STOP,
};

enum TRB{
    TRB_RELOAD=1
};


#endif /* APP_INCLUDE_GLOBAL_H_ */
