/*
 * snake_gameplay.h
 *
 *  Created on: Jul 28, 2022
 *      Author: carrolls
 */

#ifndef SNAKE_GAMEPLAY_H_
#define SNAKE_GAMEPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "smc_queue.h"

extern const uint8_t BACKGROUND_COLOR;
extern const uint8_t PADDLE_COLOR;
extern const uint8_t BALL_COLOR;
extern const uint8_t BLANK_COLOR;

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

} pong_game;

void pong_game_init(pong_game *p, Smc_queue *disp_q);
void pong_periodic_play(pong_game *p, Smc_queue *disp_q);
void paddle_update(pong_game* p, Smc_queue* move_q, Smc_queue* disp_q);
bool ball_plot(const pong_game* p, int8_t b [CHECKS_WIDE][CHECKS_WIDE]);
void paddle_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);

#endif /* SNAKE_GAMEPLAY_H_ */

