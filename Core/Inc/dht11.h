#ifndef DHT11_H
#define DHT11_H


#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f3xx.h"




typedef struct
{
	float Temperature;
	float Humidity;
}DHT_DataTypedef;


void DHT_GetData (DHT_DataTypedef *DHT_Data);
#endif
/* DHT11_H */