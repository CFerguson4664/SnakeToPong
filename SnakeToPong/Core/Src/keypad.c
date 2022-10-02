/*
 * keypad.c
 *
 *  Created on: Oct 2, 2022
 *      Author: jarre
 */

#include "main.h"
#include "keypad.h"
#include "smc_queue.h"

static enum ButtonState one = Released;
static enum ButtonState three = Released;
static enum ButtonState seven = Released;
static enum ButtonState nine = Released;

void check_buttons(Smc_queue *move_q)
{
	 // State 1
		  GPIOC -> ODR &= ~(1<<0) ;
		  GPIOC -> ODR |= (1<<4) ;


		  if(!((GPIOC -> IDR)&(1<<2)))
		  {
			  if(one == Released) {
				  Q_data btn;
				  btn.button = Left_Up;
				  move_q->put(move_q, &btn);
			  }

			  one = Pressed;
		  }
		  else
		  {
			  one = Released;
		  }

		  if (!((GPIOC -> IDR)&(1<<5)))
		  {
			  if(three == Released) {
				  Q_data btn;
				  btn.button = Right_Up;
				  move_q->put(move_q, &btn);
			  }

			  three = Pressed;
		  }
		  else {
			  three = Released;
		  }

		  // State 2
		  GPIOC -> ODR |= (1<<0) ;
		  GPIOC -> ODR &= ~(1<<4) ;

		  if(!((GPIOC -> IDR)&(1<<2)))
		  {
			  if(seven == Released) {
				  Q_data btn;
				  btn.button = Left_Down;
				  move_q->put(move_q, &btn);
			  }

			  seven = Pressed;
		  }
		  else {
			  seven = Released;
		  }

		  if (!((GPIOC -> IDR)&(1<<5)))
		  {
			  if(nine == Released) {
				  Q_data btn;
				  btn.button = Right_Down;
				  move_q->put(move_q, &btn);
			  }

			  nine = Pressed;
		  }
		  else {
			  nine = Released;
		  }
}

//void pressed_enqueue(Smc_queue *move_q){
//	Q_data btnL;
//	btnL.button = pressed_left;
//	Q_data btnR;
//	btnR.button = pressed_right;
//
//	if(pressed_left != None) {
//		move_q->put(move_q, &btnL);
//	}
//
//	if(pressed_right != None) {
//		move_q->put(move_q, &btnR);
//	}
//
//
//	pressed_left = None;
//	pressed_right = None;
//}
