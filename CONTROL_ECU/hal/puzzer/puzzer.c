/* ***************************************************************************
 *
 *Module : Buzzer
 *
 * Name  :  puzzer.c
 *
 * Author: Mohamed Faryed
 *
 * Description: source file of buzzer driver for AVR
 *
 * Created on: Feb 28, 2022
 *
 ******************************************************************************/

#include"../../mcal/gpio/gpio.h"
#include"puzzer.h"


/*
 * Description:
 *             make the pin connected to buzzer output
 *             make the buzzer OFF
 ******************************************************************************/
void BUZZER_init (void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

/*
 * Description:
 *             make the buzzer ON
 ******************************************************************************/
void BUZZER_on (void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}

/*
 * Description:
 *             make the buzzer OFF
 ******************************************************************************/
void BUZZER_off (void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

