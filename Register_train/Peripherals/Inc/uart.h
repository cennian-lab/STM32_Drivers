/*
 * uart.h
 *
 *  Created on: 05-Apr-2021
 *      Author: Ihtesham
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
/*
 * Configuring USART 3 related GPIOs
 */
void UART_3_GPIO_Config(void);

/*
 * Configurations for USART 3
 */
void UART_3_Config(void);
/*
 * Transmit function for USART 3
 */
bool UART_3_Transmit(uint32_t* data, uint8_t len, uint32_t timeout);

#endif /* INC_UART_H_ */
