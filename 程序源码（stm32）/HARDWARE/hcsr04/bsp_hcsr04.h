#ifndef __HCSR04_H
#define __HCSR04_H

#include "stm32f4xx.h"

void sr04_init(void);//配置PE2为触发信号引脚，PE3为回想信号引脚
uint32_t sr04_get_distance(void);

#endif /*__HCSR04_H*/

