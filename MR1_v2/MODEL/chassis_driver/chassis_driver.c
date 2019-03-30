#include "main.h"

CHASSIS_MOTOR_RPM CHASSIS_MOTOR_RPM_DATA;
ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;


// ��ʼ�����̵������
void cd_chassis_driver_init(void)
{
	// ���ʹ����m3508��Ϊ���̵��
	#if USE_M3508_AS_3WHEELS_CHASSIS_MOTOR
	PID_parameter_init(&CHASSIS_M3508_PID[0], 10.0, 1.0, 0.0, 15000.0, 15000.0);
	PID_parameter_init(&CHASSIS_M3508_PID[1], 10.0, 1.0, 0.0, 15000.0, 15000.0);
	PID_parameter_init(&CHASSIS_M3508_PID[2], 10.0, 1.0, 0.0, 15000.0, 15000.0);
	#endif
	
	// ���ʹ����re35��re40��Ϊ���̵��
	#if USE_ROBOMODULE_AS_4WHEELS_CHASSIS_MOTOR
	delay_ms(100);
	CAN_RoboModule_DRV_Reset(0,0);  // ��0��1�����������и�λ
	delay_ms(500);                                     
	CAN_RoboModule_DRV_Config(0,0,1,0);  // ����Ϊ1ms����һ������ �ر�������λ����
	delay_ms(50);
	CAN_RoboModule_DRV_Mode_Choice(0,0,Velocity_Mode);  // ѡ������ٶ�ģʽ
	delay_ms(500);
	#endif
}


// 3�ֻ���������ϵ���˶�ѧ
// W����ֵ-��ʱ�� ��ֵ-˳ʱ��
void cd_robot_kinematic_conversion_3wheels(float Vx_RPM, float Vy_RPM, float W_RPM)
{
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM = Vx_RPM + W_PARAMETER_3WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM = -X_PARAMETER_3WHEELS * Vx_RPM + Y_PARAMETER_3WHEELS * Vy_RPM + W_PARAMETER_3WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = -X_PARAMETER_3WHEELS * Vx_RPM - Y_PARAMETER_3WHEELS * Vy_RPM + W_PARAMETER_3WHEELS * -W_RPM;
}


// 3����������ϵ���˶�ѧ
// thetaΪ����������ϵx������������ϵx��н� ��λ����
// W����ֵ-��ʱ�� ��ֵ-˳ʱ��
void cd_world_kinematic_conversion_3wheels(float Vx_RPM, float Vy_RPM, float W_RPM, float theta)
{
	theta = PI * theta / 180.0f;
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM =  cos(theta) * Vx_RPM         + sin(theta) * Vy_RPM         + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM = -cos(PI/3.0f-theta) * Vx_RPM + sin(PI/3.0f-theta) * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = -cos(PI/3.0f+theta) * Vx_RPM - sin(PI/3.0f+theta) * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
}


// 4�ֻ���������ϵ���˶�ѧ
// W����ֵ-��ʱ�� ��ֵ-˳ʱ��
void cd_robot_kinematic_conversion_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM)
{
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM =  K_PARAMETER_4WHEELS * Vx_RPM + K_PARAMETER_4WHEELS * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM =  K_PARAMETER_4WHEELS * Vx_RPM - K_PARAMETER_4WHEELS * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = -K_PARAMETER_4WHEELS * Vx_RPM - K_PARAMETER_4WHEELS * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM = -K_PARAMETER_4WHEELS * Vx_RPM + K_PARAMETER_4WHEELS * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
}


// 4����������ϵ���˶�ѧ
// thetaΪ����������ϵx������������ϵx��н� ��λ����
// W����ֵ-��ʱ�� ��ֵ-˳ʱ��
void cd_world_kinematic_conversion_4wheels(float Vx_RPM, float Vy_RPM, float W_RPM, float theta)
{
	theta = PI * theta / 180.0f;
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM =  cos(theta+PI/4.0f) * Vx_RPM + sin(theta+PI/4.0f) * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM =  cos(theta-PI/4.0f) * Vx_RPM + sin(theta-PI/4.0f) * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = -cos(theta+PI/4.0f) * Vx_RPM - sin(theta+PI/4.0f) * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM = -cos(theta-PI/4.0f) * Vx_RPM - sin(theta-PI/4.0f) * Vy_RPM + W_PARAMETER_4WHEELS * -W_RPM;
}


// ���Ƶ��̸����ٶ�����ֵ�����ٶ�
void cd_chassis_velocity_judge(void)
{
	// ���ʹ�û���������ϵ
	#if USE_ROBOT_KINEMATIC_NOT_WORLD_KINEMATIC 
		#if USE_M3508_AS_3WHEELS_CHASSIS_MOTOR
		cd_robot_kinematic_conversion_3wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																					ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																					ROBOT_TARGET_VELOCITY_DATA.W_RPM);
		#elif USE_ROBOMODULE_AS_4WHEELS_CHASSIS_MOTOR
		
		#endif
	#else
		#if USE_M3508_AS_3WHEELS_CHASSIS_MOTOR
		cd_world_kinematic_conversion_3wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																	        ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																	        ROBOT_TARGET_VELOCITY_DATA.W_RPM ,
																	        ROBOT_REAL_POS_DATA.YAW_POS);
		#elif USE_ROBOMODULE_AS_4WHEELS_CHASSIS_MOTOR
		cd_world_kinematic_conversion_4wheels(ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
																	        ROBOT_TARGET_VELOCITY_DATA.Vy_RPM, 
																	        ROBOT_TARGET_VELOCITY_DATA.W_RPM ,
																	        ROBOT_REAL_POS_DATA.YAW_POS);
		#endif
  #endif
	
	// ������ֵ�����ٶ�
	#if USE_M3508_AS_3WHEELS_CHASSIS_MOTOR
	m3508_set_chassis_m3508_speed(CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM, 
	                              CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM, 
	                              CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM);
	#endif
	
	#if USE_ROBOMODULE_AS_4WHEELS_CHASSIS_MOTOR
	// ��Ϊ�������ԭ�����ת���������
	CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM = CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM = CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM = -CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM;
	CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM = CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM;
	
	CAN_RoboModule_DRV_Velocity_Mode(0, 1, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR1_RPM);  // ��Ϊ������ID��ͬ�����Ķ�
	CAN_RoboModule_DRV_Velocity_Mode(0, 2, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR2_RPM);
	CAN_RoboModule_DRV_Velocity_Mode(0, 3, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR3_RPM);
	CAN_RoboModule_DRV_Velocity_Mode(0, 4, 5000, CHASSIS_MOTOR_RPM_DATA.MOTOR4_RPM);
	#endif
}
