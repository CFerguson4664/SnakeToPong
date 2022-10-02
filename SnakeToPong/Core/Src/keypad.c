/*
 * keypad.c
 *
 *  Created on: Oct 2, 2022
 *      Author: jarre
 */

#include "main.h"
#include "keypad.h"
#include "smc_queue.h"

static enum Moves pressed_left = None;
static enum Moves pressed_right = None;
void check_buttons()
{
	 // State 1
		  GPIOC -> ODR &= ~(1<<0) ;
		  GPIOC -> ODR |= (1<<4) ;


		  if(!((GPIOC -> IDR)&(1<<2)))
			{
			  pressed_left = Left_Up;
			}
		  else if (!((GPIOC -> IDR)&(1<<5)))
			{
			  pressed_right = Right_Up;
			}

		  // State 2
		  GPIOC -> ODR |= (1<<0) ;
		  GPIOC -> ODR &= ~(1<<4) ;

		  if(!((GPIOC -> IDR)&(1<<2)))
			{
			  pressed_left = Left_Down;
			}
		  else if (!((GPIOC -> IDR)&(1<<5)))
			{
			  pressed_right = Right_Down;
			}
}

void pressed_enqueue(Smc_queue *move_q){
	Q_data btnL;
	btnL.button = pressed_left;
	Q_data btnR;
	btnR.button = pressed_right;

	move_q->put(move_q, &btnL);
	move_q->put(move_q, &btnR);

	pressed_left = None;
	pressed_right = None;
}
