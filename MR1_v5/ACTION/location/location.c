#include "main.h"

ROBOT_REAL_POS ROBOT_REAL_POS_DATA = {0, 0, 0, 0, 0, 0};
ROBOT_POS_OFFSET ROBOT_POS_OFFSET_DATA;
extern struct SYMBOL_SENSOR_STATE SYMBOL_SENSOR;

int flag_symbol = 0;
// 获取机器人的真实位置
void location_update_robot_real_position(void)
{
	// 记录上次的位置
//	ROBOT_REAL_POS_DATA.LAST_X_POS = ROBOT_REAL_POS_DATA.X_POS;
//	ROBOT_REAL_POS_DATA.LAST_Y_POS = ROBOT_REAL_POS_DATA.Y_POS;
//	ROBOT_REAL_POS+_DATA.LAST_YAW_POS = ROBOT_REAL_POS_DATA.YAW_POS;
	
	// 只有全场定位模块用于定位
	#if GLOBAL_LOCATION_ONLY 
	// 更新全场定位数据
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
	// 赋值给真实位置数据
	ROBOT_REAL_POS_DATA.X_POS = GL_POS_DATA.X;
	ROBOT_REAL_POS_DATA.Y_POS = GL_POS_DATA.Y;
	ROBOT_REAL_POS_DATA.YAW_POS = GL_POS_DATA.YAW;	
//	


	
	#endif
	
//	// 添加偏置量
//	ROBOT_REAL_POS_DATA.X_POS += ROBOT_POS_OFFSET_DATA.X_CS_OFFSET;
//	ROBOT_REAL_POS_DATA.Y_POS += ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET;
//	ROBOT_REAL_POS_DATA.YAW_POS += ROBOT_POS_OFFSET_DATA.YAW_CS_OFFSET;
}
