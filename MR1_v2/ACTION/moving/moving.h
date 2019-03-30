#ifndef __MOVING_H
#define __MOVING_H
#include "stm32f4xx.h"

#define absolute(x) ((x)>0? (x):-(x))
#define MM_PER_SEC_TO_RPM 2.412857f   //����ֱ��152mm��������ٱ�3591:187������ת��: n/min������һȦpi*152mm

#if GLOBAL_LOCATION_ONLY
#define  PASS_POINT_REACH_THRESHOLD_POS 34.0f
#define  PASS_POINT_REACH_THRESHOLD_YAW 1.0f  // ����Ϊcnt�������ж�����
#define  STOP_POINT_REACH_THRESHOLD_POS 5.0f
#define  STOP_POINT_REACH_THRESHOLD_YAW 10.0f
#endif



typedef struct PLANNING_PATH
{
	float x;
	float y;
	float yaw;
	float V;
}PLANNING_PATH;

typedef struct MOVING_BETWEEN_2PTS_INFO
{
	float X_GAP;
	float Y_GAP;
	float YAW_GAP;
	float GAP_DIS;
	float MOVED_DIS;
	float BETWEEN_2PTS_DIS;
	float IMMEDIATE_VELOCITY;  // ��λ��mm/s
}MOVING_BETWEEN_2PTS_INFO;

typedef struct POINTS_POS_OFFSET
{
	float x_offset;
	float y_offset;
	float yaw_offset;
}POINTS_POS_OFFSET;

extern enum MOVING_STATE_ITEMS MOVING_STATE;
extern struct ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;
extern struct PLANNING_PATH *NOW_PATH;
extern struct PLANNING_PATH DEBUG_PLANNING_PATH[];

void moving_action_task(void);
void moving_init(void);
void moving_path_tracking(void);
	 
#endif
