/*
 * show_snake.c
 *
 *  Created on: Sep 10, 2022
 *      Author: carrolls
 */

#include "snake_enums.h"
#include "snake_gameplay.h"
#include "display_VGA.h"

void incremental_show_snake(const snake_game* s, bool board_updated){
	static int16_t x = 0;
	static int16_t y = 0;
	static int8_t b[CHECKS_WIDE][CHECKS_WIDE] = {0};

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
		display_white_square_VGA(x,y);
	}
	else {
		display_dark_square_VGA(x,y);
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
	const uint16_t lim = 50;
	static uint8_t x = 0;
	static uint8_t y = 0;
	static uint16_t n = 0;
	static uint8_t color = 0;

	n++;
	if(n >= lim){
		n = 0;
		if(color == 0) {
			display_dark_square_VGA(x,y);
		}
		else {
			display_white_square_VGA(x,y);
		}

		x++;
		if (x >= CHECKS_WIDE){
			x = 0;
			y++;
			if (y >= CHECKS_WIDE){
				y = 0;
				color = color ^ 0x01;
			}
		}
	}

}
