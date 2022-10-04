// MIT License - Sean Carroll - (c) 2022
// carrolls@trine.edu 2022.7.28 - merged into project
// carrolls@trine.edu 2022.07.04
// Game "snake - Eight-by-eight board
// Board encoding: -1 = ball. 0 = empty. 1 = paddle;
// so the snake's body comprises consecutive integers.

//// STATUS - Works.

//#define UNIT_TEST_SNAKE
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pong_gameplay.h"
#include "snake_repair.h"
#include "quadknob.h"
#include "snake_enums.h"
#include "display_DOGS_102.h"
#include "keypad.h"


void pong_game_init(pong_game* p){
	//Ball Stuff
	const XY_PT initial_ball = {6,3};
	p->ball.x = initial_ball.x;
	p->ball.y = initial_ball.y;
	const XY_PT initial_balldir = {-1,-1};
	p->balldir.x = initial_balldir.x;
	p->balldir.y = initial_balldir.y;

	//Paddle Stuff
	const XY_PT initial_Lpad = {0,4}; //Left Paddle values
	const XY_PT initial_Rpad = {7,4}; //Right Paddle values

	p->Lpad.x = initial_Lpad.x; //Places the {x,y} values into the left and right paddle objects.
	p->Lpad.y = initial_Lpad.y;
	p->Rpad.x = initial_Rpad.x;
	p->Rpad.y = initial_Rpad.y;

}

void pong_periodic_play(pong_game* p){

	XY_PT tempBall = {p->ball.x, p->ball.y }; //Initialize and assign a temporary ball.
	tempBall.x += p->balldir.x;//Move the temporary ball
	tempBall.y += p->balldir.y;


	//if statement to check if temp ball hit wall
	if( tempBall.x < 0){ //if the temp ball hit left wall
		p->balldir.x = 0; //Stops the ball from moving in the x and y direction.
		p->balldir.y = 0;
	}
	else if(tempBall.x > 7){ //if temp ball hits the right wall
		p->balldir.x = 0; //Stops the ball from moving in the x and y direction.
		p->balldir.y = 0;
	}
	else if(tempBall.y < 0){ //if we hit the top wall
		p->balldir.y *= -1; //flip the y coordinate
	}
	else if(tempBall.y > 7){ //if we hit the bottom wall
		p->balldir.y *= -1; //flip the y coordinate
	}


	XY_PT tempBall2 = {p->ball.x, p->ball.y }; //Initialize and assign a temporary ball.
	tempBall2.x += p->balldir.x;//Move the temporary ball
	tempBall2.y += p->balldir.y;

	//if statement to check if the temp ball hit the paddle
	if(tempBall2.x <= 0 && (p->Lpad.y + 1 == tempBall2.y || p->Lpad.y == tempBall2.y || p->Lpad.y-1 == tempBall2.y)){ //if temp ball hits left paddle
		p->balldir.x *= -1; //reverse the x direction of the ball
	}
	else if(tempBall2.x >= 7 && (p->Rpad.y + 1 == tempBall2.y || p->Rpad.y == tempBall2.y || p->Rpad.y-1 == tempBall2.y)){ //if temp ball hits right paddle
		p->balldir.x *= -1; //reverse x direction of the ball
	}

	p->ball.x += p->balldir.x; //Moves the real ball once we check to see if the move is valid.
	p->ball.y += p->balldir.y;

}

void pacify_compiler(){

}

void paddle_update(pong_game* p,Smc_queue* q){
	Q_data msg; //Variable that will hold the input request.
	bool data_available;
	data_available = q->get(q,&msg); //Test to see of there is input data

	while(data_available) {
		if(!data_available) return;
		else{
			switch(msg.button){
			case Left_Up:
				if(p->Lpad.y > 1){
					p->Lpad.y -= 1; //move left paddle down
				}
				break;
			case Left_Down:
				if(p->Lpad.y < 6){
					p->Lpad.y += 1; //move left paddle up
				}
				break;
			case Right_Up:
				if(p->Rpad.y > 1){
					p->Rpad.y -= 1; //move right paddle up
				}
				break;
			case Right_Down:
				if(p->Rpad.y < 6){
					p->Rpad.y += 1; //Move right paddle down
				}
				break;
			default: //do nothing
				pacify_compiler();
			}
		}

		data_available = q->get(q,&msg);
	}
}

bool ball_plot(const pong_game* p, int8_t b [CHECKS_WIDE][CHECKS_WIDE]){
	bool ok = true;
	if (b[p->ball.x][p->ball.y] == -1) ok = true;
	else if (b[p->ball.x][p->ball.y] == 0){
		ok = true;
		b[p->ball.x][p->ball.y] = -1;
	}
	else{
		ok = false;
	}
	return ok;
}

void paddle_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]){
	b[p->Lpad.x][p->Lpad.y+1] = 1;
	b[p->Lpad.x][p->Lpad.y] = 1;
	b[p->Lpad.x][p->Lpad.y-1] = 1;
	b[p->Rpad.x][p->Rpad.y+1] = 1;
	b[p->Rpad.x][p->Rpad.y] = 1;
	b[p->Rpad.x][p->Rpad.y-1] = 1;
}
