/*
 * display_DOGS_102.h
 *
 *  Created on: Oct 4, 2022
 *      Author: Christopher Ferguson
 */

#ifndef INC_DISPLAY_VGA
#define INC_DISPLAY_VGA

#include <stdint.h>
#include "VGA_enums.h"
#include "VGA_main.h"

extern const uint8_t DISPLAY_WIDTH;
extern const uint8_t DISPLAY_HEIGHT;

extern const uint8_t WHITE;
extern const uint8_t BLACK;

typedef struct {
	uint8_t x;
	uint8_t y;
} Point;

typedef struct {
	Point point;
	uint8_t color;
} VGA_Pixel;

void init_display_VGA(enum VGA_Scale scale);
void display_blank_VGA(void);
void display_color_VGA(uint8_t color);
void display_checkerboard_VGA(void);
void display_snake_board_VGA(int8_t sb[CHECKS_WIDE][CHECKS_WIDE]);
void display_white_square_VGA(uint8_t l_to_r, uint8_t t_to_b);
void display_dark_square_VGA(uint8_t l_to_r, uint8_t t_to_b);
void display_square_VGA(uint8_t l_to_r, uint8_t t_to_b, uint8_t color);
void set_VGA_scale(enum VGA_Scale scale);

#endif /* INC_DISPLAY_VGA */
