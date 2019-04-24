#include "main.h"

int main(void)
{	
	
	extern struct PLANNING_PATH DEBUG_PLANNING_PATH[];

	NOW_PATH = DEBUG_PLANNING_PATH;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	gpio_init();
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	delay_ms(500);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	//adc_adc3_init();
	
	uart_init(115200);
	gl_GL_model_init();
	can1_CAN1_init();
	cd_chassis_driver_init();
	moving_init();
	tim2_TIM2_init();
	//m3508_init();
	SENSER_Init();
	
	pick_bone_PICK_BONE_Init();
	throw_bone_THROWING_Init();
	tim2_start_main_loop();
	task_frequency_control_main_loop();
	
}



