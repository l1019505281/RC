#ifndef __THROW_BONE_H
#define __THROW_BONE_H
#include "main.h"

typedef enum BONE_STATE_ITEMS
{
	FINISHED,
	NOTFINISH
}BONE_STATE_ITEMS;

void throw_bone_THROWING_Init(void);

extern struct LIMIT_SWITCH_STATE LIMIT_SWITCH;
extern THROWING_STATE_ITEMS THROWING_STATE;
#endif
