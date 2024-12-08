#ifndef DHT11_H
#define DHT11_H
#include <stdbool.h>
#include <stdint.h>

typedef struct {
float Humidity;
float Temperature;
}
DHT_DataTypedef;


void DHT_GetData (DHT_DataTypedef *DHT_Data);
#endif /* DHT11_H */