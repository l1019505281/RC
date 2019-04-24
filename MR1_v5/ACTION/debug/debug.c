#include "main.h"

extern trapezoidal_V_struct traV;
extern float V_tangent,V_parallel;
extern int flag_symbol;
// ���ô���debug
void debug_by_UART(void)
{
	float V;
	// debugȫ����λģ��
	#if DEBUG_GLOBAL_LOCATION
	printf("[GL_LOCALTION]: X: %fmm Y:%fmm  YAW: %f�� \n",
	GL_POS_DATA.X + ROBOT_POS_OFFSET_DATA.X_CS_OFFSET, 
	GL_POS_DATA.Y + ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET, 
	GL_POS_DATA.YAW + ROBOT_POS_OFFSET_DATA.YAW_CS_OFFSET);
	#endif
	
	// debug��λ����
	#if DEBUG_LOCATION
//	printf("[LOCALTION]: X: %fmm Y:%fmm YAW: %f�� \n",
//	ROBOT_REAL_POS_DATA.X_POS,
//	ROBOT_REAL_POS_DATA.Y_POS,
//	ROBOT_REAL_POS_DATA.YAW_POS);
	printf("%f\r\n",ROBOT_REAL_POS_DATA.X_POS);
	printf("%f\r\n",ROBOT_REAL_POS_DATA.Y_POS);
	printf("%f\r\n",GL_POS_DATA.YAW);//ROBOT_REAL_POS_DATA.YAW_POS);
	#endif
	
	// debug����m3508��������޸Ĳ�ͬ����ţ�
	#if DEBUG_SINGLE_M3508
	printf("[M3508]: ANGLE: %d RPM:%d CURRENT: %d \n",
	CHASSIS_M3508_REAL_INFO[0].ANGLE,
	CHASSIS_M3508_REAL_INFO[0].RPM,
	CHASSIS_M3508_REAL_INFO[0].CURRENT);
	#endif
	
	// debug���̵�m3508���
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
	
	// debug·������
	#if DEBUG_PATH_TRACKING
//	printf("[PATH_TRACKING]: PATH_POINT_COUNTER: %d VELOCITY: %f W_RPM: %d \n",
//	PATH_POINT_COUNTER,
//	MOVING_BETWEEN_2PTS_INFO_DATA.IMMEDIATE_VELOCITY,
//	ROBOT_TARGET_VELOCITY_DATA.W_RPM);


	V = V_tangent;//V_parallel;//sqrt(pow(ROBOT_TARGET_VELOCITY_DATA.Vx,2)+pow(ROBOT_TARGET_VELOCITY_DATA.Vy,2));
	//printf("%f\n\r",traV.s_total);
	printf("%d\n\r",PATH_POINT_COUNTER);
	printf("%f\n\r",V);
	printf("%d\n\r",flag_symbol);//ROBOT_TARGET_VELOCITY_DATA.W_RPM);


	#endif
}


// debug����m3508��������޸Ĳ�ͬ����ţ�
// �÷�����������������main������
void debug_single_m3508_in_main(void)
{
	int16_t m3508_rpm = 1900;
	
	delay_init(168);
	uart_init(115200);
	can1_CAN1_init();
	
	PID_parameter_init(&CHASSIS_M3508_PID[0], 10.0, 1.0, 0.0, 15000.0, 15000.0);  // �ѵ��ںõ�PID����
	
	while(1)
	{	
		delay_ms(10);
		
		PID_incremental_PID_calculation(&CHASSIS_M3508_PID[0], CHASSIS_M3508_REAL_INFO[0].RPM, m3508_rpm);
		m3508_send_chassis_m3508_current(CHASSIS_M3508_PID[0].output, 0, 0);
		
		debug_by_UART();
	}
}


// ��û��ң�������Ƶ������debug���̵��ƶ�
// USE_ROBOT_KINEMATIC_NOT_WORLD_KINEMATIC���Ϊ1
// �÷�����������������main������
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

// �ڵ���һ��·����dubug·�����ٹ���
// �÷�����������main_loop������������task��������ı�NOW_PATH��ָ��
void debug_path_tracking_with_single_path_in_task(void)
{
	PATH_POINT_SUM = (u32)(sizeof(DEBUG_PLANNING_PATH)/sizeof(DEBUG_PLANNING_PATH[0]));  // ����·����ɢ�������
	
	
	if(MOVING_STATE != MOVING_ARRIVED)  // ����δ����
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


void debug_grasping_task()
{
	if(GRASPING_STATE != GRASPING_SUCCEEDED)
	{
		pick_bone_GRASPING_TASK();
	}
	
	
}

void debug_pick_bone_task(void)
{
	//if(MOVING_STATE == MOVING_ARRIVED)GRASPING_STATE = GRASPING_START;
	GRASPING_STATE = GRASPING_START;	
	pick_bone_GRASPING_TASK();

}

void debug_throw_bone_task(void)
{
	if(GRASPING_STATE == GRASPING_SUCCEEDED)THROWING_STATE = THROWING_START;	
	throw_bone_THROWING_TASK();

}




