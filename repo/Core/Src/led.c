/*
 * led.c
 *
 *  Created on: Aug 15, 2023
 *      Author: ksj10
 */


#include "led.h"
#include "tim.h"


led_state_t led_state_inst;
led_t led_inst[HW_LED_LENGTH_MAX];
led_data_t led_data_inst;


void ledInit(int length, int bright)
{
	led_state_inst.length = length;
	led_state_inst.bright = bright;
	led_state_inst.pwm_tx_done = true;

	for(int i = 0; i < length; i++)
	{
	  ledSet(i, 0x00, 0x00, 0x00);
	}

	uint32_t tick = HAL_GetTick();
	while(true)
	{
	  if(led_state_inst.pwm_tx_done == true)
	  {
		  if(HAL_GetTick() - tick > 100)
		  {
			  break;
		  }
		  ledStart();
	  }
	}
}

void ledSet(int num, int red, int green, int blue)
{
	led_inst[num].red = red;
	led_inst[num].green = green;
	led_inst[num].blue = blue;
}

void ledStart(void)
{
	int now_data_header = 0;
	int now_led_num = 0;

	for(int i = now_led_num; i < led_state_inst.length; i++)
	{
		uint32_t data = (led_inst[i].green << 16);
		data |= (led_inst[i].red << 8);
		data |= led_inst[i].blue;

		for(int j = 23; j >= 0; j--)
		{
			if(data & (1 << j))
			{
				led_data_inst.data[now_data_header] = 51;
			}
			else
			{
				led_data_inst.data[now_data_header] = 26;
			}
			now_data_header += 1;
		}
	}
	for(int i = now_data_header; i < now_data_header + 50; i++)
	{
		led_data_inst.data[i] = 0;
	}

	led_state_inst.pwm_tx_done = false;
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)led_data_inst.data, (24 * led_state_inst.length) + 50);
}

void drawCarrot(int bright_div)
{
	uint8_t carrot_org_num[] = {0, 1, 2, 3, 4, 10, 11, 12, 13, 14, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 34, 35, 36, 37, 38, 42, 43, 44};
	uint8_t carrot_green_num[] = {39, 41, 53, 55, 57, 59};
	uint8_t empty[] = {5, 6, 7, 8, 9, 16, 23, 24, 31, 32, 33, 40, 45, 46, 47, 48, 49, 50, 51, 52, 54, 56, 58};

	for(int i = 0; i < sizeof(carrot_org_num) / sizeof(uint8_t); i++)
	{
		ledSet(carrot_org_num[i], 255 / bright_div, 69 / bright_div, 0 / bright_div);
	}
	for(int i = 0; i < sizeof(carrot_green_num) / sizeof(uint8_t); i++)
	{
		ledSet(carrot_green_num[i], 0 / bright_div, 128 / bright_div, 0 / bright_div);
	}
	for(int i = 0; i < sizeof(empty) / sizeof(uint8_t); i++)
	{
		ledSet(empty[i], 16 / bright_div, 16 / bright_div, 16 / bright_div);
	}

	ledStart();
}

void drawKoreanFlag(void)
{
	ledInit(HW_LED_LENGTH_MAX, 100);
	uint8_t black[] = {0, 7, 9, 14, 41, 46, 48, 55};
	uint8_t gray[] = {1, 6, 8, 15, 40, 47, 49, 54};
	uint8_t red[] = {28, 35, 43, 44};
	uint8_t blue[] = {19, 20, 27, 36};
	uint8_t empty[] = {2, 3, 4, 5, 10, 11, 12, 13, 16, 17, 18, 21, 22, 23, 24, 25, 26, 29, 30, 31, 32, 33, 34, 37, 38, 39, 42, 45, 50, 51, 52, 53, 56, 57, 58, 59};

	for(int i = 0; i < sizeof(black) / sizeof(uint8_t); i++)
	{
		ledSet(black[i], 1, 1, 1);
	}
	for(int i = 0; i < sizeof(gray) / sizeof(uint8_t); i++)
	{
		ledSet(gray[i], 5, 5, 5);
	}
	for(int i = 0; i < sizeof(empty) / sizeof(uint8_t); i++)
	{
		ledSet(empty[i], 16, 16, 16);
	}
	for(int i = 0; i < sizeof(red) / sizeof(uint8_t); i++)
	{
		ledSet(red[i], 255, 0, 0);
	}
	for(int i = 0; i < sizeof(blue) / sizeof(uint8_t); i++)
	{
		ledSet(blue[i], 0, 0, 255);
	}

	ledStart();
}
