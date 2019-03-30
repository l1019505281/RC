#include "pick_bone.h"
#include "main.h"

/*//////////////////////////////////////
PF 	Pin10	�������ڶ����ƶ��쳤����
		Pin11	���ڼ�ȡ
	�ߵ�ƽ�쳤���͵�ƽ������

Ҫ����״̬:
	CONTROL_MODE ���ڵ���ʱ��������ÿ���׶�bug
				AUTO_PICK_THROW,
				PICK,
				RETURN
	
��λ����
	SWITCH_EXTEN_CYLINDER,  				�����쳤����λ���� 
	SWITCH_DECLINE_MOTOR,						�����½�����λ����
	SWITCH_RETRACT_CYLINDER,				�������ض���λ����
	SWITCH_RISE_MOTOR,							������������λ����	
	SWITCH_THROW_EXTEN_CYLINDER,		һ���쳤����λ����
	SWITCH_THROW_RETRACT_CYLINDER;	һ���쳤����λ����

///////////////////////////////////////*/
CONTROL_MODE_ITEMS CONTROL_MODE;//����ѡ��״̬     �Զ�ģʽ���쳤��ȡģʽ����ȡ������ģʽ
LIMIT_SWITCH_STATE LIMIT_SWITCH;//��λ���ص�״̬

int CONTROL_PROCESS = PICK_MODE;  //�Զ�ģʽʱ�ж�ʲôʱ��ü�ȡʲôʱ��û���



void pick_bone_PICK_BONE_Init()
{
		
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��,����
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��,��λ����
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


		//GPIOF10��ʼ������
		//��ʼ��1������쳤���׵ĵ�ŷ�GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��



		//��ʼ��M3508��

		//��ʼ��1���ȡ���׵ĵ�ŷ�GPIO
		//GPIOF11��ʼ������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��

		//��ʼ��������λ���ص�GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //PD�ϵ���λ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD 0 1 4 5 6 7 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //PG�ϵ���λ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG 9 10
		
		#if 1
		#endif
	
}





//�쳤��צ����ץȡ�޹ǡ�

void pick_bone_GRASPING_TASK()
{
		if(GRASPING_STATE == GRASPING_START)
		{
			switch(CONTROL_MODE)//�ж����ڵ�״̬
			{
				case AUTO_PICK_THROW:
				{
					if(CONTROL_PROCESS == PICK_MODE)//���ȥ��
					{
						pick_bone_PICK_BONE_TASK();
					}
					if(CONTROL_PROCESS == RETURN_MODE)//����������
					{
						pick_bone_RETURN_BONE_TASK();
					}
				}
						;break;
				case PICK:
				{
					pick_bone_PICK_BONE_TASK();//����debug���ȥ�Ĺ���
				}
						;break;
				case RETURN:
				{
					pick_bone_RETURN_BONE_TASK();//����debug�������Ĺ���
				}
						;break;
				default:break;
			}
		}
}

void pick_bone_PICK_BONE_TASK()
{
		//��ȡ��λ����ֵ
		if(SWITCH_ON == LIMIT_SWITCH.SWITCH_EXTEN_CYLINDER)//���쳤���ף�������ײ����λ����1���л�����һ������
		{
			
			if(SWITCH_ON == LIMIT_SWITCH.SWITCH_DECLINE_MOTOR)//����3508���¼�צ�������ʹ�ײ����λ����2���л�����һ������
			{

				//3508�ٶȽ�Ϊ0				
				m3508_set_chassis_m3508_speed(0,0,0);
				//��צץȡ
				GPIO_SetBits(GPIOF,GPIO_Pin_11);//GPIOF11���øߣ���ȡ
				
				//��ȡ���ı�CONTROL_PROCESS
				if(CONTROL_MODE == AUTO_PICK_THROW)CONTROL_PROCESS = RETURN_MODE;
				
			}
			else
			{
				//��3508һ���½����ٶ�
				m3508_set_chassis_m3508_speed(100,100,100);
			}
			

			//
		}
		else
		{
			//���쳤������һ���ߵ�ƽ
			GPIO_SetBits(GPIOF,GPIO_Pin_10);//GPIOF10,F11���øߣ������쳤
		}
		
}



//����3508�����צ��
void pick_bone_RETURN_BONE_TASK()
{
	int flag=0;
	if(SWITCH_ON == LIMIT_SWITCH.SWITCH_RISE_MOTOR)
	{
		//3508�ٶȽ�Ϊ0
		m3508_set_chassis_m3508_speed(0,0,0);
		
		if(SWITCH_OFF == LIMIT_SWITCH.SWITCH_RETRACT_CYLINDER)
		{
			//���쳤���׵ĵ�ŷ���λ0
			GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		}
		else if(SWITCH_ON == LIMIT_SWITCH.SWITCH_RETRACT_CYLINDER)
		{
			//��λ�ñջ��������������ȷλ��
			
			flag = 1;
		}
		
	}
	else
	{
		//��3508һ���������ٶ�
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

}
