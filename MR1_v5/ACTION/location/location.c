#include "main.h"

ROBOT_REAL_POS ROBOT_REAL_POS_DATA = {0, 0, 0, 0, 0, 0};
ROBOT_POS_OFFSET ROBOT_POS_OFFSET_DATA;
extern struct SYMBOL_SENSOR_STATE SYMBOL_SENSOR;

int flag_symbol = 0;
// ��ȡ�����˵���ʵλ��
void location_update_robot_real_position(void)
{
	// ��¼�ϴε�λ��
//	ROBOT_REAL_POS_DATA.LAST_X_POS = ROBOT_REAL_POS_DATA.X_POS;
//	ROBOT_REAL_POS_DATA.LAST_Y_POS = ROBOT_REAL_POS_DATA.Y_POS;
//	ROBOT_REAL_POS+_DATA.LAST_YAW_POS = ROBOT_REAL_POS_DATA.YAW_POS;
	
	// ֻ��ȫ����λģ�����ڶ�λ
	#if GLOBAL_LOCATION_ONLY 
	// ����ȫ����λ����
	gl_update_GL_position();
	
	if(SYMBOL_SENSOR.SENSOR_LEFT == 0 && (PATH_POINT_COUNTER == 5||PATH_POINT_COUNTER == 6))
	{
			if(flag_symbol == 2)
			{
				GL_POS_DATA.X = -1360;//-1880;
				flag_symbol = 3;
			}
				if(flag_symbol == 0)flag_symbol=1;
	}
	if(flag_symbol==1&&SYMBOL_SENSOR.SENSOR_LEFT==1)flag_symbol=2;
//	if(PATH_POINT_COUNTER == 9)
//	{
//			GL_POS_DATA.X = -6450+RANGING_DISTANCE;
//	}
	// ��ֵ����ʵλ������
	ROBOT_REAL_POS_DATA.X_POS = GL_POS_DATA.X;
	ROBOT_REAL_POS_DATA.Y_POS = GL_POS_DATA.Y;
	ROBOT_REAL_POS_DATA.YAW_POS = GL_POS_DATA.YAW;	
//	


	
	#endif
	
//	// ���ƫ����
//	ROBOT_REAL_POS_DATA.X_POS += ROBOT_POS_OFFSET_DATA.X_CS_OFFSET;
//	ROBOT_REAL_POS_DATA.Y_POS += ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET;
//	ROBOT_REAL_POS_DATA.YAW_POS += ROBOT_POS_OFFSET_DATA.YAW_CS_OFFSET;
}
