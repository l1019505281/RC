#include "throw_bone.h"
#include "main.h"
/***********
PF 	Pin12 一层伸长平台的气缸
		Pin13 发射的气缸
		Pin14	调整俯仰角的气缸

***********/
BONE_STATE_ITEMS BONE_STATE;

void throw_bone_THROWING_Init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟,气缸
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟,限位开关
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


		//GPIOF12初始化设置
		//初始化一层伸长气缸的电磁阀
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化

		//初始化发射气缸的电磁阀
		//GPIOF13初始化设置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
		
		//初始化调整俯仰角的气缸
		//GPIOF14初始化设置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	
		//所有限位开关的初始化在PICK_BONE中初始化了
}

void throw_bone_THROWING_TASK()
{
	if(THROWING_STATE == THROWING_START)
	{
		if(SWITCH_OFF == LIMIT_SWITCH.SWITCH_THROW_EXTEN_CYLINDER)
		{
			//一层伸长气缸，伸长发射的平台
			GPIO_SetBits(GPIOF,GPIO_Pin_12);
		}
		else
		{
			//调整俯仰角
			GPIO_SetBits(GPIOF,GPIO_Pin_14);
			//松开夹爪(会不会一撞到就松，惯性太大
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);
			//给一个条件后发射
			
			//给发射气缸一个高电平
			GPIO_SetBits(GPIOF,GPIO_Pin_13);
			//delay?
			
			//给发射气缸一个低电平
			GPIO_ResetBits(GPIOF,GPIO_Pin_13);
			
			
			
			switch(BONE_STATE)//缩回发射的平台，判断操作手的按键，或者视觉反馈信号？
			{
				case FINISHED:
				{
				//缩回一层伸长气缸
				GPIO_ResetBits(GPIOF,GPIO_Pin_14);					
				GPIO_ResetBits(GPIOF,GPIO_Pin_10);
				}
				;break;
				case NOTFINISH:
				{
				GPIO_ResetBits(GPIOF,GPIO_Pin_14);
				GPIO_ResetBits(GPIOF,GPIO_Pin_10);
				//缩回一层伸长气缸，然后前往s2	
				}
				;break;
			}
		}

	}
}
