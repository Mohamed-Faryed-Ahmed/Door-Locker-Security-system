 /***************************************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: source file for the Timer AVR driver
 *
 * Author: Mohamed Faryed
 *
 ***************************************************************************************************/

#include "timer.h"
#include "../gpio/gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*****************************************************************************************************
 *                      Global Definition                                                            *
 *****************************************************************************************************/
static volatile uint16 timer0_re_count  = 0;

static volatile uint16 timer1_re_count  = 0;

static volatile uint16 timer2_re_count  = 0;


static volatile void (*TIMER0_callBack)(void)  = NULL_PTR;

static volatile void (*TIMER1_callBack)(void)  = NULL_PTR;
static volatile void (*TIMER1A_callBack)(void) = NULL_PTR;
static volatile void (*TIMER1B_callBack)(void) = NULL_PTR;

static volatile void (*TIMER2_callBack)(void)  = NULL_PTR;

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




/* Interrupt Service Routine for timer1A compare mode ***************************************/
ISR(TIMER1_COMPA_vect)
{
	if(TIMER1A_callBack != NULL_PTR)
	{
		TCNT1 = 0;
		(*TIMER1A_callBack)();
	}
}

/* Interrupt Service Routine for timer1B compare mode */
ISR(TIMER1_COMPB_vect)
{
	if(TIMER1B_callBack != NULL_PTR)
	{
		TCNT1 = 0;
		(*TIMER1B_callBack)();
	}
}
/*************************************************************************************************/



/* Interrupt Service Routine for timer2 compare mode */
ISR(TIMER2_COMP_vect)
{
	if(TIMER2_callBack != NULL_PTR)
	{
		TCNT2 = 0;
		(*TIMER2_callBack)();
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

/* Interrupt Service Routine for timer1 Normal mode */
ISR(TIMER1_OVF_vect)
{
	if(TIMER1_callBack != NULL_PTR)
	{
		TCNT1 = (uint8)timer1_re_count;
		(*TIMER1_callBack)();
	}
}

/* Interrupt Service Routine for timer2 Normal mode */
ISR(TIMER2_OVF_vect)
{
	if(TIMER2_callBack != NULL_PTR)
	{
		TCNT2 = (uint8)timer2_re_count;
		(*TIMER2_callBack)();
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

	if( ((s_config_ptr->timer_x) == T1) || ((s_config_ptr->timer_x) == TA1)  )
	{
		TCNT1 = 0;                                   /* Set Timer initial value to 0*/
		OCR1A = s_config_ptr->timer_compare;         /* Set Compare Value*/
		TIMSK = (TIMSK & 0xc3) | (1<<OCIE1A);        /* Enable Timer1A Compare Interrupt*/
		/* Configure timer1 control register
		 * 1. Non PWM mode FOC1A=1 FOC1B=1
		 * 2. CTC Mode WGM13=0 WGM12=1 WGM11=0 & WGM10=0
		 * 3. s_config_ptr->ocx     : status of OC1A
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B) | ((s_config_ptr->ocx)<<COM1A0);

		TCCR1B = (1<<WGM12) | ((s_config_ptr->pre_timer)<<CS10);
		TIMER1A_callBack= callBack;

		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_OUTPUT);
			GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_LOW);
		}
	}

	if((s_config_ptr->timer_x) == TB1 )
	{
		TCNT1 = 0;                                   /* Set Timer initial value to 0*/
		OCR1A = s_config_ptr->timer_compare;         /* Set Compare Value*/
		TIMSK = (TIMSK & 0xc3) | (1<<OCIE1B);        /* Enable Timer1A Compare Interrupt*/
		/* Configure timer1 control register
		 * 1. Non PWM mode FOC1A=1 FOC1B=1
		 * 2. CTC Mode WGM13=0 WGM12=1 WGM11=0 & WGM10=0
		 * 3. s_config_ptr->ocx     : status of OC1A
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B) | ((s_config_ptr->ocx)<<COM1B0);

		TCCR1B = (1<<WGM12) | ((s_config_ptr->pre_timer)<<CS10);
		TIMER1B_callBack= callBack;

		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTD_ID,PIN4_ID,PIN_OUTPUT);
			GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);
		}
	}

	if((s_config_ptr->timer_x)==T2)
	{
		ASSR = (1<<AS2);                             /* Timer/Counter2 is clocked from a Crystal Oscillator*/
		TCNT2 = 0;                                   /* Set Timer initial value to 0*/
		OCR2  = (uint8)s_config_ptr->timer_compare;  /* Set Compare Value*/
		TIMSK = (TIMSK & 0x3f) | (1<<OCIE2);         /* Enable Timer2 Compare Interrupt*/
		/* Configure timer2 control register
		 * 1. Non PWM mode FOC2=1
		 * 2. CTC Mode WGM21=1 & WGM20=0
		 * 3. s_config_ptr->ocx     : status of OC0
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR2 = (1<<FOC2) | (1<<WGM21) | ((s_config_ptr->ocx)<<COM20) | ((s_config_ptr->pre_timer)<<CS20);
		TIMER2_callBack= callBack;

		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTD_ID,PIN7_ID,PIN_OUTPUT);
			GPIO_writePin(PORTD_ID,PIN7_ID,LOGIC_LOW);
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

	if((s_config_ptr->timer_x)==T1)
	{
		timer1_re_count= s_config_ptr->timer_init;
		TCNT1 = timer1_re_count;                     /* Set Timer initial value */
		TIMSK = (TIMSK & 0xc3) | (1<<TOIE1);         /* Enable Timer1 Overflow Interrupt*/
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC1A=1  FOC1B=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. s_config_ptr->ocx     : status of OC0
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);
		TCCR1B = ((s_config_ptr->pre_timer)<<CS10);

		TIMER1_callBack= callBack;
	}

	if((s_config_ptr->timer_x)==T2)
	{
		//ASSR = (1<<AS2);                             /* Timer/Counter2 is clocked from a Crystal Oscillator*/
		timer2_re_count=(uint8)s_config_ptr->timer_init;
		TCNT2 = (uint8)timer2_re_count;              /* Set Timer initial value */
		TIMSK = (TIMSK & 0x3f) | (1<<TOIE2);         /* Enable Timer2 Overflow Interrupt*/
		/* Configure timer2 control register
		 * 1. Non PWM mode FOC2=1
		 * 2. Normal Mode WGM21=0 & WGM20=0
		 * 3. s_config_ptr->ocx     : status of OC0
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR2 = (1<<FOC2) | ((s_config_ptr->pre_timer)<<CS20);
		TIMER2_callBack= callBack;
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

	if( ((s_config_ptr->counter_x)==C1) || ((s_config_ptr->counter_x)==CA1))
	{
		TCNT1 = 0;                                    /* Set Timer initial value to 0*/
		OCR1A  = s_config_ptr->counter_compare;       /* Set Compare Value*/
		TIMSK = (TIMSK & 0xc3) | (1<<OCIE1A);         /* Enable Timer1 Compare Interrupt*/

		/* Configure timer1 control register
		 * 1. Non PWM mode FOC1A=1 FOC1B=1
		 * 2. CTC Mode WGM13=0 WGM12=1 WGM11=0 & WGM10=0
		 * 3. s_config_ptr->ocx     : status of OC1A
		 * 4. s_config_ptr->edge_state     : clock external
		 */

		TCCR1A = (1<<FOC1A) | (1<<FOC1B) | ((s_config_ptr->ocx)<<COM1A0);
		TCCR1B = (1<<WGM12) | ((s_config_ptr->edge_state)<<CS10);
		TIMER1A_callBack= callBack;
		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_OUTPUT);
			GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_LOW);
		}
	}

	if((s_config_ptr->counter_x)== CB1)
	{
		TCNT1  = 0;                                    /* Set Timer initial value to 0*/
		OCR1A  = s_config_ptr->counter_compare;        /* Set Compare Value*/
		TIMSK  = (TIMSK & 0xc3) | (1<<OCIE1B);         /* Enable Timer1 Compare Interrupt*/

		/* Configure timer1 control register
		 * 1. Non PWM mode FOC1A=1 FOC1B=1
		 * 2. CTC Mode WGM13=0 WGM12=1 WGM11=0 & WGM10=0
		 * 3. s_config_ptr->ocx            : status of OC1A
		 * 4. s_config_ptr->edge_state     : clock external
		 */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B) | ((s_config_ptr->ocx)<<COM1B0);
		TCCR1B = (1<<WGM12) | ((s_config_ptr->edge_state)<<CS10);
		TIMER1A_callBack= callBack;
		if((s_config_ptr->ocx) != DISCONECT)
		{
			GPIO_setupPinDirection(PORTD_ID,PIN4_ID,PIN_OUTPUT);
			GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);
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
	if((s_config_ptr->counter_x)==C1)
	{
		TCNT1 = timer1_re_count;                     /* Set Timer initial value */
		TIMSK = (TIMSK & 0xc3) | (1<<TOIE1);         /* Enable Timer1 Overflow Interrupt*/
		/* Configure timer0 control register
		 * 1. Non PWM mode FOC1A=1  FOC1B=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. s_config_ptr->ocx     : status of OC0
		 * 4. s_config_ptr->pre     : clock = F_CPU / prescaler
		 */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);
		TCCR1B = ((s_config_ptr->edge_state)<<CS10);

		timer1_re_count= s_config_ptr->counter_init;
		TIMER1_callBack= callBack;
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
	if((s_config_ptr->pwm_x)==PWM1A)
	{
		TCNT1 = 0;                                        /* Set Timer initial value to 0*/
		uint16 com = 100;
		if((s_config_ptr->pwm_duty) >= 100)
		{
			s_config_ptr->pwm_duty=100;
			com= (uint16)(((float)(s_config_ptr->pwm_duty)/100)*(s_config_ptr->pwm_com));
		}
		else
		{
			com= (uint16)(((float)(s_config_ptr->pwm_duty)/100)*((s_config_ptr->pwm_com)+1));
		}
		ICR1  = (s_config_ptr->pwm_com) +1;
		OCR1A = com ;                                      /* Set Compare Value*/
		TIMSK = (TIMSK & 0xc3) ;                           /*disable Timer1 Interrupt*/

		GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_OUTPUT);
		GPIO_writePin(PORTD_ID,PIN5_ID,LOGIC_LOW);
		/* Configure timer1 control register
		 * 1. PWM mode FOC1A=0
		 * 2. PWM Mode  WGM13=1 & WGM12=1 & WGM11=1 & WGM10=0
		 * 3. s_config_ptr->pwm_state :  inverting or non inverting
		 * 4. s_config_ptr->pwm_clk   :  timer frequancy
		 */

		uint16 pre = (uint16)((F_CPU) / ( (float)(s_config_ptr->pwm_clk)*(1+s_config_ptr->pwm_com)) );
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

		TCCR1A = (1<<WGM11) | (1<<WGM12) | (1<<WGM13) | ((s_config_ptr->pwm_state)<<COM1A0);
		TCCR1B =((pre)<<CS10);

	}
/*********************************************************************************************************/
	if((s_config_ptr->pwm_x)==PWM1B)
	{
		TCNT1 = 0;                                        /* Set Timer initial value to 0*/
		uint16 com = 100;
		if((s_config_ptr->pwm_duty) >= 100)
		{
			s_config_ptr->pwm_duty=100;
			com= (uint16)(((float)(s_config_ptr->pwm_duty)/100)*(s_config_ptr->pwm_com));
		}
		else
		{
			com= (uint16)(((float)(s_config_ptr->pwm_duty)/100)*((s_config_ptr->pwm_com)+1));
		}
		ICR1  = (s_config_ptr->pwm_com) +1;
		OCR1B = com ;                                      /* Set Compare Value*/
		TIMSK = (TIMSK & 0xc3) ;                           /*disable Timer1 Interrupt*/

		GPIO_setupPinDirection(PORTD_ID,PIN4_ID,PIN_OUTPUT);
		GPIO_writePin(PORTD_ID,PIN4_ID,LOGIC_LOW);
		/* Configure timer1 control register
		 * 1. PWM mode FOC1A=0
		 * 2. PWM Mode  WGM13=1 & WGM12=1 & WGM11=1 & WGM10=0
		 * 3. s_config_ptr->pwm_state :  inverting or non inverting
		 * 4. s_config_ptr->pwm_clk   :  timer frequancy
		 */

		uint16 pre = (uint16)((F_CPU) / ( (float)(s_config_ptr->pwm_clk)*(1+s_config_ptr->pwm_com)) );
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

		TCCR1A = (1<<WGM11) | (1<<WGM12) | (1<<WGM13) | ((s_config_ptr->pwm_state)<<COM1B0);
		TCCR1B =((pre)<<CS10);

	}
/*********************************************************************************************************/

	if((s_config_ptr->pwm_x)==PWM2)
	{
		ASSR = (1<<AS2);                             /* Timer/Counter2 is clocked from a Crystal Oscillator*/
		TCNT2 = 0;                                   /* Set Timer initial value to 0*/
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
		OCR2  = com ;                                     /* Set Compare Value*/
		TIMSK = (TIMSK & 0x3f) ;                          /*disable Timer2 Interrupt*/

		GPIO_setupPinDirection(PORTD_ID,PIN7_ID,PIN_OUTPUT);
		GPIO_writePin(PORTD_ID,PIN7_ID,LOGIC_LOW);

		/* Configure timer2 control register
		 * 1. PWM mode FOC2=0
		 * 2. PWM Mode WGM21=1 & WGM20=1
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

		TCCR2 = (1<<WGM20) | (1<<WGM21) | ((s_config_ptr->pwm_state)<<COM20) | ((pre)<<CS20);
	}
/*********************************************************************************************************/

}
