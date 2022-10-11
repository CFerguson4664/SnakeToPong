/*
 * show_pong.c
 *
 *  Created on: Sep 27, 2022
 *      Author: Christopher Ferguson
 */

#include "pong_gameplay.h"
#include "display_VGA.h"
#include "VGA_main.h"
#include "smc_queue.h"

extern uint8_t xCap;
extern uint8_t yCap;

static uint8_t circular_shift(uint8_t color, uint8_t numShifts) {
	uint8_t temp = color << numShifts;
	uint8_t temp2 = color >> (8 - numShifts);

	return (temp | temp2);
}

void incremental_show_pong(Smc_queue *disp_q){
	static const uint8_t checkerboard_squares = CHECKS_WIDE;

	uint8_t xOffset = (xCap - checkerboard_squares) / 2;
	uint8_t yOffset = (yCap - checkerboard_squares) / 2;

	Q_data msg; // Variable that will hold the display request.
	bool data_available;
	data_available = disp_q->get(disp_q, &msg); //Test to see of there is input data

	// For loop used to guarantee termination of the loop
	for(uint8_t iter = 0; iter < 100; iter++) {
		if(!data_available) return;
		else{
			// ASSERT - Pixel Coordinates must be between 0 and 7 inclusive, but 0 is checked by the fact it is an unsigned in
			if(msg.pixel.point.x > 7 ||  msg.pixel.point.y > 7) {
				continue; // Skip to next iteration of loop;
			}

			display_square_VGA(msg.pixel.point.x + xOffset, msg.pixel.point.y + yOffset, msg.pixel.color);
		}

		data_available = disp_q->get(disp_q, &msg);
	}
}

// Used to test only the screen
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
