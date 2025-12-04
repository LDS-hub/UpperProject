#ifndef I2C_MODEL_H
#define I2C_MODEL_H

//写SCL--完全在主机的控制下，不给从机控制
void Write_SCL(uint8_t data);

//写SDA
void Write_SDA(uint8_t data);

//读SDA
uint8_t Read_SDA(void);

void I2C_ModelInit(void);

//开始信号
void startSignal(void);

//结束信号
void endSignal(void);

//主机发送数据
void sendData(uint8_t data);

//主机接收数据
uint8_t receiveData(void);

//主机发送应答
void sendACK(uint8_t ack);

//主机接收应答
uint8_t receiveACK(void);

#endif

