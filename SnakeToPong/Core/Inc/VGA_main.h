/*
 * snake_main.h
 *
 *  Created on: Sept 27, 2022
 *      Author: Christopher Ferguson
 */

#ifndef INC_VGA_MAIN_H_
#define INC_VGA_MAIN_H_

#define CHECKS_WIDE (((uint16_t)8))
#define PIXELS_PER_CHECK (((uint16_t)8))
#define BIG_DELAY_COUNT 1234567L
#define MEMORY_BARRIER_1 0x1111
#define MEMORY_BARRIER_2 0x2222
#define MEMORY_BARRIER_3 0x3333
void VGA_main(void);


#endif /* INC_VGA_MAIN_H_ */
