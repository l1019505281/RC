#include "main.h"


ENCO_DIS ENCO_DIS_DATA;
GL_POS GL_POS_DATA = {0, 0, 0};


// ��ʼ��ȫ����λģ��
void gl_GL_model_init(void)
{
	u8 time_counter = 0;
	
	// ��ʼ���ײ�����
	encoder_TIM4_init();
  encoder_TIM8_init();
	gyro_UART4_init(115200);
	
	// �ȴ������������뷢������ ��ȡ������Z���ʼ�Ƕ�
	while(time_counter < 100)
	{
		time_counter ++;
		delay_ms(5);
	}
	
	// ����λ��ƫ���� �����������������λ��Ϊԭ�㣩
	ROBOT_POS_OFFSET_DATA.X_CS_OFFSET = 0;
	ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET = 0;
	ROBOT_POS_OFFSET_DATA.YAW_CS_OFFSET = 0;
	
	// ��ȡ��ʼƫ����
	ROBOT_POS_OFFSET_DATA.YAW_IN_OFFSET = -(float)gyro_angle.Angle[2] / 32768 * 180;  //*************************
}


// ����ȫ����λ��λλ������
void gl_update_GL_position(void)
{
	// ��ȡ��ʱ��CNT�Ĵ��������ֵ
	ENCO_DIS_DATA.CNT_A = -encoder_get_encoder_CNT(4);  //*****************************
	ENCO_DIS_DATA.CNT_B = encoder_get_encoder_CNT(8);  //*****************************
	
	// ����������н���·��ֵ
	ENCO_DIS_DATA.DELTA_DIS_A = ENCO_DIS_DATA.CNT_A * DISTANCE_PER_CNT;
	ENCO_DIS_DATA.DELTA_DIS_B = ENCO_DIS_DATA.CNT_B * DISTANCE_PER_CNT;
	
	// ���������н���·��ֵת��ȫ����λ��������λ��
	gl_encoder_to_GL_conversion();
}


// ���������н���·��ֵת��ȫ����λ��������λ��
void gl_encoder_to_GL_conversion(void)
{
	// ֱ�ӻ�ȡƫ���ǣ������м򵥴�����ʱ��Ϊ����˳ʱ��Ϊ����
//	GL_POS_DATA.YAW = 0.0;
	GL_POS_DATA.YAW = -(float)gyro_angle.Angle[2] / 32768 * 180 - ROBOT_POS_OFFSET_DATA.YAW_IN_OFFSET;  //**********************
	if(GL_POS_DATA.YAW > 180) {GL_POS_DATA.YAW = -180 + (GL_POS_DATA.YAW - 180);}
	else if(GL_POS_DATA.YAW < -180) {GL_POS_DATA.YAW = 180 + (GL_POS_DATA.YAW + 180);}
	
//	// �˶�ѧ����
//	GL_POS_DATA.X += SIN45 * cos(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B - ENCO_DIS_DATA.DELTA_DIS_A) -
//									 SIN45 * sin(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B + ENCO_DIS_DATA.DELTA_DIS_A);
//	GL_POS_DATA.Y += -(SIN45 * sin(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B - ENCO_DIS_DATA.DELTA_DIS_A) +
//									 SIN45 * cos(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B + ENCO_DIS_DATA.DELTA_DIS_A));
	GL_POS_DATA.X += ENCO_DIS_DATA.DELTA_DIS_A;
	GL_POS_DATA.Y += ENCO_DIS_DATA.DELTA_DIS_B;

}








