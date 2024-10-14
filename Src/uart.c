/*
 * uart.c
 *
 *  Created on: Sep 12, 2024
 *      Author: MadaYaswanth
 */
#include "uart.h"
#include"main.h"
#include<stdio.h>
#include<stdlib.h>
void uart1_init(void)
{
    RCC->APB2ENR |= 1<<2;   // Enable GPIOA clock
    RCC->APB2ENR |= 1<<14;
    // Configure PA9 (TX)
    GPIOA->CRH &= ~(3<<6);
    GPIOA->CRH |= 2<<6;
    GPIOA->CRH |= 3<<4;
    GPIOA->CRH &= ~(3<<8);
    GPIOA->CRH |= 1<<10;
    USART1->BRR = 0x341;
    USART1->CR1 |= 1<<13|1<<2|1<<3;
}
char uart_rx()
{
   while(!((USART1->SR>>5)&1));
   return USART1->DR;;
}
void uart_tx(uint8_t c)
{
	while(!(USART1->SR>>7&1));
	USART1->DR=c;
}
void uart_string(char *ptr)
{
	while(*ptr)
	{
	uart_tx(*ptr++);
	}
}
void uart_int(int number)
{
  uint8_t ar[10];
  int i=0;
  if(number==0)
  {
	uart_tx('0');
  }
	  int k=number;
	  while(k)
	  {
		ar[i++]=(k%10)+'0';
		k/=10;
	  }
	  for(--i;i>=0;i--)
	  {
		  uart_tx(ar[i]);
	  }

}
void uart_float(float number)
{
	char ar[10];
	int k,i=0;
	k=number;
	number=number-k;
	while(k)
	{
	ar[i++]=k%10 +'0';
	k/=10;
	}
	for(--i;i>=0;i--)
	{
		uart_tx(ar[i]);
	}
	uart_tx('.');
	for(int i=0;i<3;i++)
	{
		number*=10;
		k=number;
		uart_tx((k+'0'));
		number-=k;
	}
	uart_tx(' ');
}
