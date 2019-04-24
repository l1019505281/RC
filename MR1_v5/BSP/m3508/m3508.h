#ifndef __M3508_H
#define __M3508_H
#include "stm32f4xx.h"

#define M3508_CHASSIS_MOTOR1_CAN1_ID 0x201
#define M3508_CHASSIS_MOTOR2_CAN1_ID 0x202
#define M3508_CHASSIS_MOTOR3_CAN1_ID 0x203
#define M3508_REDUCTION_RATIO 19

typedef struct M3508_REAL_INFO
{
	uint16_t ANGLE;   								
	int16_t RPM;								
	int16_t CURRENT; 
}M3508_REAL_INFO;

void m3508_update_m3508_info(CanRxMsg *msg);
void m3508_send_chassis_m3508_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq);
void m3508_set_chassis_m3508_speed(int motor1_speed, int motor2_speed, int motor3_speed);
void m3508_init(void);

#endif
