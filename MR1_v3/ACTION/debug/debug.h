#ifndef __DEBUG_H
#define __DEBUG_H
#include "stm32f4xx.h"

#define DEBUG_GLOBAL_LOCATION 0
#define DEBUG_LOCATION 1
#define DEBUG_SINGLE_M3508 0
#define DEBUG_CHASSIS_M3508 0
#define DEBUG_PATH_TRACKING 1

extern struct GL_POS GL_POS_DATA;
extern struct ROBOT_REAL_POS ROBOT_REAL_POS_DATA;
extern struct M3508_REAL_INFO CHASSIS_M3508_REAL_INFO[3];
extern u32 PATH_POINT_COUNTER;
extern struct MOVING_BETWEEN_2PTS_INFO MOVING_BETWEEN_2PTS_INFO_DATA;
extern u32 PATH_POINT_SUM;
extern struct PLANNING_PATH DEBUG_PLANNING_PATH[1]; // 改变离散点数量时记得修改数组大小
extern struct ROBOT_TARGET_VELOCITY ROBOT_TARGET_VELOCITY_DATA;

void debug_by_UART(void);
void debug_single_m3508_in_main(void);
void debug_chassis_without_remote_control_in_main(void);
void debug_path_tracking_with_single_path_in_task(void);

#endif
