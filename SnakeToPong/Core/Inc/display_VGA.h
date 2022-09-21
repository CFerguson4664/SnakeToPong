/*
 * display_DOGS_102.h
 *
 *  Created on: Jul 26, 2022
 *      Author: carrolls
 */

#ifndef INC_DISPLAY_VGA
#define INC_DISPLAY_VGA

#define CHECKS_WIDE (((uint16_t)8))
#define DISPLAY_WIDTH (((uint16_t) 102))

typedef struct {uint8_t nCD:1; uint8_t byte;} DOGS_packet;

void display_init_VGA(void);
void display_blank_VGA(void);
void display_checkerboard_VGA(void);
void display_snake_board_VGA(int8_t sb[CHECKS_WIDE][CHECKS_WIDE]);
void display_white_square_VGA(uint8_t l_to_r, uint8_t t_to_b);
void display_dark_square_VGA(uint8_t l_to_r, uint8_t t_to_b);


#endif /* INC_DISPLAY_VGA */
