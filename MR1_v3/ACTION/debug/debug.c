#include "main.h"


// 利用串口debug
void debug_by_UART(void)
{
	// debug全场定位模块
	#if DEBUG_GLOBAL_LOCATION
	printf("[GL_LOCALTION]: X: %fmm Y:%fmm  YAW: %f° \n",
	GL_POS_DATA.X + ROBOT_POS_OFFSET_DATA.X_CS_OFFSET, 
	GL_POS_DATA.Y + ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET, 
	GL_POS_DATA.YAW + ROBOT_POS_OFFSET_DATA.YAW_CS_OFFSET);
	#endif
	
	// debug定位功能
	#if DEBUG_LOCATION
	printf("[LOCALTION]: X: %fmm Y:%fmm YAW: %f° \n",
	ROBOT_REAL_POS_DATA.X_POS,
	ROBOT_REAL_POS_DATA.Y_POS,
	ROBOT_REAL_POS_DATA.YAW_POS);
	#endif
	
	// debug单个m3508电机（可修改不同的序号）
	#if DEBUG_SINGLE_M3508
	printf("[M3508]: ANGLE: %d RPM:%d CURRENT: %d \n",
	CHASSIS_M3508_REAL_INFO[0].ANGLE,
	CHASSIS_M3508_REAL_INFO[0].RPM,
	CHASSIS_M3508_REAL_INFO[0].CURRENT);
	#endif
	
	// debug底盘的m3508电机
	#if DEBUG_CHASSIS_M3508
	printf("[MOTOR1]: ANGLE: %d RPM:%d CURRENT: %d \n",
	CHASSIS_M3508_REAL_INFO[0].ANGLE,
	CHASSIS_M3508_REAL_INFO[0].RPM,
	CHASSIS_M3508_REAL_INFO[0].CURRENT);
	printf("[MOTOR2]: ANGLE: %d RPM:%d CURRENT: %d \n",
	CHASSIS_M3508_REAL_INFO[1].ANGLE,
	CHASSIS_M3508_REAL_INFO[1].RPM,
	CHASSIS_M3508_REAL_INFO[1].CURRENT);
	printf("[MOTOR3]: ANGLE: %d RPM:%d CURRENT: %d \n",
	CHASSIS_M3508_REAL_INFO[2].ANGLE,
	CHASSIS_M3508_REAL_INFO[2].RPM,
	CHASSIS_M3508_REAL_INFO[2].CURRENT);
	#endif
	
	// debug路径跟踪
	#if DEBUG_PATH_TRACKING
	printf("[PATH_POINT_COUNTER]: %d \n", PATH_POINT_COUNTER);
  //**********************************************************

	#endif
}


// debug单个m3508电机（可修改不同的序号）
// 用法：将函数单独置于main函数中
void debug_single_m3508_in_main(void)
{
	int16_t m3508_rpm = 1900;
	
	delay_init(168);
	uart_init(115200);
	can1_CAN1_init();
	
	PID_parameter_init(&CHASSIS_M3508_PID[0], 10.0, 1.0, 0.0, 15000.0, 15000.0);  // 已调节好的PID参数
	
	while(1)
	{	
		delay_ms(10);
		
		PID_incremental_PID_calculation(&CHASSIS_M3508_PID[0], CHASSIS_M3508_REAL_INFO[0].RPM, m3508_rpm);
		m3508_send_chassis_m3508_current(CHASSIS_M3508_PID[0].output, 0, 0);
		
		debug_by_UART();
	}
}


// 在没有遥控器控制的情况下debug底盘的移动
// USE_ROBOT_KINEMATIC_NOT_WORLD_KINEMATIC需改为1
// 用法：将函数单独置于main函数中
void debug_chassis_without_remote_control_in_main(void)
{
	extern struct ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;
	
	delay_init(168);
	uart_init(115200);
	can1_CAN1_init();
	cd_chassis_driver_init();
		
	ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
	ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 1500;
	ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;
	
	while(1)
	{
		delay_ms(1);
		
		cd_chassis_velocity_judge();
		
		debug_by_UART();
	}
}

// 在单独一条路径上dubug路径跟踪功能
// 用法：正常开启main_loop，将函数置于task函数，需改变NOW_PATH的指向
void debug_path_tracking_with_single_path_in_task(void)
{
	PATH_POINT_SUM = (u32)(sizeof(DEBUG_PLANNING_PATH)/sizeof(DEBUG_PLANNING_PATH[0]));  // 计算路径离散点的总数
	
	if(MOVING_STATE != MOVING_ARRIVED)  // 若还未到达
	{
		moving_path_tracking();
	}
	else
	{
		ROBOT_TARGET_VELOCITY_DATA.Vx_RPM = 0;
		ROBOT_TARGET_VELOCITY_DATA.Vy_RPM = 0;
		ROBOT_TARGET_VELOCITY_DATA.W_RPM = 0;
	}
}







