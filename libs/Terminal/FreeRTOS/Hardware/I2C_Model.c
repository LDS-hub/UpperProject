#include "stm32f10x.h"                  // Device header
#include "I2C_Model.h"
#include "Delay.h"

/**
	1. 同步 半双工 一对多 串行 MSB（高位先行）
	2. 因为一对多，每个设备都有地址
	3. 所有设备都是开漏模式：
					开漏模式：输出寄存器上的‘0’激活N-MOS， ‘1’将端口置于高阻状态（P-MOS从不被激活）
					开漏模式时：对输入数据寄存器的都访问可得到I/O状态
	4. 使用GPIO来实现一边I2C
	5. SCL所有的操作，SCL都是拉低结束的
**/

//写SCL--完全在主机的控制下，不给从机控制
void Write_SCL(uint8_t data){
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, data);
	delayus(10);
}

//写SDA
void Write_SDA(uint8_t data){
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, data);
	delayus(10);
}

//读SDA
uint8_t Read_SDA(void){
	uint8_t data = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	delayus(10);
	return data;
}

void I2C_ModelInit(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef gpiob;
	gpiob.GPIO_Mode = GPIO_Mode_Out_OD;
	gpiob.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpiob.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpiob);
	
	Write_SCL(1);
	Write_SDA(1);
}

//开始信号
void startSignal(void){
	Write_SDA(1);
	Write_SCL(1);
	
	Write_SDA(0);
	Write_SCL(0);
}

//结束信号
void endSignal(void){
	Write_SDA(0);
	
	Write_SCL(1);
	Write_SDA(1);
}

//主机发送数据
void sendData(uint8_t data){
	for(uint8_t i = 0; i < 8; i++){
		//放数据
		Write_SDA(data & (0x80>>i));
		
		//SCL拉高
		Write_SCL(1);
		
		//从机读数据
		
		//SCL拉低
		Write_SCL(0);
	}
}

//主机接收数据
uint8_t receiveData(void){
	
	uint8_t data = 0x00;
	
	//给从机操作数据线
	Write_SDA(1);
	
	for(uint8_t i = 0; i < 8; i++){
		//从机放数据
		
		//SCL拉高
		Write_SCL(1);
		//主机从SDA读数据
		if(Read_SDA()){
			data |= (0x80>>i);
		}
		//SCL拉低
		Write_SCL(0);
	}
	return data;
}

//主机发送应答
void sendACK(uint8_t ack){
	//放应答到SDA
	Write_SDA(ack);
	//拉高SCL
	Write_SCL(1);
	//从机读应答
	
	//拉低SCL
	Write_SCL(0);
}

//主机接收应答
uint8_t receiveACK(void){
	
	//给从机操作数据线
	Write_SDA(1);
	//从机放数据
	
	//拉高SCL
	Write_SCL(1);
	//读取SDA数据
	uint8_t ack = Read_SDA();
	//SCL拉低
	Write_SCL(0);
	return ack;
}

