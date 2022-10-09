/*
 * snake_repair.c
 *
 *  Created on: Sep 10, 2022
 *      Author: CFerguson
 */

#include "display_VGA.h"
#include "pong_gameplay.h"
#include "stm32g0xx_hal.h"


void VGA_game_cleanup(){
	// Since this is just a game, we will cleanup by rebooting
	HAL_NVIC_SystemReset();
}
