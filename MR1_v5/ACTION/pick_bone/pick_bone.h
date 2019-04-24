#ifndef __PICK_BONE_H
#define __PICK_BONE_H
#include "main.h"

#define SWITCH_ON 1
#define SWITCH_OFF 0
#define RETURN_MODE 2
#define PICK_MODE 1

extern GRASPING_STATE_ITEMS GRASPING_STATE;//ץȡ�ľ���״̬


typedef struct LIMIT_SWITCH_STATE//��λ����
{
	int SWITCH_EXTEN_CYLINDER,SWITCH_DECLINE_MOTOR,SWITCH_RETRACT_CYLINDER,SWITCH_RISE_MOTOR,
	//	�쳤ǰ�ε���λ���أ�3508�½���צ����λ���أ�����ǰ�ε���λ���أ�3508������צ����λ����		
		SWITCH_THROW_EXTEN_CYLINDER,SWITCH_THROW_RETRACT_CYLINDER,
	//	����ʱ�쳤ƽ̨�ĵ�ŷ����أ�����ʱ����ƽ̨�ĵ�ŷ�����
		SWITCH_LEFT;
}LIMIT_SWITCH_STATE;


typedef enum CONTROL_MODE_ITENS  //ң��������debugʱѡ��
{
	AUTO_PICK_THROW,
	PICK,
	RETURN,
	THROW
}CONTROL_MODE_ITEMS;





void pick_bone_PICK_BONE_TASK(void);
void pick_bone_RETURN_BONE_TASK(void);
void pick_bone_GRASPING_TASK(void);
void pick_bone_PICK_BONE_Init(void);
void pick_bone_LIMIT_SWITCH_UPDATE(void);

#endif
