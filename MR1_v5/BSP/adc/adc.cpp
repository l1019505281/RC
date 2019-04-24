#include "main.h"
#include  "adc.h"

float RANGING_DISTANCE = 0;

// 初始化激光测距传感器用的ADC3
// PF9 -> ADC3CH7
void adc_adc3_init(void)
{
//  GPIO_InitTypeDef      GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // 使能GPIOF时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  // 使能ADC3时钟

//  //初始化GPIO
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA5 通道5
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // 模拟输入
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  // 不带上下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);  // 初始化  
// 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC3复位
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	 // 复位结束	 
// 
//  // 配置ADC3
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立模式
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // 两个采样阶段之间的延迟5个时钟
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMA失能
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  // 预分频4分频，ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);  // 初始化
//	
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  // 12位模式
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // 非扫描模式	
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 关闭连续转换
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // 禁止触发检测，使用软件触发
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐	
//  ADC_InitStructure.ADC_NbrOfConversion = 1;  // 1个转换在规则序列中 也就是只转换规则序列1 
//  ADC_Init(ADC3, &ADC_InitStructure);  // ADC初始化
//	
//	ADC_Cmd(ADC3, ENABLE);  // 开启AD转换器	
	
	// 声明结构体
  GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	NVIC_InitTypeDef      NVIC_InitStructure;
	
	// 开时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // 使能GPIOF时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  // 使能ADC3时钟

  //初始化GPIO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // 模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  // 不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);  // 初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	 // 复位结束	 
 
  // 配置ADC3
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  // 预分频4分频，ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMA失能（独立模式无关）
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // 两个采样阶段之间的延迟5个时钟（独立模式无关）
	ADC_CommonInit(&ADC_CommonInitStructure);  // 初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  // 12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // 非扫描模式（多通道才需要）
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // 禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;  // 1个转换在规则序列中,也就是只转换规则序列1 
  ADC_Init(ADC3, &ADC_InitStructure);  // ADC初始化
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_480Cycles);  // 配置ADC3CH7第一个转换，采样时间为480个周期
	
	// 配置中断（最高优先级）
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);		

	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); // 开启ADC3转换结束中断
	
	ADC_Cmd(ADC3, ENABLE);  // 使能ADC3
	
	ADC_SoftwareStartConv(ADC3);  // 开始ADC转换，软件触发
}


// ADC中断函数
void ADC_IRQHandler(void)
{
	u16 ADC3_value;
	
	if(ADC_GetITStatus(ADC3, ADC_IT_EOC) == SET)
	{
		ADC3_value = ADC_GetConversionValue(ADC3);  // 读取ADC3的转换值
    adc_update_ranging_info(ADC3_value);
		ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);
	}
}


// 更新测距值
void adc_update_ranging_info(u16 value)
{
	static u16 ADC_value_list[ADC_PROCESSING_VALUE_SUM] = {0};
	static u32 adc_receive_counter = 0;
	//u32 i;
	u32 sum;
	u32 t;
	u8 j=0,min,i;
	// 赋值
	ADC_value_list[adc_receive_counter] = value;
	
	// 计数器递增
	adc_receive_counter ++;
	
	// 取均值 50 -> 1ms
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
		adc_receive_counter = 0;  // 计数器清零
	
	}
}






Status InitQueue_Sq(SqQueue &S, int size, int inc)
{
    S.elem = (ElemType*)malloc(size*sizeof(ElemType));
    //给elem（栈中存放数据的数组）开辟内存，elem指向开辟的内存
    if(NULL == S.elem)return OVERFLOW;
    S.rear = 0;
    S.front = 0;
    S.size = size;
    S.increment = inc;
    return OK;
}

Status EnQueue_Sq(SqQueue &S,ElemType e)
{
    if(((S.rear+1)%S.size) == S.front)return ERROR; //溢出后报错
    S.elem[S.rear] = e; 
    S.rear = (S.rear+1)%S.size;//循环
    return OK;
}

Status DelQueue_Sq(SqQueue &S,ElemType &e)
{
    if(S.rear == S.front)return ERROR;//空栈
    e = S.elem[S.front];
    S.front  = (S.front + 1)%S.size;
    return OK;
}

Status Get_Sq(SqQueue S,ElemType &e)
{
    if(S.rear == S.front)return ERROR;//空栈
    e = S.elem[S.front];            
    return OK;
}
