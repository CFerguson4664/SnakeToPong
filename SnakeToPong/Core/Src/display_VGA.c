// carrolls@trine.edu 2022.7.22
// Now part of G071_Demo_Snake
///////////////////////////////////////////////
// DogsA - initializes and displays a checkerboard of 8x8 pixel
// squares alternating foreground/background color on a DOGS102
// 64x102 pixel graphic display ($17).
// By Sean Carroll, Trine U, v. 2022.6.22 = Nucleo-G071 SPI Master.
// Purpose = Demo project - uses CubeMX and HAL.
// NOTE - SPI1 appears to be used by the debugger.
// Software-Defined Requirements:
//     SPI.SCK out on PB10, SPI.MOSI on PC3,
//     GP_O.nReset out @ PA_0, GPO.nCS @ PD_9, GPO.nC_D @ PA_1
//  TESTED as of 2022.6.21
// POWER = 3.3 V and GND.
//////////////////////////////////////////////
// /*WIRING:
		// 15|---||-GND
		// 16|-------,
		// 17|--||,  |
		// 18|----'  |
		// 19|----||-'
		// 20|--GND
		// 21|--GND
		// 22|--3.3v
		// 23|--3.3v
		// 24|--SDA-- (NucleoG071/PC3)
		// 25|--SCK-- (NucleoG071/PB10)
		// 26|--CD--- (NucleoG071/PA1)
		// 27|--RST-- (NucleoG071/PA0)
		// 28|--CS0-- (NucleoG071/PD9 - nSS)
// */


// This display driver uses the pins defined in an .ioc file
// created with STM32CubeMX.
#include "main.h"
#include "display_VGA.h"
#include "snake_enums.h"
#include "snake_gameplay.h" // To learn "CHECKS_WIDE"

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
			xCap = 0;   // xCap = 56
			yCap = 0;	// yCap = 32
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
	const uint8_t black = 0x00;

	for(uint8_t x = 0; x < xCap; x++) {
		for(uint8_t y = 0; y < yCap; y++) {
			display_square_VGA(x,y,black);
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
	const uint8_t black = 0x00;
	const uint8_t white = 0x3F;

	for(uint8_t x = 0; x < xCap; x++) {
		for(uint8_t y = 0; y < yCap; y++) {
			uint8_t isBlack = (x & 0x01) ^ (y & 0x01);
			if(isBlack == 0) {
				display_square_VGA(x, y, black);
			}
			else {
				display_square_VGA(x, y, white);
			}
		}
	}
}


void display_snake_board_VGA(int8_t board[CHECKS_WIDE][CHECKS_WIDE]){
	const int checkerboard_squares = CHECKS_WIDE;
	const int green = 0x0C;
	const int black = 0x0C;

	uint8_t xOffset = (xCap - checkerboard_squares) / 2;
	uint8_t yOffset = (yCap - checkerboard_squares) / 2;

	for(int8_t x = 0; x < checkerboard_squares; x++) {
		for(int8_t y = 0; y < checkerboard_squares; y++) {
			if(board[x][y] != 0) {
				display_square_VGA((x + xOffset), (y + yOffset), green);
			}
			else {
				display_square_VGA((x + xOffset), (y + yOffset), black);
			}
		}
	}
}


void display_white_square_VGA(uint8_t l_to_r, uint8_t t_to_b){
	display_square_VGA(l_to_r, t_to_b, 0xFF);
}

void display_dark_square_VGA(uint8_t l_to_r, uint8_t t_to_b){
	display_square_VGA(l_to_r, t_to_b, 0x00);
}

void display_square_VGA(uint8_t l_to_r, uint8_t t_to_b, uint8_t color){
	spi_SS_Low();
	spi_msg_out_VGA(l_to_r);
	spi_msg_out_VGA(t_to_b);
	spi_msg_out_VGA(color);
	spi_SS_High();
}
