#ifndef __SYMBOL_SENSOR_H
#define __SYMBOL_SENSOR_H
#include "main.h"
#include "sys.h"

void SENSER_Init(void);
void update_Sensor(void);

typedef struct SYMBOL_SENSOR_STATE//限位开关
{
	int SENSOR_LEFT;
}SYMBOL_SENSOR_STATE;



#endif
