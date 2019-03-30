#include "main.h"


ENCO_DIS ENCO_DIS_DATA;
GL_POS GL_POS_DATA = {0, 0, 0};


// 初始化全场定位模块
void gl_GL_model_init(void)
{
	u8 time_counter = 0;
	
	// 初始化底层配置
	encoder_TIM4_init();
  encoder_TIM8_init();
	gyro_UART4_init(115200);
	
	// 等待陀螺仪启动与发送数据 读取陀螺仪Z轴初始角度
	while(time_counter < 100)
	{
		time_counter ++;
		delay_ms(5);
	}
	
	// 设置位置偏置量 （这里设机器人启动位置为原点）
	ROBOT_POS_OFFSET_DATA.X_CS_OFFSET = 0.0;
	ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET = 0.0;
//	ROBOT_POS_OFFSET_DATA.X_CS_OFFSET = -580.0;
//	ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET = 580.0;
	ROBOT_POS_OFFSET_DATA.YAW_CS_OFFSET = 0;
	//************************************************************偏航角偏置问题待解决
	GL_POS_DATA.X = ROBOT_POS_OFFSET_DATA.X_CS_OFFSET;
	GL_POS_DATA.Y = ROBOT_POS_OFFSET_DATA.Y_CS_OFFSET;
	
	// 获取初始偏航角
	ROBOT_POS_OFFSET_DATA.YAW_IN_OFFSET = -(float)GYRO_ANGLE.Angle[2] / 32768 * 180;  
}


// 更新全场定位定位位置数据
void gl_update_GL_position(void)
{
	// 读取定时器CNT寄存器里面的值
	ENCO_DIS_DATA.CNT_A = -encoder_get_encoder_CNT(4); 
	ENCO_DIS_DATA.CNT_B = encoder_get_encoder_CNT(8); 
	
	// 计算编码器行进的路程值
	ENCO_DIS_DATA.DELTA_DIS_A = ENCO_DIS_DATA.CNT_A * DISTANCE_PER_CNT;
	ENCO_DIS_DATA.DELTA_DIS_B = ENCO_DIS_DATA.CNT_B * DISTANCE_PER_CNT;
	
	// 将编码器行进的路程值转化全场定位测量出的位置
	gl_encoder_to_GL_conversion();
}


// 将编码器行进的路程值转化全场定位测量出的位置
void gl_encoder_to_GL_conversion(void)
{
	// 直接获取偏航角，并进行简单处理（逆时针为正，顺时针为负）
	GL_POS_DATA.YAW = -(float)GYRO_ANGLE.Angle[2] / 32768 * 180 - ROBOT_POS_OFFSET_DATA.YAW_IN_OFFSET; 
	if(GL_POS_DATA.YAW > 180) {GL_POS_DATA.YAW = -180 + (GL_POS_DATA.YAW - 180);}
	else if(GL_POS_DATA.YAW < -180) {GL_POS_DATA.YAW = 180 + (GL_POS_DATA.YAW + 180);}
	
	// 运动学方程
	GL_POS_DATA.X += SIN45 * cos(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B - ENCO_DIS_DATA.DELTA_DIS_A) -
									 SIN45 * sin(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B + ENCO_DIS_DATA.DELTA_DIS_A);
	GL_POS_DATA.Y += -(SIN45 * sin(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B - ENCO_DIS_DATA.DELTA_DIS_A) +
									 SIN45 * cos(GL_POS_DATA.YAW / 180.0f * PI) * (ENCO_DIS_DATA.DELTA_DIS_B + ENCO_DIS_DATA.DELTA_DIS_A));
}








