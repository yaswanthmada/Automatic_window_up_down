/*
 * systick.c
 *
 *  Created on: Oct 5, 2024
 *      Author: MadaYaswanth
 */
#include"main.h"
void systick_init()
{
        SysTick->LOAD = (6000000-1);  // Load number of clock per milli se
		SysTick->VAL = 0;   // Set initial value of time
		SysTick->CTRL |= (1<<0);  //  Counnter Enable
		SysTick->CTRL |= (1<<2)|(1<<1);  // Select Processor Clock
}
void systick_delay_ms(int ms )
{
	/*
	 8000000=1second
	 1secon=1000ms
	 1000ms=8000000
	 1ms=8000 000/1000
	 1ms=80 00
	 * */


		for(int i =0; i<ms ; i++)
		{

		while ((SysTick->CTRL & (1<<16)) == 0) {} // check if counter flag is set
		}
		  SysTick->CTRL = 0;
}
