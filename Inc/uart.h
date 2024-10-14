/*
 * uart.h
 *
 *  Created on: Sep 12, 2024
 *      Author: MadaYaswanth
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include<stdint.h>


void uart_init();
void uart_tx(uint8_t c);
char uart_rx();
void uart_string(char *);
void uart_int(int);
void uart1_init(void);
void uart_float(float );
#endif /* INC_UART_H_ */
