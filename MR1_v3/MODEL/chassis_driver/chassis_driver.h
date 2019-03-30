#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H
#include "stm32f4xx.h"

#define USE_M3508_AS_3WHEELS_CHASSIS_MOTOR 0
#define USE_ROBOMODULE_AS_4WHEELS_CHASSIS_MOTOR 1
#define USE_ROBOT_KINEMATIC_NOT_WORLD_KINEMATIC 0

#define X_PARAMETER_3WHEELS (0.5f)               
#define Y_PARAMETER_3WHEELS (0.86602540f)  // (3^(1/2))/2  
#define K_PARAMETER_4WHEELS (0.70710678f)  // (2^(1/2))/2
#define W_PARAMETER_3WHEELS (1.0f)  // ÈýÂÖµ×ÅÌ
#define W_PARAMETER_4WHEELS (1.0f)  // ËÄÂÖµ×ÅÌ

typedef struct CHASSIS_MOTOR_RPM
{
	int16_t MOTOR1_RPM;
	int16_t MOTOR2_RPM;
	int16_t MOTOR3_RPM;
	int16_t MOTOR4_RPM;
}CHASSIS_MOTOR_RPM;

typedef struct ROBOT_TARGET_VELOCITY
{
	float Vx;
	float Vy;
	float W;
	int16_t Vx_RPM;
	int16_t Vy_RPM;
	int16_t W_RPM;
}ROBOT_TARGET_VELOCITY;

extern struct PID CHASSIS_M3508_PID[3];
extern struct ROBOT_REAL_POS ROBOT_REAL_POS_DATA;

void cd_chassis_driver_init(void);
void cd_robot_kinematic_conversion_3wheels(float Vx, float Vy, float Vz);
void cd_world_kinematic_conversion_3wheels(float Vx, float Vy, float W, float theta);
void cd_world_kinematic_conversion_4wheels(float Vx, float Vy, float W, float theta);
void cd_chassis_velocity_judge(void);

#endif
