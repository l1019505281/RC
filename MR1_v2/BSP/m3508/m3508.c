#include "main.h"
/*********************
 velocity �����˵��ٶ� 
 speed    �����rpm 
 RPM      ���ת��rpm
 *********************/


M3508_REAL_INFO CHASSIS_M3508_REAL_INFO[3];  // ���ֻ�����ֻ�õ�ǰ����Ԫ�أ����ֻ������õ������ĸ�Ԫ��
PID CHASSIS_M3508_PID[3];


// ���õ��ͨ��CAN���������ݸ���m3508��״̬��Ϣ
void m3508_update_m3508_info(CanRxMsg *msg)
{
	switch(msg -> StdId)  // ����׼ID
	{
		case M3508_CHASSIS_MOTOR1_CAN1_ID:
		{ 
			CHASSIS_M3508_REAL_INFO[0].ANGLE = (msg -> Data[0] << 8) | msg -> Data[1];    // ת�ӻ�е�Ƕ�
			CHASSIS_M3508_REAL_INFO[0].RPM = (msg -> Data[2] << 8) | msg -> Data[3];      // ʵ��ת��ת��
			CHASSIS_M3508_REAL_INFO[0].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // ʵ��ת�ص���
		}; break;
		
		case M3508_CHASSIS_MOTOR2_CAN1_ID:
		{ 
			CHASSIS_M3508_REAL_INFO[1].ANGLE = (msg -> Data[0] << 8) | msg -> Data[1];    // ת�ӻ�е�Ƕ�
			CHASSIS_M3508_REAL_INFO[1].RPM = (msg -> Data[2] << 8) | msg -> Data[3];      // ʵ��ת��ת��
			CHASSIS_M3508_REAL_INFO[1].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // ʵ��ת�ص���
		}; break;

		case M3508_CHASSIS_MOTOR3_CAN1_ID:
		{ 
			CHASSIS_M3508_REAL_INFO[2].ANGLE = (msg -> Data[0] << 8) | msg -> Data[1];    // ת�ӻ�е�Ƕ�
			CHASSIS_M3508_REAL_INFO[2].RPM = (msg -> Data[2] << 8) | msg -> Data[3];      // ʵ��ת��ת��
			CHASSIS_M3508_REAL_INFO[2].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // ʵ��ת�ص���
		}; break;
		
		default: break;
	}
}


// ͨ��CAN1���͵���m3508��ת�ص���
void m3508_send_chassis_m3508_current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq)
{
	CanTxMsg tx_message;

	// �����ٲöκͿ��ƶ�
	tx_message.StdId = 0x200;  // IDΪ 1 2 3 4 �ĵ��
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
	
	// �������ݶ�
	tx_message.Data[0] = (uint8_t)(cm1_iq >> 8);
	tx_message.Data[1] = (uint8_t)cm1_iq;
	tx_message.Data[2] = (uint8_t)(cm2_iq >> 8);
	tx_message.Data[3] = (uint8_t)cm2_iq;
	tx_message.Data[4] = (uint8_t)(cm3_iq >> 8);
	tx_message.Data[5] = (uint8_t)cm3_iq;

	CAN_Transmit(CAN1, &tx_message);
}


// ͨ��CAN1���õ���m3508��ת��ת��
// motor1 2 3�ֱ����IDΪ1 2 3�ĵ��
void m3508_set_chassis_m3508_speed(int motor1_rpm, int motor2_rpm, int motor3_rpm)  // ǿ��ת��Ϊ����
{
	// ����ʽPID����
	PID_incremental_PID_calculation(&CHASSIS_M3508_PID[0], CHASSIS_M3508_REAL_INFO[0].RPM, motor1_rpm);
	PID_incremental_PID_calculation(&CHASSIS_M3508_PID[1], CHASSIS_M3508_REAL_INFO[1].RPM, motor2_rpm);
	PID_incremental_PID_calculation(&CHASSIS_M3508_PID[2], CHASSIS_M3508_REAL_INFO[2].RPM, motor3_rpm);
	
	// ���;�PID���ں�ĵ���
	m3508_send_chassis_m3508_current(CHASSIS_M3508_PID[0].output, CHASSIS_M3508_PID[1].output, CHASSIS_M3508_PID[2].output);
}



