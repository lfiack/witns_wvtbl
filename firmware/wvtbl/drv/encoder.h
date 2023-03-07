#ifndef __DRV_ENCODER_H
#define __DRV_ENCODER_H

#define TIM_CHANNEL_BLUE  TIM_CHANNEL_1
#define TIM_CHANNEL_GREEN TIM_CHANNEL_2
#define TIM_CHANNEL_RED   TIM_CHANNEL_3

#define TIMER_HANDLE      htim1

void encoder_init(void);

// brightness between 0 and 1
void encoder_set_red_led(float brightness);
void encoder_set_green_led(float brightness);
void encoder_set_blue_led(float brightness);

#endif // __DRV_ENCODER_H