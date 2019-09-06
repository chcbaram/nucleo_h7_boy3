/*
 * hw_def.h
 *
 *  Created on: 2019. 6. 14.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_


#include "def.h"
#include "bsp.h"



#define _USE_HW_MICROS
#define _USE_HW_MILLIS
#define _USE_HW_DELAY
#define _USE_HW_SDRAM
#define _USE_HW_RTOS


#define _USE_HW_SWTIMER
#define      HW_SWTIMER_MAX_CH      8

#define _USE_HW_LED
#define      HW_LED_MAX_CH          4

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1
#define      HW_USE_CMDIF_I2C       1

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       10
#define      HW_USE_CMDIF_BUTTON    1

#define _USE_HW_PWM
#define      HW_PWM_MAX_CH          1

#define _USE_HW_CMDIF
#define      HW_CMDIF_LIST_MAX              16
#define      HW_CMDIF_CMD_STR_MAX           16
#define      HW_CMDIF_CMD_BUF_LENGTH        128





#define _DEF_HW_BTN_LEFT            0
#define _DEF_HW_BTN_RIGHT           1
#define _DEF_HW_BTN_UP              2
#define _DEF_HW_BTN_DOWN            3
#define _DEF_HW_BTN_A               4
#define _DEF_HW_BTN_B               5
#define _DEF_HW_BTN_X               6
#define _DEF_HW_BTN_Y               7
#define _DEF_HW_BTN_MENU            8
#define _DEF_HW_BTN_HOME            9


#endif /* SRC_HW_HW_DEF_H_ */
