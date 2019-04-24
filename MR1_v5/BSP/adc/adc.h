#ifndef __ADC_H
#define __ADC_H
#include "stm32f4xx.h"

#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef u16 ElemType;


#define ADC_PROCESSING_VALUE_SUM 50
typedef struct SqQueue
    {
        ElemType *elem; //?????????(??????
        int front,rear;       //???????????
        int size;
        int increment; 
    }SqQueue;
void adc_adc3_init(void);
void adc_update_ranging_info(u16 value);
Status InitQueue_Sq(SqQueue &S, int size, int inc);
Status EnQueue_Sq(SqQueue &S,ElemType e);
Status Get_Sq(SqQueue S,ElemType &e);
Status DelQueue_Sq(SqQueue &S,ElemType &e);



#endif
