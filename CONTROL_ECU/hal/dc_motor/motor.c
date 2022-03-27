/* ***************************************************************************************************************
 * module : motor
 *
 * Name   : motor.c
 *
 * Author: Mohamed Faryed
 *
 * Description: Source file for motor driver
 *
 * Created on: Feb 23, 2022
 *
 ******************************************************************************************************************/

#include "motor.h"
#include <avr/io.h>
#include "../../mcal/gpio/gpio.h"
#include "../../mcal/timer/timer.h"

/********************************************************************************************************
 *                                 Function Definition
 *******************************************************************************************************/
/*
*Description:
*            The Function responsible for setup the direction for the two
*            motor pins through the GPIO driver.
*            Stop at the DC-Motor at the beginning through the GPIO driver.
********************************************************************************************************/
void DcMotor_Init(void)
{
	GPIO_setupPinDirection(MOTOR_PORT_ID1,MOTOR_PIN_ID1,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID2,MOTOR_PIN_ID2,PIN_OUTPUT);
	GPIO_writePin(MOTOR_PORT_ID1,MOTOR_PIN_ID1,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID2,MOTOR_PIN_ID2,LOGIC_LOW);
}

/*
* Description:
*              The function responsible for rotate the DC Motor CW/ or A-CW or
*              stop the motor based on the state input state value.
*              Send the required duty cycle to the PWM driver based on the required speed value.
*********************************************************************************************************/
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	if(state==CW)
	{
		PWM_Config s_pwm_config={500,0,speed,PWM0,NON_INVERTING};
		PWM_init(&s_pwm_config);

		GPIO_writePin(MOTOR_PORT_ID1,MOTOR_PIN_ID1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID2,MOTOR_PIN_ID2,LOGIC_HIGH);
	}
	else if(state==A_CW)
	{
		PWM_Config s_pwm_config={500,0,speed,PWM0,NON_INVERTING};
		PWM_init(&s_pwm_config);

		GPIO_writePin(MOTOR_PORT_ID1,MOTOR_PIN_ID1,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID2,MOTOR_PIN_ID2,LOGIC_LOW);
	}
	else if(state==STOP)
		{
		    PWM_Config s_pwm_config={500,0,0,PWM0,NON_INVERTING};
		    PWM_init(&s_pwm_config);

			GPIO_writePin(MOTOR_PORT_ID1,MOTOR_PIN_ID1,LOGIC_LOW);
			GPIO_writePin(MOTOR_PORT_ID2,MOTOR_PIN_ID2,LOGIC_LOW);
		}
}
