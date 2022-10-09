// MIT License - Harrison Hiatt - (c) 2022
// Modified by Christopher Ferguson

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pong_gameplay.h"
#include "keypad.h"

static void clear_board(Smc_queue *disp_q) {
	for(uint8_t x = 0; x < 8; x++) {
		for(uint8_t y = 0; y < 8; y++) {
			Q_data data;
			VGA_Pixel pixel;
			pixel.point.x = x;
			pixel.point.y = y;
			pixel.color = BLANK_COLOR;
			data.pixel = pixel;

			disp_q->put(disp_q, &data);
		}
	}
}

static void redraw_paddle(XY_PT *pos, Smc_queue *disp_q) {
	// ASSERT -> We are drawing a paddle so it must be either in column 0 or 7
	if(pos->x != 0 && pos-> x != 7) {
		return;
	}

	// ASSERT -> The center of the paddle must stay at least one pixel from the top and bottom of the screen
	if(pos->y <= 0 || pos->y >= 7) {
		return;
	}

	// Redraw the row
	for(uint8_t i = 0; i < 8; i++) {
		Q_data data;
		VGA_Pixel pixel;
		pixel.point.x = pos->x;
		pixel.point.y = i;

		if(i >= (pos->y-1) && i <= (pos->y+1)) {
			pixel.color = PADDLE_COLOR;
		}
		else {
			pixel.color = BLANK_COLOR;
		}

		data.pixel = pixel;

		disp_q->put(disp_q, &data);
	}
}

static void resolve_paddle_changed_pixels(XY_PT *pos, int8_t moved, Smc_queue *disp_q) {

	// ASSERT -> We are drawing a paddle so it must be either in column 0 or 7
	if(pos->x != 0 && pos-> x != 7) {
		return;
	}

	// ASSERT -> The center of the paddle must stay at least one pixel from the top and bottom of the screen
	if(pos->y <= 0 || pos->y >= 7) {
		return;
	}

	switch(moved) {
		case 0: { // The paddle didn't move, do nothing - First for efficiency
			break;
		}
		case -2: { // The paddle moved up two pixels

			// 0 -> Empty
			// R -> Cleared by this case
			// r -> Cleared by next case

			// P -> Existing Paddle
			// A -> Added by this case
			// a -> Added by next case

			// 0      a
			// 0      A  - Center
			// P      P
			// P  ->  r
			// P      R
			// 0      0
			// 0      0
			// 0      0

			// Clear the old pixel --> R
			Q_data clear_data;
			VGA_Pixel clear_pixel;
			clear_pixel.point.x = pos->x;
			clear_pixel.point.y = pos->y + 3; //Two below the paddle -> paddle is three tall, so three below its center
			clear_pixel.color = BLANK_COLOR;;
			clear_data.pixel = clear_pixel;

			disp_q->put(disp_q, &clear_data);

			// Draw the new pixel --> A
			Q_data disp_data;
			VGA_Pixel disp_pixel;
			disp_pixel.point.x = pos->x;
			disp_pixel.point.y = pos->y; //Center of the paddle
			disp_pixel.color = PADDLE_COLOR;
			disp_data.pixel = disp_pixel;

			disp_q->put(disp_q, &disp_data);

			// No break here, also need to do the next case
		}
		case -1: { // The paddle moved up one pixel

			// 0 -> Empty
			// R -> Cleared by this case

			// P -> Existing Paddle
			// A -> Added by this case

			// 0      0
			// 0      A
			// P      P - Center
			// P  ->  P
			// P      R
			// 0      0
			// 0      0
			// 0      0

			// Clear the old pixel --> R
			Q_data clear_data;
			VGA_Pixel clear_pixel;
			clear_pixel.point.x = pos->x;
			clear_pixel.point.y = pos->y + 2; //Below the paddle -> paddle is three tall, so two below its center
			clear_pixel.color = BLANK_COLOR;;
			clear_data.pixel = clear_pixel;

			disp_q->put(disp_q, &clear_data);

			// Draw the new pixel -- A
			Q_data disp_data;
			VGA_Pixel disp_pixel;
			disp_pixel.point.x = pos->x;
			disp_pixel.point.y = pos->y - 1; //Top of the paddle -> paddle is three tall, so one above its center
			disp_pixel.color = PADDLE_COLOR;;
			disp_data.pixel = disp_pixel;

			disp_q->put(disp_q, &disp_data);

			break;
		}
		case 2: { // The paddle moved down two pixels

			// 0 -> Empty
			// R -> Cleared by this case
			// r -> Cleared by next case

			// P -> Existing Paddle
			// A -> Added by this case
			// a -> Added by next case

			// 0      0
			// 0      0
			// P      R
			// P  ->  r
			// P      P
			// 0      A  - Center
			// 0      a
			// 0      0

			// Clear the old pixel --> R
			Q_data clear_data;
			VGA_Pixel clear_pixel;
			clear_pixel.point.x = pos->x;
			clear_pixel.point.y = pos->y - 3; //Two above the paddle -> paddle is three tall, so three above its center
			clear_pixel.color = BLANK_COLOR;;
			clear_data.pixel = clear_pixel;

			disp_q->put(disp_q, &clear_data);

			// Draw the new pixel --> A
			Q_data disp_data;
			VGA_Pixel disp_pixel;
			disp_pixel.point.x = pos->x;
			disp_pixel.point.y = pos->y; //Center of the paddle
			disp_pixel.color = PADDLE_COLOR;;
			disp_data.pixel = disp_pixel;

			disp_q->put(disp_q, &disp_data);

			// No break here, also need to do the next case
		}
		case 1: { // The paddle moved down one pixel

			// 0 -> Empty
			// R -> Cleared by this case

			// P -> Existing Paddle
			// A -> Added by this case

			// 0      0
			// 0      0
			// P      R
			// P  ->  P
			// P      P - Center
			// 0      A
			// 0      0
			// 0      0

			// Clear the old pixel --> R
			Q_data clear_data;
			VGA_Pixel clear_pixel;
			clear_pixel.point.x = pos->x;
			clear_pixel.point.y = pos->y - 2; //Above the paddle -> paddle is three tall, so two above its center
			clear_pixel.color = BLANK_COLOR;;
			clear_data.pixel = clear_pixel;

			disp_q->put(disp_q, &clear_data);

			// Draw the new pixel --> A
			Q_data disp_data;
			VGA_Pixel disp_pixel;
			disp_pixel.point.x = pos->x;
			disp_pixel.point.y = pos->y + 1; //Bottom of the paddle -> paddle is three tall, so one below its center
			disp_pixel.color = PADDLE_COLOR;;
			disp_data.pixel = disp_pixel;

			disp_q->put(disp_q, &disp_data);

			break;
		}
		default : {
			// The move was very large, we will redraw the paddle column
			redraw_paddle(pos, disp_q);
			break;
		}
	}
}

void pong_game_init(pong_game* p, Smc_queue *disp_q){
	//Set the initial position and velocity of the ball
	p->ball.x = INITIAL_BALL.x;
	p->ball.y = INITIAL_BALL.y;

	p->balldir.x = INITIAL_BALLDIR.x;
	p->balldir.y = INITIAL_BALLDIR.y;

	//Places the {x,y} values into the left and right paddle objects.
	p->Lpad.x = INITIAL_LPAD.x;
	p->Lpad.y = INITIAL_LPAD.y;

	p->Rpad.x = INITIAL_RPAD.x;
	p->Rpad.y = INITIAL_RPAD.y;

	clear_board(disp_q);
	redraw_paddle(&p->Rpad, disp_q);
	redraw_paddle(&p->Lpad, disp_q);

}

void pong_periodic_play(pong_game* p, Smc_queue *disp_q){

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


	// Handle removing the old ball
	Q_data clear;
	VGA_Pixel clear_old_ball;
	clear_old_ball.point.x = p->ball.x;
	clear_old_ball.point.y = p->ball.y;
	clear_old_ball.color = BLANK_COLOR;;
	clear.pixel = clear_old_ball;

	disp_q->put(disp_q, &clear);

	//Moves the real ball once we check to see if the move is valid.
	p->ball.x += p->balldir.x;
	p->ball.y += p->balldir.y;

	// Handle drawing the new ball
	Q_data draw;
	VGA_Pixel disp_new_ball;
	disp_new_ball.point.x = p->ball.x;
	disp_new_ball.point.y = p->ball.y;
	disp_new_ball.color = BALL_COLOR;
	draw.pixel = disp_new_ball;

	disp_q->put(disp_q, &draw);
}

void pacify_compiler(){

}

void paddle_update(pong_game* p, Smc_queue* move_q, Smc_queue* disp_q){
	Q_data msg; //Variable that will hold the input request.
	bool data_available;
	data_available = move_q->get(move_q, &msg); //Test to see of there is input data

	uint8_t RPad_move = 0;
	uint8_t LPad_move = 0;


	while(data_available) {
		if(!data_available) return;
		else{
			switch(msg.button){
			case Left_Up:
				if(p->Lpad.y > 1){
					p->Lpad.y -= 1; //move left paddle down
					LPad_move -= 1;
				}
				break;
			case Left_Down:
				if(p->Lpad.y < 6){
					p->Lpad.y += 1; //move left paddle up
					LPad_move += 1;
				}
				break;
			case Right_Up:
				if(p->Rpad.y > 1){
					p->Rpad.y -= 1; //move right paddle up
					RPad_move -= 1;
				}
				break;
			case Right_Down:
				if(p->Rpad.y < 6){
					p->Rpad.y += 1; //Move right paddle down
					RPad_move += 1;
				}
				break;
			default: //do nothing
				pacify_compiler();
			}
		}

		data_available = move_q->get(move_q, &msg);
	}

	resolve_paddle_changed_pixels(&p->Lpad, LPad_move, disp_q);
	resolve_paddle_changed_pixels(&p->Rpad, RPad_move, disp_q);
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
