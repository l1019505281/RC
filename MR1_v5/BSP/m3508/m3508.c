#include "main.h"
/*********************
 velocity 机器人的速度 
 speed    电机轴rpm 
 RPM      电机转子rpm
 *********************/


M3508_REAL_INFO CHASSIS_M3508_REAL_INFO[3];  // 三轮机器人只用到前三个元素，四轮机器人用到所有四个元素
PID CHASSIS_M3508_PID[3];


// 利用电机通过CAN反馈的数据更新m3508的状态信息
void m3508_update_m3508_info(CanRxMsg *msg)
{
	switch(msg -> StdId)  // 检测标准ID
	{
		case M3508_CHASSIS_MOTOR1_CAN1_ID:
		{ 
			CHASSIS_M3508_REAL_INFO[0].ANGLE = (msg -> Data[0] << 8) | msg -> Data[1];    // 转子机械角度
			CHASSIS_M3508_REAL_INFO[0].RPM = (msg -> Data[2] << 8) | msg -> Data[3];      // 实际转子转速
			CHASSIS_M3508_REAL_INFO[0].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
		}; break;
		
		case M3508_CHASSIS_MOTOR2_CAN1_ID:
		{ 
			CHASSIS_M3508_REAL_INFO[1].ANGLE = (msg -> Data[0] << 8) | msg -> Data[1];    // 转子机械角度
			CHASSIS_M3508_REAL_INFO[1].RPM = (msg -> Data[2] << 8) | msg -> Data[3];      // 实际转子转速
			CHASSIS_M3508_REAL_INFO[1].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
		}; break;

		case M3508_CHASSIS_MOTOR3_CAN1_ID:
		{ 
			CHASSIS_M3508_REAL_INFO[2].ANGLE = (msg -> Data[0] << 8) | msg -> Data[1];    // 转子机械角度
			CHASSIS_M3508_REAL_INFO[2].RPM = (msg -> Data[2] << 8) | msg -> Data[3];      // 实际转子转速
			CHASSIS_M3508_REAL_INFO[2].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
		}; break;
		
		default: break;
	}
}


// 通过CAN1发送底盘m3508的转矩电流
void m3508_send_chassis_m3508_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq)
{
	CanTxMsg tx_message;

	// 配置仲裁段和控制段
	tx_message.StdId = 0x200;  // ID为 1 2 3 4 的电机
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
	
	// 配置数据段
	tx_message.Data[0] = (uint8_t)(cm1_iq >> 8);
	tx_message.Data[1] = (uint8_t)cm1_iq;
	tx_message.Data[2] = (uint8_t)(cm2_iq >> 8);
	tx_message.Data[3] = (uint8_t)cm2_iq;
	tx_message.Data[4] = (uint8_t)(cm3_iq >> 8);
	tx_message.Data[5] = (uint8_t)cm3_iq;

	CAN_Transmit(CAN1, &tx_message);
}


// 通过CAN1设置底盘m3508的转子转速
// motor1 2 3分别代表ID为1 2 3的电机
void m3508_set_chassis_m3508_speed(int motor1_rpm, int motor2_rpm, int motor3_rpm)  // 强制转换为整型
{
	// 增量式PID计算
	PID_incremental_PID_calculation(&CHASSIS_M3508_PID[0], CHASSIS_M3508_REAL_INFO[0].RPM, motor1_rpm);
	PID_incremental_PID_calculation(&CHASSIS_M3508_PID[1], CHASSIS_M3508_REAL_INFO[1].RPM, motor2_rpm);
	PID_incremental_PID_calculation(&CHASSIS_M3508_PID[2], CHASSIS_M3508_REAL_INFO[2].RPM, motor3_rpm);
	
	// 发送经PID调节后的电流
	m3508_send_chassis_m3508_current(CHASSIS_M3508_PID[0].output, CHASSIS_M3508_PID[1].output, CHASSIS_M3508_PID[2].output);
}

void m3508_init()
{
	PID_parameter_init(&CHASSIS_M3508_PID[1], 2,0.1,0.1,1000,1000);
}

