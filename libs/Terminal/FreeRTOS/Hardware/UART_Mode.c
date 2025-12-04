#include "stm32f10x.h"                  // Device header
#include "UART_Mode.h"
#include <stdio.h>
#include <string.h>
#include "Led.h"

void USART_DMAConfig(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dma;
	dma.DMA_BufferSize = BufferSize;
	dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma.DMA_M2M = DISABLE;
	dma.DMA_Mode = DMA_Mode_Normal;
	dma.DMA_Priority = DMA_Priority_High;
	
	dma.DMA_MemoryBaseAddr = (uint32_t)&arr;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	dma.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	DMA_Init(DMA1_Channel5, &dma);
	
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void UART_init(void){
	
	//GPIO配置
	//发送
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpioa;
	gpioa.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioa.GPIO_Pin = GPIO_Pin_9;
	gpioa.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioa);
	//接收
	gpioa.GPIO_Mode = GPIO_Mode_IPU;
	gpioa.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &gpioa);
	
	//USART配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef usart;
	usart.USART_BaudRate = 9600;				//波特率
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;						//停止位
	usart.USART_WordLength = USART_WordLength_8b;				//8bit正好一个字节
	USART_Init(USART1, &usart);
	
	//DMA配置
	USART_DMAConfig();
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	//配置中断
	/*DMA代替使用RXNE中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	*/
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic);
	
	//启动
	USART_Cmd(USART1, ENABLE);
}

void Send_Byte(uint8_t data){
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
	
void Send_Arr(uint8_t *arr, uint16_t length){
	for(uint16_t i = 0; i < length; i++){
		Send_Byte(arr[i]);
	}
}

void Send_String(char *arr){
	while(*arr){
		Send_Byte(*arr++);
	}
}

int fputc(int ch,FILE *f){
	Send_Byte(ch);
	return ch;
}

char arr[200] = {0};
uint8_t flag = 0;

void USART1_IRQHandler(void){

	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET){
		arr[BufferSize - DMA_GetCurrDataCounter(DMA1_Channel5)] = '\0';
		flag = 1;
		
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		uint32_t temp;
		temp = USART1->SR;
		temp = USART1->DR;
		(void)temp;
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel5, BufferSize);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

