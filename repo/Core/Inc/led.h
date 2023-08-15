/*
 * led.h
 *
 *  Created on: Aug 15, 2023
 *      Author: ksj10
 */

#ifndef INC_LED_H_
#define INC_LED_H_


#include "main.h"
#include "stdbool.h"


#define HW_LED_LENGTH_MAX (74U)

typedef struct
{
	int length;
	int bright;
	bool pwm_tx_done;
}led_state_t;

typedef struct
{
	int red;
	int green;
	int blue;
}led_t;

typedef struct
{
	uint16_t data[24 * HW_LED_LENGTH_MAX + 50];
}led_data_t;


extern led_state_t led_state_inst;
extern led_t led_inst[HW_LED_LENGTH_MAX];
extern led_data_t led_data_inst;

void ledInit(int length, int bright);
void ledSet(int num, int red, int green, int blue);
void ledStart(void);

void drawCarrot(int bright_div);
void drawKoreanFlag(void);


#endif /* INC_LED_H_ */
