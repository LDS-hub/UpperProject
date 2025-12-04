#include "stm32f10x.h"                  // Device header
#include "Generate_JSON.h"
#include <string.h>
#include <stdio.h>


void create_humi_temp_json(char *buffer, 
									const char *driver, 
									int id, 
									float humi, 
									float temp){
	snprintf(buffer, BUFFER_SIZE, 
             "{\"driver\":\"%s\",\"id\":%d,\"data\":{\"humi\":%.2f,\"temp\":%.2f}}",
             driver, id, humi, temp);
}

