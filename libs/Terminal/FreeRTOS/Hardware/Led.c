#include "stm32f10x.h"                  // Device header
#include <stdint.h>

#include "Led.h"

void ledInit(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef gpiob;
	gpiob.GPIO_Mode = GPIO_Mode_Out_PP;
	gpiob.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpiob.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &gpiob);
}

void led01TurnOn(void){
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void led01TurnOff(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void led01Tra(void){
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)){
		led01TurnOff();
	}else{
		led01TurnOn();
	}
}

void led02TurnOn(void){
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

void led02TurnOff(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void led02Tra(void){
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)){
		led02TurnOff();
	}else{
		led02TurnOn();
	}
}
