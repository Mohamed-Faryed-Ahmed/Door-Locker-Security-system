/* ***************************************************************************
 *
 *Module : Buzzer
 *
 * Name  :  puzzer.h
 *
 * Author: Mohamed Faryed
 *
 * Description: header file of buzzer driver for AVR
 *
 * Created on: Feb 28, 2022
 *
 ******************************************************************************/


#ifndef HAL_BUZZER_PUZZER_H_
#define HAL_BUZZER_PUZZER_H_

#include"../../std_types.h"
/*********************************************************************************************
 *                                Definition                                                 *
 *********************************************************************************************/
#define BUZZER_PORT_ID  PORTB_ID
#define BUZZER_PIN_ID   PIN7_ID
/*********************************************************************************************
 *                                prototype function                                         *
 *********************************************************************************************/

/*
 * Description:
 *             make the pin connected to buzzer output
 *             make the buzzer OFF
 ******************************************************************************/
void BUZZER_init (void);

/*
 * Description:
 *             make the buzzer ON
 ******************************************************************************/
void BUZZER_on (void);

/*
 * Description:
 *             make the buzzer OFF
 ******************************************************************************/
void BUZZER_off (void);

#endif /* HAL_BUZZER_PUZZER_H_ */
