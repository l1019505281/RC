#ifndef __CAN1_H
#define __CAN1_H
#include "stm32f4xx.h"

void can1_CAN1_init(void);

extern unsigned char can_tx_success_flag;

#endif
