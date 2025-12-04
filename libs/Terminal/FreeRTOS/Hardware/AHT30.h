#ifndef AHT30_H
#define AHT30_H

typedef struct{
	float Humi;
	float Temp;
}AHT30;

extern uint8_t AHT30_DataByte[];
extern AHT30 aht30;

void AHT30_Init(void);

void AHT30_TriggerMeasure(void);

void AHT30_ReadData(AHT30 *aht30);

unsigned char Calc_CRC8(unsigned char *message,unsigned char Num);


#endif

