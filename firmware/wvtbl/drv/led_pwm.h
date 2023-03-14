#ifndef __DRV_LED_PWM_H
#define __DRV_LED_PWM_H

#include "stm32g4xx_hal.h"

#define TIM_CHANNEL_BLUE  TIM_CHANNEL_1
#define TIM_CHANNEL_GREEN TIM_CHANNEL_2
#define TIM_CHANNEL_RED   TIM_CHANNEL_3

#define TIMER_HANDLE      htim1

typedef struct h_led_pwm_struct
{
    TIM_HandleTypeDef * timer_handle;
    uint32_t timer_channel;
    float brightness;
} h_led_pwm_t;

// Starts the timer, put the brightness to minimum value
int8_t led_pwm_init(h_led_pwm_t * h_led_pwm);

// brightness between 0 and 1
void led_pwm_set_brightness(h_led_pwm_t * h_led_pwm, float brightness);

#endif //__DRV_LED_PWM_H