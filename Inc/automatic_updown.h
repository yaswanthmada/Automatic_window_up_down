/*
 * automatic_updown.h
 *
 *  Created on: Oct 4, 2024
 *      Author: MadaYaswanth
 */

#ifndef INC_AUTOMATIC_UPDOWN_H_
#define INC_AUTOMATIC_UPDOWN_H_

#define NORMAL_STATE 0
#define UP_STATE     1
#define DOWN_STATE   2
#define DOOR_OPEN    0
#define DOOR_CLOSE   1

/*SELECT_LEFT_RIGHT_DOOR =1 means left door
 SELECT_LEFT_RIGHT_DOOR =2 means right door*/
#define SELECT_LEFT_RIGHT_DOOR 1

void  window_automatic_updown_gpio_init(void);
void check_door_open_close(void);
int check_switch_status(void);
int check_left_side_switch_status(void);
void auto_up_down(void);
void window_motor_up(void);
void window_motor_stop(void);
void window_motor_down(void);
int check_can_msg(void);
#endif /* INC_AUTOMATIC_UPDOWN_H_ */
