#ifndef _KEY_H_
#define _KEY_H_
#include "Delay.h"
#include "FreeRTOS.h"
#include "task.h"

void Key_Init(void);

uint8_t getKeyNum(void);

#endif

