#include "dht.h"
#include "main.h"
//#include "tim.h"
#include <stdio.h>   // dla sprintf
#include <string.h> 
#define TYPE_DHT11    // define according to your sensor
//#define TYPE_DHT22


#define DHT_PORT GPIOA
#define DHT_PIN GPIO_PIN_7


extern UART_HandleTypeDef huart2;

/*******************************************     NO CHANGES AFTER THIS LINE      ****************************************************/

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM; uint8_t Presence = 0;


extern char uart_buf[50];
uint32_t DWT_Delay_Init(void)
{
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
     __ASM volatile ("NOP");
     __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
     if(DWT->CYCCNT)
     {
       return 0; /*clock cycle counter started*/
     }
     else
  {
    return 1; /*clock cycle counter not started*/
  }
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> add-features
__STATIC_INLINE void delay(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void DHT_Start (void)
{
	DWT_Delay_Init();
	Set_Pin_Output (DHT_PORT, DHT_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 0);   // pull the pin low

#if defined(TYPE_DHT11)
	delay (18000);   // wait for 18ms
#endif

#if defined(TYPE_DHT22)
	delay (1200);  // >1ms delay
#endif

    HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 1);   // pull the pin high
    delay (20);   // wait for 30us
	Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input
}

<<<<<<< HEAD
// void DHT_Start(void)
// {
//  /****************************************************/
// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);     //Initialize with Data pin HIGH
 
//  HAL_Delay(300);
// /****************************************************/
// Set_Pin_Output (DHT_PORT, DHT_PIN);  // set the pin as output
// HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 0);   // pull the pin low
// HAL_Delay(18);     //wait for 18ms
// HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 1);   // pull the pin high
// delay(20) ;  //20us
// Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input

// }
//#if defined(TYPE_DHT11)
//	delay (18000);   // wait for 18ms
//#endif

//#if defined(TYPE_DHT22)
//	delay (1200);  // >1ms delay
//#endif

   // HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, 1);   // pull the pin high
   // delay (20);   // wait for 30us
	//Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input
//}

=======
>>>>>>> add-features
uint8_t DHT_Check_Response(void)
{
    uint8_t Response = 0;
    delay(40);

    if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
    {
        delay(80);
        if ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))) 
            Response = 1;
        else 
            Response = -1;
    }
    while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)));   // czekaj na niski stan

    // Wyrzucenie na UART dla debugowania
    //sprintf(uart_buf, "Response: %d\r\n", Response);
    //HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);

    return Response;
}


uint8_t DHT_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));   // wait for the pin to go high
		delay (40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
<<<<<<< HEAD
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));  // wait for the pin to go low
	}
	return i;
}

void DHT_GetData(DHT_DataTypedef *DHT_Data)
{

    char uart_buf2[100]; // Bufor tekstowy dla UART

    DHT_Start();
    Presence = DHT_Check_Response();

    if (Presence == 1) {
        // Odczytujemy dane z czujnika
        Rh_byte1 = DHT_Read();
        Rh_byte2 = DHT_Read();
        Temp_byte1 = DHT_Read();
        Temp_byte2 = DHT_Read();
        SUM = DHT_Read();

        // Debugowanie odczytanych danych
        sprintf(uart_buf2, "RH1=%d, RH2=%d, T1=%d, T2=%d, SUM=%d\r\n",
                Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2, SUM);
        HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);

        // Sprawdzamy sumę kontrolną
        if (SUM == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2)) {
            // Debugowanie, jeśli suma kontrolna się zgadza
            sprintf(uart_buf2, "Checksum OK!\r\n");
            HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);

            // Przekształcenie wartości na temperaturę i wilgotność
            DHT_Data->Humidity = Rh_byte1 + (Rh_byte2 / 10.0);  // Wilgotność: 2 bajty
            DHT_Data->Temperature = Temp_byte1 + (Temp_byte2 / 10.0);  // Temperatura: 2 bajty

            // Wyświetlenie poprawnych danych
            sprintf(uart_buf2, "Temperature: %.1f, Humidity: %.1f\r\n",
                    DHT_Data->Temperature, DHT_Data->Humidity);
            HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);
        } else {
            sprintf(uart_buf2, "Checksum error!\r\n");
            HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);
        }
    } else {
        sprintf(uart_buf2, "No response from DHT sensor.\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);
    }
<<<<<<< HEAD
<<<<<<< HEAD
}
=======
}
>>>>>>> ea5ffebb7b2380fb2ffe99a64204362398866177
=======
int DHT11_read(struct DHT11_Dev* dev) {
	
	//Initialisation
	uint8_t i, j, temp;
	uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Generate START condition
	//o
	GPIO_InitStructure.GPIO_Pin = dev->pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(dev->port, &GPIO_InitStructure);
	
	//dev->port->MODER |= GPIO_MODER_MODER6_0;
	
	//Put LOW for at least 18ms
	GPIO_ResetBits(dev->port, dev->pin);
	
	//wait 18ms
	TIM2->CNT = 0;
	while((TIM2->CNT) <= 18000);
	
	//Put HIGH for 20-40us
	GPIO_SetBits(dev->port, dev->pin);
	
	//wait 40us
	TIM2->CNT = 0;
	while((TIM2->CNT) <= 40);
	//End start condition
	
	//io();
	//Input mode to receive data
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(dev->port, &GPIO_InitStructure);
	
	//DHT11 ACK
	//should be LOW for at least 80us
	//while(!GPIO_ReadInputDataBit(dev->port, dev->pin));
	TIM2->CNT = 0;
	while(!GPIO_ReadInputDataBit(dev->port, dev->pin)) {
		if(TIM2->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	//should be HIGH for at least 80us
	//while(GPIO_ReadInputDataBit(dev->port, dev->pin));		
	TIM2->CNT = 0;
	while(GPIO_ReadInputDataBit(dev->port, dev->pin)) {
		if(TIM2->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	//Read 40 bits (8*5)
	for(j = 0; j < 5; ++j) {
		for(i = 0; i < 8; ++i) {
			
			//LOW for 50us
			while(!GPIO_ReadInputDataBit(dev->port, dev->pin));
			/*TIM2->CNT = 0;
			while(!GPIO_ReadInputDataBit(dev->port, dev->pin)) {
				if(TIM2->CNT > 60)
					return DHT11_ERROR_TIMEOUT;
			}*/
			
			//Start counter
			TIM_SetCounter(TIM2, 0);
			
			//HIGH for 26-28us = 0 / 70us = 1
			while(GPIO_ReadInputDataBit(dev->port, dev->pin));
			/*while(!GPIO_ReadInputDataBit(dev->port, dev->pin)) {
				if(TIM2->CNT > 100)
					return DHT11_ERROR_TIMEOUT;
			}*/
			
			//Calc amount of time passed
			temp = TIM_GetCounter(TIM2);
			
			//shift 0
			data[j] = data[j] << 1;
			
			//if > 30us it's 1
			if(temp > 40)
				data[j] = data[j]+1;
=======
>>>>>>> add-features
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));  // wait for the pin to go low
	}
<<<<<<< HEAD
	
	//verify the Checksum
	if(data[4] != (data[0] + data[2]))
		return DHT11_ERROR_CHECKSUM;
	
	//set data
	dev->temparature = data[2];
	dev->humidity = data[0];
	
	return DHT11_SUCCESS;
}
>>>>>>> parent of dcbb533 (Add dht11)
=======
}
>>>>>>> parent of 848b49f (Merge branch 'main' of https://github.com/Marzpy/Home_sys)
=======
	return i;
}

void DHT_GetData(DHT_DataTypedef *DHT_Data)
{
    char uart_buf2[100]; // Bufor tekstowy dla UART

    DHT_Start();
    Presence = DHT_Check_Response();

    if (Presence == 1) {
        // Odczyt danych z czujnika
        Rh_byte1 = DHT_Read();
        Rh_byte2 = DHT_Read();
        Temp_byte1 = DHT_Read();
        Temp_byte2 = DHT_Read();
        SUM = DHT_Read();

        // Debugowanie odczytanych danych
        sprintf(uart_buf2, "RH1=%d, RH2=%d, T1=%d, T2=%d, SUM=%d\r\n",
                Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2, SUM);
      //  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);

        // Sprawdzanie sumy kontrolnej
        if (SUM == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2)) {
            // Przekształcenie danych w zależności od typu czujnika
#ifdef TYPE_DHT11
            DHT_Data->Humidity = Rh_byte1;  // RH1 zawiera całą część wilgotności
            DHT_Data->Temperature = Temp_byte1;  // T1 zawiera całą część temperatury
#elif defined(TYPE_DHT22)
            DHT_Data->Humidity = ((Rh_byte1 << 8) | Rh_byte2) / 10.0;  // 16-bit wilgotność
            DHT_Data->Temperature = ((Temp_byte1 << 8) | Temp_byte2) / 10.0;  // 16-bit temperatura

            // Obsługa wartości ujemnych (DHT22 używa bitu znaku)
            if (Temp_byte1 & 0x80) {
                DHT_Data->Temperature *= -1;  // Jeśli MSB ustawione, temperatura ujemna
            }
#endif

            // Wyświetlenie danych
            sprintf(uart_buf2, "Temperature: %.1f, Humidity: %.1f\r\n",
                    DHT_Data->Temperature, DHT_Data->Humidity);
          //  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);
        } else {
            sprintf(uart_buf2, "Checksum error!\r\n");
          //  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);
        }
    } else {
        sprintf(uart_buf2, "No response from DHT sensor.\r\n");
       // HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf2, strlen(uart_buf2), HAL_MAX_DELAY);
    }
}
>>>>>>> add-features
