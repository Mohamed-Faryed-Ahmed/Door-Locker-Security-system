/* ***************************************************************************************************************
 * module : motor
 *
 * Name   : motor.h
 *
 * Author: Mohamed Faryed
 *
 * Description: Header file for motor driver
 *
 * Created on: Feb 23, 2022
 *
 ******************************************************************************************************************/

#ifndef HAL_DC_MOTOR_MOTOR_H_
#define HAL_DC_MOTOR_MOTOR_H_


#include"../../std_types.h"

/********************************************************************************************************
 *                                  Definition
 *******************************************************************************************************/

#define MOTOR_PORT_ID1        PORTB_ID
#define MOTOR_PORT_ID2        PORTB_ID
#define MOTOR_PIN_ID1         PIN0_ID
#define MOTOR_PIN_ID2         PIN1_ID

/********************************************************************************************************
 *                                 Type Definition
 *******************************************************************************************************/

typedef enum{
	CW,A_CW,STOP
}DcMotor_State;

/********************************************************************************************************
 *                                 Function Proto_type
 *******************************************************************************************************/

/*
*Description:
*            The Function responsible for setup the direction for the two
*            motor pins through the GPIO driver.
*            Stop at the DC-Motor at the beginning through the GPIO driver.
********************************************************************************************************/
void DcMotor_Init(void);

/*
* Description:
*              The function responsible for rotate the DC Motor CW/ or A-CW or
*              stop the motor based on the state input state value.
*              Send the required duty cycle to the PWM driver based on the required speed value.
*********************************************************************************************************/
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* HAL_DC_MOTOR_MOTOR_H_ */

