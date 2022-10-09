/*
 * show_pong.h
 *
 *  Created on: Sep 27, 2022
 *      Author: CFerguson
 */

#ifndef INC_SHOW_PONG_H_
#define INC_SHOW_PONG_H_

#include "pong_gameplay.h"
#include "smc_queue.h"

void incremental_show_pong(Smc_queue *disp_q);
void incremental_test_screen();

#endif /* INC_SHOW_PONG_H_ */
