/*
 * automatic_upown.c
 *
 *  Created on: Oct 4, 2024
 *      Author: MadaYaswanth
 */

#include"main.h"
#include"uart.h"
#include"automatic_updown.h"
#define SELECT_LEFT_RIGHT_DOOR 2
extern int motor_current();
extern void delay_ms(int ms);
extern int can_switch_status;
 void window_automatic_updown_gpio_init()
 {
	 RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;//CLOCK FOR PORT-A
	 GPIOA->CRL&=~(0XFFFFFFFF);
	 GPIOA->CRH&=~(0XFFFFFFFF);
	 //PA-1 & PA-2 pins to control relay modules and these two pins are output pins
	 GPIOA->CRL|=(1<<5);
	 GPIOA->BSRR|=(1<<1);//PA1 IS HIGH
	 GPIOA->CRL|=(1<<9);
	 GPIOA->BSRR|=(1<<2);//PA2 IS HIGH
	 GPIOA->CRL|=(1<<15);//PA3 IS HIGH(UP_LIMIT_SWITCH)
	 GPIOA->BSRR|=(1<<3);
	 GPIOA->CRL|=(1<<19);//PA4 IS HIGH(DOWN_LIMIT_SWITCH)
	 GPIOA->BSRR|=(1<<4);
	 GPIOA->CRL|=(1<<23);//PA5 IS HIGH(WINDOW_UP_SWITCH)
	 GPIOA->BSRR|=(1<<5);
	 GPIOA->CRL|=(1<<27);//PA6 IS HIGH(WINDOW_DOWN_SWITCH)
	 GPIOA->BSRR|=(1<<6);
#if SELECT_LEFT_RIGHT_DOOR==2
	 GPIOA->CRL|=(1<<31);//PA7 IS HIGH
	 GPIOA->BSRR|=(1<<7);
	 GPIOA->CRH|=(1<<3);//PA8 IS HIGH
	 GPIOA->BSRR|=(1<<8);
#endif
 }
 void window_motor_up(void)
 {
 	  // uart_string("up1 ");
	    GPIOA->ODR|=(1<<2);//RELAY R2 OFF
	    for(int i=0;i<100;i++){}
		GPIOA->BSRR|=(1<<1)<<16;//RELAY R1 ON
 }
 void window_motor_down(void)
 {
 	   //uart_string("down1 ");
   GPIOA->ODR|=(1<<1);//RELAY R1 OFF
   for(int i=0;i<100;i++){}
   GPIOA->BSRR|=(1<<2)<<16;//RELAY R2 ON
 }
 void window_motor_stop(void)
 {
	  // 	uart_string("not1 ");
	 GPIOA->BSRR|=(1<<1);
	 GPIOA->BSRR|=(1<<2);
 }

#if SELECT_LEFT_RIGHT_DOOR==2
 int check_left_side_switch_status(void)//check driver side switch status,controlling the left side window
 {
	 up:if(((GPIOA->IDR>>7)&1)==0)
	 {
		 delay_ms(1);
	   if((GPIOA->IDR>>7&1))
	   {
		   goto up;
	   }
	   return UP_STATE;
	 }
	 down:if(((GPIOA->IDR>>8)&1)==0)
	 {
		 delay_ms(1);
	   if((GPIOA->IDR>>8&1))
	   {
		   goto down;
	   }
	   return DOWN_STATE;
	 }
	 return NORMAL_STATE;
 }
#endif
 int check_switch_status(void)
 {
#if SELECT_LEFT_RIGHT_DOOR==1
   c:if((GPIOA->IDR>>5&1)&&(GPIOA->IDR>>6&1))
   {
	   delay_ms(1);
	   if((!(GPIOA->IDR>>5&1))||(!(GPIOA->IDR>>6&1)))
	   {
		   goto c;
	   }
	   return can_switch_status;
   }
#endif
   	 sw1:if(((GPIOA->IDR>>5)&1)==0)
   	 {
   	   delay_ms(1);
   	   if((GPIOA->IDR>>5&1))
   	   {
   		   goto sw1;
   	   }

   	   return UP_STATE;
   	 }
   	 sw2:if(((GPIOA->IDR>>6)&1)==0)
   	 {
   		 delay_ms(1);
   	   if((GPIOA->IDR>>6&1))
   	   {
   		   goto sw2;
   	   }

   	   return DOWN_STATE;
   	 }

   	 return NORMAL_STATE;
}
 void auto_up_down(void)
 {
	 int switch_status=check_switch_status();
	 static  int press_time=0,current_time=0;
	 if(switch_status!=NORMAL_STATE)
	 {
		press_time=HAL_GetTick();
       if(switch_status==UP_STATE)
       {
          if(((GPIOA->IDR>>3)&1))//UP LIMIT switch not pressed
          {
        	 window_motor_up();
        	 while(((GPIOA->IDR>>5)&1)==0)
        	 {
        		current_time=(HAL_GetTick()-press_time);
        		if(current_time>500)
        		{
        			while(((GPIOA->IDR>>3)&1)&&(!((check_switch_status())==DOWN_STATE))&&(motor_current()));
             		while(!((GPIOA->IDR>>6)&1));//wait for down switch release
             		break;
        		}
        	 }
           window_motor_stop();
          }
       }
       else if(switch_status==DOWN_STATE)
       {
           if(((GPIOA->IDR>>4)&1))//DOWN LIMIT switch not pressed
           {
         	 window_motor_down();
         	 while(((GPIOA->IDR>>6)&1)==0)
         	 {
         		current_time=(HAL_GetTick()-press_time);
         		if(current_time>500)
         		{
         		while(((GPIOA->IDR>>4)&1)&&(!((check_switch_status())==UP_STATE)));
         		window_motor_stop();
         		while(!((GPIOA->IDR>>5)&1));//wait for up switch release
         		break;
         		}
         	 }
            window_motor_stop();
           }
       }
	 }
 }

