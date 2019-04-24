#include "main.h"
#include  "adc.h"

float RANGING_DISTANCE = 0;

// ��ʼ�������ഫ�����õ�ADC3
// PF9 -> ADC3CH7
void adc_adc3_init(void)
{
//  GPIO_InitTypeDef      GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // ʹ��GPIOFʱ��
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  // ʹ��ADC3ʱ��

//  //��ʼ��GPIO
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA5 ͨ��5
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // ģ������
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  // ����������
//  GPIO_Init(GPIOF, &GPIO_InitStructure);  // ��ʼ��  
// 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC3��λ
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	 // ��λ����	 
// 
//  // ����ADC3
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  // ����ģʽ
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // ���������׶�֮����ӳ�5��ʱ��
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMAʧ��
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  // Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);  // ��ʼ��
//	
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  // 12λģʽ
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // ��ɨ��ģʽ	
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // �ر�����ת��
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // ��ֹ������⣬ʹ���������
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // �Ҷ���	
//  ADC_InitStructure.ADC_NbrOfConversion = 1;  // 1��ת���ڹ��������� Ҳ����ֻת����������1 
//  ADC_Init(ADC3, &ADC_InitStructure);  // ADC��ʼ��
//	
//	ADC_Cmd(ADC3, ENABLE);  // ����ADת����	
	
	// �����ṹ��
  GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	NVIC_InitTypeDef      NVIC_InitStructure;
	
	// ��ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // ʹ��GPIOFʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  // ʹ��ADC3ʱ��

  //��ʼ��GPIO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  // ����������
  GPIO_Init(GPIOF, &GPIO_InitStructure);  // ��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC3��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	 // ��λ����	 
 
  // ����ADC3
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  // ����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  // Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMAʧ�ܣ�����ģʽ�޹أ�
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // ���������׶�֮����ӳ�5��ʱ�ӣ�����ģʽ�޹أ�
	ADC_CommonInit(&ADC_CommonInitStructure);  // ��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  // 12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // ��ɨ��ģʽ����ͨ������Ҫ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // ����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // ��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // �Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;  // 1��ת���ڹ���������,Ҳ����ֻת����������1 
  ADC_Init(ADC3, &ADC_InitStructure);  // ADC��ʼ��
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_480Cycles);  // ����ADC3CH7��һ��ת��������ʱ��Ϊ480������
	
	// �����жϣ�������ȼ���
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);		

	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); // ����ADC3ת�������ж�
	
	ADC_Cmd(ADC3, ENABLE);  // ʹ��ADC3
	
	ADC_SoftwareStartConv(ADC3);  // ��ʼADCת�����������
}


// ADC�жϺ���
void ADC_IRQHandler(void)
{
	u16 ADC3_value;
	
	if(ADC_GetITStatus(ADC3, ADC_IT_EOC) == SET)
	{
		ADC3_value = ADC_GetConversionValue(ADC3);  // ��ȡADC3��ת��ֵ
    adc_update_ranging_info(ADC3_value);
		ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);
	}
}


// ���²��ֵ
void adc_update_ranging_info(u16 value)
{
	static u16 ADC_value_list[ADC_PROCESSING_VALUE_SUM] = {0};
	static u32 adc_receive_counter = 0;
	//u32 i;
	u32 sum;
	u32 t;
	u8 j=0,min,i;
	// ��ֵ
	ADC_value_list[adc_receive_counter] = value;
	
	// ����������
	adc_receive_counter ++;
	
	// ȡ��ֵ 50 -> 1ms
	if(adc_receive_counter >= ADC_PROCESSING_VALUE_SUM)
	{
//		for(i=0, sum=0; i<ADC_PROCESSING_VALUE_SUM; i++)
//		{
//			sum += ADC_value_list[i];
//		  
//		}
		for(i=0;i<49;i++)
	{  
		min=i;      

	for(j=i+1;j<50;j++)       

		if(ADC_value_list[min]>ADC_value_list[j])       

			min=j;

		if(min!=i)        

		{
			t=ADC_value_list[min];
			ADC_value_list[min]=ADC_value_list[i]; 
			ADC_value_list[i]=t;
		}

	}
		sum=1*ADC_value_list[1]+2*ADC_value_list[2]+3*ADC_value_list[3]+4*ADC_value_list[4]+
		5*ADC_value_list[5]+6*ADC_value_list[6]+7*ADC_value_list[7]+8*ADC_value_list[8]+9*ADC_value_list[9]+
		10*ADC_value_list[10]+11*ADC_value_list[11]+12*ADC_value_list[12]+13*ADC_value_list[13]+14*ADC_value_list[14]
		+15*ADC_value_list[15]+16*ADC_value_list[16]+17*ADC_value_list[17]+18*ADC_value_list[18]+19*ADC_value_list[19]
		+20*ADC_value_list[20]+21*ADC_value_list[21]+22*ADC_value_list[22]+23*ADC_value_list[23]+24*ADC_value_list[24]
		+24*ADC_value_list[25]+23*ADC_value_list[26]+22*ADC_value_list[27]+21*ADC_value_list[28]+20*ADC_value_list[29]
		+19*ADC_value_list[30]+18*ADC_value_list[31]+17*ADC_value_list[32]+16*ADC_value_list[33]+15*ADC_value_list[34]
		+14*ADC_value_list[35]+13*ADC_value_list[36]+12*ADC_value_list[37]+11*ADC_value_list[38]+10*ADC_value_list[39]
		+9*ADC_value_list[40]+8*ADC_value_list[41]+7*ADC_value_list[42]+6*ADC_value_list[43]+5*ADC_value_list[44]
		+4*ADC_value_list[45]+3*ADC_value_list[46]+2*ADC_value_list[47]+1*ADC_value_list[48];
	//	RANGING_DISTANCE = 995.0f * ((sum / (float)ADC_PROCESSING_VALUE_SUM) / 4096.0f * 3.3f * 4.766990f) + 50.0f;  
		//RANGING_DISTANCE =(float)(sum/600)*(3.3/4096)*995*(9.89/3.1243652345)+50.00;
	RANGING_DISTANCE =(float)(sum/600-8)*(3.3/4096)*1000*(9.89/3.1243652345)+50.00;
//		printf(" %5d \r\n",sum/600 );
//	RANGING_DISTANCE =(float)(sum / (float)ADC_PROCESSING_VALUE_SUM) *(3.3/4096)*4854.1733331+50.00;
		adc_receive_counter = 0;  // ����������
	
	}
}






Status InitQueue_Sq(SqQueue &S, int size, int inc)
{
    S.elem = (ElemType*)malloc(size*sizeof(ElemType));
    //��elem��ջ�д�����ݵ����飩�����ڴ棬elemָ�򿪱ٵ��ڴ�
    if(NULL == S.elem)return OVERFLOW;
    S.rear = 0;
    S.front = 0;
    S.size = size;
    S.increment = inc;
    return OK;
}

Status EnQueue_Sq(SqQueue &S,ElemType e)
{
    if(((S.rear+1)%S.size) == S.front)return ERROR; //����󱨴�
    S.elem[S.rear] = e; 
    S.rear = (S.rear+1)%S.size;//ѭ��
    return OK;
}

Status DelQueue_Sq(SqQueue &S,ElemType &e)
{
    if(S.rear == S.front)return ERROR;//��ջ
    e = S.elem[S.front];
    S.front  = (S.front + 1)%S.size;
    return OK;
}

Status Get_Sq(SqQueue S,ElemType &e)
{
    if(S.rear == S.front)return ERROR;//��ջ
    e = S.elem[S.front];            
    return OK;
}
