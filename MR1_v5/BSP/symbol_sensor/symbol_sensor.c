#include "main.h"
#include "symbol_sensor.h"


SYMBOL_SENSOR_STATE SYMBOL_SENSOR;

void SENSER_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);	 //??PB,PE????
	
	


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //A5????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//??????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
  GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIOE2,3,4\

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //A5????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//??????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
  GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOE2,3,4\

}
 

void update_Sensor(void)
{
	SYMBOL_SENSOR.SENSOR_LEFT = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
}
