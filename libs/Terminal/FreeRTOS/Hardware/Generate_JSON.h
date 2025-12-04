#ifndef GENERATE_JSON
#define GENERATE_JSON

#include <stdio.h>
#define BUFFER_SIZE 128

void create_humi_temp_json(char *buffer, 
									const char *driver, 
									int id, 
									float humi, 
									float temp);

#endif
