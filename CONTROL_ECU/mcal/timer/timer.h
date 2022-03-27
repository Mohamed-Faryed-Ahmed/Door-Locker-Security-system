 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer AVR driver
 *
 * Author: Mohamed Faryed
 *
 *******************************************************************************/

#ifndef MCAL_TIMER_TIMER_H_
#define MCAL_TIMER_TIMER_H_


#include "../../std_types.h"



#define F_CPU 8000000UL   //8MHz Clock frequency
/*******************************************************************************
 *                      TYPE Definition                                        *
 *******************************************************************************/
typedef enum
{
	T0=0,T1=1,T2=2,TA1=4,TB1=6
}Timer_X;
typedef enum
{
	C0=0,C1=1 ,CA1=4,CB1=6
}Counter_X;

typedef enum
{
	PWM0=0,PWM1A=1,PWM1B=2,PWM2=3
}Pwm_X;

typedef enum
{
	DIS_TIMER=0,NO_PRE=1,PRE8=2,PRE64=3,PRE256=4,PRE1024=5
}Timer_Pre;

typedef enum
{
	DISCONECT=0,TOGGLE=1,CLEAR=2,SET=3
}Compare_Ocx_State;

typedef enum
{
	COUNTER_FALLING=6,COUNTER_RISING=7
}Edge_State;

typedef enum
{
	NON_INVERTING=2,INVERTING=3
}Pwm_State;

typedef struct
{
	uint16             timer_init     ;
	uint16             timer_compare  ;
	Timer_Pre          pre_timer    :3;
	Timer_X            timer_x      :3;
	Compare_Ocx_State  ocx          :2;

}TIMER_Config;

typedef struct
{
	uint16             counter_init       ;
	uint16             counter_compare    ;
	Edge_State         edge_state       :3;
	Counter_X          counter_x        :3;
	Compare_Ocx_State  ocx              :2;

}COUNTER_Config;

typedef struct
{
	uint16             pwm_clk     ;
	uint16             pwm_com     ;
	uint8              pwm_duty    ;
	Pwm_X              pwm_x     :2;
	Pwm_State          pwm_state :2;

}PWM_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TIMER_comparInit(TIMER_Config *s_config_ptr,void (*callBack)(void));
void TIMER_NormalInit(TIMER_Config *s_config_ptr,void (*callBack)(void));

void COUNTER_comparInit(COUNTER_Config *s_config_ptr,void (*callBack)(void));
void COUNTER_NormalInit(COUNTER_Config *s_config_ptr,void (*callBack)(void));

void PWM_init(PWM_Config *s_config_ptr);

#endif /* MCAL_TIMER_TIMER_H_ */
