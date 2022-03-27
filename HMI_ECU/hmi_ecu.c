/* ******************************************************************************************************
 *
 * Name : hmi_ecu.c
 *
 * Author: Mohamed Faryed
 *
 * Description: AVR application
 *
 * Created on: March 5, 2022
 *
 ********************************************************************************************************/

#include"hmi_ecu.h"

/******************************************************************************************************
 *                                 Main Function                                                      *
 ******************************************************************************************************/
int main(void)
{

	UART_Init s_uart_config={RATE9600,DISP,EIGHT_BIT,ENR,ONE_STOP};
	TIMER_Config s_timer_config={6,0,PRE8,T0,0};

	TIMER_NormalInit(&s_timer_config,TIME_delay);
	LCD_init();
	UART_init(&s_uart_config);
	SREG |=(1<<7);


	while(UART_recieveByte() != MC2_READY){}
	UART_sendByte(MC1_READY);
	val = UART_recieveByte();

	if(val == 0x10)
	{
		stepONE_ack1=1;
	}
	else
	{
		stepONE_ack1=0;
	}

	while(1)
	{
		if(stepONE_ack2 == 0 && stepONE_ack1 == 0)
		{
			stepONE() ;
			stepONE_ack2 =1;
		}

		else if(stepONE_ack2 == 0 && stepONE_ack1 == 1)
		{
			stepONETWO();
		}

		else if(stepONE_ack2 == 1 && stepONE_ack1 == 1)
		{

			TWO();
			_delay_ms(400);
		}

		else
		{
			LCD_displayStringRowColumn(0,6,"Fault");
			g_count_value=0;
			while(g_count_value <= 10){}
			stepONE_ack2 = 0;
		}
	}
}
/******************************************************************************************************
 *                                 Function Definition                                                 *
 ******************************************************************************************************/

/*
 * Description:
 *             this function is responsible for receive input from the user
 */
void supONE(uint8 step)
{
	volatile static uint8 LDC_step1 = 0;
	LDC_step1 = step;

	if(0 == LDC_step1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Enter New Pass:");
		for(uint8 i=0;i<4;i++)
		{
			while((key[i] < 0) || (key[i] > 9))
			{
				key[i]=KEYPAD_getPressedKey();
		    }
			_delay_ms(300);
			LCD_moveCursor(1,9+i);
			LCD_intgerToString(key[i]);
			_delay_ms(300);
			LCD_moveCursor(1,9+i);
			LCD_displayCharacter('*');
		}
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Then Enter </>");
		while(key[5] != 37)
		{
			key[5]=KEYPAD_getPressedKey();
		}
		key[5]=0;
		LCD_clearScreen();
	}

	if(1 == LDC_step1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"RE Enter Pass:");
		for(uint8 i=0;i<4;i++)
		{
			while((key[i] < 0) || (key[i] > 9))
			{
				key[i]=KEYPAD_getPressedKey();
		    }
			_delay_ms(300);
			LCD_moveCursor(1,9+i);
			LCD_intgerToString(key[i]);
			_delay_ms(300);
			LCD_moveCursor(1,9+i);
			LCD_displayCharacter('*');
		}
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Then Enter </>");
		while(key[5] != 37)
		{
			key[5]=KEYPAD_getPressedKey();
		}
		key[5]=0;
		LCD_clearScreen();
	}

	if(2 == LDC_step1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Enter Pass:");
		for(uint8 i=0;i<4;i++)
		{
			while((key[i] < 0) || (key[i] > 9))
			{
				key[i]=KEYPAD_getPressedKey();
		    }
			_delay_ms(300);
			LCD_moveCursor(1,9+i);
			LCD_intgerToString(key[i]);
			_delay_ms(300);
			LCD_moveCursor(1,9+i);
			LCD_displayCharacter('*');
		}
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Then Enter </>");
		while(key[5] != 37)
		{
			key[5]=KEYPAD_getPressedKey();
		}
		key[5]=0;
		LCD_clearScreen();
	}
}

/*
 * Description:
 *             this function responsible for match the new password
 *             if correct then save it in eeprom if not re enter
 */
void stepONE(void)
{
	while(0 == stepONE_ack1)
	{
		supONE(0);
		UART_sendByte(MC1_READY);

		for(uint8 i=0;i<5;i++)
		{
			UART_sendByte(key[i]);
		}
		for(uint8 i=0;i<5;i++)
		{
			key[i]=20;
		}

		supONE(1);
		UART_sendByte(MC1_READY);

		for(uint8 i=0;i<5;i++)
		{
			UART_sendByte(key[i]);
		}
		for(uint8 i=0;i<5;i++)
		{
			key[i]=20;
		}

		while(UART_recieveByte() != MC2_READY){}
		stepONE_ack1 = UART_recieveByte();

		if(stepONE_ack1 == 1)
		{
			UART_sendByte(1);

		}

		else if (stepONE_ack1 == 0)
		{
			UART_sendByte(0);

		}
	}

}

/*
 * Description:
 *             this function responsible for check password
 *             if correct then login if not try 2 other time
 */
void stepONETWO(void)
{
	uint8 save = 0;

	while(2 >= save)
	{

		supONE(2);
		UART_sendByte(MC1_READY);

		for(uint8 i=0;i<5;i++)
		{
			UART_sendByte(key[i]);
		}

		for(uint8 i=0;i<5;i++)
		{
			key[i]=20;
		}

		while(UART_recieveByte() != MC2_READY){}
		stepONE_ack2 = UART_recieveByte();

		if(1 == stepONE_ack2)
		{
			UART_sendByte(1);
			break;
		}
		UART_sendByte(0);
		save++;
		if(save >= 3 )
		{
			stepONE_ack2 = 2;
		}
	}
}

/*
 * Description:
 *             this function responsible for change password or
 *             open and close the door
 */
void TWO(void)
{
	char state='\0';
	uint8 st=0;

	while(state =='\0')
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"+ :Open Door");
		LCD_displayStringRowColumn(1,0,"- :changing Pass");
		state = KEYPAD_getPressedKey();
		_delay_ms(300);
		if(state != '+' &&  state !='-')
		{
			state='\0';
			continue;
		}

		stepONETWO();
		if(stepONE_ack2 == 2)
		{
			LCD_displayStringRowColumn(0,6,"Fault");
			g_count_value=0;
			while(g_count_value <= 10){}
			stepONE_ack2 = 0;
			break;
		}

		UART_sendByte(MC1_READY);
		UART_sendByte(state);
		st = UART_recieveByte();
		if(st == 1)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,2,"Door Opening");
			g_count_value=0;
			while(g_count_value <= 15){}

			LCD_clearScreen();
			LCD_displayStringRowColumn(0,2,"Door Closing");
			g_count_value=0;
			while(g_count_value <= 15){}

		}

		else if(st == 0)
		{
			stepONE_ack1 =0;
			_delay_ms(300);
			stepONE();
		}
	}

}

/*
 * Description:
 *             this function is a call back for timer 0
 */
void TIME_delay(void)
{

	/* over flow every 2ms*/
	static volatile uint16 count_value = 0;
	count_value++;
	if(count_value >= 4000)
	{
		g_count_value++;
		count_value = 0;
	}

}

