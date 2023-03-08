#include "led_pwm.h"

// Starts the timer, put the brightness to minimum value
void led_pwm_init(h_led_pwm_t * h_led_pwm)
{
    HAL_TIM_PWM_Start(h_led_pwm->timer_handle, h_led_pwm->timer_channel);
    led_pwm_set_brightness(h_led_pwm, 0.f);
}

// brightness between 0 and 1
void led_pwm_set_brightness(h_led_pwm_t * h_led_pwm, float brightness)
{
    uint16_t bright = 256 - (uint16_t)(brightness * 255.f);
    __HAL_TIM_SET_COMPARE(h_led_pwm->timer_handle, h_led_pwm->timer_channel, bright);
}
