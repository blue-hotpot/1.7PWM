#ifndef _BSP_PWM_H
#define _BSP_PWM_H

#include "gd32f4xx.h"
#include "systick.h"


void bsp_pwm_config(uint16_t div,uint16_t per);
void pwm_breath_led(void);


#endif



