 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Faryed
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "../../std_types.h"


#define F_CPU 8000000UL   //8MHz Clock frequency
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
//#define  F_CPU   (1000000UL)

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      TYPE Definition                                        *
 *******************************************************************************/
typedef enum
{
	I2C_PRE_1=0,I2C_PRE_4=1,I2C_PRE_16=2,I2C_PRE_64=3
}I2C_Pre_Conf;

typedef struct
{
	uint32        I2C_scl     :21;
	uint8         I2C_address :7;
	I2C_Pre_Conf  I2C_pre     :2;

}I2C_Config;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(I2C_Config *s_config_ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
