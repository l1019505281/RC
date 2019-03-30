#include "pick_bone.h"
#include "main.h"
#define DELAY_TIME 10000
/*********************************************************** 
PF 	Pin10	引脚用于二层推动伸长气缸
		Pin11	用于夹取
	高电平伸长，低电平收缩。

要调的状态:
	CONTROL_MODE 用于调试时单独调试每个阶段bug
				AUTO_PICK_THROW,
				PICK,
				RETURN
	
限位开关
	SWITCH_EXTEN_CYLINDER,  				二层伸长段限位开关 
	SWITCH_DECLINE_MOTOR,						二层下降段限位开关
	SWITCH_RETRACT_CYLINDER,				二层缩回段限位开关
	SWITCH_RISE_MOTOR,							二层上升段限位开关	
	SWITCH_THROW_EXTEN_CYLINDER,		一层伸长段限位开关
	SWITCH_THROW_RETRACT_CYLINDER;	一层伸长段限位开关
************************************************************/
                                              
CONTROL_MODE_ITEMS CONTROL_MODE;//用于选择状态     自动模式，伸长夹取模式，夹取后缩回模式
LIMIT_SWITCH_STATE LIMIT_SWITCH;//限位开关的状态

int CONTROL_PROCESS = PICK_MODE;  //自动模式时判断什么时候该夹取什么时候该回收
int time_counter = 0;


void pick_bone_PICK_BONE_Init()
{
		
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟,气缸
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟,限位开关
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


		//GPIOF10初始化设置
		//初始化1组二层伸长气缸的电磁阀GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化



		//初始化M3508的

		//初始化1组夹取气缸的电磁阀GPIO
		//GPIOF11初始化设置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
		GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化

		//初始化所有限位开关的GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //PD上的限位开关
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
		GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD 0 1 4 5 6 7 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //PG上的限位开关
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
		GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG 9 10
		
		#if 1
		CONTROL_MODE = AUTO_PICK_THROW;
		
		#endif                                                   
	
}





//伸长夹爪气缸抓取兽骨。

void pick_bone_GRASPING_TASK()
{
		if(GRASPING_STATE == GRASPING_START)
		{
			
			switch(CONTROL_MODE)//判断现在的状态
			{
				case AUTO_PICK_THROW:
				{
					
					if(CONTROL_PROCESS == PICK_MODE)//伸出去夹
					{
						pick_bone_PICK_BONE_TASK();
					}
					if(CONTROL_PROCESS == RETURN_MODE)//夹完缩回来
					{
						pick_bone_RETURN_BONE_TASK();
					}
				}
						;break;
				case PICK:
				{
					pick_bone_PICK_BONE_TASK();//单独debug伸出去的功能
				}
						;break;
				case RETURN:
				{
					pick_bone_RETURN_BONE_TASK();//单独debug缩回来的功能
				}
						;break;
				default:break;
			}
		}
}

void pick_bone_PICK_BONE_TASK()
{
		//读取限位开关值
		if(SWITCH_ON == LIMIT_SWITCH.SWITCH_EXTEN_CYLINDER)//先伸长气缸，当气缸撞到限位开关1，切换到下一个任务
		{
			
			if(SWITCH_ON == LIMIT_SWITCH.SWITCH_DECLINE_MOTOR)//驱动3508降下夹爪，当传送带撞到限位开关2，切换到下一个任务
			{

				//3508速度降为0				
				m3508_set_chassis_m3508_speed(0,0,0);
				if(DELAY_TIME <= time_counter)
				{
					//夹爪抓取
					GPIO_SetBits(GPIOF,GPIO_Pin_11);//GPIOF11设置高，夹取
					//夹取完后改变CONTROL_PROCESS
					if(CONTROL_MODE == AUTO_PICK_THROW)CONTROL_PROCESS = RETURN_MODE;
					time_counter = 0;
				}
				time_counter++;//延时后夹取
			}
			else
			{
				//给3508一个下降的速度
				m3508_set_chassis_m3508_speed(100,100,100);
			}
			

			//
		}
		else
		{
			//给伸长的气缸一个高电平
			GPIO_SetBits(GPIOF,GPIO_Pin_10);//GPIOF10,F11设置高，气缸伸长
		}
		
}



//驱动3508升起夹爪，
void pick_bone_RETURN_BONE_TASK()
{
	int flag=0;
	GPIO_ResetBits(GPIOF,GPIO_Pin_10);
	if(SWITCH_ON == LIMIT_SWITCH.SWITCH_RISE_MOTOR)
	{
		//3508速度降为0
		m3508_set_chassis_m3508_speed(0,0,0);
		
		if(SWITCH_OFF == LIMIT_SWITCH.SWITCH_RETRACT_CYLINDER)
		{
			//把伸长气缸的电磁阀置位0
			time_counter++;
			if(time_counter <= DELAY_TIME)GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		}
		else if(SWITCH_ON == LIMIT_SWITCH.SWITCH_RETRACT_CYLINDER)
		{
			//用位置闭环将电机调整到正确位置
			time_counter = 0;
			flag = 1;
		}
		
	}
	else
	{
		//给3508一个上升的速度
		m3508_set_chassis_m3508_speed(-100,-100,-100);
		
	}
	
	if(flag == 1)GRASPING_STATE = GRASPING_SUCCEEDED;
}


void pick_bone_LIMIT_SWITCH_UPDATE()
{
	LIMIT_SWITCH.SWITCH_EXTEN_CYLINDER = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1);
	LIMIT_SWITCH.SWITCH_DECLINE_MOTOR = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4);
	LIMIT_SWITCH.SWITCH_RETRACT_CYLINDER = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5);
	LIMIT_SWITCH.SWITCH_RISE_MOTOR = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7);
	LIMIT_SWITCH.SWITCH_THROW_EXTEN_CYLINDER = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
	LIMIT_SWITCH.SWITCH_THROW_RETRACT_CYLINDER = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10);
//	if(LIMIT_SWITCH.SWITCH_EXTEN_CYLINDER == 1)GPIO_SetBits(GPIOF,GPIO_Pin_10);
//	else	GPIO_ResetBits(GPIOF,GPIO_Pin_10);
	

}
