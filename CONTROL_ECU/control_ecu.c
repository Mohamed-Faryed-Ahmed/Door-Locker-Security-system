/* ******************************************************************************************************
 *
 * Name : control_ecu.c
 *
 * Author: Mohamed Faryed
 *
 * Description: AVR application
 *
 * Created on: March 5, 2022
 *
 ********************************************************************************************************/

#include"control_ecu.h"

/******************************************************************************************************
 *                                   Main  Function                                                   *
 ******************************************************************************************************/
int main(void)
{

	UART_Init s_uart_config={RATE9600,DISP,EIGHT_BIT,ENR,ONE_STOP};
	TIMER_Config s_timer_config={0,250,PRE8,T1,DISCONECT};
	I2C_Config s_i2c_config={400000,0x50,I2C_PRE_1};

	TIMER_comparInit(&s_timer_config,TIME_delay);
	UART_init(&s_uart_config);
	TWI_init(&s_i2c_config);
	DcMotor_Init();
	BUZZER_init ();
	SREG |=(1<<7);

	pass_READ();
	if(val == 0x10)
	{
		stepONE_ack1=1;
	}
	else
	{
		stepONE_ack1=0;
	}
	UART_sendByte(MC2_READY);
	while(UART_recieveByte() != MC1_READY){}
	UART_sendByte(val);

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
			stepTWO();
			_delay_ms(400);
		}

		else
		{
			stepTHREE();
			stepONE_ack2 =0;
		}
	}

}

/******************************************************************************************************
 *                                     Function Definition                                          *
 ******************************************************************************************************/

/*
 * Description:
 *             this function responsible for match the new password
 *             if correct then save it in eeprom if not re enter
 */
void stepONE(void)
{
	uint8 ack=0;
	uint8 key[5] ;
	uint8 key2[5] ;
	while(0 == stepONE_ack1)
	{
		while(UART_recieveByte() != MC1_READY){}
		for(uint8 i=0 ;i<5; i++)
		{
			key[i] = UART_recieveByte();
		}

		while(UART_recieveByte() != MC1_READY){}
		for(uint8 i=0 ;i<5; i++)
		{
			key2[i] = UART_recieveByte();
		}


		for(uint8 j=0 ;j<5; j++)
		{
			if(key[j] != key2[j])
			{
				ack=1;
			}
		}

		for(uint8 i=0 ;i<5; i++)
		{
			password[i] = key[i];
		}

		pass_WRITE(password);


		UART_sendByte(MC2_READY);

		if(ack == 0)
		{
			UART_sendByte(1);
			stepONE_ack1 = UART_recieveByte();
			ack = 1;
	    }
		else if(ack == 1)
		{
			UART_sendByte(0);
			stepONE_ack1 = UART_recieveByte();
			ack = 0;
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
	uint8 ack=0;
	uint8 key[5] ;

	while(2 >= save)
	{

		while(UART_recieveByte() != MC1_READY){}
		for(uint8 i=0 ;i<5; i++)
		{
			key[i] = UART_recieveByte();
		}

		pass_READ();

		for(uint8 j=0 ;j<5; j++)
		{
			if(key[j] != password[j])
			{
				ack=1;
			}
		}

		UART_sendByte(MC2_READY);

		if(ack == 0)
		{
			UART_sendByte(1);
			stepONE_ack2 = UART_recieveByte();
			ack = 1;
	    }
		else if(ack == 1)
		{
			UART_sendByte(0);
			stepONE_ack2 = UART_recieveByte();
			ack = 0;
		}

		if(1 == stepONE_ack2)
		{
			break;
		}
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
void stepTWO(void)
{
	char res='\0';

	stepONETWO();
	if(stepONE_ack2 == 2)
	{
		stepTHREE();
		stepONE_ack2 = 0;
		return ;
	}

	while(UART_recieveByte() != MC1_READY){}
	res = UART_recieveByte();

	if(res == '+')
	{
		UART_sendByte(1);

		DcMotor_Rotate(CW,50);
		g_count_value=0;
		while(g_count_value <= 15){}

		DcMotor_Rotate(A_CW,50);
		g_count_value=0;
		while(g_count_value <= 15){}

		DcMotor_Rotate(STOP,0);
			/*timer count*/
	}

	if(res == '-')
	{
		UART_sendByte(0);
		stepONE_ack1 = 0;
		stepONE();
	}
}

/*
 * Description:
 *             this function responsible for buzzer on or off
 */
void stepTHREE(void)
{
	BUZZER_on ();
	g_count_value=0;
	while(g_count_value <= 10){}
	BUZZER_off ();
}
/*
 * Description:
 *             this function is a call back for timer 1
 */
void TIME_delay(void)
{

	/*every over flow take time 2ms*/
	static volatile uint16 count_value = 0;
	count_value++;
	if(count_value >= 4000)
	{
		g_count_value++;
		count_value = 0;
	}

}

/*
 * Description:
 *             read password from eeprom
 */
void pass_READ(void)
{
	EEPROM_readByte(0x0311, &val);
	for(uint8 k=0;k<5;k++)
	{
		_delay_ms(10);
		EEPROM_readByte((0x0312+k), &password[k]);
	}
}

/*
 * Description:
 *             write password to eeprom
 */
void pass_WRITE(uint8 *pass)
{

	for(uint8 k=0;k<5;k++)
	{
		EEPROM_writeByte((0x0312+k), pass[k]);
		_delay_ms(10);

	}
	EEPROM_writeByte(0x0311,0x10);
}
