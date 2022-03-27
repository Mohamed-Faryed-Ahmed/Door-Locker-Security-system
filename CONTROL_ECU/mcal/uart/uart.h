 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Faryed
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_


#include "../../std_types.h"


#define F_CPU 8000000UL   //8MHz Clock frequency
#define UART_INT_RX (1U)

/*******************************************************************************
 *                      Type_definition                                        *
 *******************************************************************************/
typedef enum{
	RATE10=10,RATE300=300,RATE600=600,RATE1200=1200,RATE2400=2400,RATE4800=4800,RATE9600=9600,\
	RATE14400=14400,RATE19200=19200,RATE38400=38400,RATE57600=57600,RATE115200=115200
}UART_Baud_Rate;

typedef enum{
	DISP=0,EN_EVEN=2,EN_ODD=3
}UART_Parity;
typedef enum{
	DISR=0,ENR=1
}UART_Rx_INT;

typedef enum{
	ONE_STOP=0,TWO_STOP=1
}UART_Stop_Bit;

typedef enum{
	FIVE_BIT=0,SIX_BIT=1,SEVEN_BIT=2,EIGHT_BIT=3
}UART_Fram_bit;

typedef struct{
	UART_Baud_Rate baud_rate;
	UART_Parity    upm    :2;
	UART_Fram_bit  ucsz   :2;
	UART_Rx_INT    rxcie  :1;
	UART_Stop_Bit  usbs   :1;
}UART_Init;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_Init *s_confg_ptr);


/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);



/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);



/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);


/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_H_ */
