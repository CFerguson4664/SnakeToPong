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
#include "quadknob.h"
#include "snake_enums.h"
#include "smc_queue.h"
#include "snake_main.h" // Get checks and display width

typedef struct {int16_t x; int16_t y;} XY_PT;

typedef struct {
	//Ball stuff
	XY_PT ball;
	XY_PT balldir;

	//Paddle stuff
	XY_PT Lpad;
	XY_PT Rpad;

} pong_game;

void pong_game_init(pong_game *p);
void pong_periodic_play(pong_game *p);
void paddle_update(pong_game* p,Smc_queue* q);
bool ball_plot(const pong_game* p, int8_t b [CHECKS_WIDE][CHECKS_WIDE]);
void paddle_plot(const pong_game* p, int8_t b[CHECKS_WIDE][CHECKS_WIDE]);

#endif /* SNAKE_GAMEPLAY_H_ */

