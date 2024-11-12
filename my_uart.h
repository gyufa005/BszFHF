/*
 * my_uart.h
 *
 *  Created on: Oct 23, 2024
 *      Author: Fazekas Gyula
 */

#ifndef MY_UART_H_
#define MY_UART_H_

volatile char lastcharacter;
void my_uart_init(void);
int16_t UART_RXnb(USART_TypeDef *usart);
#endif /* MY_UART_H_ */
