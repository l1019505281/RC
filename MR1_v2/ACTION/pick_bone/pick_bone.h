#ifndef __PICK_BONE_H
#define __PICK_BONE_H
#include "main.h"

#define SWITCH_ON 1
#define SWITCH_OFF 0
#define RETURN_MODE 2
#define PICK_MODE 1

extern GRASPING_STATE_ITEMS GRASPING_STATE;//抓取的具体状态


typedef struct LIMIT_SWITCH_STATE//限位开关
{
	int SWITCH_EXTEN_CYLINDER,SWITCH_DECLINE_MOTOR,SWITCH_RETRACT_CYLINDER,SWITCH_RISE_MOTOR,
	//	伸长前段的限位开关，3508下降夹爪的限位开关，回缩前段的限位开关，3508上升夹爪的限位开关		
		SWITCH_THROW_EXTEN_CYLINDER,SWITCH_THROW_RETRACT_CYLINDER;
	//	发射时伸长平台的电磁阀开关，发射时回缩平台的电磁阀开关
}LIMIT_SWITCH_STATE;


typedef enum CONTROL_MODE_ITENS  //遥控器或者debug时选的
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
