#include "throw_bone.h"
/***********
PF 	Pin12 һ���쳤ƽ̨������
		Pin13 ���������
		Pin14	���������ǵ�����

***********/
BONE_STATE_ITEMS BONE_STATE;

void throw_bone_THROWING_Init()
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��,����
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��,��λ����
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


		//GPIOF12��ʼ������
		//��ʼ��һ���쳤���׵ĵ�ŷ�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��

		//��ʼ���������׵ĵ�ŷ�
		//GPIOF13��ʼ������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
		
		//��ʼ�����������ǵ�����
		//GPIOF14��ʼ������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	
		//������λ���صĳ�ʼ����PICK_BONE�г�ʼ����
}

void throw_bone_THROWING_TASK()
{
	if(THROWING_STATE == THROWING_START)
	{
		if(SWITCH_OFF == LIMIT_SWITCH.SWITCH_THROW_EXTEN_CYLINDER)
		{
			//һ���쳤���ף��쳤�����ƽ̨
			GPIO_SetBits(GPIOF,GPIO_Pin_12);
		}
		else
		{
			//����������
			GPIO_SetBits(GPIOF,GPIO_Pin_14);
			//�ɿ���צ(�᲻��һײ�����ɣ�����̫��
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);
			//��һ����������
			
			//����������һ���ߵ�ƽ
			GPIO_SetBits(GPIOF,GPIO_Pin_13);
			//delay?
			
			//����������һ���͵�ƽ
			GPIO_ResetBits(GPIOF,GPIO_Pin_13);
			
			
			
			switch(BONE_STATE)//���ط����ƽ̨���жϲ����ֵİ����������Ӿ������źţ�
			{
				case FINISHED:
				{
				//����һ���쳤����
				GPIO_ResetBits(GPIOF,GPIO_Pin_14);					
				GPIO_ResetBits(GPIOF,GPIO_Pin_10);
				}
				;break;
				case NOTFINISH:
				{
				GPIO_ResetBits(GPIOF,GPIO_Pin_14);
				GPIO_ResetBits(GPIOF,GPIO_Pin_10);
				//����һ���쳤���ף�Ȼ��ǰ��s2	
				}
				;break;
			}
		}

	}
}
