#include "bsp_pwm.h"
#include "bsp_led.h"


//函数前加static,保证函数只能在本函数下调用，防止其他地方调用函数
void bsp_pwm_config(uint16_t div,uint16_t per)
{
	//GPIO配置
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_5);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);
	gpio_af_set(GPIOA,GPIO_AF_1,GPIO_PIN_5);//选择复用
	//
	timer_parameter_struct timer_pwm_initpara;//定义定时器结构体

	timer_pwm_initpara.prescaler = div-1;  //结构体配置                      
	timer_pwm_initpara.alignedmode = TIMER_COUNTER_EDGE;                   
	timer_pwm_initpara.counterdirection = TIMER_COUNTER_UP;                  
	timer_pwm_initpara.clockdivision = TIMER_CKDIV_DIV1;                     
	timer_pwm_initpara.period = per-1;                        
	timer_pwm_initpara.repetitioncounter = 0; 
	
	rcu_periph_clock_enable(RCU_TIMER1);//开启定时器时钟
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//配置分频器
	
	timer_deinit(TIMER1);
	timer_init(TIMER1,&timer_pwm_initpara);//定时器初始化
	timer_enable(TIMER1);//定时器开启
	
	timer_oc_parameter_struct timer_ocinitpara;//配置定时器输出结构体
	
	timer_ocinitpara.ocpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocinitpara.outputstate = TIMER_CCX_ENABLE;
	
	timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocinitpara);
	
	timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,0);//脉冲配置（占空比）
	timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);//模式配置（pwm模式）
	timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);//
	timer_auto_reload_shadow_enable(TIMER1);
	
}
void pwm_breath_led(void)
{
	static uint16_t direct = 0;
	static uint16_t value = 0;
	if(direct==0)
	{
		value+=200;
		if(value>=10000)
			direct =1;
	}
	if(direct==1)
	{
		value-=200;
		if(value<=0)
			direct =0;
	}
	timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,value);
	delay_1ms(50);//定时器频率为10000，则周期为0.1ms,需要大于0.1ms延时！！！
}




