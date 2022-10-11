// Christopher Ferguson
// 10/4/2022

// This display driver uses the pins defined in an .ioc file
// created with STM32CubeMX.
#include "main.h"
#include "VGA_main.h"
#include "display_VGA.h"

/////////////////////////////////////////////////////////////////////////////
// Hand-written code that Cube won't help with
/////////////////////////////////////////////////////////////////////////////

extern  SPI_HandleTypeDef hspi2; //TUTORIAL ...
// I wrote this line to borrow the hspi2 struct which was defined
// for me by STM32CubeMX in MAIN.C. I have to, or the the calls
// to HAL_SPI_Transmit(&hspi2 ...) won't build.

uint8_t xCap = 0;
uint8_t yCap = 0;
static enum VGA_Scale currentScale = UNKNOWN;

void init_display_VGA(enum VGA_Scale scale) {
	const uint8_t numBlanks = 5;

	set_VGA_scale(scale);

	for(uint8_t i = 0; i < numBlanks; i++) {
		display_blank_VGA();
	}
}

void set_VGA_scale(enum VGA_Scale scale) {
	switch(scale) {
		case ONEX : {
			currentScale = ONEX;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
			xCap = DISPLAY_WIDTH;   // xCap = 56
			yCap = DISPLAY_HEIGHT;	// yCap = 32
			break;
		}
		case TWOX : {
			currentScale = TWOX;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
			xCap = DISPLAY_WIDTH / 2;   // xCap = 28
			yCap = DISPLAY_HEIGHT / 2;  // yCap = 16
			break;
		}
		case FOURX : {
			currentScale = FOURX;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
			xCap = DISPLAY_WIDTH / 4;   // xCap = 14
			yCap = DISPLAY_HEIGHT / 4;  // yCap = 8
			break;
		}
		default: {
			currentScale = UNKNOWN;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
			xCap = 0;   // xCap = 0
			yCap = 0;	// yCap = 0
			break;
		}
	}
}

static void spi_msg_out_VGA(uint8_t pkt){
	const uint32_t SPI_timeout = 100;

	// Send one packet (a byte).
	// Use the HAL.
	// SPI1: Send pkt (1 byte), and timeout only after 100 ticks
	// EXPECT ABOUT 16 us of delay from PD_6 low to SCK action.
	HAL_SPI_Transmit(&hspi2, &pkt, 1, SPI_timeout);
}

static void spi_SS_Low() {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
}

static void spi_SS_High() {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);  // * GPIO_PIN_9 == 1<<9
}


void display_blank_VGA(){
	for(uint8_t x = 0; x < xCap; x++) {
		for(uint8_t y = 0; y < yCap; y++) {
			display_square_VGA(x,y,BLACK);
		}
	}
}

void display_color_VGA(uint8_t color){
	for(uint8_t x = 0; x < xCap; x++) {
		for(uint8_t y = 0; y < yCap; y++) {
			display_square_VGA(x,y,color);
		}
	}
}


void display_checkerboard_VGA(void){
	for(uint8_t x = 0; x < xCap; x++) {
		for(uint8_t y = 0; y < yCap; y++) {
			uint8_t isBlack = (x & 0x01) ^ (y & 0x01);
			if(isBlack == 0) {
				display_square_VGA(x, y, BLACK);
			}
			else {
				display_square_VGA(x, y, WHITE);
			}
		}
	}
}

void display_white_square_VGA(uint8_t l_to_r, uint8_t t_to_b){
	display_square_VGA(l_to_r, t_to_b, WHITE);
}

void display_dark_square_VGA(uint8_t l_to_r, uint8_t t_to_b){
	display_square_VGA(l_to_r, t_to_b, BLACK);
}

void display_square_VGA(uint8_t l_to_r, uint8_t t_to_b, uint8_t color){
	spi_SS_Low();
	spi_msg_out_VGA(l_to_r);
	spi_msg_out_VGA(t_to_b);
	spi_msg_out_VGA(color);
	spi_SS_High();
}
