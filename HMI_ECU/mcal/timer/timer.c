 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: source file for the Timer AVR driver
 *
 * Author: Mohamed Faryed
 *
 *******************************************************************************/

#include "timer.h"
#include "../gpio/gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>


static volatile uint16 timer0_re_count  = 0;

static volatile void (*TIMER0_callBack)(void)  = NULL_PTR;

/*******************************************************************************
 *                      ISR Definition                                         *
 *******************************************************************************/

/* Interrupt Service Routine for timer0 compare mode */
ISR(TIMER0_COMP_vect)
{
	if(TIMER0_callBack != NULL_PTR)
	{
		TCNT0 = 0;
		(*TIMER0_callBack)();
	}
}


/* Interrupt Service Routine for timer0 Normal mode */
ISR(TIMER0_OVF_vect)
{
	if(TIMER0_callBack != NULL_PTR)
	{
		TCNT0 = (uint8)timer0_re_count;
		(*TIMER0_callBack)();
	}
}
/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
void TIMER_comparInit(TIMER_Config *s_config_ptr,void (*callBack)(void))
{
	if((s_config_ptr->timer_x)==T0)
	{
		TCNT0 = 0;                                   /* Set Timer initial value to 0*/
		OCR0  = (uint8)s_config_ptr->timer_compare;  /* Set Compare Value*/
		TIMSK = (TIMSK & 0xfc) | (1<<OCIE0);         /* Enable Timer0 Compare Interrupt*/
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC0=1
		 * 2. CTC Mode WGM01=1 & WGM00=0
		 * 3. s_config_ptr->ocx     : status of OC0
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR0 = (1<<FOC0) | (1<<WGM01) | ((s_config_ptr->ocx)<<COM00) | ((s_config_ptr->pre_timer)<<CS00);
		TIMER0_callBack= callBack;

		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
			GPIO_writePin(PORTB_ID,PIN3_ID,LOGIC_LOW);
		}
	}
}



void TIMER_NormalInit(TIMER_Config *s_config_ptr,void (*callBack)(void))
{
	if((s_config_ptr->timer_x)==T0)
	{
		timer0_re_count=(uint8)s_config_ptr->timer_init;
		TCNT0 = (uint8)timer0_re_count;              /* Set Timer initial value */
		TIMSK = (TIMSK & 0xfc) | (1<<TOIE0);         /* Enable Timer0 Overflow Interrupt*/
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC0=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. s_config_ptr->ocx     : status of OC0
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR0 = (1<<FOC0) | ((s_config_ptr->pre_timer)<<CS00);
		TIMER0_callBack= callBack;
	}
}

void COUNTER_comparInit(COUNTER_Config *s_config_ptr,void (*callBack)(void))
{
	if((s_config_ptr->counter_x)==C0)
	{
		TCNT0 = 0;                                   /* Set Timer initial value to 0*/
		OCR0  = s_config_ptr->counter_compare;       /* Set Compare Value*/
		TIMSK = (TIMSK & 0xfc) | (1<<OCIE0);         /* Enable Timer0 Compare Interrupt*/
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC0=1
		 * 2. CTC Mode WGM01=1 & WGM00=0
		 * 3. s_config_ptr->ocx        : status of OC0
		 * 4. s_config_ptr->edge_state : External clock
		 */
		TCCR0 = (1<<FOC0) | (1<<WGM01) | ((s_config_ptr->ocx)<<COM00) | ((s_config_ptr->edge_state)<<CS00);
		TIMER0_callBack= callBack;

		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
			GPIO_writePin(PORTB_ID,PIN3_ID,LOGIC_LOW);
		}
	}

}


void COUNTER_NormalInit(COUNTER_Config *s_config_ptr,void (*callBack)(void))
{
/*********************************************************************************************************/
	if((s_config_ptr->counter_x)==C0)
	{
		TCNT0 = (uint8)s_config_ptr->counter_init;   /* Set Timer initial value to 0*/
		TIMSK = (TIMSK & 0xfc) | (1<<TOIE0);         /* Enable Timer0 Compare Interrupt*/
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC0=1
		 * 2. CTC Mode WGM01=0 & WGM00=0
		 * 3. s_config_ptr->ocx        : status of OC0
		 * 4. s_config_ptr->edge_state : External clock
		 */
		TCCR0 = (1<<FOC0) | ((s_config_ptr->edge_state)<<CS00);
		timer0_re_count=(uint8)s_config_ptr->counter_init;
		TIMER0_callBack= callBack;
	}
/*********************************************************************************************************/
}


void PWM_init(PWM_Config *s_config_ptr)
{
/*********************************************************************************************************/
	if((s_config_ptr->pwm_x)==PWM0)
	{
		TCNT0 = 0;                                        /* Set Timer initial value to 0*/
		uint8 com = 50;
		if((s_config_ptr->pwm_duty) >= 100)
		{
			s_config_ptr->pwm_duty=100;
			com= (uint8)(((float)(s_config_ptr->pwm_duty)/100)*255);
		}
		else
		{
			com= (uint8)(((float)(s_config_ptr->pwm_duty)/100)*256);
		}
		OCR0  = com ;                                     /* Set Compare Value*/
		TIMSK = (TIMSK & 0xfc) ;                          /*disable Timer0 Interrupt*/

		GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
		GPIO_writePin(PORTB_ID,PIN3_ID,LOGIC_LOW);

		/* Configure timer0 control register
		 * 1. PWM mode FOC0=0
		 * 2. PWM Mode WGM01=1 & WGM00=1
		 * 3. s_config_ptr->pwm_state :  inverting or non inverting
		 * 4. s_config_ptr->pwm_clk   :  timer frequancy
		 */

		uint16 pre = (uint16)((F_CPU)/((float)(s_config_ptr->pwm_clk)*256));
		if((pre > 0) && (pre <= 4))
		{
			pre =1;
		}
		else if((pre > 4) && (pre <= 10))
		{
			pre =2;
		}
		else if((pre > 10) && (pre <= 70))
		{
			pre =3;
		}
		else if((pre > 70) && (pre <= 300))
		{
			pre =4;
		}
		else if((pre > 300) && (pre <= 1024))
		{
			pre =5;
		}
		else
		{
			pre = 0;
		}

		TCCR0 = (1<<WGM00) | (1<<WGM01) | ((s_config_ptr->pwm_state)<<COM00) | ((pre)<<CS00);
	}

/*********************************************************************************************************/

}
