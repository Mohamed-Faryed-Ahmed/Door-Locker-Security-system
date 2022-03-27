
/* ***************************************************************************
 *
 * Name : hmi_ecu.h
 *
 * Author: Mohamed Faryed
 *
 * Description:
 *
 * Created on: March 5, 2022
 *
 ******************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#include "hal/lcd/lcd.h"
#include "hal/keypad/keypad.h"

#include "mcal/uart/uart.h"
#include "mcal/timer/timer.h"
#include "mcal/gpio/gpio.h"
/******************************************************************************************************
 *                                  Definition                                                        *
 ******************************************************************************************************/
#define MC2_READY 0xff
#define MC1_READY 0xf0

/******************************************************************************************************
 *                                 Global Definition                                                  *
 ******************************************************************************************************/
volatile uint8 key[5]={20,20,20,20,20};
volatile uint8 g_count_value =0;
volatile uint8 stepONE_ack1 = 0;
volatile uint8 stepONE_ack2 = 0;
volatile uint8 val=0;

/******************************************************************************************************
 *                                 Function Prototype                                                 *
 ******************************************************************************************************/
void supONE(uint8 step);
void stepONE(void);
void stepONETWO(void);
void TWO(void);
void TIME_delay(void);

#endif /* MAIN_H_ */
