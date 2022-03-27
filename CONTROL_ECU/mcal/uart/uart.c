 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Faryed
 *
 *******************************************************************************/
#include <avr/io.h>         /* To use the UART Registers */
#include <avr/interrupt.h>
#include "../../common_macros.h"
#include "uart.h"


/*******************************************************************************
*                      Static Global Definitions                                  *
*******************************************************************************/
#if(UART_INT_RX== 1)
static volatile uint8 g_valueRx = 0;
static volatile uint8 Rx = 0;
#endif

/*******************************************************************************
*                      ISR Definitions                                  *
*******************************************************************************/
#if(UART_INT_RX == 1)
ISR(USART_RXC_vect)
{
	Rx = 1;
	g_valueRx = UDR ;
}
#endif


/*******************************************************************************
*                      Functions Definitions                                  *
*******************************************************************************/

/*
* Description :
* Functional responsible for Initialize the UART device by:
* 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
* 2. Enable the UART.
* 3. Setup the UART baud rate.
*/
void UART_init(const UART_Init *s_confg_ptr)
{
	uint16 ubrr_value = 0;

	/************************** UCSRA Description ***************************/
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/************************** *s_baudRate_ptr Description **************************
	* udrie : Disable/Enable USART Data Register Empty Interrupt
	* rxcie : USART RX Complete Interrupt Enable/Disable
	*********************************************************************************/
	UCSRB= (UCSRB & 0x7f)|((s_confg_ptr->rxcie)<<RXCIE);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC = (1<<URSEL) ;
	/************************** *s_baudRate_ptr Description **************************
	* baud_rate  : baud_rate Value
	* usbs       : number of stop bit
	* upm        : parity bit
	* UCSZ1:0    : For 8/7/6/5 -bit data mode
	*********************************************************************************/
	UCSRC = (UCSRC & 0xc1) | ((s_confg_ptr->upm)<<UPM0)\
			| ((s_confg_ptr->usbs)<<USBS) | ((s_confg_ptr->ucsz)<<UCSZ0);
	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ((s_confg_ptr->baud_rate) * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

/*
* Description :
* Functional responsible for send byte to another UART device.
*/
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}


#if(UART_INT_RX== 0)
/*
* Description :
* Functional responsible for receive byte from another UART device.
*/
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
   return UDR;
}
#endif

#if(UART_INT_RX== 1)
/*
* Description :
* Functional responsible for receive byte from another UART device by interrupt.
*/
uint8 UART_recieveByte(void)
{
	while(Rx == 0){}
	Rx=0;
	return g_valueRx;
}
#endif

/*
* Description :
* Send the required string through UART to the other UART device.
*/
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}

/*
* Description :
* Receive the required string until the '#' symbol through UART from the other UART device.
*/
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}


