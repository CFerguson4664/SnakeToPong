/*
 * snake_gameplay.h
 *
 *  Created on: Sept 27, 2022
 *      Author: Harrison Hiatt
 *      Modified By: Christopher Ferguson
 */

#ifndef PONG_GAMEPLAY_H_
#define PONG_GAMEPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "smc_queue.h"

extern const uint8_t BACKGROUND_COLOR;
extern const uint8_t PADDLE_COLOR;
extern const uint8_t BALL_COLOR;
extern const uint8_t BLANK_COLOR;

extern const uint8_t PADDLE_WIN_COLOR;
extern const uint8_t PADDLE_LOSE_COLOR;

typedef struct {
	int16_t x;
	int16_t y;
} XY_PT;

// Ball Constants
extern const XY_PT INITIAL_BALL;
extern const XY_PT INITIAL_BALLDIR;

// Paddle Constants
extern const XY_PT INITIAL_LPAD; //Left Paddle values
extern const XY_PT INITIAL_RPAD; //Right Paddle values

typedef struct {
	//Ball stuff
	XY_PT ball;
	XY_PT balldir;

	//Paddle stuff
	XY_PT Lpad;
	XY_PT Rpad;

	// Detect if the game has ended
	bool GameEnd;
} pong_game;

void pong_game_init(Smc_queue *disp_q);
void pong_periodic_play(Smc_queue *disp_q);
void paddle_update(Smc_queue* move_q, Smc_queue* disp_q);

#endif /* PONG_GAMEPLAY_H_ */

