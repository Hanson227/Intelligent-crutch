#ifndef __buzzer_H__
#define __buzzer_H__

#include "./hcsr04/bsp_hcsr04.h"
#include "delay.h"

void tim13_init(void)
void tim13_set_freq(uint32_t freq)
void tim13_set_duty(uint32_t duty)
void beep_init(void)
void buzzer(void);

#endif

