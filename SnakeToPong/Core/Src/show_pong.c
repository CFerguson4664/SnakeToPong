/*
 * show_pong.c
 *
 *  Created on: Sep 27, 2022
 *      Author: CFerguson
 */

#include "snake_enums.h"
#include "snake_gameplay.h"
#include "display_VGA.h"

extern uint8_t xCap;
extern uint8_t yCap;

static uint8_t circular_shift(uint8_t color, uint8_t numShifts) {
	uint8_t temp = color << numShifts;
	uint8_t temp2 = color >> (8 - numShifts);

	return (temp | temp2);
}

void incremental_show_pong(const snake_game* s, bool board_updated){
	static int16_t x = 0;
	static int16_t y = 0;
	static int8_t b[CHECKS_WIDE][CHECKS_WIDE] = {0};

	const int checkerboard_squares = CHECKS_WIDE;
	const int green = 0x0C;
	const int red = 0x30;
	const int black = 0x00;

	uint8_t xOffset = (xCap - checkerboard_squares) / 2;
	uint8_t yOffset = (yCap - checkerboard_squares) / 2;

	if (board_updated){
		// clear canvas
		for (int r = 0; r < CHECKS_WIDE; r++){
			for (int c = 0; c < CHECKS_WIDE; c++){
				b[r][c] = 0;
			}
		}
		// paint canvas
		snake_plot(s,b);
		fruit_plot(s,b);
		// restart at top-left
		x = 0;
		y = 0;
	}
	if (b[x][y] == 0){
		display_square_VGA(x + xOffset, y + yOffset, black);
	}
	else if(b[x][y] == -1)
	{
		display_square_VGA(x + xOffset,y + yOffset, red);
	}
	else {
		display_square_VGA(x + xOffset,y + yOffset, green);
	}

	// Update the statics so that the next plot is a new cell.
	x++;
	if (x >= CHECKS_WIDE){
		x = 0;
		y++;
		if (y >= CHECKS_WIDE){
			y = 0;
		}
	}
}



void incremental_test_screen() {
	const uint16_t rep = 1;
	static uint8_t x = 0;
	static uint8_t y = 0;
	static uint8_t color = 3;
	static uint16_t n = 0;

	n++;
	if(n >= rep) {
		n = 0;
		display_square_VGA(x,y,color);

		x++;
		if (x >= xCap){
			x = 0;
			y++;
			if (y >= yCap){
				y = 0;
				color = circular_shift(color, 1);
			}
		}
	}
}
