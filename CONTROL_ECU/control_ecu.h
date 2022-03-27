/* ***************************************************************************
 *
 * Name : micro_config.h
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

#include"micro_config.h"

#include "hal/puzzer/puzzer.h"
#include "hal/external_eeprom_twi/external_eeprom.h"

#include "mcal/uart/uart.h"
#include "mcal/twi/twi.h"
#include "mcal/timer/timer.h"
#include "mcal/gpio/gpio.h"

#include "hal/dc_motor/motor.h"
#include "hal/puzzer/puzzer.h"

/******************************************************************************************************
 *                                       definition                                                   *
 ******************************************************************************************************/
#define MC2_READY 0xff
#define MC1_READY 0xf0

/******************************************************************************************************
 *                                      Global definition                                             *
 ******************************************************************************************************/
volatile uint8 password[5] ;
volatile uint8 g_count_value= 0;
volatile uint8 stepONE_ack1 = 0;
volatile uint8 stepONE_ack2 = 0;
volatile uint8 val = 0;

/******************************************************************************************************
 *                                      Prototype function                                            *
 ******************************************************************************************************/

void stepONE(void);
void stepONETWO(void);
void stepTWO(void);
void stepTHREE(void);

void pass_WRITE(uint8 *pass);
void pass_READ(void);
void TIME_delay(void);

#endif /* MAIN_H_ */
