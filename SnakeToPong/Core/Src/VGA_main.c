// Snake game demo for STM Board Nucleo-G071RB
// carrolls@trine.edu
// Copyright 2022 Sean Carroll
// 2022.7.26
//
// Modified by Christopher Ferguson
// 2022.9.27


// SURPRISE - I had to add "#include <stddef.h>' into file sysmem.c to define
// symbol NULL. An oversight by STM? Nice that it seems to be allowed --
// CLEAN and autobuild did not overwrite my improvement.

// This demo project shows coding examples of:
// * Comments
// * JPL compliance
// * Encapsulated, asynchronous IPC (queues)
// * ANSI C
// * OOP
// * Task-decoupling
// * Separated timing and operation logic
//
//
// Disclaimer:
// Yes - it could be prettier, but this is a demo of
// coding practices, not gaming.
// NOTES ON INTERRUPTS
// * I selected timer TIM17 as a periodic timer interrupt (simple timer)
// * CubeMX will generate "static void MX_TIM17_Init(void)" in main.c
// * I wrote the handler - named
// >> "void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)"
// >> as "{timer_isr_count--;}"
// >> where "timer_isr_count" is a volatile int global that I defined.
// * I added the line "HAL_TIM_Base_Start_IT(&htim17);" right after the
// >> CubeMX code calling "MX_TIM17_Init();"
//
// >> >> DOCUMENTATION is tricky: How did I find the name for the ISR, which
// >> >> appears to be "HAL_TIM_PeriodElapsedCallback"? Did I make it up?
// >> >> TWO answers: (1) Find a set of example projects online, and look
// >> >> through them until you find one that uses the interrupt you want.
// >> >> Answer(2): the ISR name is fixed by the Hardware Abstraction Layer
// >> >> header files. To find the name for any interrupt, start
// >> >> by finding the file Core/Inc/stm32g0xx_hal_conf.h
// >> >> in the project -- it points to all the other include files that our
// >> >> chip needs. There is one include file for each peripheral type,
// >> >> including the generic timer. Then find the name for the file that
// >> >> holds timer ISR names. Depending on how you like the IDE, there
// >> >> are 2 seek methods...
// >> >>
// >> >> File-seeking method #2A: You can use the CubeIDE to expand
// >> >> [> stm32g0xx_hal_conf.h] by clicking on the [ > ]. You'll see about
// >> >> 30 files, and stm32g0xx_hal_tim.h is one of them. Double-click it to
// >> >> find the line of stm32g0xx_hal_conf.h that #includes it.
// >> >>
// >> >> File-seeking method 2B: Slower, but does not depend on CubeIDE to
// >> >> extract file names. Open the file stm32g0xx_hal_conf.h. Near the
// >> >> bottom of that file, you'll find an "#ifdef HAL_TIM_MODULE_ENABLED"
// >> >> which is should be true if we intend to use any timers. This is the
// >> >> same line that you'd find using file-seeking method 2A.
// >> >> ONCE YOU FIND THE FILENAME "stm32g0xx_hal_tim.h"
// >> >> Once the file. Search for the word "callback" because that is what
// >> >> the HAL calls developer-written ISRs. My advice - Search backwards
// >> >> from the bottom of the file - usually constants are defined first,
// >> >> and function names at the end. In the version of the file from year
// >> >> 2022, the callback names for timers start at line # 2380, and the 1st
// >> >> one is void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
// >> >>
// >> >> THAT'S WHAT I WANT! "Call this every time the period elapses"
// >> >> sounds very promising. When I tried to out, it works as expected,
// >> >> after I debugged it: the configuration given does not START the
// >> >> timer. Add in the line "HAL_TIM_Base_Start_IT(&htim17);" right at the top
// >> >> of the User Code Part 2 in main.c.


#include <stdint.h>
#include <stdbool.h>
#include <cmsis_gcc.h>
#include <VGA_main.h>
#include "main.h"
#include "pong_gameplay.h"
#include "display_VGA.h"
#include "VGA_enums.h"
#include "smc_queue.h"
#include "show_pong.h"
#include "keypad.h"


// Color Constants
const uint8_t WHITE = 0x3F;
const uint8_t BLACK = 0x00;

// Object Color Constants
const uint8_t BACKGROUND_COLOR = 0x0F;
const uint8_t BLANK_COLOR = 0x00;
const uint8_t BALL_COLOR = 0x3C;
const uint8_t PADDLE_COLOR = 0x03;

const uint8_t PADDLE_WIN_COLOR = 0x0C;
const uint8_t PADDLE_LOSE_COLOR = 0x30;


// Display Constants
const uint8_t DISPLAY_WIDTH = 56;
const uint8_t DISPLAY_HEIGHT = 32;

// Ball Constants
const XY_PT INITIAL_BALL = {6,3};
const XY_PT INITIAL_BALLDIR = {-1,-1};

// Paddle Constants
const XY_PT INITIAL_LPAD = {0,4}; //Left Paddle values
const XY_PT INITIAL_RPAD = {7,4}; //Right Paddle values

///////////////////////////
// Test -without input, the expected output = the ball bounces around until a player wins
// Without_Input - Works!
// #define TEST_WITHOUT_INPUT_VGA
// #define TEST_ONLY_SCREEN


extern volatile int32_t timer_isr_countdown; // Required to control timing

void VGA_ram_health(uint16_t dummy_var, uint16_t pattern){
	// Check if ram is valid
	if (dummy_var != pattern){
		display_checkerboard_VGA();

		// Since this is just a game, we will reboot if we get a ram error
		HAL_NVIC_SystemReset();
	}
}

void VGA_main(void){
	const int32_t timer_isr_500ms_restart = 500;
	const int32_t timer_isr_2000ms_restart = 2000;

	// INITIALIZE THE GAME

	// Construct IPC
	Smc_queue move_q;
	volatile uint16_t ram_dummy_1 = MEMORY_BARRIER_1;
	smc_queue_init(&move_q);

	Smc_queue disp_q;
	volatile uint16_t ram_dummy_2 = MEMORY_BARRIER_2;
	smc_queue_init(&disp_q);

	// Initialize the game
	pong_game_init(&disp_q);

	// Output object
	// Initialize Screen Resolution
	init_display_VGA(FOURX); // FOURX -> Display an 8x8 grid

	// Welcome screen = Checker board for 2 seconds.
	timer_isr_countdown = timer_isr_2000ms_restart;
	display_checkerboard_VGA();
	while (timer_isr_countdown > 0){}


	// Confirm all the rules and paint the initial background.
	timer_isr_countdown = timer_isr_500ms_restart;
	display_color_VGA(BACKGROUND_COLOR);

	// OPERATE THE GAME
	int32_t prior_timer_countdown = timer_isr_countdown;

	while(1){
		VGA_ram_health(ram_dummy_1, MEMORY_BARRIER_1);
		VGA_ram_health(ram_dummy_2, MEMORY_BARRIER_2);

		// ASSERT TIMER COUNTDOWN IN RANGE
		if ((timer_isr_countdown > timer_isr_500ms_restart) || (timer_isr_countdown < 0)){
			display_checkerboard_VGA();

			// Since this is just a game, we will reset if the timer is out of range
			HAL_NVIC_SystemReset();
		}

#ifndef TEST_ONLY_SCREEN
#ifndef TEST_WITHOUT_INPUT_VGA
		// Check for user input every 1 ms & paint one block of the display.
		if (prior_timer_countdown != timer_isr_countdown ){
			prior_timer_countdown = timer_isr_countdown;

			check_buttons(&move_q);
			paddle_update(&move_q, &disp_q);

			incremental_show_pong(&disp_q);
		}
		if (timer_isr_countdown <= 0) {
			// Move and animate every 500 ms
			timer_isr_countdown = timer_isr_500ms_restart;

			check_buttons(&move_q);
			paddle_update(&move_q, &disp_q);

			pong_periodic_play(&disp_q);

			incremental_show_pong(&disp_q);
		}
#endif
#ifdef TEST_WITHOUT_INPUT_VGA
		// Normally "check for user input every 1 ms & show" - here just update display
		if (prior_timer_countdown != timer_isr_countdown ){
			prior_timer_countdown = timer_isr_countdown;

			incremental_show_pong(&disp_q);
		}
		if (timer_isr_countdown <= 0) {
			// Move and animate every 500 ms
			timer_isr_countdown = timer_isr_500ms_restart;

			paddle_update(&move_q, &disp_q);
			pong_periodic_play(&disp_q);

			incremental_show_pong(&disp_q);
		}
#endif
#endif
#ifdef TEST_ONLY_SCREEN
		// Normally "check for user input every 1 ms & show" - here just update display
		if (prior_timer_countdown != timer_isr_countdown ){
			prior_timer_countdown = timer_isr_countdown;

			incremental_test_screen(); //Can be used to test only the screen
		}
		if (timer_isr_countdown <= 0) {
			// Move and animate every 500 ms
			timer_isr_countdown = timer_isr_500ms_restart;

			incremental_test_screen(); //Can be used to test only the screen
		}
#endif
	}
}
