#ifndef __GLOBAL_LOCATION_H
#define __GLOBAL_LOCATION_H
#include "stm32f4xx.h"

#define WHEEL_DIAMETER 50.8f  // ����ֱ��
#define ENCODER_RESIOLUTION 500  // �������ֱ���
#define DISTANCE_PER_CNT (PI*WHEEL_DIAMETER/(ENCODER_RESIOLUTION*4))  // ÿ��CNT�����·��ֵ
#define SIN45 0.70710678f 

// �������н��ľ�������
typedef struct ENCO_DIS
{
	int CNT_A;
	int CNT_B;
	float DELTA_DIS_A;
	float DELTA_DIS_B;
}ENCO_DIS;

// ȫ����λģ�鶨λ��λ��
typedef struct GL_POS
{
	float X;
	float Y;
	float YAW;
}GL_POS;

extern struct SAngle 	gyro_angle;
extern struct ROBOT_POS_OFFSET ROBOT_POS_OFFSET_DATA;
extern struct ENCO_DIS ENCO_DIS_DATA;

void gl_GL_model_init(void);
void gl_update_GL_position(void);
void gl_encoder_to_GL_conversion(void);

#endif
