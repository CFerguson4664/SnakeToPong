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
#include "snake_gameplay.h"
#include "snake_repair.h"
#include "quadknob.h"
#include "snake_enums.h"
#include "display_DOGS_102.h"

#define ERROR_DISPLAY_BAD_HEADING {{1,0},{1,1},{1,2}, {1,4},{1,5},{1,6}}
#define ERROR_DISPLAY_BLOCK_COUNT 6


void pong_game_init(pong_game* p){
	//Ball Stuff
	const XY_PT initial_ball = {3,3};
	p->ball.x = initial_ball.x;
	p->ball.y = initial_ball.y;
	const XY_PT initial_balldir = {-1,-1};
	p->balldir.x = initial_balldir.x;
	p->balldir.y = initial_balldir.y;

	//Paddle Stuff
	const XY_PT initial_Lpad = {0,4};
	const XY_PT initial_Rpad = {7,4};

	p->Lpad.x = initial_Lpad.x;
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

	}
	else if(tempBall.x > 7){ //if temp ball hits the right wall

	}
	else if(tempBall.y < 0){ //if we hit the top wall
		p->balldir.y *= -1; //flip the y coordinate
	}
	else if(tempBall.y > 7){ //if we hit the bottom wall
		p->balldir.y *= -1; //flip the y coordinate
	}
	//if statement to check if the temp ball hit the paddle
	if(tempBall.x < 1 && (Lpad.y + 1 == tempBall.y || Lpad.y == tempBall.y || Lpad.y-1 == tempBall.y)){ //if temp ball hits left paddle
		p->balldir.x *= -1;
	}
	else if(tempBall.x > 7 && (Rpad.y + 1 == tempBall.y || Rpad.y == tempBall.y || Rpad.y-1 == tempBall.y)){ //if temp ball hits right paddle
		p->balldir.x *= -1;
	}

	p->ball.x += p->balldir.x; //Moves the real ball once we check to see if the move is valid.
	p->ball.y += p->balldir.y;

}

void pacify_compiler(){

}

void paddle_update(pong_game* p,Smc_queue* q){
	Q_data msg; //Variable that will hold the input request.
	bool data_available;
	data_availabe = q->get(q,&msg);
	if(!data_available) return;
	else{
		switch(msg.int_val){
		case 1:
			if(p->Lpad.y > 0){
				p->Lpad.y -= 1;
			}
			break;
		case 2:
			if(p->Lpad.y < 7){
				p->Lpad.y += 1;
			}
			break;
		case 3:
			if(p->Rpad.y > 0){
				p->Rpad.y -= 1;
			}
			break;
		case 4:
			if(p->Rpad.y < 7){
				p->Rpad.y += 1;
			}
			break;
		default: //do nothing
			pacify_compiler();
		}
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
